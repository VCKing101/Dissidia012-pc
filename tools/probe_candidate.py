"""Compile a candidate and compare explicitly specified retail function ranges."""

import argparse
import hashlib
import json
import struct
from pathlib import Path
import rabbitizer

from compile_psp import compile_object
from mips_relocations import configured_addresses, resolve_function
from probe_compilers import functions_from_object

ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path)
    parser.add_argument("--function", action="append", required=True, metavar="NAME:ADDRESS:SIZE")
    parser.add_argument("--symbol", action="append", default=[], metavar="NAME=ADDRESS")
    parser.add_argument("--show-differences", action="store_true")
    args = parser.parse_args()
    config = json.loads((ROOT / "config/extra.matches.json").read_text())
    addresses = configured_addresses(config)
    specs = []
    for text in args.function:
        name, address, size = text.split(":")
        specs.append({"name": name, "address": int(address, 0), "size": int(size, 0)})
        addresses[name] = int(address, 0)
    for text in args.symbol:
        name, address = text.split("=")
        addresses[name] = int(address, 0)
    output = ROOT / "analysis/candidate-probes" / args.source.stem
    output.mkdir(parents=True, exist_ok=True)
    obj = output / "candidate.o"
    process = compile_object(config["compiler"], config["flags"], args.source, obj.relative_to(ROOT))
    (output / "compile.log").write_text(process.stdout + process.stderr, encoding="utf-8")
    if process.returncode:
        raise RuntimeError(f"Compiler failed; see {output / 'compile.log'}")
    functions = functions_from_object(obj)
    reference = (ROOT / "orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_EXTRA_APP.ELF").read_bytes()
    pinned = json.loads((ROOT / "config/ulus10566.json").read_text())["extra_sha256"]
    if hashlib.sha256(reference).hexdigest() != pinned:
        raise ValueError("Reference overlay differs from the pinned retail input")
    results = []
    for spec in specs:
        offset = spec["address"] - 0x089DD4C0 + 0x54
        if spec["size"] <= 0 or not 0x54 <= offset < offset + spec["size"] <= 0x5F4C:
            raise ValueError("Candidate range lies outside the overlay payload")
        actual = resolve_function(functions[spec["name"]], addresses)
        expected = reference[offset:offset + spec["size"]]
        differences = [{"offset": i, "compiled": actual[i:i+4].hex(), "retail": expected[i:i+4].hex()}
                       for i in range(0, max(len(actual), len(expected)), 4) if actual[i:i+4] != expected[i:i+4]]
        result = {**spec, "compiled_size": len(actual), "match": actual == expected, "different_words": differences}
        results.append(result)
        print(f"{spec['name']}: {len(actual)}/{spec['size']} bytes; {'MATCH' if result['match'] else str(len(differences)) + ' differing words'}")
        if args.show_differences:
            for difference in differences[:40]:
                address = spec["address"] + difference["offset"]
                decoded = []
                for field in ("compiled", "retail"):
                    data = bytes.fromhex(difference[field])
                    decoded.append(rabbitizer.Instruction(struct.unpack("<I", data)[0], address,
                                    rabbitizer.InstrCategory.R4000ALLEGREX).disassemble() if len(data) == 4 else "<absent>")
                print(f"  +0x{difference['offset']:03X}: {decoded[0]} | retail: {decoded[1]}")
    (output / "results.json").write_text(json.dumps(results, indent=2) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()

"""Resolve and compare the battle source-recovery batch against pinned retail."""
import argparse
import hashlib
import json
import re
import struct
import sys
from pathlib import Path
import rabbitizer

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from compile_psp import compile_object
from mips_relocations import configured_addresses, resolve_function
from probe_compilers import functions_from_object


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("sources", nargs="*")
    parser.add_argument("--symbol", action="append", default=[])
    parser.add_argument("--details", action="store_true")
    parser.add_argument("--unused-inline", action="append", default=[])
    args = parser.parse_args()
    manifest = json.loads((ROOT / "config/battle.matches.json").read_text())
    inventory = json.loads((ROOT / "analysis/ULUS10566/battle/inventory.json").read_text())
    specs = {int(f["address"], 16): f for f in inventory["functions"]}
    addresses = configured_addresses(manifest)
    addresses.update({"func_" + f"{a:08X}": a for a in specs})
    addresses.update({"func_088DA060": 0x088DA060})
    for assignment in args.symbol:
        name, address = assignment.split("=", 1)
        addresses[name] = int(address, 0)
    reference = (ROOT / "orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_BATTLE_APP.ELF").read_bytes()
    if hashlib.sha256(reference).hexdigest() != "1f96cfb3be771ff6e94741ae3916fab340df74e36ede07ca39545d015f4c49dc":
        raise ValueError("Battle retail input changed")
    output = ROOT / "analysis/battle-source-recovery"
    output.mkdir(parents=True, exist_ok=True)
    rows = []
    arguments = args.sources
    if not arguments:
        arguments = [u["source"] for u in manifest["units"]]
    for argument in arguments:
        source = Path(argument)
        obj = output / (source.stem + ".o")
        process = compile_object(manifest["compiler"], manifest["flags"], source, obj.relative_to(ROOT))
        obj.with_suffix(".log").write_text(process.stdout + process.stderr)
        if process.returncode:
            raise RuntimeError(obj.with_suffix(".log").read_text())
        for name, function in functions_from_object(obj).items():
            inline_names = set(args.unused_inline)
            for unit in manifest["units"]:
                if unit["source"] == source.as_posix():
                    inline_names.update(unit.get("unused_inline_symbols", []))
            if name in inline_names or not re.fullmatch(r"func_[0-9A-Fa-f]+", name):
                print("Compiler helper:", name, len(function["bytes"]), "bytes; no original-function credit", flush=True)
                continue
            address = int(name.removeprefix("func_"), 16)
            spec = specs[address]
            actual = resolve_function(function, addresses)
            offset = address - 0x089DD4C0 + 0x54
            expected = reference[offset:offset + spec["body_bytes"]]
            row = {"name": name, "address": spec["address"], "size": len(expected),
                   "compiled_size": len(actual), "match": actual == expected,
                   "source": source.as_posix(), "relocations": function["relocations"],
                   "raw_object_sha256": hashlib.sha256(obj.read_bytes()).hexdigest()}
            callers = []
            for candidate in inventory["functions"]:
                if f"{address:08x}" not in candidate["direct_call_targets"]:
                    continue
                for body_range in candidate["ranges"]:
                    start, end = int(body_range["start"], 16), int(body_range["end"], 16)
                    for pc in range(start, end, 4):
                        word = struct.unpack_from("<I", reference, pc-0x089DD4C0+0x54)[0]
                        if word >> 26 == 3 and ((pc+4)&0xF0000000 | (word&0x03FFFFFF)<<2) == address:
                            callers.append({"function": candidate["address"], "call_pc": f"0x{pc:08X}"})
            row["checked_direct_callers"] = callers
            rows.append(row)
            print(name, len(actual), "/", len(expected), "MATCH" if row["match"] else "DIFF", flush=True)
            if not row["match"] and args.details:
                for i in range(0, max(len(actual), len(expected)), 4):
                    if actual[i:i+4] == expected[i:i+4]: continue
                    texts = []
                    for blob in (actual, expected):
                        texts.append(rabbitizer.Instruction(struct.unpack_from("<I", blob, i)[0], address+i,
                            rabbitizer.InstrCategory.R4000ALLEGREX).disassemble() if i+4 <= len(blob) else "<absent>")
                    print(hex(i), " | ".join(texts))
    (output / "results.json").write_text(json.dumps(rows, indent=2) + "\n")
    if not rows or not all(row["match"] for row in rows):
        raise ValueError("Battle recovery batch contains an empty result or a raw C mismatch")


if __name__ == "__main__":
    main()

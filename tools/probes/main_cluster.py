"""Bounded raw relocation comparison for main callback/manager source work."""
import argparse
import hashlib
import json
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from compile_psp import compile_object
from probe_compilers import functions_from_object
from mips_relocations import configured_addresses, resolve_function
from elftools.elf.elffile import ELFFile
import rabbitizer

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("source")
    parser.add_argument("--function", action="append", required=True)
    parser.add_argument("--symbol", action="append", default=[])
    args = parser.parse_args()
    config = json.loads((ROOT / "config/main.matches.json").read_text())
    addresses = configured_addresses(config)
    specs = []
    for value in args.function:
        name, address, size = value.split(":")
        address, size = int(address, 0), int(size, 0)
        specs.append((name, address, size))
        addresses[name] = address
    for value in args.symbol:
        name, address = value.split("=")
        addresses[name] = int(address, 0)
    output = ROOT / "build/main-source-cluster" / Path(args.source).stem
    output.mkdir(parents=True, exist_ok=True)
    obj = output / "raw.o"
    result = compile_object(config["compiler"], config["flags"], args.source, obj.relative_to(ROOT))
    (output / "compile.log").write_text(result.stdout + result.stderr)
    if result.returncode: raise RuntimeError(result.stdout + result.stderr)
    functions = functions_from_object(obj)
    ref = ROOT / "analysis/ULUS10566/main/EBOOT.ELF"
    if hashlib.sha256(ref.read_bytes()).hexdigest() != "0bc8e779aad2814a3d1bd77346f69ca5e16665845ed053cd0ad44a9cbfaefe6d":
        raise ValueError("Reference mismatch")
    rows = []
    with ref.open("rb") as stream:
        elf = ELFFile(stream)
        for name, address, size in specs:
            section = next(s for s in elf.iter_sections() if s["sh_flags"] & 4 and
                           s["sh_addr"] <= address < address + size <= s["sh_addr"] + s["sh_size"])
            expected = section.data()[address-section["sh_addr"]:address-section["sh_addr"]+size]
            actual = resolve_function(functions[name], addresses)
            diffs = []
            print(f"{name}: {len(actual)}/{size} bytes / match={actual == expected}")
            for offset in range(0, max(len(actual), size), 4):
                if actual[offset:offset+4] == expected[offset:offset+4]: continue
                pair = []
                for data in (actual[offset:offset+4], expected[offset:offset+4]):
                    pair.append(rabbitizer.Instruction(struct.unpack('<I',data)[0], address+offset,
                                rabbitizer.InstrCategory.R4000ALLEGREX).disassemble() if len(data)==4 else '<absent>')
                print(f" +{offset:03X}: {pair[0]} | {pair[1]}")
                diffs.append(offset)
            rows.append({"name":name,"address":hex(address),"size":size,"compiled_size":len(actual),"match":actual==expected,"differences":diffs})
    (output / "results.json").write_text(json.dumps(rows,indent=2)+'\n')

if __name__ == '__main__': main()

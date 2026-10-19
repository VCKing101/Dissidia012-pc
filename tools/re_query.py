"""Small static queries over the pinned modules for reverse engineering.

    re_query.py dis <module> <address> [--bytes N]   disassemble the containing function
    re_query.py callers <module> <address>           direct jal/j sites targeting an address
    re_query.py func <module> <address>              containing function and its direct callees
    re_query.py words <module> <hex-word> [mask]     instruction words matching (word & mask)

Overlays share addresses, so every query is module-qualified. Calls from an
overlay into the main executable are found by querying the overlay module
with a main-executable target address.
"""

import argparse
import bisect
import json
import struct
from pathlib import Path

import rabbitizer

ROOT = Path(__file__).resolve().parents[1]


class Module:
    def __init__(self, name):
        scope = json.loads((ROOT / "config/progress.json").read_text())
        entry = next(m for m in scope["modules"] if m["id"] == name)
        self.name = name
        self.data = (ROOT / entry["path"]).read_bytes()
        phoff = struct.unpack_from("<I", self.data, 28)[0]
        phentsize, phnum = struct.unpack_from("<HH", self.data, 42)
        self.segments = []
        for index in range(phnum):
            kind, offset, vaddr, _, filesz = struct.unpack_from("<IIIII", self.data, phoff + index * phentsize)
            if kind == 1:
                self.segments.append((vaddr, offset, filesz))
        inventory = json.loads((ROOT / f"config/inventory/{name}.json").read_text())
        self.functions = sorted((int(f["address"], 16), f["body_bytes"], f) for f in inventory["functions"])
        self.starts = [f[0] for f in self.functions]

    def read(self, address, size):
        for vaddr, offset, filesz in self.segments:
            if vaddr <= address and address + size <= vaddr + filesz:
                return self.data[offset + address - vaddr:offset + address - vaddr + size]
        raise ValueError(f"{address:#010x} is not file-backed in {self.name}")

    def containing(self, address):
        index = bisect.bisect_right(self.starts, address) - 1
        if index >= 0:
            start, size, record = self.functions[index]
            if start <= address < start + size:
                return start, size, record
        return None

    def words(self):
        for vaddr, offset, filesz in self.segments:
            for position in range(0, filesz - 3, 4):
                yield vaddr + position, struct.unpack_from("<I", self.data, offset + position)[0]


def disassemble(module, start, size):
    rabbitizer.config.regNames_gprAbiNames = rabbitizer.Abi.O32
    lines = []
    for address in range(start, start + size, 4):
        word = struct.unpack("<I", module.read(address, 4))[0]
        instruction = rabbitizer.Instruction(word, address, rabbitizer.InstrCategory.R4000ALLEGREX)
        lines.append(f"{address:08X}: {word:08X}  {instruction.disassemble()}")
    return lines


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("command", choices=("dis", "callers", "func", "words"))
    parser.add_argument("module", choices=("main", "menu", "battle", "extra"))
    parser.add_argument("value", type=lambda v: int(v, 16))
    parser.add_argument("mask", nargs="?", type=lambda v: int(v, 16), default=0xFFFFFFFF)
    parser.add_argument("--bytes", type=int)
    args = parser.parse_args()
    module = Module(args.module)
    if args.command == "dis":
        found = module.containing(args.value)
        start, size = (found[0], found[1]) if found else (args.value, args.bytes or 64)
        if args.bytes:
            start, size = args.value, args.bytes
        print(f"# {args.module} {start:#010x} size {size}" + ("" if found else " (no inventory function)"))
        print("\n".join(disassemble(module, start, size)))
    elif args.command == "func":
        found = module.containing(args.value)
        if not found:
            raise SystemExit("No inventory function contains that address")
        start, size, record = found
        print(json.dumps({k: record[k] for k in ("id", "body_bytes", "direct_call_targets", "matched_c", "primary_owner")}, indent=1))
    elif args.command == "callers":
        for address, word in module.words():
            if word >> 26 in (2, 3) and ((address & 0xF0000000) | ((word & 0x03FFFFFF) << 2)) == args.value:
                found = module.containing(address)
                where = f"in {found[0]:#010x} (size {found[1]})" if found else "outside inventory functions"
                print(f"{'jal' if word >> 26 == 3 else 'j  '} at {address:#010x} {where}")
    else:
        for address, word in module.words():
            if word & args.mask == args.value & args.mask:
                found = module.containing(address)
                print(f"{address:#010x} {word:08X} " + (f"in {found[0]:#010x}" if found else "-"))


if __name__ == "__main__":
    main()

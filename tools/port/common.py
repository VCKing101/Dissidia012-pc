"""Shared loaders for the port survey scripts.

Reads the four decrypted ELFs from build/ULUS10566 and the maintained function
inventories in config/inventory. Nothing here writes game bytes anywhere; the
survey scripts emit only addresses, opcode names, NIDs and counts.
"""

import hashlib
import json
import struct
from functools import lru_cache
from pathlib import Path

import rabbitizer
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[2]
BUILD = ROOT / "build/ULUS10566"
OUT = ROOT / "build/scratch/port"
MODULES = {
    "main": "EBOOT.ELF",
    "menu": "OVL_MENU_APP.ELF",
    "battle": "OVL_BATTLE_APP.ELF",
    "extra": "OVL_EXTRA_APP.ELF",
}
PRX = ROOT / "orig/ULUS10566/PSP_GAME/USRDIR/DATA/MODULE/LIBSUPPREACC.PRX"
IMPORT_STUBS = (0x089C5770, 0x089C60B0)  # main section 7, 296 eight-byte stubs

REG = ["zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
       "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"]


def h(value):
    return f"0x{value:08X}"


class Module:
    """One loaded binary: initialized bytes by virtual address plus inventory."""

    def __init__(self, name):
        self.name = name
        self.path = BUILD / MODULES[name]
        raw = self.path.read_bytes()
        self.sha256 = hashlib.sha256(raw).hexdigest()
        self.inventory = json.loads((ROOT / f"config/inventory/{name}.json").read_text(encoding="utf-8"))
        if self.inventory["reference_sha256"] != self.sha256:
            raise SystemExit(f"{name}: inventory hash does not match {self.path}")
        self.chunks = []  # (start, bytes, executable)
        with self.path.open("rb") as stream:
            elf = ELFFile(stream)
            for section in elf.iter_sections():
                if section["sh_type"] == "SHT_PROGBITS" and section["sh_flags"] & 2 and section["sh_size"]:
                    self.chunks.append((section["sh_addr"], section.data(), bool(section["sh_flags"] & 4)))
            self.bss = [(s["sh_addr"], s["sh_addr"] + s["sh_size"]) for s in elf.iter_sections()
                        if s["sh_type"] == "SHT_NOBITS" and s["sh_size"]]
        self.chunks.sort()
        self.start = self.chunks[0][0]
        self.end = max(a + len(d) for a, d, _ in self.chunks)
        self.functions = self.inventory["functions"]
        self.func_ranges = sorted((int(r["start"], 16), int(r["end"], 16), f["address"])
                                  for f in self.functions for r in f["ranges"])
        self.func_starts = {int(f["address"], 16) for f in self.functions}

    def contains(self, address):
        return any(a <= address < a + len(d) for a, d, _ in self.chunks)

    def read(self, address, size):
        for start, data, _ in self.chunks:
            if start <= address and address + size <= start + len(data):
                return data[address - start:address - start + size]
        return None

    def word(self, address):
        blob = self.read(address, 4)
        return None if blob is None else struct.unpack("<I", blob)[0]

    def words(self, start, end):
        blob = self.read(start, end - start)
        return struct.unpack(f"<{(end - start) // 4}I", blob)

    def iter_function_words(self):
        """Yield (address, word, function_address) over every function range."""
        for start, end, owner in self.func_ranges:
            for offset, word in enumerate(self.words(start, end)):
                yield start + offset * 4, word, owner

    def iter_ranges(self, key):
        for item in self.inventory[key]:
            yield int(item["start"], 16), int(item["end"], 16)

    def owner_of(self, address):
        # binary search over sorted ranges
        lo, hi = 0, len(self.func_ranges)
        while lo < hi:
            mid = (lo + hi) // 2
            if self.func_ranges[mid][0] <= address:
                lo = mid + 1
            else:
                hi = mid
        if lo and self.func_ranges[lo - 1][0] <= address < self.func_ranges[lo - 1][1]:
            return self.func_ranges[lo - 1][2]
        return None


@lru_cache(maxsize=None)
def decode(word):
    """Opcode name for a word (independent of address)."""
    return rabbitizer.Instruction(word, 0, rabbitizer.InstrCategory.R4000ALLEGREX).getOpcodeName()


def instr(word, address):
    return rabbitizer.Instruction(word, address, rabbitizer.InstrCategory.R4000ALLEGREX)


def op(word):
    return word >> 26


def jal_target(word, address):
    return ((address + 4) & 0xF0000000) | ((word & 0x03FFFFFF) << 2)


def is_jal(word):
    return op(word) == 3


def is_j(word):
    return op(word) == 2


def rs(word):
    return (word >> 21) & 31


def rt(word):
    return (word >> 16) & 31


def rd(word):
    return (word >> 11) & 31


def imm(word):
    return word & 0xFFFF


def simm(word):
    value = word & 0xFFFF
    return value - 0x10000 if value & 0x8000 else value


def is_jr(word):
    return op(word) == 0 and (word & 0x3F) == 8


def is_jalr(word):
    return op(word) == 0 and (word & 0x3F) == 9


def load_all():
    return {name: Module(name) for name in MODULES}


def write_json(name, data):
    OUT.mkdir(parents=True, exist_ok=True)
    path = OUT / name
    path.write_text(json.dumps(data, indent=1, sort_keys=False) + "\n", encoding="utf-8")
    return path


def nid_names():
    path = OUT / "ppsspp_nids.json"
    if not path.exists():
        raise SystemExit("Run tools/port/ppsspp_nids.py first")
    tables = json.loads(path.read_text(encoding="utf-8"))
    flat = {}
    for module, entries in tables.items():
        for nid, name in entries.items():
            flat.setdefault(nid, set()).add(name)
    return tables, flat


def track_constants(module, call_address, window=40):
    """Rough backward constant tracker for argument registers before a call.

    Walks forward from up to `window` instructions before the call, through
    its delay slot, folding lui/addiu/ori/move/li with constants. Resets the
    whole state at branch targets is not attempted; results are hints only.
    """
    regs = {}
    start = max(module.start, call_address - window * 4)
    owner = module.owner_of(call_address)
    for address in range(start, call_address + 8, 4):
        if owner is not None and module.owner_of(address) != owner:
            regs = {}
            continue
        w = module.word(address)
        if w is None:
            continue
        if address == call_address:
            continue
        o = op(w)
        t = rt(w)
        s = rs(w)
        if o == 0x0F:  # lui
            regs[t] = imm(w) << 16
        elif o == 0x09:  # addiu
            if s == 0:
                regs[t] = simm(w) & 0xFFFFFFFF
            elif regs.get(s) is not None:
                regs[t] = (regs[s] + simm(w)) & 0xFFFFFFFF
            else:
                regs[t] = None
        elif o == 0x0D:  # ori
            if s == 0:
                regs[t] = imm(w)
            elif s in regs and regs[s] is not None:
                regs[t] = regs[s] | imm(w)
            else:
                regs[t] = None
        elif o == 0 and (w & 0x3F) in (0x21, 0x25) and rt(w) == 0:  # move rd, rs
            regs[rd(w)] = regs.get(s) if s else 0
        else:
            # any other write to a GPR clobbers it (approximate: rt for I-type, rd for R-type)
            if o == 0:
                regs[rd(w)] = None
            elif o in (0x08, 0x0A, 0x0B, 0x0C, 0x0E, 0x20, 0x21, 0x23, 0x24, 0x25):
                regs[t] = None
            elif o == 3:
                regs = {}
    return {REG[r]: v for r, v in regs.items() if v is not None}

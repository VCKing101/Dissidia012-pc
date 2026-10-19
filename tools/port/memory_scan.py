"""Hard-coded address usage and GE command construction in code.

For every `lui` in the inventoried function bodies, find the first later
instruction in the same function that reads the register and classify the
use: memory base (load/store), pointer arithmetic (addiu/ori/addu/or), float
(mtc1), or other. The upper half then places the value in a PSP address
region: scratchpad, VRAM (cached/uncached), user RAM (cached/uncached alias),
extended RAM of PSP-2000+, kernel, hardware registers.

Also locates the statically linked libGu by its context block (found through
the functions that call sceGeListUpdateStallAddr / sceGeListEnQueue) and
counts GE-command-looking constants (`lui rX, CMD<<8` combined with `or`)
inside and outside that cluster.

Writes build/scratch/port/memory_scan.json.
"""

import json
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402
from common import op, rs, rt, rd, imm, simm, h  # noqa: E402

REGIONS = [
    ("scratchpad", 0x00010000, 0x00014000),
    ("VRAM cached", 0x04000000, 0x04200000),
    ("VRAM uncached", 0x44000000, 0x44200000),
    ("user RAM 0x08800000-0x09FFFFFF", 0x08800000, 0x0A000000),
    ("kernel-partition RAM 0x08000000-0x087FFFFF", 0x08000000, 0x08800000),
    ("extended RAM 0x0A000000-0x0BFFFFFF (PSP-2000+)", 0x0A000000, 0x0C000000),
    ("user RAM uncached alias 0x48800000+", 0x48800000, 0x4A000000),
    ("kernel 0x88000000+", 0x88000000, 0x8C000000),
    ("hardware registers 0xBC000000+", 0xBC000000, 0xBE000000),
]

GE_COMMANDS = {
    0x01: "VADDR", 0x02: "IADDR", 0x04: "PRIM", 0x05: "BEZIER", 0x06: "SPLINE", 0x08: "JUMP", 0x0A: "CALL",
    0x0B: "RET", 0x0C: "END", 0x0E: "SIGNAL", 0x0F: "FINISH", 0x10: "BASE", 0x12: "VERTEXTYPE",
    0x14: "OFFSETADDR", 0x9C: "FRAMEBUFPTR", 0x9D: "FRAMEBUFWIDTH", 0x9E: "ZBUFPTR", 0x9F: "ZBUFWIDTH",
    0xA0: "TEXADDR0", 0xA8: "TEXBUFWIDTH0", 0xB0: "CLUTADDR", 0xB1: "CLUTADDRUPPER", 0xB2: "TRANSFERSRC",
    0xB3: "TRANSFERSRCW", 0xB4: "TRANSFERDST", 0xB5: "TRANSFERDSTW", 0xB8: "TEXSIZE0", 0xC2: "TEXMODE",
    0xC3: "TEXFORMAT", 0xC4: "LOADCLUT", 0xC5: "CLUTFORMAT", 0xC6: "TEXFILTER", 0xCB: "TEXFLUSH",
    0xCC: "TEXSYNC", 0xD2: "FRAMEBUFPIXFORMAT", 0xD3: "CLEAR", 0xEA: "TRANSFERSTART", 0xEB: "TRANSFERSRCPOS",
    0xEC: "TRANSFERDSTPOS", 0xEE: "TRANSFERSIZE",
}
GE_ANY = set(GE_COMMANDS) | set(range(0xA0, 0xB0)) | set(range(0xB8, 0xC0))


def region_of(value):
    for name, start, end in REGIONS:
        if start <= value < end:
            return name
    return None


def first_use(words, index, reg):
    for j in range(index + 1, min(index + 12, len(words))):
        w = words[j]
        o = op(w)
        reads = False
        kind = "other"
        if o == 0 and (w & 0x3F) not in (0x00, 0x02, 0x03) and (rs(w) == reg or rt(w) == reg):
            reads, kind = True, "alu"
            if (w & 0x3F) in (0x25, 0x21, 0x20) and (rs(w) == reg or rt(w) == reg):
                kind = "or/addu"
        elif o == 0 and (w & 0x3F) in (0x00, 0x02, 0x03) and rt(w) == reg:
            reads, kind = True, "shift"
        elif o in (0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2A, 0x2B, 0x2E, 0x31, 0x39, 0x32, 0x36, 0x3A, 0x3E, 0x35, 0x3D) and rs(w) == reg:
            reads, kind = True, "memory base"
        elif o in (0x28, 0x29, 0x2B) and rt(w) == reg:
            reads, kind = True, "stored value"
        elif o in (0x09, 0x0D, 0x0C, 0x0E) and rs(w) == reg:
            reads, kind = True, "addiu/ori"
        elif o == 0x11 and ((w >> 21) & 31) == 4 and rt(w) == reg:
            reads, kind = True, "mtc1 (float)"
        elif o == 0x12 and ((w >> 21) & 31) == 7 and rt(w) == reg:
            reads, kind = True, "mtv (vfpu float)"
        elif o in (0x04, 0x05) and (rs(w) == reg or rt(w) == reg):
            reads, kind = True, "compare"
        if reads:
            return j, kind
        # register overwritten?
        if o == 0x0F and rt(w) == reg:
            return None, "dead"
    return None, "unused"


def main():
    modules = common.load_all()
    imports = json.loads((common.OUT / "imports.json").read_text(encoding="utf-8"))
    report = {"regions": {}, "ge": {}}
    region_use = defaultdict(Counter)
    region_sites = defaultdict(list)
    region_funcs = defaultdict(set)
    ge_const = defaultdict(Counter)
    ge_funcs = defaultdict(set)
    # functions holding the GE/libGu import calls
    gu_seed = set()
    for row in imports["imports"]:
        if row["binary"] == "main" and row["group"] == "GE":
            for s in row["sites"].get("main", []):
                gu_seed.add(modules["main"].owner_of(int(s, 16)))
    # context addresses: lui+addiu/lw targets in seed functions, in BSS
    main = modules["main"]
    seed_ctx = Counter()
    for start, end, owner in main.func_ranges:
        if owner not in gu_seed:
            continue
        words = main.words(start, end)
        for i, w in enumerate(words):
            if op(w) == 0x0F:
                for j in range(i + 1, min(i + 6, len(words))):
                    x = words[j]
                    if rs(x) == rt(w) and op(x) in (0x09, 0x23, 0x2B):
                        value = ((imm(w) << 16) + simm(x)) & 0xFFFFFFFF
                        if any(a <= value < b for a, b in main.bss):
                            seed_ctx[value & ~0xFF] += 1
                        break
    gu_blocks = {a for a, c in seed_ctx.most_common(2)}
    gu_funcs = set()
    for name, module in modules.items():
        for start, end, owner in module.func_ranges:
            words = module.words(start, end)
            for i, w in enumerate(words):
                if op(w) != 0x0F:
                    continue
                reg = rt(w)
                j, kind = first_use(words, i, reg)
                value = imm(w) << 16
                if j is not None and kind in ("addiu/ori", "memory base"):
                    x = words[j]
                    value = ((imm(w) << 16) + (simm(x) if op(x) != 0x0D else imm(x))) & 0xFFFFFFFF
                if name == "main" and (value & ~0xFF) in gu_blocks or ((value & ~0xFF) - 0x100) in gu_blocks:
                    gu_funcs.add((name, owner))
                reg_name = region_of(value)
                if reg_name and not reg_name.startswith("user RAM 0x088"):
                    region_use[reg_name][kind] += 1
                    region_funcs[reg_name].add(f"{name}:{owner}")
                    if len(region_sites[reg_name]) < 40:
                        region_sites[reg_name].append(f"{name}:{h(start + i * 4)} {h(value)} {kind}")
                elif reg_name:
                    region_use[reg_name][kind] += 1
                cmd = imm(w) >> 8
                if (imm(w) & 0xFF) == 0 and cmd in GE_ANY and kind == "or/addu":
                    ge_const[(name, owner)][GE_COMMANDS.get(cmd, f"0x{cmd:02X}")] += 1
    gu_set = {o for m, o in gu_funcs if m == "main"} | gu_seed
    gu_addrs = sorted(int(o, 16) for o in gu_set if o)
    inside = Counter()
    outside = Counter()
    outside_funcs = Counter()
    for (name, owner), counts in ge_const.items():
        target = inside if (name == "main" and owner in gu_set) else outside
        target.update(counts)
        if target is outside:
            outside_funcs[f"{name}:{owner}"] += sum(counts.values())
    report["regions"] = {k: {"uses": dict(region_use[k]), "functions": len(region_funcs[k]),
                             "examples": region_sites[k]} for k in region_use}
    report["ge"] = {
        "gu_context_blocks": [h(a) for a in gu_blocks],
        "libgu_functions": len(gu_set),
        "libgu_span": [h(gu_addrs[0]), h(gu_addrs[-1])] if gu_addrs else None,
        "libgu_function_list": [h(a) for a in gu_addrs],
        "ge_command_constants_in_libgu": dict(inside.most_common()),
        "ge_command_constants_elsewhere": dict(outside.most_common()),
        "functions_elsewhere_building_ge_words": len(outside_funcs),
        "top_functions_elsewhere": outside_funcs.most_common(30),
    }
    path = common.write_json("memory_scan.json", report)
    for k, v in report["regions"].items():
        print(f"{k:50s} {v['uses']} fns={v['functions']}")
    g = report["ge"]
    print("GU context:", g["gu_context_blocks"], "libgu fns", g["libgu_functions"], g["libgu_span"])
    print("GE consts in libGu:", g["ge_command_constants_in_libgu"])
    print("GE consts elsewhere:", g["ge_command_constants_elsewhere"], "fns", g["functions_elsewhere_building_ge_words"])
    print("top elsewhere:", g["top_functions_elsewhere"][:15])
    print(f"-> {path}")


if __name__ == "__main__":
    main()

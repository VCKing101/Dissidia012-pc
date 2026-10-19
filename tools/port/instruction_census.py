"""Instruction census over the inventoried function bodies of all four modules.

Decodes every word inside the maintained inventory's function ranges with
rabbitizer (R4000ALLEGREX category, which includes the VFPU) and counts opcode
names, grouped into classes relevant to a static recompiler. Also counts the
decoded instructions the inventory places outside functions, separately.

Writes build/scratch/port/instruction_census.json.
"""

import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

BRANCH_LIKELY = {"beql", "bnel", "blezl", "bgtzl", "bltzl", "bgezl", "bltzall", "bgezall",
                 "bc1fl", "bc1tl", "bvfl", "bvtl", "beqzl", "bnezl"}
ALLEGREX = {"ext", "ins", "min", "max", "clz", "clo", "seb", "seh", "wsbh", "wsbw", "bitrev",
            "madd", "maddu", "msub", "msubu", "rotr", "rotrv", "movz", "movn"}
SYSTEMISH = {"syscall", "break", "sync", "cache", "ll", "sc", "mfic", "mtic", "halt", "sleep",
             "eret", "deret", "dbreak", "dret", "mfc0", "mtc0", "cfc0", "ctc0", "wait",
             "teq", "tne", "tge", "tgeu", "tlt", "tltu", "teqi", "tnei", "tgei", "tgeiu", "tlti", "tltiu"}
UNALIGNED = {"lwl", "lwr", "swl", "swr"}
FPU_PREFIX = ("lwc1", "swc1", "mtc1", "mfc1", "ctc1", "cfc1", "bc1")
VFPU_CLASSES = [
    ("load/store", lambda n: n in {"lv.s", "lv.q", "sv.s", "sv.q", "lvl.q", "lvr.q", "svl.q", "svr.q", "lv", "sv", "lvl", "lvr", "svl", "svr", "sv.q.wb", "usv.q", "ulv.q"}),
    ("prefix", lambda n: n.startswith("vpfx")),
    ("branch", lambda n: n.startswith("bv")),
    ("move to/from GPR/control", lambda n: n in {"mfv", "mtv", "mfvc", "mtvc", "vmfvc", "vmtvc"}),
    ("matrix ops", lambda n: n.split(".")[0] in {"vmmul", "vtfm2", "vtfm3", "vtfm4", "vhtfm2", "vhtfm3", "vhtfm4",
                                                 "vmscl", "vmidt", "vmzero", "vmone", "vmmov", "vcrsp", "vqmul", "vdet", "vcrs"}),
    ("transcendental/reciprocal", lambda n: n.split(".")[0] in {"vrcp", "vrsq", "vsqrt", "vsin", "vcos", "vexp2", "vlog2",
                                                                "vasin", "vnrcp", "vnsin", "vrexp2", "vdiv"}),
    ("compare/select", lambda n: n.split(".")[0] in {"vcmp", "vcmovt", "vcmovf", "vslt", "vsge", "vmin", "vmax", "vsgn"}),
    ("conversion/pack", lambda n: n.split(".")[0] in {"vf2in", "vf2iz", "vf2iu", "vf2id", "vi2f", "vf2h", "vh2f", "vi2uc", "vi2c",
                                                      "vi2us", "vi2s", "vus2i", "vs2i", "vuc2i", "vc2i", "vt4444", "vt5551",
                                                      "vt5650", "vsbz", "vlgb", "vsbn", "vsocp", "vsat0", "vsat1", "vocp", "vbfy1", "vbfy2", "vsrt1", "vsrt2", "vsrt3", "vsrt4", "vwbn"}),
    ("constants/init", lambda n: n.split(".")[0] in {"vzero", "vone", "vfim", "vcst", "viim", "vidt"}),
    ("random", lambda n: n.split(".")[0].startswith("vrnd")),
    ("sync/control", lambda n: n.split(".")[0] in {"vsync", "vnop", "vflush", "vsync2"}),
    ("move/rotate", lambda n: n.split(".")[0] in {"vmov", "vrot", "vabs", "vneg"}),
    ("arithmetic", lambda n: n.startswith("v")),
]


def is_vfpu(name):
    return (name.startswith("v") or name in {"lv.s", "lv.q", "sv.s", "sv.q", "lvl.q", "lvr.q", "svl.q", "svr.q",
                                             "mfv", "mtv", "mfvc", "mtvc", "sv.q.wb", "usv.q", "ulv.q"}
            or name.startswith("bv"))


def is_fpu(name):
    if name.startswith(FPU_PREFIX):
        return True
    parts = name.split(".")
    return len(parts) >= 2 and parts[-1] in {"s", "w", "d"} and not name.startswith("v")


def vfpu_class(name):
    for label, test in VFPU_CLASSES:
        if test(name):
            return label
    return "other"


def census(module, words):
    counts = Counter()
    for word in words:
        counts[common.decode(word)] += 1
    return counts


def summarize(counts):
    total = sum(counts.values())
    vfpu = {n: c for n, c in counts.items() if is_vfpu(n)}
    fpu = {n: c for n, c in counts.items() if is_fpu(n) and n not in vfpu}
    classes = defaultdict(int)
    for n, c in vfpu.items():
        classes[vfpu_class(n)] += c
    pick = lambda names: {n: counts[n] for n in sorted(names) if counts.get(n)}
    return {
        "total": total,
        "nop": counts.get("nop", 0),
        "invalid": counts.get("INVALID", 0),
        "vfpu_total": sum(vfpu.values()),
        "vfpu_distinct": len(vfpu),
        "vfpu_classes": dict(sorted(classes.items(), key=lambda kv: -kv[1])),
        "fpu_total": sum(fpu.values()),
        "fpu": dict(sorted(fpu.items(), key=lambda kv: -kv[1])),
        "allegrex": pick(ALLEGREX),
        "branch_likely": pick(BRANCH_LIKELY),
        "branch_likely_total": sum(pick(BRANCH_LIKELY).values()),
        "system": pick(SYSTEMISH),
        "unaligned": pick(UNALIGNED),
        "jal": counts.get("jal", 0), "j": counts.get("j", 0), "jr": counts.get("jr", 0),
        "jalr": counts.get("jalr", 0),
        "vfpu_opcodes": dict(sorted(vfpu.items(), key=lambda kv: -kv[1])),
        "all_opcodes": dict(sorted(counts.items(), key=lambda kv: -kv[1])),
    }


def main():
    modules = common.load_all()
    report = {"note": "Counts over inventory function-body ranges (provisional boundaries).", "modules": {}}
    grand = Counter()
    vfpu_funcs = {}
    sites = defaultdict(list)  # where the rare system ops are
    for name, module in modules.items():
        counts = Counter()
        func_vfpu = Counter()
        for address, word, owner in module.iter_function_words():
            n = common.decode(word)
            counts[n] += 1
            if is_vfpu(n):
                func_vfpu[owner] += 1
            if n in SYSTEMISH or n == "INVALID":
                if len(sites[(name, n)]) < 200:
                    sites[(name, n)].append([common.h(address), owner])
        outside = Counter()
        in_funcs = set()
        for start, end in module.iter_ranges("instruction_ranges"):
            for offset, word in enumerate(module.words(start, end)):
                address = start + offset * 4
                if module.owner_of(address) is None:
                    outside[common.decode(word)] += 1
        grand.update(counts)
        report["modules"][name] = summarize(counts)
        report["modules"][name]["functions_with_vfpu"] = len(func_vfpu)
        report["modules"][name]["top_vfpu_functions"] = [[f, c] for f, c in func_vfpu.most_common(25)]
        report["modules"][name]["outside_functions"] = {"total": sum(outside.values()),
                                                        "top": dict(outside.most_common(15))}
        vfpu_funcs[name] = len(func_vfpu)
    report["all"] = summarize(grand)
    report["all"]["functions_with_vfpu"] = sum(vfpu_funcs.values())
    report["rare_sites"] = {f"{m}:{n}": v for (m, n), v in sorted(sites.items())}
    path = common.write_json("instruction_census.json", report)
    for name, s in list(report["modules"].items()) + [("all", report["all"])]:
        print(f"{name:7s} total {s['total']:8d} vfpu {s['vfpu_total']:6d} ({s['vfpu_distinct']} ops, "
              f"{s.get('functions_with_vfpu')} fns) fpu {s['fpu_total']:6d} likely {s['branch_likely_total']:5d} "
              f"jr {s['jr']} jalr {s['jalr']} invalid {s['invalid']}")
    a = report["all"]
    print("vfpu classes:", a["vfpu_classes"])
    print("allegrex:", a["allegrex"])
    print("system:", a["system"])
    print("unaligned:", a["unaligned"])
    print("branch likely:", a["branch_likely"])
    print("vfpu opcodes:", a["vfpu_opcodes"])
    print(f"-> {path}")


if __name__ == "__main__":
    main()

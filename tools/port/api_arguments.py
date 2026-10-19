"""Recover constant arguments at call sites of selected OS imports (threads,
semaphores, memory, display, audio, module loading, timing).

At each direct call to an import stub the argument registers are folded from
lui/addiu/ori/move sequences in the preceding instructions of the same
function (common.track_constants). When an argument is not constant and the
call sits in a small wrapper, the wrapper's own direct callers are examined
one level up. Pointer arguments that land on printable strings are shown as
strings. Results are hints, not proofs.

Writes build/scratch/port/api_arguments.json.
"""

import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

ARGS = ["a0", "a1", "a2", "a3", "t0", "t1"]
WANTED = {
    "sceKernelCreateThread": ["name", "entry", "priority", "stack", "attr", "opt"],
    "sceKernelChangeThreadPriority": ["thid", "priority"],
    "sceKernelChangeCurrentThreadAttr": ["clear", "set"],
    "sceKernelCreateSema": ["name", "attr", "init", "max"],
    "sceKernelCreateEventFlag": ["name", "attr", "bits"],
    "sceKernelCreateCallback": ["name", "func", "arg"],
    "sceKernelDelayThread": ["usec"],
    "sceKernelDelayThreadCB": ["usec"],
    "sceKernelAllocPartitionMemory": ["partition", "name", "type", "size", "addr"],
    "sceKernelAllocMemoryBlock": ["name", "type", "size", "opt"],
    "sceKernelLoadModule": ["path", "flags", "opt"],
    "sceUtilityLoadModule": ["module"],
    "sceUtilityUnloadModule": ["module"],
    "sceDisplaySetMode": ["mode", "width", "height"],
    "sceDisplaySetFrameBuf": ["addr", "bufwidth", "format", "sync"],
    "sceDisplayWaitVblankStart": [],
    "sceCtrlSetSamplingMode": ["mode"],
    "sceCtrlSetSamplingCycle": ["cycle"],
    "scePower_469989ad": ["pll", "cpu", "bus"],
    "sceAudioChReserve": ["channel", "samples", "format"],
    "sceAudioOutput2Reserve": ["samples"],
    "sceAudioSetChannelDataLen": ["channel", "samples"],
    "__sceSasInit": ["core", "grain", "voices", "outmode", "rate"],
    "__sceSasSetOutputmode": ["core", "mode"],
    "sceGeListEnQueue": ["list", "stall", "cbid", "arg"],
    "sceGeListEnQueueHead": ["list", "stall", "cbid", "arg"],
    "sceGeSetCallback": ["cb"],
    "sceIoOpen": ["path", "flags", "mode"],
    "sceIoOpenAsync": ["path", "flags", "mode"],
    "sceIoDevctl": ["dev", "cmd"],
    "sceKernelVolatileMemLock": ["unk", "ptr", "size"],
    "sceMpegCreate": ["mpeg", "data", "size", "ring", "frame_width"],
    "sceMpegRingbufferConstruct": ["ring", "packets"],
    "sceAtracSetDataAndGetID": ["buf", "size"],
    "sceKernelDcacheWritebackRange": ["addr", "size"],
    "sceKernelIcacheInvalidateRange": ["addr", "size"],
    "sceKernelDcacheInvalidateRange": ["addr", "size"],
}


def string_at(modules, value):
    for module in modules.values():
        blob = module.read(value, 1)
        if blob is None:
            continue
        out = bytearray()
        a = value
        while len(out) < 80:
            b = module.read(a, 1)
            if not b or b[0] == 0:
                break
            if b[0] < 0x20 or b[0] > 0x7E:
                return None
            out += b
            a += 1
        if len(out) >= 2:
            return f"{module.name}:\"{out.decode()}\""
        return None
    return None


def describe(modules, regs, names):
    out = {}
    for label, reg in zip(names, ARGS):
        if reg not in regs:
            out[label] = None
            continue
        v = regs[reg]
        s = string_at(modules, v) if 0x08800000 <= v < 0x0A000000 else None
        out[label] = s or common.h(v)
    return out


def direct_callers(modules, target):
    sites = []
    for name, module in modules.items():
        for start, end in module.iter_ranges("instruction_ranges"):
            for offset, w in enumerate(module.words(start, end)):
                if common.is_jal(w) and common.jal_target(w, start + offset * 4) == target:
                    sites.append((name, start + offset * 4))
    return sites


def main():
    modules = common.load_all()
    imports = json.loads((common.OUT / "imports.json").read_text(encoding="utf-8"))
    report = {}
    for row in imports["imports"]:
        if row["binary"] != "main" or row["name"] not in WANTED:
            continue
        names = WANTED[row["name"]]
        entries = []
        for module_name, sites in row["sites"].items():
            module = modules[module_name]
            for site in sites:
                address = int(site, 16)
                regs = common.track_constants(module, address)
                entry = {"site": f"{module_name}:{site}", "function": module.owner_of(address),
                         "args": describe(modules, regs, names)}
                missing = [label for label in entry["args"] if entry["args"][label] is None]
                owner = module.owner_of(address)
                func = next((f for f in module.functions if f["address"] == owner), None)
                if missing and func and func["body_bytes"] <= 400:
                    up = []
                    for caller_module, caller in direct_callers(modules, int(owner, 16))[:12]:
                        r2 = common.track_constants(modules[caller_module], caller)
                        up.append({"site": f"{caller_module}:{common.h(caller)}",
                                   "args": describe(modules, r2, names)})
                    entry["wrapper_callers"] = up
                entries.append(entry)
        report[row["name"]] = entries
    path = common.write_json("api_arguments.json", report)
    for name, entries in report.items():
        print(name)
        for e in entries:
            print("  ", e["site"], e["function"], e["args"])
            for up in e.get("wrapper_callers", [])[:6]:
                print("       <-", up["site"], up["args"])
    print(f"-> {path}")


if __name__ == "__main__":
    main()

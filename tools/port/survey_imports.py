"""Survey the PSP OS/library surface the game imports (the HLE surface a native
runtime must provide).

Reads the import tables of EBOOT.ELF and the disc PRX (LIBSUPPREACC.PRX), names
NIDs from the PPSSPP tables extracted by ppsspp_nids.py, groups them, and counts
direct call sites (jal / j to the stub) in main and every overlay.

Writes build/scratch/port/imports.json and prints a summary.
"""

import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

import common  # noqa: E402
from inspect_imports import inspect  # noqa: E402  (existing tool, read-only use)

GROUP_ORDER = [
    "kernel: threads", "kernel: semaphores", "kernel: event flags", "kernel: callbacks",
    "kernel: time", "kernel: interrupts", "kernel: memory", "kernel: cache", "kernel: modules",
    "kernel: process/exit", "kernel: stdio/debug", "kernel: libc helpers", "I/O", "controller",
    "display", "GE", "audio: sceAudio", "audio: sceSas", "audio: sceAtrac", "video: sceMpeg",
    "utility: savedata", "utility: message dialog", "utility: OSK", "utility: screenshot",
    "utility: modules", "utility: system params", "power", "net/adhoc", "UMD", "font",
    "DRM", "other",
]


def classify(library, name):
    n = name or ""
    if library == "ThreadManForUser":
        if "Sema" in n:
            return "kernel: semaphores"
        if "EventFlag" in n:
            return "kernel: event flags"
        if "Callback" in n and "Thread" not in n:
            return "kernel: callbacks"
        if "SystemTime" in n:
            return "kernel: time"
        return "kernel: threads"
    if library == "Kernel_Library":
        if "Intr" in n:
            return "kernel: interrupts"
        if "ThreadId" in n:
            return "kernel: threads"
        return "kernel: libc helpers"
    if library == "SysMemUserForUser":
        if "Printf" in n:
            return "kernel: stdio/debug"
        return "kernel: memory"
    if library == "UtilsForUser":
        if "cache" in n.lower():
            return "kernel: cache"
        if "GPI" in n:
            return "kernel: stdio/debug"
        return "kernel: time"
    if library == "sceSuspendForUser":
        return "kernel: memory" if "VolatileMem" in n else "power"
    if library == "ModuleMgrForUser":
        return "kernel: modules"
    if library == "LoadExecForUser":
        return "kernel: process/exit"
    if library == "StdioForUser":
        return "kernel: stdio/debug"
    if library == "sceDmac":
        return "kernel: memory"
    if library == "IoFileMgrForUser":
        return "I/O"
    if library == "sceCtrl":
        return "controller"
    if library == "sceDisplay":
        return "display"
    if library.startswith("sceGe"):
        return "GE"
    if library == "sceAudio":
        return "audio: sceAudio"
    if library == "sceSasCore":
        return "audio: sceSas"
    if library.lower().startswith("sceatrac"):
        return "audio: sceAtrac"
    if library.startswith("sceMpeg") or library.startswith("scePsmf"):
        return "video: sceMpeg"
    if library == "sceUtility":
        if "Savedata" in n:
            return "utility: savedata"
        if "MsgDialog" in n:
            return "utility: message dialog"
        if "Osk" in n:
            return "utility: OSK"
        if "Screenshot" in n:
            return "utility: screenshot"
        if "Module" in n:
            return "utility: modules"
        return "utility: system params"
    if library == "sceImpose":
        return "utility: system params"
    if library == "scePower":
        return "power"
    if library.startswith("sceNet") or library.startswith("sceWlan"):
        return "net/adhoc"
    if library.startswith("sceUmd"):
        return "UMD"
    if library == "sceLibFont":
        return "font"
    if "NpDrm" in library:
        return "DRM"
    return "other"


def resolve(tables, flat, library, nid):
    name = tables.get(library, {}).get(nid)
    if name:
        return name, "ppsspp"
    candidates = flat.get(nid)
    if candidates and len(candidates) == 1:
        return next(iter(candidates)), "ppsspp-other-table"
    if candidates:
        return sorted(candidates)[0], "ppsspp-ambiguous"
    return None, "unresolved"


def call_sites(modules, stubs):
    """Count direct jal/j into each stub address from every module's code."""
    counts = defaultdict(lambda: defaultdict(lambda: {"jal": 0, "j": 0, "sites": []}))
    for name, module in modules.items():
        for start, end in module.iter_ranges("instruction_ranges"):
            for offset, word in enumerate(module.words(start, end)):
                address = start + offset * 4
                if common.is_jal(word) or common.is_j(word):
                    target = common.jal_target(word, address)
                    if target in stubs:
                        entry = counts[target][name]
                        entry["jal" if common.is_jal(word) else "j"] += 1
                        if len(entry["sites"]) < 64:
                            entry["sites"].append(common.h(address))
    return counts


def main():
    tables, flat = common.nid_names()
    modules = common.load_all()
    report = {"binaries": {}, "groups": {}, "unresolved": []}
    stubs = {}
    rows = []
    main_imports = inspect(modules["main"].path)
    prx_imports = inspect(common.PRX)
    for label, data in (("main", main_imports), ("LIBSUPPREACC.PRX", prx_imports)):
        report["binaries"][label] = {
            "module": data["module"],
            "library_count": data["library_count"],
            "function_count": data["function_count"],
            "exports": [{"library": e["library"], "functions": [f["nid"] for f in e["functions"]]}
                        for e in data["export_libraries"]],
        }
        for library in data["libraries"]:
            for function in library["functions"]:
                name, source = resolve(tables, flat, library["library"], function["nid"])
                row = {"binary": label, "library": library["library"], "nid": function["nid"],
                       "name": name, "name_source": source, "stub": function["address"],
                       "group": classify(library["library"], name)}
                rows.append(row)
                if label == "main":
                    stubs[int(function["address"], 16)] = row
    counts = call_sites(modules, stubs)
    for address, row in stubs.items():
        per = counts.get(address, {})
        row["call_sites"] = {m: {"jal": v["jal"], "j": v["j"]} for m, v in per.items()}
        row["call_site_total"] = sum(v["jal"] + v["j"] for v in per.values())
        row["sites"] = {m: v["sites"] for m, v in per.items()}
    groups = defaultdict(lambda: {"functions": 0, "main": 0, "prx_only": 0, "call_sites": 0, "libraries": set()})
    main_keys = {(r["library"], r["nid"]) for r in rows if r["binary"] == "main"}
    for row in rows:
        g = groups[row["group"]]
        g["libraries"].add(row["library"])
        if row["binary"] == "main":
            g["functions"] += 1
            g["main"] += 1
            g["call_sites"] += row.get("call_site_total", 0)
        elif (row["library"], row["nid"]) not in main_keys:
            g["functions"] += 1
            g["prx_only"] += 1
        if row["name_source"] == "unresolved":
            report["unresolved"].append({k: row[k] for k in ("binary", "library", "nid")})
    report["groups"] = {k: {**groups[k], "libraries": sorted(groups[k]["libraries"])}
                        for k in GROUP_ORDER if k in groups}
    report["imports"] = rows
    unused = [r for r in rows if r["binary"] == "main" and r.get("call_site_total", 0) == 0]
    report["unused_main_imports"] = [f'{r["library"]}:{r["name"] or r["nid"]}' for r in unused]
    path = common.write_json("imports.json", report)

    distinct = len({(r["library"], r["nid"]) for r in rows})
    print(f"main: {main_imports['library_count']} libraries / {main_imports['function_count']} functions; "
          f"PRX: {prx_imports['library_count']} / {prx_imports['function_count']}; distinct {distinct}")
    print(f"unresolved NIDs: {len(report['unresolved'])}; main imports with no direct call site: {len(unused)}")
    for name, g in report["groups"].items():
        print(f"  {name:26s} {g['functions']:4d} fn  call sites {g['call_sites']:5d}  {', '.join(g['libraries'])}")
    lib_counts = Counter(r["library"] for r in rows if r["binary"] == "main")
    print("libraries:", dict(lib_counts))
    print(f"-> {path}")


if __name__ == "__main__":
    main()

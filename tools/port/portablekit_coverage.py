"""PortableKit coverage of what Dissidia 012 (ULUS10566) needs.

Reads a PortableKit checkout as data (nothing in it is built or run) and the
survey outputs in build/scratch/port/, and answers three questions:

  hle:      for each of the game's imports (EBOOT.ELF and LIBSUPPREACC.PRX),
            does PortableKit register a handler, and how far is it written?
  cpu:      for each opcode the instruction census counted, does PortableKit's
            decoder name it, does the interpreter execute it and does the
            recompiler lower it?
  boot:     checks of the ELFs against what PortableKit's loader assumes
            (entry point, module_start export, overlay identification).

Usage:
    .venv\\Scripts\\python.exe tools/port/portablekit_coverage.py tools/local/port/PortableKit

Writes build/scratch/port/portablekit_coverage.json. Output holds names,
addresses, NIDs, counts and PortableKit source line numbers; no game bytes and
no PortableKit code are copied. The HLE classification is a source-text
heuristic (see classify_handler); the authoritative list for a run is the
port's own <PREFIX>_LIST_STUBS=1 output.
"""

import argparse
import json
import re
import struct
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import common  # noqa: E402

OUT = common.OUT / "portablekit_coverage.json"

# Groups the gap analysis reports on, in the survey's group names.
FOCUS_GROUPS = [
    "kernel: threads", "kernel: semaphores", "kernel: event flags", "kernel: callbacks", "kernel: time",
    "kernel: interrupts", "kernel: memory", "kernel: cache", "kernel: modules", "kernel: process/exit",
    "kernel: stdio/debug", "kernel: libc helpers", "I/O", "display", "GE", "controller", "audio: sceAudio",
    "audio: sceSas", "audio: sceAtrac", "video: sceMpeg", "utility: savedata", "utility: message dialog",
    "utility: OSK", "utility: screenshot", "utility: modules", "utility: system params", "font", "UMD",
    "power", "DRM", "net/adhoc", "other",
]


def line_of(text, pos):
    return text.count("\n", 0, pos) + 1


# --------------------------------------------------------------------------
# HLE


def load_nids(root):
    by_nid, by_name = {}, {}
    for raw in (root / "configs/nids.csv").read_text(encoding="utf-8").splitlines():
        if not raw or raw.startswith("#") or raw.startswith("library,"):
            continue
        library, nid, name = raw.split(",")[:3]
        by_nid[(library, int(nid, 16))] = name
        by_name[(library, name)] = int(nid, 16)
    return by_nid, by_name


def matching_close(text, start, open_char="{", close_char="}"):
    """Index just past the bracket that closes the one at text[start]."""
    depth = 0
    i = start
    in_string = False
    while i < len(text):
        c = text[i]
        if in_string:
            if c == "\\":
                i += 1
            elif c == '"':
                in_string = False
        elif c == '"':
            in_string = True
        elif c == open_char:
            depth += 1
        elif c == close_char:
            depth -= 1
            if depth == 0:
                return i + 1
        i += 1
    return len(text)


TRIVIAL_HANDLER_NAMES = {"success", "succeed", "no_data"}
UNVERIFIED_MARKS = ("UNVERIFIED", "unverified(", "rtc_unverified(", "answering(")


def classify_handler(text, after):
    """Look at the handler argument that follows `hle.add(lib, name,` at `after`.

    Returns (kind, snippet_length): kind is 'unverified' when the handler logs
    UNVERIFIED (or is one of hle_net_offline's answering() stubs), 'constant'
    when it is a named no-op helper or a lambda whose body only finishes with a
    value, otherwise 'implemented'.
    """
    rest = text[after:after + 200].lstrip()
    ident = re.match(r"([A-Za-z_]\w*)\s*[,)]", rest)
    if ident and ident.group(1) in TRIVIAL_HANDLER_NAMES:
        return "constant", 0
    if rest.startswith("answering("):
        return "unverified", 0
    brace = text.find("{", after)
    lam = text.find("[", after)
    if lam == -1 or brace == -1 or lam > after + 400:
        # A handler built elsewhere (a factory or a variable).
        return "implemented", 0
    end = matching_close(text, brace)
    body = text[brace + 1:end - 1]
    if any(mark in body for mark in UNVERIFIED_MARKS):
        return "unverified", len(body)
    compact = re.sub(r"\s+", "", body)
    constant = r"(0x[0-9A-Fa-f]+|\d+)u?|(error::)?k[A-Z]\w*|static_cast<std::uint32_t>\((error::)?k[A-Z]\w*\)"
    if re.fullmatch(r"(kernel\(\)\.)?finish\(ctx,(" + constant + r")\);", compact) or compact == "":
        return "constant", len(body)
    return "implemented", len(body)


def parse_registrations(root):
    """(library, name) -> list of {file, line, kind, try_add}."""
    registrations = defaultdict(list)
    pattern = re.compile(r"hle\.(add|try_add)\(\s*\"([^\"]+)\"\s*,\s*(\"([^\"]+)\"|([A-Za-z_]\w*))\s*,")
    loop = re.compile(r"for\s*\(\s*const\s+char\s*\*\s*(\w+)\s*:\s*\{")
    for path in sorted((root / "host").rglob("*.cpp")):
        text = path.read_text(encoding="utf-8", errors="replace")
        loops = []
        for m in loop.finditer(text):
            brace = m.end() - 1
            end = matching_close(text, brace)
            loops.append((m.start(), m.group(1), re.findall(r"\"([^\"]+)\"", text[brace:end])))
        for m in pattern.finditer(text):
            library = m.group(2)
            if m.group(4) is not None:
                names = [m.group(4)]
            else:
                variable = m.group(5)
                candidates = [entry for entry in loops if entry[0] < m.start() and entry[1] == variable]
                names = candidates[-1][2] if candidates else []
            kind, _ = classify_handler(text, m.end())
            rel = path.relative_to(root).as_posix()
            for name in names:
                registrations[(library, name)].append(
                    {"file": rel, "line": line_of(text, m.start()), "kind": kind, "try_add": m.group(1) == "try_add"})
    return registrations


def hle_coverage(root, imports):
    by_nid, by_name = load_nids(root)
    registrations = parse_registrations(root)
    rows = []
    for item in imports:
        library = item["library"]
        nid = int(item["nid"], 16)
        pk_name = by_nid.get((library, nid))
        regs = registrations.get((library, pk_name), []) if pk_name else []
        if library == "scesupPreAcc":
            status = "missing: disc PRX export (not HLE)"
        elif pk_name is None:
            status = "missing: NID not in configs/nids.csv"
        elif not regs:
            status = "missing: no handler (logging stub at run time)"
        else:
            kinds = {r["kind"] for r in regs}
            if "unverified" in kinds:
                status = "implemented, UNVERIFIED"
            elif kinds == {"constant"}:
                status = "constant answer"
            else:
                status = "implemented"
        rows.append({
            "binary": item["binary"], "library": library, "nid": item["nid"],
            "name": item["name"], "portablekit_name": pk_name, "group": item["group"],
            "call_sites": item.get("call_site_total", 0), "status": status,
            "registered_at": [f'{r["file"]}:{r["line"]}' for r in regs],
        })
    registered_names = {k for k, v in registrations.items() if v}
    return rows, len(registered_names), len(by_nid)


def summarize_hle(rows):
    def bucket(status):
        if status.startswith("missing"):
            return "missing"
        return {"implemented": "implemented", "implemented, UNVERIFIED": "unverified",
                "constant answer": "constant"}[status]

    unique = {}
    for row in rows:
        key = (row["library"], row["nid"])
        if key not in unique:
            unique[key] = dict(row)
        else:
            unique[key]["call_sites"] = max(unique[key]["call_sites"], row["call_sites"])
            unique[key]["binary"] = "main+PRX"
    totals = Counter(bucket(r["status"]) for r in unique.values())
    by_group = defaultdict(Counter)
    by_library = defaultdict(Counter)
    for r in unique.values():
        by_group[r["group"]][bucket(r["status"])] += 1
        by_group[r["group"]]["functions"] += 1
        by_group[r["group"]]["call_sites"] += r["call_sites"]
        by_library[r["library"]][bucket(r["status"])] += 1
        by_library[r["library"]]["functions"] += 1
    gaps = sorted((r for r in unique.values() if not r["status"].startswith("implemented")),
                  key=lambda r: (-r["call_sites"], r["library"], r["name"]))
    return {
        "distinct_imports": len(unique),
        "totals": dict(totals),
        "by_group": {g: dict(by_group[g]) for g in FOCUS_GROUPS if g in by_group},
        "by_library": {k: dict(v) for k, v in sorted(by_library.items())},
        "gaps_by_call_sites": [
            {k: r[k] for k in ("library", "name", "nid", "group", "call_sites", "status", "binary")} for r in gaps],
    }


# --------------------------------------------------------------------------
# CPU

# rabbitizer spellings that PortableKit's decoder names differently, or
# pseudo-instructions that are another instruction underneath.
CENSUS_ALIASES = {
    "move": "or", "b": "beq", "beqz": "beq", "bnez": "bne", "negu": "subu", "not": "nor",
    "vuc2ifs": "vuc2i", "vnop": "vflush", "vtfm3": "vtfm", "vtfm4": "vtfm", "vhtfm4": "vhtfm",
    "vcrs": "vcrs.t", "viim": "viim.s", "vfim": "vfim.s", "vcmp": "vcmp",
    "c.lt.s": "c.cond.s", "c.le.s": "c.cond.s", "c.eq.s": "c.cond.s",
}
# Names PortableKit decodes under one kind with no literal of their own.
EXTRA_KIND_NAMES = {"vhtfm": "Vtfm", "vflush": "Vflush", "vnop": "Vflush"}
# Census names that keep their suffix in PortableKit's decoder.
KEEP_SUFFIX = {"lv.q", "sv.q", "lv.s", "sv.s", "add.s", "sub.s", "mul.s", "div.s", "sqrt.s", "abs.s", "mov.s",
               "neg.s", "round.w.s", "trunc.w.s", "ceil.w.s", "floor.w.s", "cvt.w.s", "cvt.s.w", "c.cond.s",
               "lvl.q", "lvr.q", "svl.q", "svr.q"}


def decoder_table(root):
    path = root / "src/decoder.cpp"
    text = path.read_text(encoding="utf-8")
    table = {}
    starts = [m for m in re.finditer(r"d\.kind\s*=\s*OpcodeKind::(\w+)\s*;", text)]
    for index, m in enumerate(starts):
        end = starts[index + 1].start() if index + 1 < len(starts) else len(text)
        chunk = text[m.end():end]
        # The literals of a chunk name its kind, up to the next case label.
        cut = re.search(r"\bcase\b|\bdefault\b|\} else", chunk)
        if cut:
            chunk = chunk[:cut.start()]
        for s in re.finditer(r"\"([^\"]+)\"", chunk):
            name = s.group(1)
            if m.group(1) in ("Vfpu", "Unsupported") or name.endswith("?"):
                continue
            table.setdefault(name, (m.group(1), line_of(text, m.start())))
    return table


def case_lines(path, patterns):
    text = path.read_text(encoding="utf-8")
    lines = {}
    for pattern in patterns:
        for m in re.finditer(pattern, text):
            lines.setdefault(m.group(1), line_of(text, m.start()))
    return text, lines


def unsupported_kinds(text, lines):
    """Kinds whose own case raises unsupported: always ('stops'), or only for
    some operand values, under an `if` ('partial')."""
    out = {}
    for kind, line in lines.items():
        body = "\n".join(text.splitlines()[line - 1:line + 3])
        if "unsupported(" in body and "lowered yet" not in body:
            out[kind] = "partial" if re.search(r"\bif\s*\(", body) else "stops"
    return out


CONTROL_FLOW = {"beq", "bne", "beql", "bnel", "blez", "bgtz", "blezl", "bgtzl", "bltz", "bgez", "bltzl", "bgezl",
                "bltzal", "bgezal", "bltzall", "bgezall", "j", "jal", "jr", "jalr", "bc1f", "bc1t", "bc1fl", "bc1tl",
                "bvf", "bvt", "bvfl", "bvtl", "b", "beqz", "bnez", "bal", "beqzl", "bnezl", "syscall"}


def control_flow_in_delay_slots():
    """Branch or jump words whose delay slot is itself control flow, inside
    function bodies: PortableKit stops on these in both execution paths."""
    sites = []
    for name in common.MODULES:
        module = common.Module(name)
        previous = None
        for address, word, _owner in module.iter_function_words():
            op = common.decode(word)
            if previous is not None and previous[0] == address - 4 and previous[1] in CONTROL_FLOW - {"syscall"} \
                    and op in CONTROL_FLOW:
                sites.append(f"{name}:0x{address:08X}")
            previous = (address, op)
    return sites


def fpu_control_registers():
    """Which FPU control registers the game's cfc1/ctc1 name (field rd)."""
    seen = defaultdict(Counter)
    for name in common.MODULES:
        module = common.Module(name)
        for _address, word, _owner in module.iter_function_words():
            if word >> 26 == 0x11 and common.rs(word) in (2, 6):
                seen["cfc1" if common.rs(word) == 2 else "ctc1"][common.rd(word)] += 1
    return {k: dict(v) for k, v in seen.items()}


def extra_names(root):
    text = (root / "src/extra_instructions.cpp").read_text(encoding="utf-8")
    m = re.search(r"\{\s*\"\"\s*,(.*?)\}", text, re.S)
    return set(re.findall(r"\"([^\"]+)\"", m.group(1))) if m else set()


def verified_by_game(root):
    text = (root / "docs/INSTRUCTION_COVERAGE.md").read_text(encoding="utf-8")
    section = text.split("## Verified by a game", 1)[1].split("##", 1)[0]
    return set(re.findall(r"^\| `([^`]+)`", section, re.M))


def pk_name_for(census_name):
    name = CENSUS_ALIASES.get(census_name, census_name)
    if name in KEEP_SUFFIX or name in EXTRA_KIND_NAMES or "." not in name:
        return name
    base = name.split(".")[0]
    return CENSUS_ALIASES.get(base, base)


def cpu_coverage(root, census):
    decoder = decoder_table(root)
    interp_text, interp = case_lines(root / "src/interpreter.cpp", [r"case\s+OpcodeKind::(\w+)\s*:"])
    codegen_text, codegen = case_lines(root / "tools/codegen_main.cpp",
                                       [r"case\s+psprecomp::OpcodeKind::(\w+)\s*:", r"case\s+K::(\w+)\s*:"])
    # emit_function_source() lowers jumps itself: J/JAL by name, JR/JALR in
    # its register-jump branch ("jump_target = ").
    emit = codegen_text.find("std::string emit_function_source")
    for kind, needle in (("J", "OpcodeKind::J ||"), ("Jal", "OpcodeKind::Jal)"), ("Jalr", "OpcodeKind::Jalr"),
                         ("Jr", "jump_target = ")):
        at = codegen_text.find(needle, emit)
        if at != -1 and kind not in codegen:
            codegen[kind] = line_of(codegen_text, at)
    stop_interp = unsupported_kinds(interp_text, interp)
    stop_codegen = unsupported_kinds(codegen_text, codegen)
    extras = extra_names(root)
    verified = verified_by_game(root)

    vfpu = census["all"]["vfpu_opcodes"]
    rows = []
    for name, count in sorted(census["all"]["all_opcodes"].items(), key=lambda kv: -kv[1]):
        if name == "nop":
            continue
        pk = pk_name_for(name)
        kind, dline = (EXTRA_KIND_NAMES[pk], None) if pk in EXTRA_KIND_NAMES else decoder.get(pk, (None, None))
        if pk in extras:
            kind = "Extra"
        if dline is None and kind is not None:
            dline = next((line for line_name, (k, line) in decoder.items() if k == kind), None)
        if kind is None:
            status_i = status_c = "not decoded"
        else:
            if kind == "Extra":
                status_i = status_c = "extra (UNVERIFIED)"
            else:
                status_i = stop_interp.get(kind, "yes") if kind in interp else "stops (no case)"
                status_c = stop_codegen.get(kind, "yes") if kind in codegen else "stops (not lowered)"
        rows.append({
            "census_name": name, "count": count, "vfpu": name in vfpu, "portablekit_name": pk, "kind": kind,
            "decoder_line": dline, "interpreter": status_i, "interpreter_line": interp.get(kind),
            "recompiler": status_c, "recompiler_line": codegen.get(kind),
            "verified_by_a_game": pk in verified or name in verified,
        })
    vrows = [r for r in rows if r["vfpu"]]
    summary = {
        "opcodes": len(rows),
        "vfpu_opcodes": len(vrows),
        "vfpu_instructions": sum(r["count"] for r in vrows),
        "vfpu_both_yes": sum(1 for r in vrows if r["interpreter"] == "yes" and r["recompiler"] == "yes"),
        "vfpu_extra_unverified": [r["census_name"] for r in vrows if r["kind"] == "Extra"],
        "vfpu_verified_by_a_game": [r["census_name"] for r in vrows if r["verified_by_a_game"]],
        "fpu_control_registers_used": fpu_control_registers(),
        "control_flow_in_delay_slots": control_flow_in_delay_slots(),
        "not_fully_supported": [
            {k: r[k] for k in ("census_name", "count", "kind", "interpreter", "recompiler")}
            for r in rows if r["interpreter"] != "yes" or r["recompiler"] != "yes"],
    }
    return rows, summary


# --------------------------------------------------------------------------
# Boot and overlay checks


def fnv1a64(data):
    value = 0xCBF29CE484222325
    for byte in data:
        value = ((value ^ byte) * 0x100000001B3) & 0xFFFFFFFFFFFFFFFF
    return value


def boot_checks():
    import inspect_imports  # tools/inspect_imports.py

    main_path = common.BUILD / "EBOOT.ELF"
    raw = main_path.read_bytes()
    e_type, _machine, _version, e_entry = struct.unpack_from("<HHII", raw, 16)
    info = inspect_imports.inspect(main_path)
    module_start = None
    for library in info["export_libraries"]:
        for function in library["functions"]:
            if function["nid"] == "0xD632ACDB":
                module_start = function["address"]
    checks = {
        "main_e_type": hex(e_type), "main_e_entry": f"0x{e_entry:08X}",
        "main_module_start_export": module_start,
        "note_entry": "PortableKit starts the loader thread at Elf32Image::runtime_entry() (e_entry for an "
                      "ET_EXEC); it does not read the module_start export.",
    }
    overlays = {}
    for name in ("menu", "battle", "extra"):
        path = common.BUILD / common.MODULES[name]
        data = path.read_bytes()
        e_type = struct.unpack_from("<H", data, 16)[0]
        phoff = struct.unpack_from("<I", data, 28)[0]
        _type, offset, vaddr, _paddr, filesz, memsz, _flags, _align = struct.unpack_from("<8I", data, phoff)
        image = data[offset:offset + filesz]
        tag = struct.unpack_from("<I", image, 0)[0]
        overlays[name] = {
            "e_type": hex(e_type), "base": f"0x{vaddr:08X}", "bytes": filesz, "memsz": memsz,
            "descriptor_tag": tag, "starts_with_MWo3": image[:4] == b"MWo3",
            "fnv1a64_first_64_bytes": f"0x{fnv1a64(image[:64]):016X}",
            "fnv1a64_first_4096_bytes": f"0x{fnv1a64(image[:4096]):016X}",
        }
    hashes = [o["fnv1a64_first_64_bytes"] for o in overlays.values()]
    checks["overlays"] = overlays
    checks["overlay_first_64_bytes_distinct"] = len(set(hashes)) == len(hashes)
    checks["note_overlays"] = ("host/overlays.cpp identifies a loaded image by slot base plus FNV-1a of its first "
                               "64 + code_size bytes; tools/add_overlay.py requires the 'MWo3' header, which these "
                               "overlays do not have, so meta.txt must be written by the profile's own tool.")
    return checks


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("portablekit", type=Path, help="PortableKit checkout (read as data only)")
    parser.add_argument("--output", type=Path, default=OUT)
    args = parser.parse_args()
    root = args.portablekit.resolve()
    if not (root / "src/decoder.cpp").is_file():
        raise SystemExit(f"{root} does not look like a PortableKit checkout")

    imports = json.loads((common.OUT / "imports.json").read_text(encoding="utf-8"))["imports"]
    census = json.loads((common.OUT / "instruction_census.json").read_text(encoding="utf-8"))

    hle_rows, registered, nid_count = hle_coverage(root, imports)
    hle_summary = summarize_hle(hle_rows)
    cpu_rows, cpu_summary = cpu_coverage(root, census)
    boot = boot_checks()

    head = (root / ".git/HEAD").read_text().strip() if (root / ".git/HEAD").is_file() else "?"
    report = {
        "portablekit": {"path": str(root), "head": head, "nids_in_table": nid_count,
                        "registered_hle_names": registered},
        "hle": {"summary": hle_summary, "imports": hle_rows},
        "cpu": {"summary": cpu_summary, "opcodes": cpu_rows},
        "boot": boot,
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=1) + "\n", encoding="utf-8")

    s = hle_summary
    print(f"HLE: {s['distinct_imports']} distinct imports: {s['totals']}")
    for group, counts in s["by_group"].items():
        print(f"  {group:28} {counts}")
    c = cpu_summary
    print(f"CPU: {c['opcodes']} opcodes; VFPU {c['vfpu_opcodes']} ({c['vfpu_instructions']} instructions), "
          f"{c['vfpu_both_yes']} in both interpreter and recompiler")
    for row in c["not_fully_supported"]:
        print(f"  {row}")
    print(f"Boot: e_entry {boot['main_e_entry']}, module_start {boot['main_module_start_export']}, "
          f"overlay heads distinct: {boot['overlay_first_64_bytes_distinct']}")
    print(f"Wrote {args.output}")


if __name__ == "__main__":
    main()

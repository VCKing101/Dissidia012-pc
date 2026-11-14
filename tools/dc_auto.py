"""Turn Ghidra pseudocode into candidate C and keep the exact matches.

  dc_auto.py MODULE [--max BYTES] [--min BYTES] [--limit N] [--write]

Each unmatched, unclaimed function's provisional pseudocode is rewritten into
plain C (Ghidra size types become C types, FUN_/DAT_ names become func_/D_
address names, callees get prototypes from their own pseudocode), compiled in
one batch with the module's pinned profile, and compared with retail bytes
exactly as `dc.py check` does. Signed and unsigned variants are tried.

With --write, matching sources are written to src/MODULE/auto/ for review and
integration with `dc.py integrate`. Nothing is integrated automatically.
"""

import argparse
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import dc  # noqa: E402
from compile_psp import compile_many  # noqa: E402
from mips_relocations import resolve_function  # noqa: E402

ROOT = dc.ROOT
REJECT = re.compile(r"\b(?:in_\w+|unaff_\w+|extraout_\w+|halt_baddata|CONCAT\d+|SUB\d+|ZEXT\d+|SEXT\d+|CARRY\d|SCARRY\d|SBORROW\d|"
                    r"POPCOUNT|LZCOUNT|trap|software_bkpt|syscall|switchD_\w+|caseD_\w+|joined_r0x\w+|__\w+|"
                    r"UNRECOVERED_JUMPTABLE\w*|uRam\w+|iRam\w+|cRam\w+|bRam\w+|sRam\w+|_\w+_\d+_\d+_)\b|\.\_|"
                    r"WARNING|\bfloat\d+\b|\bunkbyte\d+|\bunkuint\d+|\bunkint\d+|\bfloat10\b")
SIGNATURE = re.compile(r"^\s*(?P<ret>[\w\s\*]+?)\s*\b(?P<name>\w+)\s*\((?P<params>[^)]*)\)\s*$", re.M)
HARMLESS = re.compile(r"/\* WARNING: (?:Globals starting with '_' overlap smaller symbols|Removing unreachable block)[^*]*\*/\n?")
DATA_NAME = re.compile(r"\b_?(?:DAT|PTR_\w*?|s_\w*?|u_\w*?|LAB|switchdataD)_([0-9a-fA-F]{8})\b")
FLOAT_LITERAL = re.compile(r"(?<![\w.])(\d+\.\d+(?:[eE][-+]?\d+)?)(?![\w.])")


def type_map(signed, cpp=False):
    char = "char" if signed else "unsigned char"
    short = "short" if signed else "unsigned short"
    word = "int" if signed else "unsigned int"
    return [
        (r"\bundefined8\b", "long long" if signed else "unsigned long long"),
        (r"\bundefined4\b", word), (r"\bundefined2\b", short), (r"\bundefined1\b", char),
        (r"\bundefined\b", char), (r"\bulonglong\b", "unsigned long long"), (r"\blonglong\b", "long long"),
        (r"\buint\b", "unsigned int"), (r"\bushort\b", "unsigned short"), (r"\buchar\b", "unsigned char"),
        (r"\bbyte\b", "unsigned char"), (r"\bsbyte\b", "signed char"), (r"\bdword\b", "unsigned int"),
        (r"\bword\b", "unsigned short"), (r"\bulong\b", "unsigned long"), (r"\bpointer\b", "void *"),
    ] + ([] if cpp else [(r"\bbool\b", "unsigned char"), (r"\btrue\b", "1"), (r"\bfalse\b", "0")]) +([] if signed else [(r"(?<!unsigned )(?<!signed )\bchar\b", "unsigned char")])


class Pseudocode:
    def __init__(self, module):
        path = ROOT / f"analysis/ULUS10566/{module}/research/pseudocode.c"
        self.blocks = {}
        self.names = {}
        if not path.exists():
            return
        text = path.read_text(encoding="utf-8", errors="replace")
        parts = re.split(r"/\* @@ ([0-9a-f]{8}) size (\d+) \*/", text)
        for i in range(1, len(parts) - 2, 3):
            address = int(parts[i], 16)
            body = parts[i + 2].strip()
            self.blocks[address] = body
            match = SIGNATURE.search(body.split("{", 1)[0])
            if match:
                self.names[match.group("name")] = address

    def signature(self, address):
        body = self.blocks.get(address)
        if not body:
            return None
        head = body.split("{", 1)[0]
        match = SIGNATURE.search(head)
        return match


HOIST = re.compile(r"if \((?P<c>[^\n]*)\) \{\n(?P<i1>\s*)(?P<v>\w+) = (?P<a>[^;\n]+);\n\s*\}\n\s*else \{\n"
                   r"\s*(?P=v) = (?P<b>[^;\n]+);\n\s*\}\n\s*return (?P=v);")


def hoist_returns(code):
    """Ghidra's `v = a / v = b; return v;` becomes two returns, as SNC's line info needs."""
    def replace(m):
        indent = m["i1"]
        return (f"if ({m['c']}) {{\n{indent}return {m['a']};\n  }}\n"
                f"  else {{\n{indent}return {m['b']};\n  }}")
    changed = HOIST.sub(replace, code)
    if changed == code:
        return code
    for name in set(re.findall(r"^\s+[\w\s\*]+?\b(\w+);$", changed, re.M)):
        if len(re.findall(rf"\b{name}\b", changed)) == 1:
            changed = re.sub(rf"^\s+[\w\s\*]+?\b{name};\n", "", changed, flags=re.M)
    return changed


def int_bases(code):
    """Retype Ghidra's `char *param_N` struct bases as integers, as `*(T *)(p + K)` codegen needs."""
    head, sep, body = code.partition("{")
    params = re.findall(r"\b((?:unsigned |signed )?char) \*(param_\d+)\b", head)
    for ctype, name in params:
        head = re.sub(rf"\b{ctype} \*{name}\b", f"int {name}", head)
        body = re.sub(rf"(?<!\w)\*{name}\b", f"*({ctype} *){name}", body)
        body = re.sub(rf"\b{name}\[", f"(({ctype} *){name})[", body)
    return head + sep + body


def translate(module, address, sources, signed=True, loose=False, hoist=False, intbase=False, cpp=False):
    """Return C (or C++) text for the function, or None when the pseudocode is unusable."""
    own, main = sources[module], sources["main"]
    body = own.blocks.get(address)
    if body:
        body = HARMLESS.sub("", body)
    if not body or REJECT.search(body):
        return None
    if "(code" not in body and "code *" in body:
        return None
    known = {**main.names, **own.names}

    def rename_calls(text):
        text = re.sub(r"\b(?:FUN_|func_0x)([0-9a-fA-F]{8})\b", lambda m: f"func_{int(m.group(1), 16):08X}", text)
        for name, target in sorted(known.items(), key=lambda kv: -len(kv[0])):
            if not name.startswith(("FUN_", "func_")) and re.search(rf"\b{re.escape(name)}\b", text):
                text = re.sub(rf"\b{re.escape(name)}\b", f"func_{target:08X}", text)
        return text

    def fix_types(text):
        for pattern, replacement in type_map(signed, cpp):
            text = re.sub(pattern, replacement, text)
        return FLOAT_LITERAL.sub(lambda m: m.group(1) + "f", text)

    code = rename_calls(body)
    data_names = {}

    def data_ref(match):
        name = f"D_{int(match.group(1), 16):08X}"
        data_names[name] = match.group(0)
        return name

    code = DATA_NAME.sub(data_ref, code)
    if re.search(r"\b(?:LAB|DAT|PTR|FUN)_", code) or re.search(r"\b0x[0-9a-f]{7,8}\b(?![^(]*\*\))", code) and "*)0x" in code:
        return None
    code = fix_types(code)
    if hoist:
        code = hoist_returns(code)
    if intbase:
        code = int_bases(code)
    own_name = f"func_{address:08X}"
    callees = sorted(set(re.findall(r"\bfunc_([0-9A-F]{8})\b", code)) - {f"{address:08X}"})
    lines = []
    if "code" in re.findall(r"\b(\w+)\b", code):
        lines.append("typedef int code(...);" if cpp else "typedef int code();")
    unspecified = "..." if cpp else ""
    for callee in callees:
        target = int(callee, 16)
        source = own if target in own.blocks else main
        match = source.signature(target)
        if match is None:
            lines.append(f"extern int func_{callee}({unspecified});")
            continue
        ret = fix_types(match.group("ret").strip())
        params = fix_types(rename_calls(match.group("params")).strip())
        if loose or re.search(r"\b(?:in_|unaff_|code\b)", params) or "..." in params:
            params = unspecified
        if ret.startswith("code") or "code" in ret:
            ret = "int"
        if loose and ret == "void":
            ret = "int"  # Ghidra often declares a callee void and then uses its result
        lines.append(f"extern {ret} func_{callee}({params});")
    for name in sorted(data_names):
        if re.search(rf"&{name}\b", code) and re.search(rf"\(&{name}\)\[", code):
            lines.append(f"extern int {name}[];")
        elif re.search(rf"\*{name}\b", code):
            lines.append(f"extern int *{name};")
        else:
            lines.append(f"extern int {name};")
    if re.search(rf"\b{own_name}\b", code.split("{", 1)[1] if "{" in code else ""):
        pass  # recursion: the definition itself is the prototype
    text = "\n".join(lines + ["", code.strip(), ""])
    if cpp:
        text = 'extern "C" {\n' + text + "}\n"
    return text


def check_object(module, obj, address, base, payload, inventory_map, known):
    functions, other = dc.object_functions(obj)
    if other:
        return False, "data sections"
    target = [f for f in functions if f["name"] == f"func_{address:08X}"]
    if len(functions) != 1 or not target:
        return False, "function count"
    function = target[0]
    size = inventory_map[address]["body_bytes"]
    known = {**known, dc.c_rodata.TEXT: address - function["offset"]}  # jumps to labels inside the function
    names, missing = dc.resolve_names(function["relocations"], known)
    if missing:
        return False, "unresolved " + ",".join(missing)
    names[function["name"]] = address
    try:
        resolved = resolve_function({"name": function["name"], "bytes": function["bytes"],
                                     "relocations": function["relocations"]}, names)
    except ValueError as error:
        return False, str(error)
    expected = payload[address - base:address - base + size]
    if resolved == expected:
        return True, "match"
    differing = sum(1 for i in range(0, max(len(resolved), len(expected)), 4) if resolved[i:i + 4] != expected[i:i + 4])
    return False, f"{differing} words differ ({len(resolved)}/{size} bytes)"


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("module", choices=dc.FEATURES)
    parser.add_argument("--min", type=int, default=8)
    parser.add_argument("--max", type=int, default=96)
    parser.add_argument("--limit", type=int, default=100000)
    parser.add_argument("--batch", type=int, default=400)
    parser.add_argument("--write", action="store_true")
    parser.add_argument("--report", default=None)
    parser.add_argument("--start", type=lambda v: int(v, 16), default=0, help="lowest address (hex)")
    parser.add_argument("--end", type=lambda v: int(v, 16), default=0xFFFFFFFF, help="end address (hex, exclusive)")
    args = parser.parse_args()
    module = args.module
    sources = {m: Pseudocode(m) for m in {module, "main"}}
    base, payload = dc.module_image(module)
    inventory_map = dc.functions_by_address(module)
    taken = {int(f["address"], 16) for u in dc.matches(module)["units"] for f in u["functions"]}
    claimed = dc.claimed_addresses(module)
    existing_auto = {int(p.stem.split("_")[-1], 16) for p in (ROOT / f"src/{module}/auto").glob("*.c*")}
    known = dc.all_known_addresses(module)
    candidates = []
    for address, function in sorted(inventory_map.items()):
        size = function["body_bytes"]
        if function.get("matched_c") or function.get("thunk") or address in taken or address in claimed \
                or address in existing_auto or dc.excluded(module, address) or len(function["ranges"]) != 1 or not (args.min <= size <= args.max) \
                or not (args.start <= address < args.end):
            continue
        try:
            words = dc.words_at(base, payload, address, size)
        except SystemExit:
            continue
        calls, flags = dc.classify(words, address)
        if flags & {"jump-table-or-tailcall", "invalid", "vfpu"}:
            continue
        candidates.append(address)
    candidates = candidates[:args.limit]
    work = ROOT / f"build/dc-auto/{module}"
    (work / "src").mkdir(parents=True, exist_ok=True)
    matched = {}
    reasons = {}
    best = {}
    tried = {}
    config = dc.matches(module)
    untranslatable = sum(1 for a in candidates if translate(module, a, sources) is None)
    print(f"{len(candidates)} candidates; {untranslatable} rejected before compiling")
    # Each pass retries only what is still unmatched: signed types first,
    # then unsigned bytes/words, then unprototyped callees for call mismatches.
    # The C++ passes reproduce bool returns and C++ scoping that C cannot.
    passes = [(c + v + suffix, s, l, h, i, cpp)
              for c, cpp in (("", False), ("c", True))
              for suffix, h, i in (("", False, False), ("h", True, False), ("hi", True, True))
              for v, s, l in (("s", True, False), ("u", False, False), ("sl", True, True), ("ul", False, True))]
    for variant, signed, loose, hoist, intbase, cpp in passes:
        jobs = []
        for address in candidates:
            if address in matched:
                continue
            if loose and reasons.get(address) != "compile error":
                continue
            if cpp and "bool" not in sources[module].blocks.get(address, ""):
                continue
            if cpp and dc.range_profile(module, address):
                continue  # the GCC-built SDK libraries are C
            text = translate(module, address, sources, signed, loose, hoist, intbase, cpp)
            if text is None or text in tried.setdefault(address, set()):
                continue
            tried[address].add(text)
            path = work / "src" / f"{variant}_{address:08x}.{'cpp' if cpp else 'c'}"
            path.write_text(text, encoding="utf-8", newline="\n")
            jobs.append((address, path))
        # Each address compiles with the profile that built it (main's SDK
        # libraries: Sony psp-gcc); batches never mix profiles.
        jobs.sort(key=lambda job: dc.range_profile(module, job[0]) or "")
        batches = []
        for profile in sorted({dc.range_profile(module, a) or "" for a, _ in jobs}):
            group = [job for job in jobs if (dc.range_profile(module, job[0]) or "") == profile]
            batches += [(profile or None, group[i:i + args.batch]) for i in range(0, len(group), args.batch)]
        done = 0
        for profile, chunk in batches:
            pairs = [(path.relative_to(ROOT).as_posix(), (work / "obj" / (path.stem + ".o")).relative_to(ROOT).as_posix())
                     for _, path in chunk]
            compiler, flags = dc.profile_compiler(module, profile)
            results = compile_many(compiler, flags, pairs)
            done += len(chunk)
            for (address, path), result, (_, obj) in zip(chunk, results, pairs):
                if result.returncode:
                    reasons.setdefault(address, "compile error")
                    continue
                ok, reason = check_object(module, ROOT / obj, address, base, payload, inventory_map, known)
                if ok:
                    matched[address] = path
                    continue
                if reason.split()[0].isdigit() and (address not in best or int(reason.split()[0]) < best[address][0]):
                    best[address] = (int(reason.split()[0]), path.relative_to(ROOT).as_posix())
                if reasons.get(address) in (None, "compile error") or reason.split()[0].isdigit() and \
                        reasons[address].split()[0].isdigit() and int(reason.split()[0]) < int(reasons[address].split()[0]):
                    reasons[address] = reason
            print(f"  pass {variant}: {done}/{len(jobs)} compiled; matches so far {len(matched)}", flush=True)
    total_bytes = sum(inventory_map[a]["body_bytes"] for a in matched)
    print(f"MATCHED {len(matched)} functions / {total_bytes} bytes")
    if args.report:
        Path(args.report).write_text(json.dumps({"matched": {f"0x{a:08X}": str(p) for a, p in matched.items()},
                                                 "failed": {f"0x{a:08X}": r for a, r in reasons.items() if a not in matched},
                                                 "closest": {f"0x{a:08X}": p for a, (_, p) in best.items() if a not in matched}},
                                                indent=1), encoding="utf-8")
    if args.write:
        out = ROOT / f"src/{module}/auto"
        out.mkdir(parents=True, exist_ok=True)
        for address, path in sorted(matched.items()):
            profile = dc.range_profile(module, address)
            built = {"sdk_gcc": "Sony psp-gcc 1.3.1 -O2 -G0 (sdk_gcc profile)",
                     "sdk_gcc_o3": "Sony psp-gcc 1.3.1 -O3 -G0 (sdk_gcc_o3 profile)"}.get(profile, "SNC -O2 -g")
            header = (f"/* func_{address:08X}: generated from provisional Ghidra pseudocode by tools/dc_auto.py;\n"
                      f"   byte-identical with {built}. Names are placeholders. */\n")
            (out / f"{module}_{address:08x}{path.suffix}").write_text(header + path.read_text(), encoding="utf-8", newline="\n")
        print(f"wrote {len(matched)} sources to {out.relative_to(ROOT)}")


if __name__ == "__main__":
    main()

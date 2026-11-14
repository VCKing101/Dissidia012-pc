"""Fast per-function matching loop for every scoped module.

  dc.py candidates MODULE [--max N] [--min N] [--limit N] [--leaf] [--all]
  dc.py context MODULE ADDRESS          target assembly, Ghidra pseudocode, callers
  dc.py check MODULE SOURCE [...]       compile and compare every func_XXXXXXXX
  dc.py integrate MODULE SOURCE [...]   check, then add exact units to the build

`check` compiles with the module's pinned profile (SNC 1.2.7503.0 -O2 -g),
or with a named build profile from the module's match configuration: main's
SDK libraries (0x089985B0-0x089C2730) were built with Sony psp-gcc 1.3.1
-O2 -G0 (its SHA-1 and graphics libraries, 0x0899F3FC-0x089A2C24, with -O3),
so sources whose address (file name, else every func_ name in the file) lies
there use the "sdk_gcc" or "sdk_gcc_o3" profile. --profile NAME overrides that;
--profile default forces the module's pinned compiler. It resolves relocations against real addresses and compares whole functions with
retail bytes. Nothing is masked. Symbols named func_XXXXXXXX, D_XXXXXXXX or
jtbl_XXXXXXXX resolve to their hexadecimal address; other names come from the
module's match configuration. Switch jump tables (.rodata) are placed where the
retail HI16/LO16 pair at the same instructions points and compared too; see
tools/c_rodata.py. Table bytes are data and earn no C credit.

A full module build (tools/build_<module>.py) remains the final proof.
"""

import argparse
import atexit
import shutil
import difflib
import functools
import hashlib
import json
import os
import re
import struct
import sys
import tempfile
from pathlib import Path

import rabbitizer
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

sys.path.insert(0, str(Path(__file__).resolve().parent))
import c_rodata  # noqa: E402
from compile_psp import compile_object, prefetch, unit_compiler  # noqa: E402
from mips_relocations import configured_addresses, resolve_function  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
FEATURES = {"main": "main_helpers", "menu": "menu_helpers", "battle": "battle_helpers", "extra": "data_install"}
# Ranges whose bytes never count as C: main's SDK import stubs are patched by the loader.
EXCLUDED = {"main": [(0x089C5770, 0x089C60B0)]}
# Code built with another compiler than the module's pinned one. Main's SDK
# libraries are Sony psp-gcc output: their register moves are all addu, never
# SNC's or, from the first function at 0x089985B0 to the last before 0x089C2730.
# Inside them the SHA-1 and graphics libraries were built with -O3 (37 sources
# that differ under -O2 match under -O3); the first entry that holds an address wins.
PROFILE_RANGES = {"main": [(0x0899F3FC, 0x089A2C24, "sdk_gcc_o3"), (0x089985B0, 0x089C2730, "sdk_gcc")]}
PROFILE_FEATURES = {"sdk_gcc": "sdk_libraries", "sdk_gcc_o3": "sdk_libraries"}
FILE_ADDRESS = re.compile(r"_([0-9A-Fa-f]{8})$")
FUNC_TOKEN = re.compile(r"\bfunc_([0-9A-Fa-f]{8})\b")


def range_profile(module, address):
    return next((name for start, end, name in PROFILE_RANGES.get(module, ()) if start <= address < end), None)


@functools.lru_cache(maxsize=None)
def source_profile(module, source, override=None):
    """The build profile name for a source (None: the module's pinned compiler)."""
    if override:
        return None if override == "default" else override
    match = FILE_ADDRESS.search(Path(source).stem)
    if match:
        return range_profile(module, int(match.group(1), 16))
    try:
        text = Path(source).read_text(encoding="utf-8", errors="replace")
    except OSError:
        return None
    profiles = {range_profile(module, int(token, 16)) for token in FUNC_TOKEN.findall(text)}
    return profiles.pop() if len(profiles) == 1 else None


def profile_compiler(module, profile):
    config = matches(module)
    if profile and profile not in config.get("profiles", {}):
        raise SystemExit(f"{module} has no build profile {profile!r}")
    return unit_compiler(config, {"name": "<check>", **({"profile": profile} if profile else {})})
AUTO_NAME = re.compile(r"^(?:func|D|jtbl|sym)_([0-9A-Fa-f]{8})$")


@functools.lru_cache(maxsize=None)
def module_image(module):
    """Return (base address, payload bytes) for the module's loaded code."""
    if module == "extra":
        path, base, offset, size = "orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_EXTRA_APP.ELF", 0x089DD4C0, 0x54, 24312
        sha = json.loads((ROOT / "config/ulus10566.json").read_text())["extra_sha256"]
    else:
        spec = json.loads((ROOT / f"config/{module}.reference.json").read_text())
        path, sha = spec["path"], spec["sha256"]
        if module == "main":
            segment = spec["load_segments"][0]
            base, offset, size = int(segment["address"], 16), segment["file_offset"], segment["file_bytes"]
        else:
            base, offset, size = int(spec["payload_address"], 16), spec["payload_file_offset"], spec["payload_bytes"]
    data = (ROOT / path).read_bytes()
    if hashlib.sha256(data).hexdigest() != sha:
        raise SystemExit(f"{path} differs from the pinned reference")
    return base, data[offset:offset + size]


@functools.lru_cache(maxsize=None)
def inventory(module):
    return json.loads((ROOT / f"config/inventory/{module}.json").read_text())


@functools.lru_cache(maxsize=None)
def functions_by_address(module):
    return {int(f["address"], 16): f for f in inventory(module)["functions"]}


def matches(module):
    return json.loads((ROOT / f"config/{module}.matches.json").read_text())


# ---------------------------------------------------------------- disassembly

def _setup_rabbitizer():
    rabbitizer.config.pseudos_enablePseudos = True
    rabbitizer.config.regNames_gprAbiNames = rabbitizer.Abi.O32
    rabbitizer.config.misc_omit0XOnSmallImm = False


def disassemble(words, start, symbolize=True):
    """Disassemble words at `start`, naming call targets and hi/lo address pairs."""
    _setup_rabbitizer()
    lines = []
    hi = {}
    instructions = []
    for i, word in enumerate(words):
        instructions.append(rabbitizer.Instruction(word, start + 4 * i, rabbitizer.InstrCategory.R4000ALLEGREX))
    targets = set()
    for ins in instructions:
        if ins.isBranch():
            targets.add(ins.getBranchVramGeneric())
    for ins in instructions:
        address = ins.vram
        text = ins.disassemble()
        note = ""
        name = ins.getOpcodeName()
        if symbolize:
            if ins.isJumpWithAddress():
                target = ins.getInstrIndexAsVram()
                text = re.sub(r"0x[0-9A-Fa-f]+$", f"func_{target:08X}", text)
            elif ins.isBranch():
                target = ins.getBranchVramGeneric()
                mnemonic, _, operands = text.partition(" ")
                parts = [p.strip() for p in operands.strip().split(",")] if operands.strip() else []
                # The branch offset is always the final operand; it may itself
                # contain commas only in rabbitizer's ". + 4 + (x << 2)" form, which has none.
                kept = [p for p in parts if not (p.startswith(".") or p.startswith("0x") or p.lstrip("-").isdigit())]
                text = f"{mnemonic:<11} " + ", ".join(kept + [f".L{target:08X}"])
            if name == "lui":
                hi[ins.rt.value] = ins.getProcessedImmediate() << 16
            else:
                rs = ins.rs.value if ins.hasOperandAlias(rabbitizer.OperandType.cpu_rs) else None
                if rs in hi and rs != 0 and (name in ("addiu", "ori") or ins.doesLoad() or ins.doesStore()):
                    imm = ins.getProcessedImmediate()
                    value = (hi[rs] | imm) if name == "ori" else (hi[rs] + imm) & 0xFFFFFFFF
                    note = f"  # {value:08X}"
                try:
                    dest = ins.getDestinationGpr()
                except Exception:
                    dest = None
                if dest is not None and dest.value in hi and not (ins.doesStore()):
                    hi.pop(dest.value, None)
            if name in ("jr",) and ins.rs.value != 31:
                note += "  # register jump: jump table or tail call"
        label = f".L{address:08X}:\n" if address in targets else ""
        lines.append(f"{label}  {address:08X}: {word_of(ins):08X}  {text}{note}")
    return lines


def word_of(ins):
    return ins.getRaw()


def words_at(base, payload, address, size):
    offset = address - base
    if offset < 0 or offset + size > len(payload):
        raise SystemExit(f"0x{address:08X}+{size} is outside the module payload")
    return list(struct.unpack_from(f"<{size // 4}I", payload, offset))


# ---------------------------------------------------------------- candidates

def classify(words, start):
    _setup_rabbitizer()
    flags = set()
    calls = 0
    for i, word in enumerate(words):
        ins = rabbitizer.Instruction(word, start + 4 * i, rabbitizer.InstrCategory.R4000ALLEGREX)
        name = ins.getOpcodeName()
        if name == "jal":
            calls += 1
        if name == "jalr":
            calls += 1
            flags.add("indirect-call")
        if name == "jr" and ins.rs.value != 31:
            flags.add("jump-table-or-tailcall")
        if name.startswith("v") or name in ("lv.s", "sv.s", "lv.q", "sv.q", "mfv", "mtv", "mfvc", "mtvc"):
            flags.add("vfpu")
        if name.endswith(".s") and not name.startswith(("lv", "sv")) or name in ("lwc1", "swc1", "mtc1", "mfc1"):
            flags.add("fpu")
        if not ins.isValid():
            flags.add("invalid")
    return calls, flags


def cmd_candidates(args):
    base, payload = module_image(args.module)
    taken = set()
    for unit in matches(args.module)["units"]:
        for function in unit["functions"]:
            taken.add(int(function["address"], 16))
    claimed = claimed_addresses(args.module)
    rows = []
    for f in inventory(args.module)["functions"]:
        address = int(f["address"], 16)
        size = f["body_bytes"]
        if f.get("matched_c") or address in taken or f.get("thunk") or len(f["ranges"]) != 1:
            continue
        if (not args.all and address in claimed) or excluded(args.module, address):
            continue
        if not (args.min <= size <= args.max):
            continue
        try:
            words = words_at(base, payload, address, size)
        except SystemExit:
            continue
        calls, flags = classify(words, address)
        if args.leaf and calls:
            continue
        if not args.all and flags & {"jump-table-or-tailcall", "invalid", "vfpu"}:
            continue
        rows.append((size, address, calls, sorted(flags)))
    rows.sort(key=lambda r: (r[0], r[1]) if not args.by_address else (r[1],))
    for size, address, calls, flags in rows[:args.limit]:
        print(f"0x{address:08X} {size:5d} calls={calls} {' '.join(flags)}")
    print(f"# {len(rows)} candidates shown up to {args.limit}", file=sys.stderr)


def excluded(module, address):
    return any(start <= address < end for start, end in EXCLUDED.get(module, ()))


def claimed_addresses(module):
    """Addresses already handed to an agent (work/claims/<module>-*.txt)."""
    claimed = set()
    for path in (ROOT / "build/dc-claims").glob(f"{module}-*.txt"):
        for line in path.read_text().split():
            try:
                claimed.add(int(line, 16))
            except ValueError:
                pass
    return claimed


# ---------------------------------------------------------------- context

def pseudocode(module, address):
    path = ROOT / f"analysis/ULUS10566/{module}/research/pseudocode.c"
    if not path.exists():
        return None
    text = path.read_text(encoding="utf-8", errors="replace")
    marker = f"/* @@ {address:08x} "
    start = text.find(marker)
    if start < 0:
        return None
    end = text.find("/* @@ ", start + len(marker))
    return text[start:end if end > 0 else len(text)].rstrip()


def callers(module, address):
    path = ROOT / f"analysis/ULUS10566/{module}/research/refs.tsv"
    if not path.exists():
        return []
    want = f"{address:08x}"
    found = []
    with path.open(encoding="utf-8") as stream:
        next(stream, None)
        for line in stream:
            parts = line.rstrip("\n").split("\t")
            if len(parts) >= 4 and parts[1] == want:
                found.append((parts[0], parts[2], parts[3]))
    return found


def cmd_context(args):
    address = int(args.address, 16)
    base, payload = module_image(args.module)
    function = functions_by_address(args.module).get(address)
    if function is None:
        raise SystemExit(f"No inventory function at 0x{address:08X} in {args.module}")
    size = function["body_bytes"]
    print(f"== {args.module} func_{address:08X} size {size} (0x{size:X}) matched_c={function.get('matched_c')}")
    print(f"   ranges: {function['ranges']}  thunk={function.get('thunk')}")
    print(f"   direct calls: {', '.join('func_' + t.upper() for t in function.get('direct_call_targets', [])) or 'none'}")
    words = words_at(base, payload, address, size)
    calls, flags = classify(words, address)
    print(f"   calls={calls} flags={' '.join(sorted(flags)) or 'none'}")
    refs = callers(args.module, address)
    if refs:
        print(f"   referenced from ({len(refs)}): " + ", ".join(f"{frm} in {fn} [{kind}]" for frm, kind, fn in refs[:12]))
    print("\n-- target assembly")
    print("\n".join(disassemble(words, address)))
    code = pseudocode(args.module, address)
    print("\n-- Ghidra pseudocode (provisional, often wrong about types)")
    print(code or "(none)")
    for extra in args.neighbors and (function_neighbors(args.module, address)) or []:
        print(extra)


def function_neighbors(module, address):
    ordered = sorted(functions_by_address(module))
    index = ordered.index(address)
    out = []
    for other in ordered[max(0, index - 1):index] + ordered[index + 1:index + 2]:
        f = functions_by_address(module)[other]
        out.append(f"   neighbor func_{other:08X} size {f['body_bytes']} matched_c={f.get('matched_c')}")
    return out


# ---------------------------------------------------------------- check

def object_functions(path):
    """Return ordered function records with relocations and section offsets."""
    functions = []
    with Path(path).open("rb") as stream:
        elf = ELFFile(stream)
        symtab = elf.get_section_by_name(".symtab")
        text_index = None
        for index, section in enumerate(elf.iter_sections()):
            if section.name == ".text":
                text_index = index
        symbols = list(symtab.iter_symbols())
        for symbol in symbols:
            if symbol["st_info"]["type"] != "STT_FUNC" or not isinstance(symbol["st_shndx"], int):
                continue
            section = elf.get_section(symbol["st_shndx"])
            start, size = symbol["st_value"], symbol["st_size"]
            relocations = []
            for rel in elf.iter_sections():
                if not isinstance(rel, RelocationSection) or rel["sh_info"] != symbol["st_shndx"]:
                    continue
                table = elf.get_section(rel["sh_link"])
                for r in rel.iter_relocations():
                    if start <= r["r_offset"] < start + size:
                        target = table.get_symbol(r["r_info_sym"])
                        name = target.name
                        if not name and target["st_info"]["type"] == "STT_SECTION":
                            name = "<section " + elf.get_section(target["st_shndx"]).name + ">"
                        relocations.append({"offset": r["r_offset"] - start, "type": r["r_info_type"], "symbol": name})
            functions.append({"name": symbol.name, "section": section.name, "offset": start,
                              "bytes": section.data()[start:start + size], "relocations": relocations,
                              "binding": symbol["st_info"]["bind"]})
        other = [s.name for s in elf.iter_sections()
                 if s.name in (".rodata", ".data", ".sdata", ".bss", ".sbss") and s["sh_size"]]
    functions.sort(key=lambda f: (f["section"], f["offset"]))
    return functions, other


@functools.lru_cache(maxsize=None)
def all_known_addresses(module):
    addresses = {}
    for other in ("main", "extra", "menu", "battle"):
        try:
            config = matches(other)
        except FileNotFoundError:
            continue
        # Only main symbols are global to every module; overlay symbols
        # resolve within their own overlay namespace.
        if other in ("main", module):
            addresses.update(configured_addresses(config))
    return addresses


def resolve_names(relocations, known):
    addresses = {}
    missing = []
    for relocation in relocations:
        name = relocation["symbol"]
        if name in known:
            addresses[name] = known[name]
            continue
        match = AUTO_NAME.match(name)
        if match:
            addresses[name] = int(match.group(1), 16)
        else:
            missing.append(name)
    return addresses, sorted(set(missing))


def object_path(module, relative):
    """A per-process, per-path object file, so concurrent checks never share one."""
    base = ROOT / "build/dc" / module / str(os.getpid())
    if not base.exists():
        base.mkdir(parents=True, exist_ok=True)
        atexit.register(shutil.rmtree, base, True)
    tag = hashlib.sha1(Path(relative).as_posix().encode()).hexdigest()[:8]
    return base / f"{Path(relative).stem}-{tag}.o"


def compile_source(module, source, workdir, profile=None):
    source = Path(source).resolve()
    try:
        relative = source.relative_to(ROOT)
    except ValueError:
        raise SystemExit(f"{source} must be inside the repository so WSL can compile it")
    obj = object_path(module, relative)
    compiler, flags = profile_compiler(module, profile)
    process = compile_object(compiler, flags, relative.as_posix(), obj.relative_to(ROOT).as_posix())
    if process.returncode:
        sys.stdout.write(process.stdout + process.stderr)
        raise SystemExit(f"Compiler failed for {relative}")
    return obj, process


def expand_sources(sources):
    """Directories expand to their .c/.cpp files; @file reads one path per line."""
    out = []
    for source in sources:
        path = Path(source)
        if source.startswith("@"):
            out += [line.strip() for line in Path(source[1:]).read_text().splitlines() if line.strip()]
        elif path.is_dir():
            out += sorted(str(p) for p in path.iterdir() if p.suffix in (".c", ".cpp"))
        else:
            out.append(source)
    return out


def prefetch_sources(module, sources, profiles):
    """Batch-compile now, one batch per profile; `profiles` maps each source to its profile."""
    groups = {}
    for source in sources:
        relative = Path(source).resolve().relative_to(ROOT)
        job = (relative.as_posix(), object_path(module, relative).relative_to(ROOT).as_posix())
        groups.setdefault(profiles[source], []).append(job)
    for profile, jobs in groups.items():
        compiler, flags = profile_compiler(module, profile)
        prefetch(compiler, flags, jobs)


def check_source(module, source, verbose=True, show="diff", placement=None, profile=None):
    """Compile and compare; `placement` (a dict) receives the unit's "rodata" when it matches."""
    base, payload = module_image(module)
    inventory_map = functions_by_address(module)
    work = ROOT / "build/dc" / module
    work.mkdir(parents=True, exist_ok=True)
    obj, process = compile_source(module, source, work, profile)
    tag = f" ({profile})" if profile else ""
    # Sony's psp-gcc driver always warns that the SDK's link-time helpers
    # (pspspecs, pspfixup, psplibgen) are missing; they play no part in -S.
    warnings = "\n".join(line for line in (process.stdout + process.stderr).strip().splitlines()
                         if "pspspecs" not in line)
    functions, other_sections = object_functions(obj)
    known = all_known_addresses(module)
    rodata = place_rodata(obj, functions, inventory_map, base, payload)
    unit_known = {**known, **rodata["symbols"]} if rodata else known
    results = []
    for function in functions:
        match = AUTO_NAME.match(function["name"])
        if not match or not function["name"].startswith("func_"):
            if verbose:
                print(f"{function['name']}: skipped (not named func_XXXXXXXX)")
            continue
        address = int(match.group(1), 16)
        target = inventory_map.get(address)
        if target is None:
            print(f"{function['name']}: no inventory function at this address")
            results.append({"name": function["name"], "address": address, "match": False, "reason": "no inventory function"})
            continue
        size = target["body_bytes"]
        # A jump to a label inside the unit (GCC "j", asm "j 1f") is relative to its own .text.
        function_known = {**unit_known, c_rodata.TEXT: address - function["offset"]} if function["section"] == ".text" else unit_known
        names, missing = resolve_names(function["relocations"], function_known)
        names[function["name"]] = address
        record = {"name": function["name"], "address": address, "size": size,
                  "compiled_size": len(function["bytes"]), "section": function["section"],
                  "relocation_symbols": sorted({r["symbol"] for r in function["relocations"]})}
        if missing:
            record.update(match=False, reason="unresolved relocation symbols: " + ", ".join(missing))
            results.append(record)
            print(f"{function['name']}: UNRESOLVED {', '.join(missing)}"
                  + ("  (string literals/switch tables/static data go to their own section; use extern D_XXXXXXXX)"
                     if any(m.startswith("<section") for m in missing) else ""))
            continue
        resolved = resolve_function({"name": function["name"], "bytes": function["bytes"],
                                     "relocations": function["relocations"]}, names)
        expected = payload[address - base:address - base + size]
        diff_words = sum(1 for i in range(0, max(len(resolved), len(expected)), 4)
                         if resolved[i:i + 4] != expected[i:i + 4])
        record.update(match=resolved == expected, differing_words=diff_words)
        results.append(record)
        status = "MATCH" if record["match"] else f"{diff_words} differing words"
        print(f"{function['name']}: {len(resolved)}/{size} bytes; {status}  [{Path(source).name}]{tag}")
        if range_profile(module, address) != profile and verbose:
            print(f"  note: 0x{address:08X} belongs to profile {range_profile(module, address) or 'default'}, "
                  f"but this compile used {profile or 'default'}")
        if not record["match"] and verbose and show != "none":
            print_diff(resolved, expected, address, show)
    extra_text = [f["name"] for f in functions if f["section"] == ".text" and not AUTO_NAME.match(f["name"])]
    if extra_text:
        print(f"NOT LINKABLE: .text also holds {extra_text}; only func_XXXXXXXX may be emitted there "
              "(static/inline helpers get standalone copies; define helpers as member functions inside a C++ struct body)")
        for record in results:
            record["match"] = False
    else:
        offset = 0
        for function in [f for f in functions if f["section"] == ".text"]:
            if function["offset"] != offset:
                print("NOT LINKABLE: .text functions are not contiguous")
                for record in results:
                    record["match"] = False
                break
            offset += len(function["bytes"])
    if rodata is not None:
        placed = compare_rodata(rodata, known, base, payload, verbose and show != "none")
        if placed is None:
            for record in results:
                record["match"] = False
        else:
            other_sections = [name for name in other_sections if name != ".rodata"]
            if placement is not None:
                placement["rodata"] = placed
                placement["rodata_references"] = sorted({r["symbol"] for r in rodata["rodata"]["relocations"]
                                                         if r["symbol"] != c_rodata.TEXT})
    if other_sections:
        print(f"note: object also has data sections {other_sections}; only .text is linked, so any reference into them fails")
    if verbose and warnings and "warning" in warnings.lower():
        print("compiler: " + warnings[-800:])
    return results, functions


def place_rodata(obj, functions, inventory_map, base, payload):
    """Find where the object's .rodata (switch jump tables) sits in retail.

    Returns None without .rodata, else a state whose "symbols" resolve the
    tables' local names; the address comes from the retail HI16/LO16 pair at
    the same instructions and is proven by compare_rodata.
    """
    try:
        rodata = c_rodata.read_rodata(obj)
    except ValueError as error:
        return {"error": str(error), "symbols": {}}
    if rodata is None:
        return None
    text = []
    for function in functions:
        match = AUTO_NAME.match(function["name"])
        if function["section"] == ".text" and match and function["name"].startswith("func_"):
            address = int(match.group(1), 16)
            if address in inventory_map:
                text.append({"address": address, "offset": function["offset"], "bytes": function["bytes"],
                             "relocations": function["relocations"]})

    def retail_word(address):
        offset = address - base
        return struct.unpack_from("<I", payload, offset)[0] if 0 <= offset <= len(payload) - 4 else None

    starts = {t["address"] - t["offset"] for t in text}
    candidates = c_rodata.derive_address(rodata, text, retail_word)
    address = candidates[0] if candidates else None
    return {"rodata": rodata, "address": address, "candidates": candidates,
            "text_start": next(iter(starts)) if len(starts) == 1 else None,
            "symbols": c_rodata.symbol_addresses(rodata, address if address is not None else 0)}


def compare_rodata(state, known, base, payload, verbose=True):
    """Compare the placed .rodata with retail; return the unit placement or None."""
    if "error" in state:
        print(f"NOT LINKABLE: {state['error']}")
        return None
    rodata = state["rodata"]
    size = rodata["size"]
    label = f"rodata ({', '.join(sorted(rodata['symbols'])) or 'no symbols'}): {size} bytes"
    if state["address"] is None:
        print(f"{label}; NOT PLACED: no HI16/LO16 pair against it lines up with a retail instruction")
        return None
    if state["text_start"] is None:
        print(f"{label}; NOT PLACED: the .text functions do not share one start address")
        return None
    address = state["address"]
    if len(state["candidates"]) > 1:
        print(f"{label}; retail HI16/LO16 pairs disagree: "
              + ", ".join(f"0x{a:08X}" for a in state["candidates"]))
    offset = address - base
    if not 0 <= offset <= len(payload) - size:
        print(f"{label}; NOT PLACED: 0x{address:08X} is outside the module payload")
        return None
    targets, missing = resolve_names([r for r in rodata["relocations"] if r["symbol"] != c_rodata.TEXT], known)
    if missing:
        print(f"{label}; UNRESOLVED table entries: {', '.join(missing)}")
        return None
    resolved = c_rodata.resolve_rodata(rodata, {**targets, c_rodata.TEXT: state["text_start"]})
    expected = payload[offset:offset + size]
    differing = [i for i in range(0, size, 4) if resolved[i:i + 4] != expected[i:i + 4]]
    print(f"{label} at 0x{address:08X}; " + ("MATCH" if not differing else f"{len(differing)} differing words"))
    if differing and verbose:
        for i in differing[:12]:
            compiled_word = struct.unpack_from("<I", resolved, i)[0]
            retail_word = struct.unpack_from("<I", expected, i)[0]
            print(f" ! 0x{address + i:08X}: compiled {compiled_word:08X} | retail {retail_word:08X}")
    return None if differing else {"address": f"0x{address:08X}", "size": size}


def print_diff(compiled, expected, address, show):
    a = disassemble(list(struct.unpack(f"<{len(compiled) // 4}I", compiled)), address, symbolize=True)
    b = disassemble(list(struct.unpack(f"<{len(expected) // 4}I", expected)), address, symbolize=True)
    strip = lambda line: re.sub(r"^\s*[0-9A-F]{8}: [0-9A-F]{8}\s+", "", line.split("\n")[-1])
    sa, sb = [strip(x) for x in a], [strip(x) for x in b]
    width = 46
    print(f"   {'compiled':<{width}} | retail")
    matcher = difflib.SequenceMatcher(a=sa, b=sb, autojunk=False)
    for tag, i1, i2, j1, j2 in matcher.get_opcodes():
        if tag == "equal":
            for i, j in zip(range(i1, i2), range(j1, j2)):
                same_word = compiled[4 * i:4 * i + 4] == expected[4 * j:4 * j + 4]
                mark = " " if same_word else "r"  # r: same text, different encoding/reloc
                print(f" {mark} {sa[i][:width]:<{width}} | {sb[j]}")
        else:
            n = max(i2 - i1, j2 - j1)
            for k in range(n):
                left = sa[i1 + k] if i1 + k < i2 else ""
                right = sb[j1 + k] if j1 + k < j2 else ""
                print(f" ! {left[:width]:<{width}} | {right}")


def cmd_check(args):
    total = ok = 0
    args.sources = expand_sources(args.sources)
    profiles = {s: source_profile(args.module, s, args.profile) for s in args.sources}
    prefetch_sources(args.module, args.sources, profiles)
    for source in args.sources:
        results, _ = check_source(args.module, source, show=args.show, profile=profiles[source])
        total += len(results)
        ok += sum(1 for r in results if r.get("match"))
    print(f"== {ok}/{total} functions match")
    return 0 if total and ok == total else 1


# ---------------------------------------------------------------- integrate

class Skip(Exception):
    pass


def cmd_integrate(args):
    config_path = ROOT / f"config/{args.module}.matches.json"
    progress_path = ROOT / "config/progress.json"
    config = json.loads(config_path.read_text())
    progress = json.loads(progress_path.read_text())
    inventory_map = functions_by_address(args.module)
    existing = {}
    for unit in config["units"]:
        for function in unit["functions"]:
            existing[int(function["address"], 16)] = unit["name"]
    names = {unit["name"] for unit in config["units"]}
    added = []
    registered = {unit["source"] for unit in config["units"]}
    args.sources = [s for s in expand_sources(args.sources)
                    if Path(os.path.abspath(s)).relative_to(ROOT).as_posix() not in registered]
    profiles = {s: source_profile(args.module, s, args.profile) for s in args.sources}
    prefetch_sources(args.module, args.sources, profiles)
    skipped = []
    for source in args.sources:
        try:
            profile = profiles[source]
            source = Path(source).resolve()
            unit_name = source.stem
            if not re.fullmatch(r"[a-z_][a-z_0-9]*", unit_name):
                raise Skip(f"Unit name must be lower_snake_case: {unit_name}")
            if unit_name in names or unit_name in progress["matched_unit_features"]:
                raise Skip(f"Unit name already used: {unit_name}")
            placement = {}
            results, functions = check_source(args.module, source, show="none", placement=placement, profile=profile)
            if not results or not all(r.get("match") for r in results):
                raise Skip(f"{source.name}: not every func_ matches; not integrated")
            ordered = [f for f in functions if f["section"] == ".text"]
            inline = [f for f in functions if f["section"] == ".gnu.linkonce.t." + f["name"] and f["binding"] == "STB_WEAK"]
            if len(ordered) + len(inline) != len(functions) or len(ordered) != len(results):
                raise Skip(f"{source.name}: every function must be a matching func_XXXXXXXX in .text")
            specs = []
            expected_offset = 0
            start = results[0]["address"]
            for function, result in zip(ordered, results):
                if excluded(args.module, result["address"]):
                    raise Skip(f"{source.name}: 0x{result['address']:08X} is in an excluded range (import stubs)")
                if function["offset"] != expected_offset or result["address"] != start + expected_offset:
                    raise Skip(f"{source.name}: functions must be contiguous and in address order")
                if result["address"] in existing:
                    raise Skip(f"{source.name}: 0x{result['address']:08X} is already in unit {existing[result['address']]}")
                if range_profile(args.module, result["address"]) != profile:
                    raise Skip(f"{source.name}: 0x{result['address']:08X} was built with profile "
                               f"{range_profile(args.module, result['address']) or 'default'}, not {profile or 'default'}")
                specs.append({"name": function["name"], "address": f"0x{result['address']:08X}", "size": result["size"]})
                expected_offset += result["size"]
            rodata = placement.get("rodata")
            if rodata:
                if args.module == "extra":
                    raise Skip(f"{source.name}: the extra build cannot place .rodata (jump tables) yet")
                low = int(rodata["address"], 16)
                high = low + rodata["size"]
                code = [(int(r["start"], 16), int(r["end"], 16)) for r in inventory(args.module)["instruction_ranges"]]
                if any(a < high and low < z for a, z in code):
                    raise Skip(f"{source.name}: its .rodata at 0x{low:08X} overlaps inventory instruction ranges")
                for other in config["units"]:
                    if other.get("rodata"):
                        a = int(other["rodata"]["address"], 16)
                        if a < high and low < a + other["rodata"]["size"]:
                            raise Skip(f"{source.name}: its .rodata overlaps that of unit {other['name']}")
            symbols = config.setdefault("symbols", {})
            known = configured_addresses(config)
            references = [r["symbol"] for f in ordered for r in f["relocations"]] + placement.get("rodata_references", [])
            for name in references:
                match = AUTO_NAME.match(name)
                if name not in known and match:
                    symbols.setdefault(name, f"0x{int(match.group(1), 16):08X}")
            relative = source.relative_to(ROOT).as_posix()
            unit = {"name": unit_name, "source": relative, "functions": specs}
            if profile:
                unit["profile"] = profile
            if inline:
                unit["unused_inline_symbols"] = sorted(f["name"] for f in inline)
            if rodata:
                unit["rodata"] = rodata
            config["units"].append(unit)
            progress["matched_unit_features"][unit_name] = args.feature or PROFILE_FEATURES.get(profile) or FEATURES[args.module]
            names.add(unit_name)
            for spec in specs:
                existing[int(spec["address"], 16)] = unit_name
            added.append((unit_name, specs))
        except Skip as reason:
            skipped.append(str(reason))
            print(f"SKIPPED {reason}")
    config["units"].sort(key=lambda u: int(u["functions"][0]["address"], 16))
    config_path.write_text(json.dumps(config, indent=2) + "\n", encoding="utf-8", newline="\n")
    progress_path.write_text(json.dumps(progress, indent=2) + "\n", encoding="utf-8", newline="\n")
    for unit_name, specs in added:
        print(f"integrated {unit_name}: " + ", ".join(f"{s['name']} ({s['size']})" for s in specs))
        unit = next(u for u in config["units"] if u["name"] == unit_name)
        if unit.get("rodata"):
            print(f"  .rodata {unit['rodata']['size']} bytes at {unit['rodata']['address']} (data; no C credit)")
    print(f"{len(added)} units integrated, {len(skipped)} skipped. Run tools/build_{args.module}.py to prove the linked module.")


def cmd_sync_symbols(args):
    """Add address-named relocation targets of every integrated unit to the config.

    Needed after an integrated source is edited (for example a readability
    rewrite that names a callback func_X where the generated code said D_X).
    """
    config_path = ROOT / f"config/{args.module}.matches.json"
    config = json.loads(config_path.read_text())
    sources = [unit["source"] for unit in config["units"]]
    profiles = {unit["source"]: unit.get("profile") for unit in config["units"]}
    prefetch_sources(args.module, sources, profiles)
    known = configured_addresses(config)
    symbols = config.setdefault("symbols", {})
    added = {}
    for source in sources:
        obj, _ = compile_source(args.module, ROOT / source, ROOT / "build/dc" / args.module, profiles[source])
        functions, _ = object_functions(obj)
        for function in functions:
            for relocation in function["relocations"]:
                name = relocation["symbol"]
                match = AUTO_NAME.match(name)
                if match and name not in known and name not in symbols:
                    symbols[name] = added[name] = f"0x{int(match.group(1), 16):08X}"
    config_path.write_text(json.dumps(config, indent=2) + "\n", encoding="utf-8", newline="\n")
    print(f"{len(added)} symbols added: {', '.join(sorted(added)) or 'none'}")


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("candidates")
    p.add_argument("module", choices=FEATURES)
    p.add_argument("--min", type=int, default=8)
    p.add_argument("--max", type=int, default=64)
    p.add_argument("--limit", type=int, default=60)
    p.add_argument("--leaf", action="store_true", help="only functions without calls")
    p.add_argument("--all", action="store_true", help="include VFPU, jump tables and claimed functions")
    p.add_argument("--by-address", action="store_true")
    p.set_defaults(handler=cmd_candidates)
    p = sub.add_parser("context")
    p.add_argument("module", choices=FEATURES)
    p.add_argument("address")
    p.add_argument("--neighbors", action="store_true")
    p.set_defaults(handler=cmd_context)
    p = sub.add_parser("check")
    p.add_argument("module", choices=FEATURES)
    p.add_argument("sources", nargs="+")
    p.add_argument("--show", choices=("diff", "none"), default="diff")
    p.add_argument("--profile", help="build profile name, or 'default'; normally chosen by address")
    p.set_defaults(handler=cmd_check)
    p = sub.add_parser("integrate")
    p.add_argument("module", choices=FEATURES)
    p.add_argument("sources", nargs="+")
    p.add_argument("--feature")
    p.add_argument("--profile", help="build profile name, or 'default'; normally chosen by address")
    p.set_defaults(handler=cmd_integrate)
    p = sub.add_parser("sync-symbols")
    p.add_argument("module", choices=FEATURES)
    p.set_defaults(handler=cmd_sync_symbols)
    args = parser.parse_args()
    return args.handler(args) or 0


if __name__ == "__main__":
    sys.exit(main())

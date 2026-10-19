"""Reconstruct only the file-backed bytes of the main ELF's two load segments.

Inventory instructions become exact assembly; remaining loaded bytes stay
explicit opaque inputs. The runtime VMA gap and main BSS have no build bytes.
"""

import hashlib
import io
import json
import re
import struct
from pathlib import Path

import rabbitizer
from elftools.elf.elffile import ELFFile

from compile_psp import compile_object
from compiler_inline_sections import inspect_inline_object, validate_inline_manifest
from mips_relocations import configured_addresses, resolve_function
from probe_compilers import functions_from_object

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/ULUS10566"
REFERENCE = "analysis/ULUS10566/main/EBOOT.ELF"
REFERENCE_SHA256 = "0bc8e779aad2814a3d1bd77346f69ca5e16665845ed053cd0ad44a9cbfaefe6d"
IMPORT_START, IMPORT_END = 0x089C5770, 0x089C60B0


def load_segments(reference):
    """Return ELF PT_LOAD geometry; file/memory coordinates are separate."""
    elf = ELFFile(io.BytesIO(reference))
    segments = []
    for program_index, segment in enumerate(elf.iter_segments()):
        if segment["p_type"] != "PT_LOAD":
            continue
        segments.append({"index": program_index,
                         "address": f"0x{segment['p_vaddr']:08X}",
                         "file_offset": segment["p_offset"],
                         "file_bytes": segment["p_filesz"],
                         "memory_bytes": segment["p_memsz"]})
    return segments


def input_layout_sha256(inventory, segments):
    layout = {key: inventory[key] for key in ("module", "reference_sha256", "instruction_ranges")}
    layout["load_segments"] = segments
    return hashlib.sha256(json.dumps(layout, sort_keys=True, separators=(",", ":")).encode()).hexdigest()


def reference_bytes_at(reference, segments, address, size):
    """Map a wholly file-backed VMA range without silently crossing a gap."""
    if size <= 0:
        raise ValueError("Main byte range must be nonempty")
    for segment in segments:
        base = int(segment["address"], 16)
        if base <= address and address + size <= base + segment["file_bytes"]:
            offset = segment["file_offset"] + address - base
            return reference[offset:offset + size]
    raise ValueError("Main byte range crosses or exceeds a file-backed load segment")


def compile_units(matches, reference, segments):
    units = sorted(matches.get("units", []), key=lambda u: int(u["functions"][0]["address"], 16))
    if len({unit["name"] for unit in units}) != len(units):
        raise ValueError("Duplicate main C unit names would overwrite object evidence")
    previous_end = 0
    compiled = []
    forbidden_inline = validate_inline_manifest(matches)
    addresses = configured_addresses(matches)
    for unit in units:
        name = unit["name"]
        if not re.fullmatch(r"[a-z_][a-z_0-9]*", name):
            raise ValueError("Invalid main C unit name")
        specifications = unit["functions"]
        start = int(specifications[0]["address"], 16)
        end = start
        for function in specifications:
            if int(function["address"], 16) != end or function["size"] <= 0 or function["size"] % 4:
                raise ValueError("Each main C unit must cover a contiguous aligned range")
            end += function["size"]
        reference_bytes_at(reference, segments, start, end-start)
        if start < previous_end:
            raise ValueError("Main C units overlap")
        if start < IMPORT_END and IMPORT_START < end:
            raise ValueError("Main SDK import stubs cannot count as matching C")
        previous_end = end
        raw = Path(f"build/ULUS10566/main_{name}.raw.o")
        obj = Path(f"build/ULUS10566/main_{name}.o")
        process = compile_object(matches["compiler"], matches["flags"], unit["source"], raw)
        (BUILD / f"main_{name}.compile.log").write_text(process.stdout + process.stderr, encoding="utf-8")
        if process.returncode:
            raise RuntimeError(f"Main compilation failed: {name}")
        symbols = functions_from_object(ROOT / raw)
        inline_definitions = inspect_inline_object(ROOT / raw, unit, forbidden_inline)
        for specification in specifications:
            symbol = symbols[specification["name"]]
            address = int(specification["address"], 16)
            if symbol["address"] != address-start:
                raise ValueError("Main compiler changed C function order")
            if resolve_function(symbol, addresses) != reference_bytes_at(reference, segments, address, specification["size"]):
                raise ValueError(f"Main C symbol does not match: {specification['name']}")
        data = bytearray((ROOT / raw).read_bytes())
        flags = struct.unpack_from("<I", data, 36)[0]
        if flags & 0x00FF0000 != 0x00A20000:
            raise ValueError("Unexpected SNC machine identifier")
        struct.pack_into("<I", data, 36, (flags & ~0x00FF0000) | 0x00840000)
        (ROOT / obj).write_bytes(data)
        compiled.append({"start": start, "end": end, "object": obj.as_posix(), "unit": name, "discarded_inline_definitions": inline_definitions})
    return compiled


def instruction_text(word, address):
    instruction = rabbitizer.Instruction(word, address, rabbitizer.InstrCategory.R4000ALLEGREX)
    if not instruction.isValid() or not instruction.isImplemented():
        return f".word 0x{word:08X}", "rabbitizer-invalid-or-unimplemented"
    text = instruction.disassemble()
    # Rabbitizer names COP1 control registers even with numeric FPR settings;
    # Sony gas expects the numeric control register operand.
    if instruction.getOpcodeName() in ("cfc1", "ctc1"):
        text = text.rsplit(",", 1)[0] + f", ${word >> 11 & 31}"
    if instruction.getOpcodeName() in ("lwl", "lwr", "swl", "swr"):
        return f".word 0x{word:08X} # {text}", "legacy-gas-allegrex-unaligned-opcode"
    # This gas accepts destination prefixes but rejects every source-prefix
    # form tested, including the identity swizzle. It also predates vuc2ifs.
    # Record exact words explicitly; do not expand these to macros.
    if instruction.getOpcodeName() in ("vpfxs", "vpfxt"):
        return f".word 0x{word:08X} # {text}", "legacy-gas-vfpu-source-prefix"
    if instruction.getOpcodeName().startswith("vuc2ifs"):
        return f".word 0x{word:08X} # {text}", "legacy-gas-vuc2ifs-opcode"
    if instruction.getOpcodeName() == "vpfxd":
        opcode, operands = text.split(None, 1)
        text = f"{opcode} [{operands}]"
    if text.lstrip().startswith(".word"):
        return f".word 0x{word:08X}", "rabbitizer-data-emission"
    return text, None


def prepare():
    BUILD.mkdir(parents=True, exist_ok=True)
    reference_config_path = ROOT / "config/main.reference.json"
    spec = json.loads(reference_config_path.read_text())
    if spec["module"] != "main" or spec["path"] != REFERENCE or spec["sha256"] != REFERENCE_SHA256:
        raise ValueError("Main reference configuration differs from the verified baseline")
    reference = (ROOT / REFERENCE).read_bytes()
    if hashlib.sha256(reference).hexdigest() != REFERENCE_SHA256:
        raise ValueError("Main reference hash mismatch")
    segments = load_segments(reference)
    if spec["load_segments"] != segments:
        raise ValueError("Main PT_LOAD geometry differs from reference configuration")
    inventory = json.loads((ROOT / "config/inventory/main.json").read_text())
    if inventory["module"] != "main" or inventory["reference_sha256"] != REFERENCE_SHA256:
        raise ValueError("Main inventory identity mismatch")
    matches_path = ROOT / "config/main.matches.json"
    matches = json.loads(matches_path.read_text()) if matches_path.exists() else {"units": [], "symbols": {}}
    ranges = [(int(r["start"], 16), int(r["end"], 16)) for r in inventory["instruction_ranges"]]
    previous = 0
    for start, end in ranges:
        if start < previous or start >= end or start % 4 or end % 4:
            raise ValueError("Invalid main instruction ranges")
        reference_bytes_at(reference, segments, start, end-start)
        previous = end
    compiled = compile_units(matches, reference, segments)
    for unit in compiled:
        if not any(start <= unit["start"] and unit["end"] <= end for start, end in ranges):
            raise ValueError("Main C unit extends beyond an inventory instruction range")
    rabbitizer.config.pseudos_enablePseudos = False
    rabbitizer.config.regNames_gprAbiNames = rabbitizer.Abi.NUMERIC
    rabbitizer.config.regNames_fprAbiNames = rabbitizer.Abi.NUMERIC
    rabbitizer.config.toolchainTweaks_gnuMode = True
    lines = ["# Generated by tools/prepare_main.py; opaque ranges preserve original bytes.",
             ".set noreorder", ".set noat", ".set nomacro"]
    jumps, fallbacks, parts, segment_reports = {}, [], [], []
    assembly_bytes = opaque_bytes = c_bytes = 0
    for segment in segments:
        index = segment["index"]
        base = int(segment["address"], 16)
        limit = base + segment["file_bytes"]
        payload = reference[segment["file_offset"]:segment["file_offset"]+segment["file_bytes"]]
        (BUILD / f"main.original.segment{index}.bin").write_bytes(payload)
        selected_ranges = [(a, z) for a, z in ranges if base <= a < limit]
        selected_units = [u for u in compiled if base <= u["start"] < limit]
        boundaries = sorted({base, limit, *(v for r in selected_ranges for v in r), *(v for u in selected_units for v in (u["start"], u["end"]))})
        sections, segment_parts = [], []
        for chunk, (start, end) in enumerate(zip(boundaries, boundaries[1:])):
            part = {"segment_index": index, "start": f"0x{start:08X}", "end": f"0x{end:08X}"}
            unit = next((u for u in selected_units if u["start"] <= start and end <= u["end"]), None)
            if unit:
                if start != unit["start"] or end != unit["end"]:
                    raise ValueError("Main C unit has an internal inventory boundary")
                sections += [f'ASSERT(ABSOLUTE(.) == 0x{start:08X}, "main C start {unit["unit"]}");',
                             f'{unit["object"]}(.text)',
                             f'ASSERT(ABSOLUTE(.) == 0x{end:08X}, "main C end {unit["unit"]}");']
                c_bytes += end-start
                part.update(kind="matching-c", unit=unit["unit"])
            else:
                section = f".text.main.segment{index}.{chunk}"
                lines.append(f'\n.section {section}, "ax", @progbits')
                sections += [f'ASSERT(ABSOLUTE(.) == 0x{start:08X}, "main segment {index} chunk {chunk} start");',
                             f'build/ULUS10566/main.o({section})',
                             f'ASSERT(ABSOLUTE(.) == 0x{end:08X}, "main segment {index} chunk {chunk} end");']
                is_code = any(a <= start and end <= z for a, z in selected_ranges)
                if is_code:
                    for address in range(start, end, 4):
                        word = struct.unpack_from("<I", payload, address-base)[0]
                        text, fallback = instruction_text(word, address)
                        if fallback:
                            fallbacks.append({"address": f"0x{address:08X}", "word": f"0x{word:08X}", "reason": fallback})
                        for name in re.findall(r"\bfunc_([0-9A-Fa-f]+)\b", text):
                            jumps[f"func_{name}"] = int(name, 16)
                        lines.append(f"/* {address:08X} {word:08X} */ {text}")
                    assembly_bytes += end-start
                else:
                    lines.append(f'.incbin "build/ULUS10566/main.original.segment{index}.bin", {start-base}, {end-start}')
                    opaque_bytes += end-start
                part["kind"] = "inventory-instructions" if is_code else "opaque-original-bytes"
            segment_parts.append(part)
            parts.append(part)
        (BUILD / f"main.segment{index}.sections.ld").write_text("\n".join(sections)+"\n", encoding="utf-8", newline="\n")
        segment_reports.append({**segment, "parts": segment_parts})
    definitions = {**jumps, **{name: int(value, 16) for name, value in matches.get("symbols", {}).items()}}
    for name, address in definitions.items():
        if not re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", name) or not 0 <= address <= 0xFFFFFFFF:
            raise ValueError("Invalid main linker symbol")
    (BUILD / "main.remaining.s").write_text("\n".join(lines)+"\n", encoding="utf-8", newline="\n")
    (BUILD / "main.symbols.ld").write_text("\n".join(f"PROVIDE({name} = 0x{address:08X});" for name, address in sorted(definitions.items()))+"\n", encoding="utf-8", newline="\n")
    (BUILD / "main.objects.txt").write_text("\n".join(u["object"] for u in compiled)+"\n", encoding="utf-8", newline="\n")
    assembler = "tools/compilers/psp-gcc-1.3.1/bin/psp-as"
    report = {"schema_version": 1, "module": "main", "reference_sha256": REFERENCE_SHA256,
              "discarded_inline_definitions": [{"unit": u["unit"], **h} for u in sorted(compiled, key=lambda u: u["unit"]) for h in u["discarded_inline_definitions"]],
              "input_layout_sha256": input_layout_sha256(inventory, segments),
              "load_segments": segments, "segments": segment_reports,
              "payload_bytes": sum(s["file_bytes"] for s in segments),
              "reconstructed_load_bytes": sum(s["file_bytes"] for s in segments), "matching_c_bytes": c_bytes,
              "inventory_instruction_bytes": assembly_bytes+c_bytes,
              "assembly_instruction_bytes": assembly_bytes-4*len(fallbacks),
              "raw_instruction_fallback_bytes": 4*len(fallbacks), "opaque_original_bytes": opaque_bytes,
              "raw_instruction_fallbacks": fallbacks, "parts": parts, "assembler": assembler,
              "assembler_sha256": hashlib.sha256((ROOT / assembler).read_bytes()).hexdigest(),
              "reference_config_sha256": hashlib.sha256(reference_config_path.read_bytes()).hexdigest(),
              "scope": "File-backed PT_LOAD reconstruction; opaque bytes, BSS and original packaging earn no source credit."}
    (BUILD / "main.prepare.json").write_text(json.dumps(report, indent=2)+"\n", encoding="utf-8")
    return report


if __name__ == "__main__":
    report = prepare()
    print(json.dumps({key: value for key, value in report.items() if key not in ("parts", "segments", "raw_instruction_fallbacks")}))

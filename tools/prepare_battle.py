"""Prepare a fixed-address battle build, preserving unclassified bytes explicitly.

Only inventory instruction ranges are disassembled. The complement remains an
opaque original-byte input; neither that input nor assembly earns matching C.
"""

import hashlib
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
BASE = 0x089DD4C0
SIZE = 1596080
END = BASE + SIZE
OFFSET = 0x54
REFERENCE = "orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_BATTLE_APP.ELF"
REFERENCE_SHA256 = "1f96cfb3be771ff6e94741ae3916fab340df74e36ede07ca39545d015f4c49dc"
BUILD = ROOT / "build/ULUS10566"


def compile_units(matches, payload):
    """Compile and independently resolve each C symbol before linking."""
    units = sorted(matches.get("units", []), key=lambda u: int(u["functions"][0]["address"], 16))
    previous_end = BASE
    compiled = []
    forbidden_inline = validate_inline_manifest(matches)
    addresses = configured_addresses(matches)
    for unit in units:
        name = unit["name"]
        if not re.fullmatch(r"[a-z_][a-z_0-9]*", name):
            raise ValueError("Invalid battle C unit name")
        specifications = unit["functions"]
        start = int(specifications[0]["address"], 16)
        end = start
        for function in specifications:
            if int(function["address"], 16) != end or function["size"] <= 0 or function["size"] % 4:
                raise ValueError("Each C unit must cover a contiguous aligned range")
            end += function["size"]
        if not BASE <= start < end <= END or start < previous_end:
            raise ValueError("Battle C units overlap or exceed the loaded payload")
        previous_end = end
        raw = Path(f"build/ULUS10566/battle_{name}.raw.o")
        obj = Path(f"build/ULUS10566/battle_{name}.o")
        process = compile_object(matches["compiler"], matches["flags"], unit["source"], raw)
        (BUILD / f"battle_{name}.compile.log").write_text(process.stdout + process.stderr, encoding="utf-8")
        if process.returncode:
            raise RuntimeError(f"Battle compilation failed: {name}")
        symbols = functions_from_object(ROOT / raw)
        inline_definitions = inspect_inline_object(ROOT / raw, unit, forbidden_inline)
        for specification in specifications:
            symbol = symbols[specification["name"]]
            address = int(specification["address"], 16)
            if symbol["address"] != address - start:
                raise ValueError("Battle compiler changed C function order")
            if resolve_function(symbol, addresses) != payload[address - BASE:address - BASE + specification["size"]]:
                raise ValueError(f"Battle C symbol does not match: {specification['name']}")
        data = bytearray((ROOT / raw).read_bytes())
        flags = struct.unpack_from("<I", data, 36)[0]
        if flags & 0x00FF0000 != 0x00A20000:
            raise ValueError("Unexpected SNC machine identifier")
        struct.pack_into("<I", data, 36, (flags & ~0x00FF0000) | 0x00840000)
        (ROOT / obj).write_bytes(data)
        compiled.append({"start": start, "end": end, "object": obj.as_posix(), "unit": name, "discarded_inline_definitions": inline_definitions})
    return compiled


def instruction_text(word, address):
    """Use exact instruction forms; never enable instruction-expanding macros."""
    instruction = rabbitizer.Instruction(word, address, rabbitizer.InstrCategory.R4000ALLEGREX)
    if not instruction.isValid() or not instruction.isImplemented():
        return f".word 0x{word:08X}", "rabbitizer-invalid-or-unimplemented"
    text = instruction.disassemble()
    # Historical Sony gas accepts VFPU prefix lists in brackets. Numeric GPRs
    # below also avoid its ABI-register parser incompatibility with modern gas.
    if instruction.getOpcodeName() in ("vpfxs", "vpfxt", "vpfxd"):
        opcode, operands = text.split(None, 1)
        if opcode in ("vpfxs", "vpfxt"):
            operands = re.sub(r"[XYZW]", lambda match: match.group().lower(), operands)
            if "0" in operands.split(", "):
                return f".word 0x{word:08X} # {text}", "legacy-gas-vfpu-source-prefix-zero"
        text = f"{opcode} [{operands}]"
    if text.lstrip().startswith(".word"):
        return f".word 0x{word:08X}", "rabbitizer-data-emission"
    return text, None


def prepare():
    BUILD.mkdir(parents=True, exist_ok=True)
    reference_config_path = ROOT / "config/battle.reference.json"
    reference_config = json.loads(reference_config_path.read_text())
    if (reference_config["module"] != "battle" or reference_config["path"] != REFERENCE
            or reference_config["sha256"] != REFERENCE_SHA256
            or int(reference_config["payload_address"], 16) != BASE
            or reference_config["payload_file_offset"] != OFFSET
            or reference_config["payload_bytes"] != SIZE):
        raise ValueError("Battle reference configuration differs from the verified baseline")
    reference = (ROOT / REFERENCE).read_bytes()
    if hashlib.sha256(reference).hexdigest() != REFERENCE_SHA256:
        raise ValueError("Battle reference hash mismatch")
    payload = reference[OFFSET:OFFSET + SIZE]
    inventory = json.loads((ROOT / "config/inventory/battle.json").read_text())
    if inventory["module"] != "battle" or inventory["reference_sha256"] != REFERENCE_SHA256:
        raise ValueError("Battle inventory identity mismatch")
    matches_path = ROOT / "config/battle.matches.json"
    matches = json.loads(matches_path.read_text()) if matches_path.exists() else {"units": [], "symbols": {}}
    compiled = compile_units(matches, payload)
    ranges = [(int(r["start"], 16), int(r["end"], 16)) for r in inventory["instruction_ranges"]]
    previous = BASE
    for start, end in ranges:
        if start < previous or not BASE <= start < end <= END or start % 4 or end % 4:
            raise ValueError("Invalid battle instruction ranges")
        previous = end
    for unit in compiled:
        if not any(start <= unit["start"] and unit["end"] <= end for start, end in ranges):
            raise ValueError("Battle C unit extends beyond an inventory instruction range")
    (BUILD / "battle.original.payload.bin").write_bytes(payload)
    rabbitizer.config.pseudos_enablePseudos = False
    rabbitizer.config.regNames_gprAbiNames = rabbitizer.Abi.NUMERIC
    rabbitizer.config.regNames_fprAbiNames = rabbitizer.Abi.NUMERIC
    rabbitizer.config.toolchainTweaks_gnuMode = True
    boundaries = sorted({BASE, END, *(v for r in ranges for v in r), *(v for u in compiled for v in (u["start"], u["end"]))})
    lines = ["# Generated by tools/prepare_battle.py. Opaque ranges preserve original bytes.",
             ".set noreorder", ".set noat", ".set nomacro"]
    sections = []
    jumps = {}
    fallbacks = []
    assembly_bytes = opaque_bytes = c_bytes = 0
    parts = []
    for index, (start, end) in enumerate(zip(boundaries, boundaries[1:])):
        unit = next((u for u in compiled if u["start"] <= start and end <= u["end"]), None)
        if unit:
            if start != unit["start"] or end != unit["end"]:
                raise ValueError("C unit has an internal inventory boundary")
            sections.append(f'ASSERT(ABSOLUTE(.) == 0x{start:08X}, "battle C start {unit["unit"]}");')
            sections.append(f'{unit["object"]}(.text)')
            sections.append(f'ASSERT(ABSOLUTE(.) == 0x{end:08X}, "battle C end {unit["unit"]}");')
            c_bytes += end - start
            parts.append({"start": f"0x{start:08X}", "end": f"0x{end:08X}", "kind": "matching-c", "unit": unit["unit"]})
            continue
        section = f".text.battle.{index}"
        lines.append(f'\n.section {section}, "ax", @progbits')
        sections += [f'ASSERT(ABSOLUTE(.) == 0x{start:08X}, "battle chunk {index} start");', f'build/ULUS10566/battle.o({section})',
                     f'ASSERT(ABSOLUTE(.) == 0x{end:08X}, "battle chunk {index} end");']
        is_code = any(a <= start and end <= b for a, b in ranges)
        if not is_code:
            lines.append(f'.incbin "build/ULUS10566/battle.original.payload.bin", {start - BASE}, {end - start}')
            opaque_bytes += end - start
        else:
            for address in range(start, end, 4):
                word = struct.unpack_from("<I", payload, address - BASE)[0]
                text, fallback = instruction_text(word, address)
                if fallback:
                    fallbacks.append({"address": f"0x{address:08X}", "word": f"0x{word:08X}", "reason": fallback})
                for name in re.findall(r"\bfunc_([0-9A-Fa-f]+)\b", text):
                    jumps[f"func_{name}"] = int(name, 16)
                lines.append(f"/* {address:08X} {word:08X} */ {text}")
            assembly_bytes += end - start
        parts.append({"start": f"0x{start:08X}", "end": f"0x{end:08X}", "kind": "inventory-instructions" if is_code else "opaque-original-bytes"})
    definitions = {**jumps, **{name: int(value, 16) for name, value in matches.get("symbols", {}).items()}}
    for name, address in definitions.items():
        if not re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", name) or not 0 <= address <= 0xFFFFFFFF:
            raise ValueError("Invalid battle linker symbol")
    (BUILD / "battle.remaining.s").write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")
    (BUILD / "battle.sections.ld").write_text("\n".join(sections) + "\n", encoding="utf-8", newline="\n")
    (BUILD / "battle.symbols.ld").write_text("\n".join(f"PROVIDE({name} = 0x{address:08X});" for name, address in sorted(definitions.items())) + "\n", encoding="utf-8", newline="\n")
    (BUILD / "battle.objects.txt").write_text("\n".join(u["object"] for u in compiled) + "\n", encoding="utf-8", newline="\n")
    report = {"schema_version": 1, "module": "battle", "reference_sha256": REFERENCE_SHA256,
              "discarded_inline_definitions": [{"unit": u["unit"], **h} for u in sorted(compiled, key=lambda u: u["unit"]) for h in u["discarded_inline_definitions"]],
              "input_layout_sha256": hashlib.sha256(json.dumps({"module": inventory["module"], "reference_sha256": inventory["reference_sha256"], "instruction_ranges": inventory["instruction_ranges"]}, sort_keys=True, separators=(",", ":")).encode()).hexdigest(),
              "payload_bytes": SIZE, "matching_c_bytes": c_bytes, "inventory_instruction_bytes": assembly_bytes + c_bytes,
              "assembly_instruction_bytes": assembly_bytes - len(fallbacks) * 4,
              "raw_instruction_fallback_bytes": len(fallbacks) * 4, "opaque_original_bytes": opaque_bytes,
              "raw_instruction_fallbacks": fallbacks, "parts": parts,
              "assembler": "tools/compilers/psp-gcc-1.3.1/bin/psp-as",
              "assembler_sha256": hashlib.sha256((ROOT / "tools/compilers/psp-gcc-1.3.1/bin/psp-as").read_bytes()).hexdigest(),
              "reference_config_sha256": hashlib.sha256(reference_config_path.read_bytes()).hexdigest(),
              "scope": "Static reconstruction; opaque original bytes earn no source or data reconstruction credit."}
    (BUILD / "battle.prepare.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    return report


if __name__ == "__main__":
    report = prepare()
    print(json.dumps({key: value for key, value in report.items() if key not in ("parts", "raw_instruction_fallbacks")}))

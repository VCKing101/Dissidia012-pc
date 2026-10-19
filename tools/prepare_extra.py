"""Compile matched C units and generate assembly/link sections around them."""

import json
import re
import struct
from pathlib import Path

from elftools.elf.elffile import ELFFile

from compile_psp import compile_object
from probe_compilers import functions_from_object
from mips_relocations import configured_addresses, resolve_function

ROOT = Path(__file__).resolve().parents[1]


def validate_c_functions(object_path, reference, specifications, addresses=None):
    symbols = functions_from_object(object_path)
    for specification in specifications:
        symbol = symbols[specification["name"]]
        offset = int(specification["address"], 16) - 0x089DD4C0 + 0x54
        if resolve_function(symbol, addresses or {}) != reference[offset:offset + specification["size"]]:
            raise ValueError(f"C function does not match retail: {specification['name']}")


def prepare():
    matches = json.loads((ROOT / "config/extra.matches.json").read_text())
    build = ROOT / "build/ULUS10566"
    reference = (ROOT / "orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_EXTRA_APP.ELF").read_bytes()
    assembly = (ROOT / "asm/ULUS10566/extra/ovl_extra_app.s").read_text()
    units = sorted(matches["units"], key=lambda unit: int(unit["functions"][0]["address"], 16))
    remaining = []
    sections = []
    objects = []
    cursor = 0
    previous_end = 0x089DD4C0
    for index, unit in enumerate(units):
        name = unit["name"]
        if not re.fullmatch(r"[a-z_]+", name):
            raise ValueError("Invalid C unit name")
        functions = unit["functions"]
        first_address = int(functions[0]["address"], 16)
        last_end = first_address
        for function in functions:
            if int(function["address"], 16) != last_end:
                raise ValueError("Each C unit must cover a contiguous function range")
            last_end += function["size"]
        if first_address < previous_end:
            raise ValueError("C units overlap")
        previous_end = last_end
        raw_path = Path(f"build/ULUS10566/{name}.raw.o")
        object_path = Path(f"build/ULUS10566/{name}.o")
        process = compile_object(matches["compiler"], matches["flags"], unit["source"], raw_path)
        (build / f"{name}.compile.log").write_text(process.stdout + process.stderr, encoding="utf-8")
        if process.returncode:
            raise RuntimeError(f"C compilation failed; see {name}.compile.log")
        validate_c_functions(ROOT / raw_path, reference, functions, configured_addresses(matches))
        symbols = functions_from_object(ROOT / raw_path)
        with (ROOT / raw_path).open("rb") as stream:
            text_size = ELFFile(stream).get_section_by_name(".text")["sh_size"]
        if text_size != last_end - first_address:
            raise ValueError("C unit includes unexpected text or padding")
        for function in functions:
            if symbols[function["name"]]["address"] != int(function["address"], 16) - first_address:
                raise ValueError("Compiler changed C function ordering")
        # Preserve raw compiler output. Normalize only SNC's old PSP machine
        # identifier in a separate link copy; modern ld treats it as Loongson.
        object_data = bytearray((ROOT / raw_path).read_bytes())
        flags = struct.unpack_from("<I", object_data, 36)[0]
        if flags & 0x00FF0000 != 0x00A20000:
            raise ValueError("Unexpected SNC machine flags; normalization needs review")
        struct.pack_into("<I", object_data, 36, (flags & ~0x00FF0000) | 0x00840000)
        (ROOT / object_path).write_bytes(object_data)
        objects.append(object_path.as_posix())
        start = assembly.index("glabel " + functions[0]["name"] + "\n", cursor)
        marker = "endlabel " + functions[-1]["name"]
        end = assembly.index(marker, start) + len(marker)
        chunk = assembly[cursor:start]
        section_name = f".text.asm.{index}"
        if index == 0:
            chunk = chunk.replace('.section .text, "ax"', f'.section {section_name}, "ax"', 1)
        else:
            chunk = f'\n.section {section_name}, "ax"\n' + chunk
        remaining.append(chunk)
        sections.extend([f"build/ULUS10566/extra.o({section_name})", f"{object_path.as_posix()}(.text)"])
        cursor = end
    tail_section = f".text.asm.{len(units)}"
    remaining.append(f'\n.section {tail_section}, "ax"\n' + assembly[cursor:])
    sections.append(f"build/ULUS10566/extra.o({tail_section})")
    (build / "extra.remaining.s").write_text("".join(remaining), encoding="utf-8", newline="\n")
    (build / "extra.sections.ld").write_text("\n".join(sections) + "\n", encoding="utf-8", newline="\n")
    (build / "extra.objects.txt").write_text("\n".join(objects) + "\n", encoding="utf-8", newline="\n")
    # C can expose references that splat did not name in the remaining assembly.
    # PROVIDE leaves existing overlay labels and splat definitions authoritative.
    definitions = []
    for name, value in matches.get("symbols", {}).items():
        if not re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", name):
            raise ValueError("Invalid linker symbol name")
        address = int(value, 16)
        if not 0 <= address <= 0xFFFFFFFF:
            raise ValueError("Linker symbol is outside the PSP address space")
        definitions.append(f"PROVIDE({name} = 0x{address:08X});")
    (build / "extra.c_symbols.ld").write_text("\n".join(definitions) + "\n", encoding="utf-8", newline="\n")


if __name__ == "__main__":
    prepare()

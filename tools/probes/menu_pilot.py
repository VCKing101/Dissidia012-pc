"""Independently compare menu pilot C under the pinned compiler profile."""

import hashlib
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from compile_psp import compile_object
from mips_relocations import configured_addresses, resolve_function
from probe_compilers import functions_from_object
from elftools.elf.elffile import ELFFile


def main():
    matches = json.loads((ROOT / "config/menu.matches.json").read_text())
    reference = (ROOT / "orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_MENU_APP.ELF").read_bytes()
    if hashlib.sha256(reference).hexdigest() != "f021f96fa05da5907563a28c232966aa8a344526d6b8bdee728053ec75224d0a":
        raise ValueError("Menu reference hash mismatch")
    output = ROOT / "analysis/menu-c-pilot"
    output.mkdir(parents=True, exist_ok=True)
    rows = []
    addresses = configured_addresses(matches)
    for unit in matches["units"]:
        path = output / (unit["name"] + ".raw.o")
        process = compile_object(matches["compiler"], matches["flags"], unit["source"], path.relative_to(ROOT))
        path.with_suffix(".log").write_text(process.stdout + process.stderr, encoding="utf-8")
        if process.returncode:
            raise RuntimeError(f"Compiler failed: {unit['name']}")
        functions = functions_from_object(path)
        first_address = int(unit["functions"][0]["address"], 16)
        expected_offset = 0
        with path.open("rb") as stream:
            elf = ELFFile(stream)
            text_bytes = elf.get_section_by_name(".text")["sh_size"]
            for section in elf.iter_sections():
                if section["sh_flags"] & 2 and section["sh_size"] and section.name not in (".text", ".reginfo"):
                    raise ValueError(f"Unexpected allocated C section: {section.name}")
        for specification in unit["functions"]:
            address = int(specification["address"], 16)
            symbol = functions[specification["name"]]
            compiled = resolve_function(symbol, addresses)
            offset = address - 0x089DD4C0 + 0x54
            expected = reference[offset:offset + specification["size"]]
            ordered = address == first_address + expected_offset and symbol["address"] == expected_offset
            row = {**specification, "compiled_size": len(compiled), "compiler_order_match": ordered,
                   "match": compiled == expected and ordered,
                   "different_words": sum(compiled[i:i+4] != expected[i:i+4]
                                          for i in range(0, max(len(compiled), len(expected)), 4))}
            rows.append(row)
            print(f"{row['name']}: {row['compiled_size']} bytes / match={row['match']} / {row['different_words']} different words", flush=True)
            expected_offset += specification["size"]
        if text_bytes != expected_offset:
            raise ValueError(f"Unexpected text extent or padding: {unit['name']}")
    (output / "results.json").write_text(json.dumps(rows, indent=2) + "\n", encoding="utf-8")
    if not all(row["match"] for row in rows):
        raise ValueError("At least one menu pilot function does not match")


if __name__ == "__main__":
    main()

"""Independently compare main pilot C under the pinned compiler profile."""

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
from compiler_inline_sections import audit_inline_sections, validate_inline_manifest


def main():
    matches = json.loads((ROOT / "config/main.matches.json").read_text())
    reference = (ROOT / "analysis/ULUS10566/main/EBOOT.ELF").read_bytes()
    if hashlib.sha256(reference).hexdigest() != "0bc8e779aad2814a3d1bd77346f69ca5e16665845ed053cd0ad44a9cbfaefe6d":
        raise ValueError("Main reference hash mismatch")
    output = ROOT / "analysis/main-c-pilot"
    output.mkdir(parents=True, exist_ok=True)
    rows = []
    forbidden_inline = validate_inline_manifest(matches)
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
            audit_inline_sections(elf, unit, forbidden_inline)
        for specification in unit["functions"]:
            address = int(specification["address"], 16)
            symbol = functions[specification["name"]]
            compiled = resolve_function(symbol, addresses)
            if 0x089C5770 <= address < 0x089C60B0:
                raise ValueError("SDK import stubs cannot be counted as main C")
            with (ROOT / "analysis/ULUS10566/main/EBOOT.ELF").open("rb") as reference_stream:
                reference_elf = ELFFile(reference_stream)
                matching_sections = [s for s in reference_elf.iter_sections()
                                     if s["sh_type"] != "SHT_NOBITS"
                                     and s["sh_flags"] & 4
                                     and s["sh_addr"] <= address
                                     and address + specification["size"] <= s["sh_addr"] + s["sh_size"]]
                if len(matching_sections) != 1:
                    raise ValueError("Main function is not backed by one executable ELF section")
                section = matching_sections[0]
                offset = section["sh_offset"] + address - section["sh_addr"]
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
        raise ValueError("At least one main pilot function does not match")


if __name__ == "__main__":
    main()

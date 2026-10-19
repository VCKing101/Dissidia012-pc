"""Audit unused SNC inline definitions without changing compiler objects.

C++ in-class methods emit weak linkonce bodies even when all calls are inlined.
Only explicitly listed, wholly unused bodies may be discarded by the linker;
their bytes and symbols never count toward recovered original functions.
"""

import hashlib
import re
from pathlib import Path
from elftools.elf.elffile import ELFFile


def inline_symbol_names(units):
    names = set()
    for unit in units:
        allowed = unit.get("unused_inline_symbols", [])
        if (not isinstance(allowed, list) or any(not isinstance(n, str) or
                not re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", n) for n in allowed) or
                len(set(allowed)) != len(allowed)):
            raise ValueError("Invalid unused inline symbol allowlist")
        names.update(allowed)
    return names


def validate_inline_manifest(matches):
    names = inline_symbol_names(matches["units"])
    selected = {f["name"] for u in matches["units"] for f in u["functions"]}
    if names & selected or names & set(matches.get("symbols", {})):
        raise ValueError("Discarded inline helpers cannot be recovered or external address symbols")
    return names


def audit_inline_sections(elf, unit, forbidden_inline_symbols=()):
    allowed = unit.get("unused_inline_symbols", [])
    if (not isinstance(allowed, list) or any(not isinstance(n, str) or
            not re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", n) for n in allowed) or
            len(set(allowed)) != len(allowed)):
        raise ValueError("Invalid unused inline symbol allowlist")
    selected = {f["name"] for f in unit["functions"]}
    if selected & set(allowed):
        raise ValueError("Recovered functions cannot be discarded inline helpers")
    sections = list(elf.iter_sections())
    symbols = list(elf.get_section_by_name(".symtab").iter_symbols())
    discarded, rows = set(), []
    for name in sorted(allowed):
        found = [(i, s) for i, s in enumerate(sections) if s.name == ".gnu.linkonce.t." + name]
        if len(found) != 1:
            raise ValueError("Listed inline helper has no unique linkonce section")
        index, section = found[0]
        definitions = [s for s in symbols if s["st_shndx"] == index]
        funcs = [s for s in definitions if s["st_info"]["type"] == "STT_FUNC"]
        if (section["sh_type"] != "SHT_PROGBITS" or section["sh_flags"] != 6 or
                section["sh_size"] <= 0 or section["sh_size"] % 4 or len(funcs) != 1):
            raise ValueError("Inline helper must occupy one read-only executable function section")
        symbol = funcs[0]
        if (symbol.name != name or symbol["st_info"]["bind"] != "STB_WEAK" or
                symbol["st_value"] != 0 or symbol["st_size"] != section["sh_size"] or
                any(s["st_info"]["type"] not in ("STT_FUNC", "STT_SECTION") for s in definitions)):
            raise ValueError("Inline helper is not a complete weak compiler function")
        discarded.add(index)
        rows.append({"name": name, "section": section.name, "bytes": section["sh_size"],
                     "sha256": hashlib.sha256(section.data()).hexdigest()})
    expected = sum(f["size"] for f in unit["functions"])
    text = elf.get_section_by_name(".text")
    if text is None or text["sh_size"] != expected:
        raise ValueError("Source unit includes unexpected text or padding")
    defined_funcs = {s.name for s in symbols if s["st_info"]["type"] == "STT_FUNC"
                     and isinstance(s["st_shndx"], int)}
    if defined_funcs != selected | set(allowed):
        raise ValueError("Source unit has unlisted function definitions")
    for index, section in enumerate(sections):
        if (section["sh_flags"] & 2 and section["sh_size"] and
                section.name not in (".text", ".reginfo") and index not in discarded):
            raise ValueError(f"Unexpected allocated source section: {section.name}")
        if section["sh_type"] not in ("SHT_REL", "SHT_RELA"):
            continue
        origin = section["sh_info"]
        # Debug relocations may describe discarded definitions. Retained
        # allocated bytes must never call or point into one of those sections.
        if origin in discarded or not sections[origin]["sh_flags"] & 2:
            continue
        table = sections[section["sh_link"]]
        for relocation in section.iter_relocations():
            target = table.get_symbol(relocation["r_info_sym"])
            if (target["st_shndx"] in discarded or target.name in allowed or
                    target.name in forbidden_inline_symbols):
                raise ValueError("Live allocated bytes reference a discarded inline helper")
    return rows


def inspect_inline_object(path, unit, forbidden_inline_symbols=()):
    with Path(path).open("rb") as stream:
        return audit_inline_sections(ELFFile(stream), unit, forbidden_inline_symbols)


def verify_inline_units(root, module, units, linked, prepared):
    rows = []
    forbidden = inline_symbol_names(units)
    for unit in units:
        helpers = inspect_inline_object(root / f"build/ULUS10566/{module}_{unit['name']}.raw.o", unit, forbidden)
        if any(h["name"] in linked for h in helpers):
            raise ValueError("Unused inline helper survived into linked original code")
        rows.extend({"unit": unit["name"], **helper} for helper in helpers)
    rows.sort(key=lambda h: (h["unit"], h["name"]))
    if prepared.get("discarded_inline_definitions", []) != rows:
        raise ValueError("Discarded inline definition evidence differs from raw objects")
    return rows

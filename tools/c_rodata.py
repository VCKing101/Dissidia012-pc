"""Place a matched C unit's switch jump tables (.rodata) at their retail address.

SNC puts each jump table of a `switch` in the object's `.rodata` under a local
`$LSW_n` symbol (tables 8-byte aligned, zero padding between them). The
function loads an entry through a HI16/LO16 pair against that symbol, and
every entry is an R_MIPS_32 against the object's `.text` section with the
offset in place. Retail keeps each original object's tables next to its code.

A unit's whole `.rodata` section is placed at one retail address, recorded in
the unit as `"rodata": {"address": "0x...", "size": N}`. `dc.py check` derives
that address from the retail HI16/LO16 pair at the same instructions; the
function bytes and the table bytes (entries resolved against the unit's real
`.text` address) must both equal retail. Table bytes are data: they never
count as matched C code.

Anything else fails loudly: other relocation types in `.rodata`, entries
pointing into another section, global data symbols, or code that reaches
`.rodata` through its section symbol.
"""

import struct
from collections import Counter
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

R_MIPS_32, R_MIPS_HI16, R_MIPS_LO16 = 2, 5, 6
TEXT = "<section .text>"


def read_rodata(path):
    """Return None when the object has no .rodata bytes, else its bytes, local symbols and relocations."""
    with Path(path).open("rb") as stream:
        elf = ELFFile(stream)
        sections = list(elf.iter_sections())
        found = [i for i, s in enumerate(sections) if s.name == ".rodata"]
        if not found or not sections[found[0]]["sh_size"]:
            return None
        if len(found) != 1:
            raise ValueError("Object has more than one .rodata section")
        index = found[0]
        section = sections[index]
        text = [i for i, s in enumerate(sections) if s.name == ".text"]
        if (section["sh_type"] != "SHT_PROGBITS" or section["sh_flags"] != 2
                or section["sh_size"] % 4 or len(text) != 1):
            raise ValueError(".rodata must be read-only PROGBITS of whole words beside one .text")
        symtab = elf.get_section_by_name(".symtab")
        symbols = {}
        section_symbols = set()
        for symbol in symtab.iter_symbols():
            if symbol["st_shndx"] != index:
                continue
            if symbol["st_info"]["type"] == "STT_SECTION":
                section_symbols.add(symbol.name)
                continue
            if symbol["st_info"]["bind"] != "STB_LOCAL":
                raise ValueError(f"Global data symbol in .rodata is not supported: {symbol.name}")
            if not 0 <= symbol["st_value"] <= section["sh_size"] or symbols.get(symbol.name, symbol["st_value"]) != symbol["st_value"]:
                raise ValueError(f"Invalid .rodata symbol: {symbol.name}")
            symbols[symbol.name] = symbol["st_value"]
        relocations = []
        for rel in sections:
            if not isinstance(rel, RelocationSection):
                continue
            if rel.is_RELA():
                raise ValueError("RELA objects are not supported")
            table = sections[rel["sh_link"]]
            origin = rel["sh_info"]
            for r in rel.iter_relocations():
                target = table.get_symbol(r["r_info_sym"])
                if origin == text[0] and target["st_info"]["type"] == "STT_SECTION" and target["st_shndx"] == index:
                    raise ValueError("Code refers to .rodata through its section symbol; only named tables are supported")
                if origin != index:
                    continue
                if r["r_info_type"] != R_MIPS_32 or r["r_offset"] % 4 or r["r_offset"] + 4 > section["sh_size"]:
                    raise ValueError(f"Unsupported .rodata relocation type {r['r_info_type']} at 0x{r['r_offset']:X}")
                if target["st_info"]["type"] == "STT_SECTION":
                    if target["st_shndx"] != text[0]:
                        raise ValueError("A .rodata entry points into a section other than .text")
                    name = TEXT
                else:
                    name = target.name
                    if not name or target["st_shndx"] == index:
                        raise ValueError("A .rodata entry points at unnamed or self data")
                relocations.append({"offset": r["r_offset"], "type": R_MIPS_32, "symbol": name})
        return {"bytes": section.data(), "size": section["sh_size"], "align": section["sh_addralign"],
                "symbols": symbols, "relocations": relocations}


def symbol_addresses(rodata, address):
    return {name: address + offset for name, offset in rodata["symbols"].items()}


def _signed16(value):
    return value - 0x10000 if value & 0x8000 else value


def derive_address(rodata, functions, retail_word):
    """Retail .rodata start candidates from HI16/LO16 pairs, most common first.

    functions: [{"address", "bytes", "relocations"}] at their retail addresses;
    retail_word(address) returns the retail word or None outside the module.
    A pair counts only when retail has the same instruction fields there.
    """
    found = Counter()
    for function in functions:
        pending = {}
        for relocation in function["relocations"]:
            name, offset = relocation["symbol"], relocation["offset"]
            if name not in rodata["symbols"]:
                continue
            if relocation["type"] == R_MIPS_HI16:
                pending.setdefault(name, []).append(offset)
            elif relocation["type"] == R_MIPS_LO16:
                low = struct.unpack_from("<I", function["bytes"], offset)[0]
                retail_low = retail_word(function["address"] + offset)
                for high_offset in pending.pop(name, []):
                    high = struct.unpack_from("<I", function["bytes"], high_offset)[0]
                    retail_high = retail_word(function["address"] + high_offset)
                    if retail_low is None or retail_high is None:
                        continue
                    if retail_high >> 16 != high >> 16 or retail_low >> 16 != low >> 16:
                        continue
                    addend = ((high & 0xFFFF) << 16) + _signed16(low & 0xFFFF)
                    target = ((retail_high & 0xFFFF) << 16) + _signed16(retail_low & 0xFFFF)
                    found[(target - addend - rodata["symbols"][name]) & 0xFFFFFFFF] += 1
    return [address for address, _ in found.most_common()]


def resolve_rodata(rodata, addresses):
    """Apply R_MIPS_32 entries; addresses must name TEXT (the unit's .text start)."""
    data = bytearray(rodata["bytes"])
    for relocation in rodata["relocations"]:
        if relocation["symbol"] not in addresses:
            raise ValueError(f"Unknown .rodata relocation target: {relocation['symbol']}")
        word = struct.unpack_from("<I", data, relocation["offset"])[0]
        struct.pack_into("<I", data, relocation["offset"], (addresses[relocation["symbol"]] + word) & 0xFFFFFFFF)
    return bytes(data)


def unit_rodata(unit):
    """Return the unit's declared (address, size), or None. The shape is strict."""
    spec = unit.get("rodata")
    if spec is None:
        return None
    if (not isinstance(spec, dict) or set(spec) != {"address", "size"} or not isinstance(spec["address"], str)
            or not isinstance(spec["size"], int) or isinstance(spec["size"], bool)):
        raise ValueError(f"Invalid rodata declaration in unit {unit.get('name')}")
    address = int(spec["address"], 16)
    if address % 4 or spec["size"] <= 0 or spec["size"] % 4 or address + spec["size"] > 0xFFFFFFFF:
        raise ValueError(f"Invalid rodata range in unit {unit.get('name')}")
    return address, spec["size"]


def check_unit(path, unit, addresses, retail_bytes):
    """Validate a compiled unit's .rodata against its declaration and retail.

    Returns (local symbol addresses for resolving the unit's functions,
    {"start", "end"} placement or None). retail_bytes(address, size) must
    raise for a range outside the module's file-backed bytes.
    """
    rodata = read_rodata(path)
    declared = unit_rodata(unit)
    if rodata is None and declared is None:
        return {}, None
    if rodata is None or declared is None:
        raise ValueError(f"C unit {unit['name']}: compiled .rodata and its rodata declaration disagree")
    address, size = declared
    if rodata["size"] != size:
        raise ValueError(f"C unit {unit['name']}: .rodata is {rodata['size']} bytes, declared {size}")
    clashes = set(rodata["symbols"]) & set(addresses)
    if clashes:
        raise ValueError(f"C unit {unit['name']}: .rodata symbols shadow configured names: {sorted(clashes)}")
    text_start = int(unit["functions"][0]["address"], 16)
    if resolve_rodata(rodata, {**addresses, TEXT: text_start}) != retail_bytes(address, size):
        raise ValueError(f"C unit read-only data does not match: {unit['name']}")
    return symbol_addresses(rodata, address), {"start": address, "end": address + size}

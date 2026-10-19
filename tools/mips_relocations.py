"""Resolve the supported MIPS REL fields for exact compiled-function comparison.

Unlike masking operands, this checks resolved addresses and preserves opcode /
register bits. Unsupported relocations and unresolved symbols are errors.
"""

import struct


def resolve_function(function, addresses):
    data = bytearray(function["bytes"])
    pending_high = {}
    # MIPS HI16/LO16 pairing follows relocation-table order. A compiler may
    # group several HI16 records before their shared LO16 even when the
    # instruction offsets are not increasing; sorting destroys that pairing.
    for relocation in function.get("relocations", []):
        offset, kind, symbol = relocation["offset"], relocation["type"], relocation["symbol"]
        if offset % 4 or offset < 0 or offset + 4 > len(data):
            raise ValueError("Relocation does not address a complete instruction")
        if symbol not in addresses:
            raise ValueError(f"Unknown relocation target: {symbol}")
        address = addresses[symbol]
        if not 0 <= address <= 0xFFFFFFFF:
            raise ValueError("Relocation target is outside the PSP address space")
        word = struct.unpack_from("<I", data, offset)[0]
        if kind == 4:  # R_MIPS_26
            value = address + ((word & 0x03FFFFFF) << 2)
            if value & 3:
                raise ValueError("Unaligned MIPS jump target")
            caller = addresses.get(function.get("name"))
            if caller is None:
                raise ValueError("Unknown caller address for MIPS jump relocation")
            if not 0 <= value <= 0xFFFFFFFF or value & 0xF0000000 != (caller + offset + 4) & 0xF0000000:
                raise ValueError("MIPS jump target is outside the caller's 256 MiB region")
            word = (word & 0xFC000000) | ((value >> 2) & 0x03FFFFFF)
            struct.pack_into("<I", data, offset, word)
        elif kind == 5:  # R_MIPS_HI16: requires the paired signed LO16 addend
            pending_high.setdefault(symbol, []).append((offset, word))
        elif kind == 6:  # R_MIPS_LO16
            low = word & 0xFFFF
            low_addend = low - 0x10000 if low & 0x8000 else low
            high_pairs = pending_high.pop(symbol, [])
            # LO16 may legitimately reuse a high-half load resolved by an
            # earlier LO16. Its own low result is independent of HI16 pairing.
            for high_offset, high_word in high_pairs:
                value = address + ((high_word & 0xFFFF) << 16) + low_addend
                high_word = (high_word & 0xFFFF0000) | (((value + 0x8000) >> 16) & 0xFFFF)
                struct.pack_into("<I", data, high_offset, high_word)
            word = (word & 0xFFFF0000) | ((address + low_addend) & 0xFFFF)
            struct.pack_into("<I", data, offset, word)
        else:
            raise ValueError(f"Unsupported MIPS relocation type: {kind}")
    if pending_high:
        raise ValueError("HI16 relocation has no paired LO16")
    return bytes(data)


def configured_addresses(matches):
    addresses = {name: int(value, 16) for name, value in matches.get("symbols", {}).items()}
    for unit in matches["units"]:
        for function in unit["functions"]:
            addresses[function["name"]] = int(function["address"], 16)
    return addresses

"""Validate a partial menu data review using table readers and pointer setters.

This establishes data regions, not complete function boundaries, original
type names, or feature ownership. Review metadata never includes game bytes.
"""

import math
import struct
import hashlib
from elftools.elf.elffile import ELFFile


def elf_word_reader(path, expected_sha256):
    if hashlib.sha256(path.read_bytes()).hexdigest() != expected_sha256:
        raise ValueError("Main reader reference hash mismatch")
    with path.open("rb") as stream:
        elf = ELFFile(stream)
        blocks = [(s["sh_addr"], s.data()) for s in elf.iter_sections()
                  if s["sh_type"] != "SHT_NOBITS" and s["sh_flags"] & 4]

    def read(address):
        for start, data in blocks:
            if address % 4 == 0 and start <= address <= start+len(data)-4:
                return struct.unpack_from("<I", data, address-start)[0]
        raise ValueError("Reader evidence is outside main file-backed code")
    return read


def validate_menu_data(payload, base, review, read_main_word, main_starts, local_starts):
    end = base + len(payload)

    def contents(start, finish):
        if not base <= start < finish <= end:
            raise ValueError("Menu data review leaves payload")
        return payload[start-base:finish-base]

    def word(address):
        return struct.unpack("<I", contents(address, address+4))[0]

    def pointer(evidence, expected, register):
        high, low = int(evidence["high_pc"], 16), int(evidence["low_pc"], 16)
        hi, lo = word(high), word(low)
        low_op = 49 if evidence.get("memory_load") else 9
        if (high % 4 or low % 4 or high >= low or hi >> 16 != (0x3C00 | register) or
                lo >> 26 != low_op or (lo >> 21) & 31 != register or
                (low_op == 9 and (lo >> 16) & 31 != register)):
            raise ValueError("Reviewed pointer lacks its LUI/addiu materialization")
        immediate = lo & 0xFFFF
        immediate = immediate if immediate < 0x8000 else immediate - 0x10000
        if ((hi & 0xFFFF) << 16) + immediate != expected:
            raise ValueError("Reviewed pointer does not address its data region")

    readers = {}
    for name, reader in review["readers"].items():
        entry = int(name.split(":")[1], 16)
        if not name.startswith("main:") or entry not in main_starts:
            raise ValueError("Table reader does not resolve to main code")
        load_pc, limit_pc, branch_pc, step_pc = (int(reader[k], 16) for k in
            ("sentinel_load_pc", "sentinel_constant_pc", "sentinel_branch_pc", "stride_pc"))
        load, limit, branch, step = map(read_main_word, (load_pc, limit_pc, branch_pc, step_pc))
        cursor, loaded, sentinel = (load >> 21) & 31, (load >> 16) & 31, (limit >> 16) & 31
        if (load >> 26 != 37 or load & 0xFFFF != reader["sentinel_offset"] or
                limit >> 26 != 13 or (limit >> 21) & 31 or limit & 0xFFFF != 0xFFFF or
                branch >> 26 != 4 or {(branch >> 21) & 31, (branch >> 16) & 31} != {loaded, sentinel} or
                step >> 26 != 9 or (step >> 21) & 31 != cursor or (step >> 16) & 31 != cursor or
                step & 0xFFFF != reader["stride"]):
            raise ValueError("Main reader disagrees with reviewed stride/sentinel")
        readers[name] = reader

    data, padding, tables = [], [], []
    for region in review["data_regions"]:
        start, finish = int(region["start"], 16), int(region["end"], 16)
        blob = contents(start, finish)
        kind = region["kind"]
        if kind == "ascii_resource_paths":
            strings = [s for s in blob.split(b"\0") if s]
            if not blob.endswith(b"\0") or len(strings) != region["string_count"]:
                raise ValueError("Resource path pool differs from its review")
            if any(not s.decode("ascii").startswith("menu/") or not s.endswith(b".bin") for s in strings):
                raise ValueError("Resource path pool contains an unexpected path")
        elif kind == "sentinel_halfword_table":
            evidence = region["evidence"]
            reader = readers[evidence["consumer"]]
            stride, offset = reader["stride"], reader["sentinel_offset"]
            if region["stride"] != stride or region["sentinel_offset"] != offset:
                raise ValueError("Table schema differs from its consumer")
            if start % 2 or len(blob) % stride or not 0 <= offset <= stride-2:
                raise ValueError("Halfword table has incomplete records")
            indices = [i for i in range(0, len(blob), stride) if struct.unpack_from("<H", blob, i+offset)[0] == 0xFFFF]
            if indices != [len(blob)-stride]:
                raise ValueError("Table must end at the first consumer-visible sentinel")
            register = evidence.get("pointer_register", 6)
            pointer(evidence, start, register)
            if register != 6:
                move = word(int(evidence["argument_move_pc"], 16))
                if move != (register << 21) | (6 << 11) | 0x25:
                    raise ValueError("Saved table pointer is not passed in a2")
            call = int(evidence["call_pc"], 16)
            target = int(evidence["consumer"].split(":")[1], 16)
            instruction = word(call)
            if instruction >> 26 != 3 or ((call+4) & 0xF0000000) | ((instruction & 0x3FFFFFF) << 2) != target:
                raise ValueError("Table call no longer reaches its reviewed reader")
            tables.append({"start": region["start"], "end": region["end"],
                           "record_bytes": stride, "records_including_sentinel": len(blob)//stride,
                           "sentinel_offset": offset, "consumer": evidence["consumer"]})
        elif kind == "numeric_configuration_island":
            for getter in region["getters"]:
                if int(getter["entry"], 16) not in local_starts:
                    raise ValueError("Configuration getter is missing from menu code")
                target = int(getter["target"], 16)
                if not start <= target < finish:
                    raise ValueError("Configuration getter leaves the reviewed island")
                pointer(getter, target, 2)
                if word(int(getter["return_pc"], 16)) != 0x03E00008:
                    raise ValueError("Configuration getter no longer returns its pointer")
            init = region["initializer"]
            pointer(init["constant_pointer"], start, 4)
            value = struct.unpack_from("<f", blob)[0]
            if not math.isfinite(value) or not 3.14 < value < 3.15:
                raise ValueError("Initializer's reviewed float constant differs")
            load = word(int(init["load_pc"], 16))
            if load >> 26 != 49 or (load >> 21) & 31 != 4 or (load >> 16) & 31 != 12:
                raise ValueError("Initializer no longer reads the float constant")
            pointer(init["object_pointer"], int(init["object_base"], 16), 4)
            for pc, offset in zip(init["write_pcs"], (0x20, 0x60)):
                store = word(int(pc, 16))
                if store != (57 << 26) | (4 << 21) | (12 << 16) | offset:
                    raise ValueError("Initializer no longer writes the reviewed object fields")
                contents(int(init["object_base"], 16)+offset, int(init["object_base"], 16)+offset+4)
            slot = int(region["initializer_pointer_slot"], 16)
            if word(slot) != int(init["entry"], 16) or int(init["entry"], 16) not in local_starts:
                raise ValueError("Embedded initializer pointer differs")
        else:
            raise ValueError(f"Unknown menu review data kind: {kind}")
        data.append((start, finish))
    for region in review["padding_regions"]:
        start, finish = int(region["start"], 16), int(region["end"], 16)
        if any(contents(start, finish)):
            raise ValueError("Reviewed menu padding is nonzero")
        padding.append((start, finish))
    ordered = sorted(data + padding)
    if any(z > a for (_, z), (a, _) in zip(ordered, ordered[1:])):
        raise ValueError("Menu data/padding reviews overlap")
    return {"scope": review["scope"], "data": data, "padding": padding, "tables": tables,
            "validated_data_regions": review["data_regions"], "reader_count": len(readers)}

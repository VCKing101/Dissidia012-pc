"""Resolve main overlay dispatch tables and their module-specific call roots."""

import hashlib
import struct
from elftools.elf.elffile import ELFFile


class FileImage:
    def __init__(self, path, expected_sha256):
        if hashlib.sha256(path.read_bytes()).hexdigest() != expected_sha256:
            raise ValueError("Dispatch reference hash mismatch")
        with path.open("rb") as stream:
            elf = ELFFile(stream)
            self.blocks = [(s["sh_addr"], s.data(), bool(s["sh_flags"] & 4)) for s in elf.iter_sections()
                           if s["sh_type"] != "SHT_NOBITS" and s["sh_flags"] & 2 and s["sh_size"]]

    def read(self, address, size, executable=False):
        for start, data, code in self.blocks:
            if size > 0 and start <= address and address+size <= start+len(data) and (code or not executable):
                return data[address-start:address-start+size]
        raise ValueError("Dispatch evidence leaves file-backed memory")

    def word(self, address, executable=False):
        if address % 4:
            raise ValueError("Unaligned dispatch evidence")
        return struct.unpack("<I", self.read(address, 4, executable))[0]


def inspect_dispatch(image, functions, review):
    functions = {int(f["address"], 16): f for f in functions}
    if [(s["index"], s["module"]) for s in review["slots"]] != [(0, "menu"), (1, "battle"), (2, "extra")]:
        raise ValueError("Dispatch review must include the three distinct mode bindings")

    def jal(pc, target):
        instruction = image.word(pc, True)
        if instruction >> 26 != 3 or (((pc+4) & 0xF0000000) | ((instruction & 0x3FFFFFF) << 2)) != target:
            raise ValueError("Dispatch lifecycle call target differs")

    for kind, selector in review["selectors"].items():
        table = int(review[kind+"_table"], 16)
        high, low = (image.word(int(selector[k], 16), True) for k in ("high_pc", "low_pc"))
        immediate = low & 0xFFFF
        immediate = immediate if immediate < 0x8000 else immediate-0x10000
        if high >> 16 != 0x3C05 or low >> 16 != 0x24A5 or ((high & 0xFFFF) << 16)+immediate != table:
            raise ValueError("Dispatch selector points to a different table")
        expected = {"index_pc": 0x00042080, "add_pc": 0x00852021,
                    "load_pc": 0x8C870000 if kind == "filename" else 0x8C840000}
        if kind != "filename":
            expected["indirect_call_pc"] = 0x0080F809
        for field, instruction in expected.items():
            if image.word(int(selector[field], 16), True) != instruction:
                raise ValueError("Dispatch index/load/call sequence differs")
    for call in review["lifecycle_calls"]:
        jal(int(call["pc"], 16), int(call["target"], 16))

    # Selector instruction operands above show that all three tables use the
    # same signed mode index. The main loader invokes initialization after the
    # selected file has loaded, and shutdown before unloading that module.
    slots, roots = [], {m: [] for m in ("main", "menu", "battle", "extra")}
    for slot in review["slots"]:
        index, module = slot["index"], slot["module"]
        pointers = {kind: image.word(int(review[kind+"_table"], 16)+index*4)
                    for kind in ("filename", "initialize", "shutdown")}
        filename = slot["filename"]
        if image.read(pointers["filename"], len(filename)+1) != filename.encode("ascii")+b"\0":
            raise ValueError("Dispatch filename does not identify the reviewed module")
        for kind in ("initialize", "shutdown"):
            if pointers[kind] != int(slot[kind], 16):
                raise ValueError("Dispatch function table differs from the review")
        initializer = functions.get(pointers["initialize"])
        if initializer is None:
            raise ValueError("Main dispatch initializer lacks a function boundary")
        lower, upper = (int(slot[k], 16) for k in ("payload_start", "payload_end"))
        for region in initializer["ranges"]:
            start, end = int(region["start"], 16), int(region["end"], 16)
            for pc in range(start, end, 4):
                instruction = image.word(pc, True)
                if instruction >> 26 != 3:
                    continue
                target = ((pc+4) & 0xF0000000) | ((instruction & 0x3FFFFFF) << 2)
                if lower <= target < upper:
                    roots[module].append({"address": f"0x{target:08X}", "caller": initializer["id"],
                                          "call_pc": f"0x{pc:08X}", "dispatch_index": index})
        roots["main"].append({"address": slot["shutdown"], "table_slot":
                              f"0x{int(review['shutdown_table'], 16)+index*4:08X}",
                              "dispatch_index": index, "active_module": module})
        slots.append({"index": index, "module": module, "filename_pointer": f"0x{pointers['filename']:08X}",
                      "initialize": f"main:0x{pointers['initialize']:08X}",
                      "shutdown": f"main:0x{pointers['shutdown']:08X}"})
    for module, rows in roots.items():
        roots[module] = sorted(rows, key=lambda row: (int(row["address"], 16), row.get("call_pc", "")))
    return {"scope": "Static overlay selection, lifecycle dispatch and direct calls from initialization; remaining boundaries provisional",
            "slots": slots, "roots": roots}


def validate_dispatch_entries(report, inventories):
    by_module = {m["module"]: {int(f["address"], 16): f for f in m["functions"]} for m in inventories}
    entries = {}
    for module, rows in report["roots"].items():
        entries[module] = []
        for row in rows:
            address = int(row["address"], 16)
            function = by_module[module].get(address)
            if function is None:
                raise ValueError(f"Dispatch target is not a {module} function entry: 0x{address:08X}")
            entries[module].append({**row, "id": function["id"], "body_bytes": function["body_bytes"]})
    return {**report, "roots": entries}


def validate_shutdown_bodies(image, functions, review):
    by_address = {int(f["address"], 16): f for f in functions}
    result = []
    for slot in review["slots"]:
        entry = int(slot["shutdown"], 16)
        function = by_address[entry]
        ranges = function["ranges"]
        if ranges != [{"start": f"0x{entry:08X}", "end": f"0x{entry+function['body_bytes']:08X}"}]:
            raise ValueError("Dispatch shutdown body is not contiguous")
        words = list(struct.unpack(f"<{function['body_bytes']//4}I", image.read(entry, function["body_bytes"], True)))
        if len(words) < 5 or words[-2] != 0x03E00008:
            raise ValueError("Dispatch shutdown lacks its reviewed return/delay slot")
        del words[-2]
        if len(words) % 2:
            raise ValueError("Dispatch shutdown has incomplete clear-word pairs")
        cleared = []
        for high, store in zip(words[::2], words[1::2]):
            if high >> 16 != 0x3C04 or store >> 16 != 0xAC80:
                raise ValueError("Dispatch shutdown differs from its absolute zero-store form")
            offset = store & 0xFFFF
            offset = offset if offset < 0x8000 else offset-0x10000
            address = ((high & 0xFFFF) << 16)+offset
            if address % 4:
                raise ValueError("Dispatch shutdown clears an unaligned word")
            cleared.append(f"0x{address:08X}")
        result.append({"id": function["id"], "active_module": slot["module"],
                       "body_bytes": function["body_bytes"], "cleared_word_addresses": cleared})
    return result


def trace_dispatch_calls(image, functions, roots, module):
    """Trace actual JAL words in one module; overlapping overlay VMA is local."""
    by_address = {int(f["address"], 16): f for f in functions}
    reached, pending = {}, []
    for row in roots:
        address = int(row["address"], 16)
        if address not in reached:
            reached[address] = {"id": f"{module}:0x{address:08X}", "root": f"{module}:0x{address:08X}",
                                "evidence": row.get("context", "dispatch table or selected main initializer call")}
            pending.append(address)
    index = 0
    while index < len(pending):
        entry = pending[index]
        index += 1
        for region in by_address[entry]["ranges"]:
            start, end = int(region["start"], 16), int(region["end"], 16)
            for pc in range(start, end, 4):
                instruction = image.word(pc, True)
                if instruction >> 26 != 3:
                    continue
                target = ((pc+4) & 0xF0000000) | ((instruction & 0x3FFFFFF) << 2)
                if target in by_address and target not in reached:
                    reached[target] = {"id": f"{module}:0x{target:08X}", "root": reached[entry]["root"],
                                       "caller": by_address[entry]["id"], "call_pc": f"0x{pc:08X}",
                                       "evidence": "local direct JAL from a reached function"}
                    pending.append(target)
    return [reached[a] for a in sorted(reached)]

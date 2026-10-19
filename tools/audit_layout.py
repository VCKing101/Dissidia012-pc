"""Validate the reviewed extra-overlay code/data layout against its bytes."""

import struct


def checked_descriptor(payload, base):
    if len(payload) < 16:
        raise ValueError("Overlay descriptor is truncated")
    tag, initializers, cleanup, initializer_table, cleanup_table = struct.unpack_from("<IHHII", payload)
    for count, pointer in ((initializers, initializer_table), (cleanup, cleanup_table)):
        if count and (pointer % 4 or not base + 16 <= pointer <= base + len(payload) - count * 4):
            raise ValueError("Overlay descriptor table leaves its payload")
    return {"tag": tag, "initializer_count": initializers, "cleanup_count": cleanup,
            "initializer_table": f"0x{initializer_table:08X}", "cleanup_table": f"0x{cleanup_table:08X}"}


def validate_overlay_hooks(payload, base, module, local_starts, main_starts):
    """Resolve loader hooks without conflating overlapping overlay namespaces.

    Main 0x0893F810 walks u32 initializer pointers; 0x0893F888 walks
    pointers to {a1, a0, callback} cleanup records. Object arguments may point
    outside the file-backed payload. This is a partial data/entry audit only.
    """
    descriptor = checked_descriptor(payload, base)
    expected = {"menu": (2, 8, 0), "battle": (1, 21, 5)}
    if module not in expected or tuple(descriptor[k] for k in
            ("tag", "initializer_count", "cleanup_count")) != expected[module]:
        raise ValueError("Overlay hook descriptor differs from the pinned layout")
    regions = [{"start": f"0x{base:08X}", "end": f"0x{base+16:08X}", "kind": "overlay_descriptor"}]

    def table(field, count):
        pointer = int(descriptor[field], 16)
        if count:
            regions.append({"start": f"0x{pointer:08X}", "end": f"0x{pointer+count*4:08X}", "kind": field})
        return [struct.unpack_from("<I", payload, pointer-base+i*4)[0] for i in range(count)]

    initializers = table("initializer_table", descriptor["initializer_count"])
    for target in initializers:
        if target % 4 or target not in local_starts:
            raise ValueError("Initializer does not resolve to an overlay function entry")
    cleanup = []
    for address in table("cleanup_table", descriptor["cleanup_count"]):
        if address % 4 or not base+16 <= address <= base+len(payload)-12:
            raise ValueError("Cleanup record leaves the overlay payload")
        a1, a0, target = struct.unpack_from("<III", payload, address-base)
        # Local payload addresses must resolve locally; they cannot be rescued
        # by another overlay or the main inventory sharing the same number.
        owner = module if base <= target < base+len(payload) else "main"
        starts = local_starts if owner == module else main_starts
        if target % 4 or target not in starts:
            raise ValueError("Cleanup callback does not resolve to a function entry")
        regions.append({"start": f"0x{address:08X}", "end": f"0x{address+12:08X}", "kind": "cleanup_record"})
        cleanup.append({"record": f"0x{address:08X}", "a1": f"0x{a1:08X}", "a0": f"0x{a0:08X}",
                        "callback": f"{owner}:0x{target:08X}"})
    ordered = sorted((int(r["start"], 16), int(r["end"], 16)) for r in regions)
    if any(end > start for (_, end), (start, _) in zip(ordered, ordered[1:])):
        raise ValueError("Overlay hook data regions overlap")
    return {"scope": "Loader descriptor, hook entry points and cleanup records only; remaining boundaries/layout provisional",
            "descriptor": descriptor, "initializers": [f"{module}:0x{a:08X}" for a in initializers],
            "cleanup": cleanup, "data_regions": regions,
            "consumers": {"initializer_walker": "main:0x0893F810", "cleanup_walker": "main:0x0893F888"}}


def validate_extra_layout(payload, base, function_ranges, function_starts, review):
    def parse(regions):
        return [(int(r["start"], 16), int(r["end"], 16)) for r in regions]

    def read(start, end):
        if not base <= start < end <= base + len(payload):
            raise ValueError("Reviewed region leaves the extra payload")
        return payload[start-base:end-base]

    code = parse(review["code_ranges"])
    data = parse(review["data_regions"])
    padding = parse(review["padding_regions"])
    if code != function_ranges:
        raise ValueError("Reviewed code ranges do not equal the checked function-body union")
    cursor = base
    for start, end in sorted(code + data + padding):
        if start != cursor or end <= start:
            raise ValueError("Reviewed layout has an overlap or a gap")
        read(start, end)
        cursor = end
    if cursor != base + len(payload):
        raise ValueError("Reviewed layout does not cover the whole payload")
    for start, end in padding:
        if any(read(start, end)):
            raise ValueError("Reviewed zero-fill/padding contains nonzero bytes")
    descriptor = checked_descriptor(payload, base)
    if descriptor["tag"] != 3 or descriptor["initializer_count"] or descriptor["cleanup_count"]:
        raise ValueError("Extra descriptor differs from the reviewed zero-hook layout")
    evidence = []
    for region in review["data_regions"]:
        start, end = int(region["start"], 16), int(region["end"], 16)
        contents = read(start, end)
        kind = region["kind"]
        if kind in ("ascii_string", "ascii_pool"):
            contents.decode("ascii")
            if contents[-1] != 0:
                raise ValueError("Reviewed ASCII region is not terminated")
        elif kind == "utf16_string":
            contents.decode("utf-16le")
            if contents[-2:] != b"\0\0":
                raise ValueError("Reviewed UTF-16 region is not terminated")
        elif kind == "halfword_records":
            if len(contents) % 8 or contents[-8:] != b"\xff" * 8:
                raise ValueError("Halfword record region lacks its full sentinel record")
        elif kind == "jump_table":
            if len(contents) != 7 * 4:
                raise ValueError("Jump table disagrees with the seven-case bounds check")
            targets = struct.unpack("<7I", contents)
            if not all(any(a <= target < z for a, z in code) for target in targets):
                raise ValueError("Jump target leaves the checked code regions")
        elif kind == "member_dispatch_table":
            if len(contents) != 21 * 8:
                raise ValueError("Dispatch table disagrees with the 168-byte copy and eight-byte indexing")
            for offset in range(0, len(contents), 8):
                adjustment, virtual_selector, target = struct.unpack_from("<hhI", contents, offset)
                if adjustment or virtual_selector or target not in function_starts:
                    raise ValueError("Dispatch record differs from the reviewed direct-member form")
        elif kind == "string_pointer_records":
            words = struct.unpack("<12I", contents)
            for index in (0, 1, 3, 5, 7, 9):
                if not 0x089E2E38 <= words[index] < 0x089E2F12:
                    raise ValueError("String descriptor pointer leaves its string pool")
            if words[10:] != (0xFFFF, 0):
                raise ValueError("String descriptor table lacks its sentinel")
        elif kind not in ("overlay_descriptor", "static_globals", "binary_configuration"):
            raise ValueError(f"Unknown reviewed data kind: {kind}")
        evidence.append({"start": region["start"], "end": region["end"], "kind": kind})
    return {"code": code, "data": data, "padding": padding,
            "descriptor": descriptor, "validated_data_regions": evidence}

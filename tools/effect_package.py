"""Read, write and add to a fighter's effect package (`FEP\\xcd`, the `.fep` member of an objx `effe` chunk).

A package is a graph of blocks joined by pointers, stored as offsets from the start of the
file. After loading, the game turns every pointer into an address (main 0x0894351C). This
module finds the same pointers by walking the same structure, splits the file into blocks at
their targets, and writes blocks back with the pointers recomputed. An unchanged package is
written back byte for byte.

Layout (from main 0x0894351C; field names partly from dissidia-fan's notes on the format):

    header 0x48   +0x00 'FEP\\xcd', u16 1, u16 3
                  +0x08 ptr {u32 n, ptr descriptors}: n x 8 bytes {u8, u8, u16, u16 parameter
                        size, u16 state size}, one per channel type (the same in every file)
                  +0x0C runtime flags (0 in files), +0x10 package id base, +0x14 file size
                  +0x1C u16 effects, +0x1E u16 bodies, +0x20 u16 x4 TEX, MDL, ANM, VTL counts
                  +0x28 ptr effects[], +0x2C ptr bodies[], +0x30..+0x3C ptr TEX/MDL/ANM/VTL refs[]
                  +0x40 ptr (if < 0x500000) {u16 n, ptr items[]}, item +8 ptr
                  +0x44 ptr names {u32 n, ptr entries}: entries 12 bytes {u32 effect id, u32 length, ptr text}
    effect  12    {u32 id, u16 body, u16 leaves, ptr leaves[]}; leaf +0x0C ptr (may be 0)
                  id = package base | group << 10 | leaf bits; a package holds groups 0..127
    body    52    {.., +0x10 u16 spawns, +0x12 u16 nodes, +0x18 u16 serials, +0x1A u16 p48,
                  +0x1C ptr spawns[] (item +4 ptr), +0x20 ptr nodes[] (may hold 0),
                  +0x24 ptr channels, +0x28 ptr p40[12] (item +4 ptr), +0x2C ptr serials[] (item +4 ptr),
                  +0x30 ptr p48[]}
    node    72    {+0x08 u8 tracks (0: nothing below is a pointer), +0x12 u8 has_init, +0x13 u8 segments,
                  +0x1C ptr tracks[] (track 12 bytes {u16 channel, u16, u32 keys, ptr keys}),
                  +0x20 ptr initial parameters (0 without has_init), +0x24 ptr segments[] (item +8 ptr),
                  +0x28..+0x44 eight ptr lists}
    parameters    {u16 frame0, u16 frame1, u32 n, ptr table}; table n x {u16 track, u16 offset, u16, u16},
                  each track's parameter block at offset - 12 into the table block
    resource 20   {f32, u32 id, u32 size, ptr data, i32 -1}; size 0 = a shared library resource

Which parameter fields name a resource (by index into its table) is in RESOURCE_FIELDS; models
and texture animations name their textures by id.
"""

import struct

MAGIC = b"FEP\xcd"
KINDS = ("TEX", "MDL", "ANM", "VTL")

# Channel -> [(resource kind, byte offset in the channel's parameter block, struct format)].
# The parameter fields that hold an index into a resource table; from dissidia-fan's analysis
# of the channels' component code (their dfx/fep.py LINKS), checked here against every
# package of both games (all indices in range).
RESOURCE_FIELDS = {
    10: [("ANM", 12, "<i")], 11: [("MDL", 12, "<i")], 20: [("MDL", 12, "<i")],
    21: [("ANM", 12, "<i"), ("MDL", 244, "<h"), ("VTL", 246, "<h")],
    26: [("MDL", 14, "<h"), ("VTL", 16, "<h")], 30: [("MDL", 14, "<h"), ("VTL", 16, "<h")],
    150: [("MDL", 14, "<h"), ("VTL", 16, "<h")],
    53: [("ANM", 16, "<i")], 54: [("ANM", 160, "<i")], 55: [("ANM", 16, "<i")],
    56: [("ANM", 16, "<i")], 57: [("ANM", 16, "<i")],
    63: [("TEX", 0, "<i")], 78: [("MDL", 4, "<h"), ("VTL", 6, "<h")], 80: [("ANM", 44, "<i")],
    105: [("TEX", 44, "<i")], 108: [("MDL", 16, "<h"), ("VTL", 18, "<h")], 119: [("MDL", 12, "<i")],
}


class EffectError(ValueError):
    pass


def u16(data, at):
    return struct.unpack_from("<H", data, at)[0]


def u32(data, at):
    return struct.unpack_from("<I", data, at)[0]


def pointer_slots(data):
    """{slot offset: target offset} of every pointer the game relocates (main 0x0894351C)."""
    slots = {}
    size = len(data)

    def ptr(slot, allow_zero=False):
        value = u32(data, slot)
        if value == 0 and allow_zero:
            return 0
        if not 0 < value <= size:
            raise EffectError(f"pointer {value:#x} at {slot:#x} is outside the package")
        slots[slot] = value
        return value

    if data[:4] != MAGIC:
        raise EffectError("not an effect package (FEP)")
    at = ptr(0x08, True)
    if at:
        ptr(at + 4)
    at = ptr(0x44, True)
    if at:
        entries = ptr(at + 4)
        for i in range(u32(data, at)):
            ptr(entries + 12 * i + 8)
    if 0 < u32(data, 0x40) < 0x500000:
        at = ptr(0x40)
        items = ptr(at + 4)
        for i in range(u16(data, at)):
            item = ptr(items + 4 * i)
            ptr(item + 8)
    for k in range(4):
        table = ptr(0x30 + 4 * k, True)
        if table:
            for i in range(u16(data, 0x20 + 2 * k)):
                ref = ptr(table + 4 * i)
                ptr(ref + 12)
    table = ptr(0x28)
    for i in range(u16(data, 0x1C)):
        effect = ptr(table + 4 * i)
        leaves = ptr(effect + 8)
        for j in range(u16(data, effect + 6)):
            leaf = ptr(leaves + 4 * j)
            ptr(leaf + 12, True)
    table = ptr(0x2C)
    for i in range(u16(data, 0x1E)):
        body = ptr(table + 4 * i)
        spawns = ptr(body + 0x1C)
        for j in range(u16(data, body + 0x10)):
            item = ptr(spawns + 4 * j)
            ptr(item + 4)
        nodes = ptr(body + 0x20)
        for j in range(u16(data, body + 0x12)):
            node = ptr(nodes + 4 * j, True)
            if not node or data[node + 8] == 0:
                continue
            tracks = ptr(node + 0x1C)
            for k in range(data[node + 8]):
                track = ptr(tracks + 4 * k)
                ptr(track + 8)
            if data[node + 0x12]:
                init = ptr(node + 0x20)
                ptr(init + 8)
            segments = ptr(node + 0x24)
            for k in range(data[node + 0x13]):
                segment = ptr(segments + 4 * k)
                ptr(segment + 8)
            for k in range(8):
                ptr(node + 0x28 + 4 * k)
        ptr(body + 0x24)
        p40 = ptr(body + 0x28)
        for j in range(12):
            item = ptr(p40 + 4 * j)
            ptr(item + 4)
        serials = ptr(body + 0x2C)
        for j in range(u16(data, body + 0x18)):
            item = ptr(serials + 4 * j)
            ptr(item + 4)
        p48 = ptr(body + 0x30)
        for j in range(u16(data, body + 0x1A)):
            ptr(p48 + 4 * j)
    return slots


class Block:
    """Bytes of the package from one pointer target to the next, with the pointers inside them
    (`ptrs`: offset in the block -> target Block)."""

    __slots__ = ("data", "ptrs", "pos")

    def __init__(self, data, pos=None):
        self.data = bytearray(data)
        self.ptrs = {}
        self.pos = pos

    def get(self, at):
        return self.ptrs.get(at)


class Package:
    def __init__(self, data):
        data = bytes(data)
        slots = pointer_slots(data)
        starts = sorted(set(slots.values()) | {0})
        self.blocks = []
        by_pos = {}
        for i, start in enumerate(starts):
            end = starts[i + 1] if i + 1 < len(starts) else len(data)
            block = Block(data[start:end], start)
            self.blocks.append(block)
            by_pos[start] = block
        import bisect
        for slot, target in slots.items():
            owner = self.blocks[bisect.bisect_right(starts, slot) - 1]
            owner.ptrs[slot - owner.pos] = by_pos[target]
        self.header = self.blocks[0]

    # -- writing
    def to_bytes(self):
        out = bytearray()
        where = {}
        for block in self.blocks:
            align = block_align(block)
            out += bytes(-len(out) % align)
            where[id(block)] = len(out)
            out += block.data
        for block in self.blocks:
            base = where[id(block)]
            for at, target in block.ptrs.items():
                struct.pack_into("<I", out, base + at, where[id(target)])
        struct.pack_into("<I", out, 0x14, len(out))
        return bytes(out)

    # -- reading
    def count(self, at):
        return u16(self.header.data, at)

    def table(self, at, count):
        table = self.header.get(at)
        return [table.get(4 * i) for i in range(count)] if table else []

    def effects(self):
        return self.table(0x28, self.count(0x1C))

    def bodies(self):
        return self.table(0x2C, self.count(0x1E))

    def refs(self, kind):
        k = KINDS.index(kind)
        return self.table(0x30 + 4 * k, self.count(0x20 + 2 * k))

    def names(self):
        """[(effect id, name)] in name-table order (an effect placement's +0x28 is an index here)."""
        head = self.header.get(0x44)
        entries = head.get(4)
        out = []
        for i in range(u32(head.data, 0)):
            text = entries.get(12 * i + 8).data
            out.append((u32(entries.data, 12 * i), bytes(text[:text.index(0)]).decode("latin-1")))
        return out

    def descriptors(self):
        head = self.header.get(0x08)
        table = head.get(4)
        return [struct.unpack_from("<BBHHH", table.data, 8 * i) for i in range(u32(head.data, 0))]

    def base(self):
        return u32(self.header.data, 0x10)

    def parameter_fields(self, body):
        """[(table block, offset, kind, format)]: every resource index in a body's nodes' parameters."""
        sizes = [d[3] for d in self.descriptors()]
        out = []
        nodes = body.get(0x20)
        for j in range(u16(body.data, 0x12)):
            node = nodes.get(4 * j) if nodes else None
            if node is None or node.data[8] == 0:
                continue
            tracks = node.get(0x1C)
            channels = [u16(tracks.get(4 * k).data, 0) for k in range(node.data[8])]
            sets = [node.get(0x20)] if node.data[0x12] else []
            segments = node.get(0x24)
            sets += [segments.get(4 * k) for k in range(node.data[0x13])]
            for params in sets:
                table = params.get(8)
                for q in range(u32(params.data, 4)):
                    track, offset = struct.unpack_from("<HH", table.data, 8 * q)
                    channel = channels[track]
                    for kind, field, fmt in RESOURCE_FIELDS.get(channel, ()):
                        at = offset - 12 + field
                        if field + struct.calcsize(fmt) <= sizes[channel] and at + struct.calcsize(fmt) <= len(table.data):
                            out.append((table, at, kind, fmt))
        return out

    def resources_of(self, body):
        """{kind: set of indices} a body's nodes name, textures of its models and animations included."""
        used = {kind: set() for kind in KINDS}
        for table, at, kind, fmt in self.parameter_fields(body):
            value = struct.unpack_from(fmt, table.data, at)[0]
            if value >= 0:
                used[kind].add(value)
        by_id = {u32(ref.data, 4): i for i, ref in enumerate(self.refs("TEX"))}
        for kind in ("MDL", "ANM"):
            refs = self.refs(kind)
            for index in used[kind]:
                for tex in texture_ids(kind, refs[index]):
                    if tex in by_id:
                        used["TEX"].add(by_id[tex])
        return used


def reachable(block):
    """Every block a block leads to through its pointers, itself first."""
    seen, order, stack = set(), [], [block]
    while stack:
        current = stack.pop()
        if id(current) in seen:
            continue
        seen.add(id(current))
        order.append(current)
        stack += [current.ptrs[at] for at in sorted(current.ptrs, reverse=True)]
    return order


def clone(blocks):
    """Copies of `blocks` with their pointers among themselves redirected; {id(old): new}."""
    copies = {}
    for block in blocks:
        copies[id(block)] = Block(block.data, block.pos)
    for block in blocks:
        new = copies[id(block)]
        for at, target in block.ptrs.items():
            new.ptrs[at] = copies.get(id(target), target)
    return copies


def insert_pointer(table, count, target):
    """Add a pointer after the first `count` of a pointer table block."""
    at = 4 * count
    table.data[at:at] = bytes(4)
    table.ptrs = {(k + 4 if k >= at else k): v for k, v in table.ptrs.items()}
    table.ptrs[at] = target


def first_free_group(package):
    used = {(u32(e.data, 0) >> 10) & 0x7F for e in package.effects()}
    for group in range(128):
        if group not in used:
            return group
    raise EffectError("the package already holds 128 effects")


def copy_effect(target, source, name, new_name=None):
    """Copy effect `name` of package `source` into `target`, with its body, its nodes and the
    resources they use (a resource `target` already holds, byte for byte, is shared). Returns
    (the copy's index in the name table, its id). Placement records (FEPA) name an effect by
    that index."""
    names = source.names()
    found = [i for i, (_, n) in enumerate(names) if n == name]
    if not found:
        raise EffectError(f"{name}: no such effect in the source package")
    effect_id = names[found[0]][0]
    effect = next(e for e in source.effects() if u32(e.data, 0) == effect_id)
    body = source.bodies()[u16(effect.data, 4)]
    used = source.resources_of(body)

    # Resources: textures first, since models and texture animations name them by id.
    mapping = {kind: {} for kind in KINDS}
    texture_ids_map = {}
    for kind in KINDS:
        source_refs, target_refs = source.refs(kind), target.refs(kind)
        target_ids = {u32(r.data, 4) for r in target_refs}
        for index in sorted(used[kind]):
            ref = source_refs[index]
            ref_id, data = u32(ref.data, 4), resource_bytes(ref)
            if data is not None and kind in ("MDL", "ANM"):
                patched = bytearray(data)
                for at in texture_id_fields(kind, data):
                    struct.pack_into("<I", patched, at, texture_ids_map.get(u32(data, at), u32(data, at)))
                data = bytes(patched)
            match = None
            for t_index, t_ref in enumerate(target_refs):
                if data is None and u32(t_ref.data, 8) == 0 and u32(t_ref.data, 4) == ref_id:
                    match = t_index
                elif data is not None and resource_bytes(t_ref) == data:
                    match = t_index
                if match is not None:
                    break
            if match is not None:
                mapping[kind][index] = match
                if kind == "TEX":
                    texture_ids_map[ref_id] = u32(target_refs[match].data, 4)
                continue
            new_id = ref_id
            if data is not None and new_id in target_ids:
                family = [i for i in target_ids if i >> 24 == ref_id >> 24]
                new_id = max(family) + 1
            target_ids.add(new_id)
            if kind == "TEX":
                texture_ids_map[ref_id] = new_id
            new_ref = Block(ref.data, ref.pos)
            struct.pack_into("<I", new_ref.data, 4, new_id)
            if data is None:
                new_ref.ptrs[12] = new_ref
            else:
                payload = Block(ref.get(12).data, ref.get(12).pos)
                payload.data[:len(data)] = data
                id_at = 8 if kind == "MDL" else 4
                struct.pack_into("<I", payload.data, id_at, new_id)
                new_ref.ptrs[12] = payload
                target.blocks.append(payload)
            target.blocks.append(new_ref)
            k = KINDS.index(kind)
            count = u16(target.header.data, 0x20 + 2 * k)
            insert_pointer(target.header.get(0x30 + 4 * k), count, new_ref)
            struct.pack_into("<H", target.header.data, 0x20 + 2 * k, count + 1)
            mapping[kind][index] = count
            target_refs = target.refs(kind)

    # The body, its resource indices renumbered.
    body_blocks = reachable(body)
    copies = clone(body_blocks)
    for table, at, kind, fmt in source.parameter_fields(body):
        value = struct.unpack_from(fmt, table.data, at)[0]
        if value >= 0:
            struct.pack_into(fmt, copies[id(table)].data, at, mapping[kind][value])
    new_body = copies[id(body)]
    target.blocks += [copies[id(b)] for b in body_blocks]
    body_index = u16(target.header.data, 0x1E)
    insert_pointer(target.header.get(0x2C), body_index, new_body)
    struct.pack_into("<H", target.header.data, 0x1E, body_index + 1)

    # The effect entry and its leaves, under a group of the target's own.
    new_id = target.base() | first_free_group(target) << 10 | (effect_id & 0x3FF)
    effect_blocks = reachable(effect)
    copies = clone(effect_blocks)
    new_effect = copies[id(effect)]
    struct.pack_into("<IH", new_effect.data, 0, new_id, body_index)
    leaves = effect.get(8)
    for j in range(u16(effect.data, 6)):
        leaf = copies[id(leaves.get(4 * j))]
        if u32(leaf.data, 0) & ~0x3FF == effect_id & ~0x3FF:
            struct.pack_into("<I", leaf.data, 0, new_id & ~0x3FF | (u32(leaf.data, 0) & 0x3FF))
    target.blocks += [copies[id(b)] for b in effect_blocks]
    count = u16(target.header.data, 0x1C)
    insert_pointer(target.header.get(0x28), count, new_effect)
    struct.pack_into("<H", target.header.data, 0x1C, count + 1)

    # Its name.
    text = (new_name or name).encode("latin-1") + b"\0"
    string = Block(text + bytes(-len(text) % 4), 0)
    head = target.header.get(0x44)
    entries = head.get(4)
    index = u32(head.data, 0)
    at = 12 * index
    entries.data[at:at] = struct.pack("<III", new_id, len(text) - 1, 0)
    entries.ptrs = {(k + 12 if k >= at else k): v for k, v in entries.ptrs.items()}
    entries.ptrs[at + 8] = string
    struct.pack_into("<I", head.data, 0, index + 1)
    target.blocks.append(string)
    return index, new_id


def resource_bytes(ref):
    """The data of a resource reference (None for a shared library resource)."""
    size = u32(ref.data, 8)
    return bytes(ref.get(12).data[:size]) if size else None


def texture_id_fields(kind, data):
    """Offsets of the texture ids inside a model (MDL materials, 24 bytes from +0x10's offset,
    count at +0x0C) or a texture animation (ANM: count at +0x20, offset at +0x30)."""
    if not data:
        return []
    if kind == "MDL" and data[:4] == b"MDL\0":
        count, offset = u32(data, 0x0C), u32(data, 0x10)
        return [offset + 24 * i for i in range(count)]
    if kind == "ANM" and data[:4] == b"ANM\0":
        count, offset = u32(data, 0x20), u32(data, 0x30)
        return [offset + 4 * i for i in range(count)]
    return []


def texture_ids(kind, ref):
    data = resource_bytes(ref)
    return [u32(data, at) for at in texture_id_fields(kind, data)]


def block_align(block):
    """Where a block may start: what its original offset gives (16 at most), else 4."""
    pos = block.pos if block.pos is not None else 0
    for align in (16, 8, 4):
        if pos % align == 0:
            return align
    return 4

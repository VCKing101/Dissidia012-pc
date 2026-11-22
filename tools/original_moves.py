"""Bring a fighter's moves from the original Dissidia (ULUS10437, 2008) into Dissidia 012.

    python tools/original_moves.py check --og-iso OG.iso [--iso 012.iso]
    python tools/original_moves.py build --og-iso OG.iso [--iso 012.iso] OUTDIR [--swap OG=012 ...]

The two games share an engine. The original's tables are older versions of 012's; every rule
below was checked against all the records the games share (docs/research/original-bartz.md):

    plya   0x70 -> 0x74 bytes. The button class moves out of flag bits 0x2000/0x4000 into +0x0F.
    atkp   0x3C -> 0x44 bytes. 012 adds +0x06/+0x07 and a hit-stop block at +0x14.
    comb   fixed 8-stage entries -> counted ones. Stages are the same 8 bytes.
    magi   0x268 -> 0xCC bytes. 138 inline floats become u8 indices into one float pool.
           Origin and target codes were renumbered.
    FEPA   248 -> 260 bytes.
    tracks the same, but eight event types have more arguments in 012: the original's come
           first and 012 adds zeros.

`check` converts the original fighter and compares every record it shares with 012's.
`build` merges the original commands into 012's fighter as new commands, the way a move is
copied between characters, and writes the changed package files into OUTDIR under their
package paths. --swap OG=012 puts an original command in place of a 012 one instead of adding
it. That is a test aid: it needs no new ability records.

The tool reads both discs and writes only into OUTDIR. Nothing from either disc is kept
in the repository.
"""

import argparse
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import effect_package  # noqa: E402
from package_archive import Archive  # noqa: E402

ISO_012 = ROOT / "Dissidia 012 - Duodecim Final Fantasy (USA) (En,Fr,De,Es,It).iso"


class MoveError(ValueError):
    """The data is not as expected. The message says where."""


def u16(data, offset):
    return struct.unpack_from("<H", data, offset)[0]


def s16(data, offset):
    return struct.unpack_from("<h", data, offset)[0]


def u32(data, offset):
    return struct.unpack_from("<I", data, offset)[0]


def put16(data, offset, value):
    struct.pack_into("<H", data, offset, value & 0xFFFF)


def put32(data, offset, value):
    struct.pack_into("<I", data, offset, value & 0xFFFFFFFF)


# ---------------------------------------------------------------------------------------------
# Containers


class Arc:
    """`ARC\\x01`: u32 count, 8 bytes kept, then {u32 kind, char tag[4], u32 offset, u32 size}."""

    ALIGN = {14: 64, 3: 16, 2: 16}

    def __init__(self, data):
        if data[:4] != b"ARC\x01":
            raise MoveError("not an ARC archive")
        count = u32(data, 4)
        self.extra = data[8:16]
        self.chunks = []
        for index in range(count):
            kind, tag, offset, size = struct.unpack_from("<I4sII", data, 16 + index * 16)
            self.chunks.append([kind, tag, bytes(data[offset:offset + size])])

    def find(self, tag=None, kind=None):
        for entry in self.chunks:
            if (tag is None or entry[1] == tag) and (kind is None or entry[0] == kind):
                return entry
        return None

    def get(self, tag=None, kind=None):
        entry = self.find(tag, kind)
        if entry is None:
            raise MoveError(f"no chunk {tag or kind}")
        return entry[2]

    def set(self, data, tag=None, kind=None):
        self.find(tag, kind)[2] = bytes(data)

    def to_bytes(self):
        table, body = bytearray(), bytearray()
        position = 16 + len(self.chunks) * 16
        for kind, tag, data in self.chunks:
            pad = -position % self.ALIGN.get(kind, 4)
            body += bytes(pad)
            position += pad
            table += struct.pack("<I4sII", kind, tag, position, len(data))
            body += data
            position += len(data)
        return b"ARC\x01" + struct.pack("<I", len(self.chunks)) + self.extra + bytes(table) + bytes(body)


class Mpk:
    """`MPK `: u32 0x20070606, u32 count, u32 0, {u32 name offset, u32 data offset, u32 size, u32 0},
    the names, then the members on 16 bytes."""

    def __init__(self, data):
        if data[:4] != b"MPK ":
            raise MoveError("not an MPK archive")
        self.version = u32(data, 4)
        self.members = []
        for index in range(u32(data, 8)):
            name_at, offset, size, _ = struct.unpack_from("<4I", data, 16 + index * 16)
            name = data[name_at:data.index(b"\0", name_at)].decode("latin-1")
            self.members.append([name, bytes(data[offset:offset + size])])

    def get(self, suffix):
        for name, data in self.members:
            if name.endswith(suffix):
                return data
        raise MoveError(f"no member *{suffix}")

    def set(self, suffix, data):
        for member in self.members:
            if member[0].endswith(suffix):
                member[1] = bytes(data)
                return
        raise MoveError(f"no member *{suffix}")

    def to_bytes(self):
        names = bytearray()
        name_at = []
        start = 16 + len(self.members) * 16
        for name, _ in self.members:
            name_at.append(start + len(names))
            names += name.encode("latin-1") + b"\0"
        position = start + len(names)
        position += -position % 16
        table, body = bytearray(), bytearray()
        for (name, data), at in zip(self.members, name_at):
            table += struct.pack("<4I", at, position, len(data), 0)
            body += data + bytes(-len(data) % 16)
            position += len(data) + (-len(data) % 16)
        head = b"MPK " + struct.pack("<III", self.version, len(self.members), 0)
        data = head + bytes(table) + bytes(names)
        data += bytes(-len(data) % 16)
        return data + bytes(body)


# ---------------------------------------------------------------------------------------------
# 012's tables


def records(data, size, head=8, count_at=4):
    count = u32(data, count_at)
    if head + count * size > len(data):
        raise MoveError(f"{count} records of {size} bytes do not fit {len(data)} bytes")
    return [bytearray(data[head + index * size:head + (index + 1) * size]) for index in range(count)]


def join_records(head, rows):
    return bytes(head) + b"".join(bytes(row) for row in rows)


def read_comb(data):
    """[{command, alt, prerequisite, stages: [8-byte stage]}] of a 012 `comb` chunk."""
    if u16(data, 0) != 6:
        raise MoveError("comb: not version 6")
    count = s16(data, 2)
    entries = []
    for offset in struct.unpack_from(f"<{count}I", data, 4):
        command, stages, alt, prerequisite = struct.unpack_from("<hhhh", data, offset)
        entries.append({"command": command, "alt": alt, "prerequisite": prerequisite,
                        "stages": [bytearray(data[offset + 8 + 8 * k:offset + 16 + 8 * k]) for k in range(stages)]})
    return entries


def write_comb(entries):
    start = 4 + 4 * len(entries)
    offsets, body = [], bytearray()
    for entry in entries:
        offsets.append(start + len(body))
        body += struct.pack("<hhhh", entry["command"], len(entry["stages"]), entry["alt"], entry["prerequisite"])
        for stage in entry["stages"]:
            body += stage
    return struct.pack(f"<Hh{len(entries)}I", 6, len(entries), *offsets) + bytes(body)


MAGI_PARAMS = 46


def read_magi(data):
    """012 `magi`: (version, [{head: 0x40 bytes, params: [(min, max, step) floats], tail}])."""
    if data[:4] != b"MAGI":
        raise MoveError("magi: no MAGI magic")
    version, count, floats = struct.unpack_from("<III", data, 4)
    pool = struct.unpack_from(f"<{floats}f", data, 16 + count * 0xCC)
    out = []
    for index in range(count):
        row = data[16 + index * 0xCC:16 + (index + 1) * 0xCC]
        indices = row[0x40:0x40 + 3 * MAGI_PARAMS]
        out.append({"head": bytearray(row[:0x40]), "params": [tuple(pool[k] for k in indices[3 * p:3 * p + 3])
                                                               for p in range(MAGI_PARAMS)],
                    "tail": bytes(row[0x40 + 3 * MAGI_PARAMS:]), "indices": bytes(indices)})
    return version, out, list(pool)


def write_magi(version, rows, pool=None):
    """Rows keep their own pool indices where a pool is given and they have them; new rows
    get indices into the pool, which grows (128 values at most)."""
    pool = list(pool or [])
    bits = [struct.pack("<f", value) for value in pool]
    body = bytearray()
    for row in sorted(rows, key=lambda r: u16(r["head"], 0)):
        if row.get("indices") is not None:
            indices = row["indices"]
        else:
            indices = bytearray()
            for triple in row["params"]:
                for value in triple:
                    packed = struct.pack("<f", value)
                    if packed not in bits:
                        bits.append(packed)
                        pool.append(value)
                    indices.append(bits.index(packed))
        body += row["head"] + bytes(indices) + (row.get("tail") or bytes(2))
    if len(pool) > 128:
        raise MoveError(f"magi: {len(pool)} pool values, the most is 128")
    return b"MAGI" + struct.pack("<III", version, len(rows), len(pool)) + bytes(body) + b"".join(bits)


def read_motions(data):
    """Motion table (kind 7): [8-byte entry {u16 motion id, u16 animation, u16 variant, u16}]."""
    return records(data, 8)


def write_motions(head, rows):
    rows = sorted(rows, key=lambda r: u16(r, 0))
    return struct.pack("<II", u32(head, 0), len(rows)) + b"".join(bytes(r) for r in rows)


def read_tracks(data):
    """Event tracks (kind 8): [{anim, ranged: [(header 12 bytes, args)], instant: [(header 8, args)]}]."""
    count = u32(data, 4)
    tracks = []
    for offset in struct.unpack_from(f"<{count}I", data, 8):
        anim, n_ranged, n_instant, instant_at, size = struct.unpack_from("<HBBHH", data, offset)
        cursor = offset + 8
        ranged, instant = [], []
        for _ in range(n_ranged):
            head = bytearray(data[cursor:cursor + 12])
            argc = head[5]
            ranged.append([head, list(struct.unpack_from(f"<{argc}I", data, cursor + 12))])
            cursor += 12 + 4 * argc
        if cursor != offset + instant_at:
            raise MoveError(f"track {anim}: ranged events end at {cursor - offset}, instant start at {instant_at}")
        for _ in range(n_instant):
            head = bytearray(data[cursor:cursor + 8])
            argc = head[3]
            instant.append([head, list(struct.unpack_from(f"<{argc}I", data, cursor + 8))])
            cursor += 8 + 4 * argc
        if cursor != offset + size:
            raise MoveError(f"track {anim}: events end at {cursor - offset}, the track at {size}")
        tracks.append({"anim": anim, "ranged": ranged, "instant": instant})
    return u32(data, 0), tracks


def write_tracks(version, tracks):
    bodies = []
    for track in tracks:
        events = bytearray()
        for head, args in track["ranged"]:
            head[5] = len(args)
            events += head + struct.pack(f"<{len(args)}I", *[a & 0xFFFFFFFF for a in args])
        instant_at = 8 + len(events)
        for head, args in track["instant"]:
            head[3] = len(args)
            events += head + struct.pack(f"<{len(args)}I", *[a & 0xFFFFFFFF for a in args])
        bodies.append(struct.pack("<HBBHH", track["anim"], len(track["ranged"]), len(track["instant"]),
                                  instant_at, 8 + len(events)) + bytes(events))
    offsets, position = [], 8 + 4 * len(tracks)
    for body in bodies:
        offsets.append(position)
        position += len(body)
    return struct.pack(f"<II{len(tracks)}I", version, len(tracks), *offsets) + b"".join(bodies)


def event_type(head, ranged):
    return head[4] if ranged else head[2]


def read_shapes(data):
    """Collision shapes (kind 5): (u64 default-on mask, [0x20-byte record]); record byte 0 = shape id."""
    mask, count = struct.unpack_from("<QI", data, 0)
    return mask, [bytearray(data[16 + 32 * i:48 + 32 * i]) for i in range(count)]


def write_shapes(mask, rows):
    return struct.pack("<QII", mask, len(rows), 0) + b"".join(bytes(r) for r in rows)


def read_brat(data):
    """CPU weights: 16-byte head, u32 rows, rows {u32 command, 6 x {u16 offset, u16 count}}, u32 pool size, pool."""
    head, count = data[:16], u32(data, 16)
    rows = []
    for index in range(count):
        command = u32(data, 20 + 28 * index)
        rows.append((command, [struct.unpack_from("<HH", data, 24 + 28 * index + 4 * k) for k in range(6)]))
    pool_at = 20 + 28 * count
    pool = data[pool_at + 4:pool_at + 4 + u32(data, pool_at)]
    return head, rows, bytearray(pool)


def write_brat(head, rows, pool):
    out = bytearray(head) + struct.pack("<I", len(rows))
    for command, situations in rows:
        out += struct.pack("<I", command)
        for offset, count in situations:
            out += struct.pack("<HH", offset, count)
    out += struct.pack("<I", len(pool)) + pool
    return bytes(out + bytes(-len(out) % 4))


# ---------------------------------------------------------------------------------------------
# The original's tables, converted

# Event types whose argument count grew in 012: (ranged, type) -> 012's count. The original's
# arguments are the first ones and 012 adds zeros (every matched pair over 22 fighters).
ARGC_012 = {(False, 0x10): 13, (False, 0x15): 3, (False, 0x25): 9,
            (True, 0x11): 8, (True, 0x18): 5, (True, 0x1E): 4, (True, 0x43): 7, (True, 0x45): 1,
            (True, 0x24): 5, (True, 0x3C): 2}


def og_plya(row):
    if len(row) != 0x70:
        raise MoveError("plya: an original record is 0x70 bytes")
    flags = u32(row, 0x14)
    out = bytearray(0x74)
    out[0x00:0x0F] = row[0x00:0x0F]
    out[0x0F] = (flags >> 13) & 3
    out[0x10:0x12] = row[0x10:0x12]
    out[0x12:0x14] = b"\xff\xff"
    out[0x14:0x16] = row[0x12:0x14]
    put32(out, 0x18, flags & ~0x6000)
    out[0x1C:0x74] = row[0x18:0x70]
    return out


def og_atkp(row):
    if len(row) != 0x3C:
        raise MoveError("atkp: an original record is 0x3C bytes")
    out = bytearray(0x44)
    out[0x00:0x06] = row[0x00:0x06]
    out[0x06] = 0xFF
    put32(out, 0x08, u16(row, 0x06))
    out[0x0C:0x14] = row[0x08:0x10]
    out[0x18:0x44] = row[0x10:0x3C]
    return out


def og_comb(data):
    if u32(data, 0) != 4:
        raise MoveError("comb: the original's is version 4")
    entries = []
    for index in range(u32(data, 4)):
        at = 8 + index * 0x48
        command, word2, alt, prerequisite = struct.unpack_from("<hhhh", data, at)
        stages = [bytearray(data[at + 8 + 8 * k:at + 16 + 8 * k]) for k in range(8)]
        entries.append({"command": command, "alt": alt, "prerequisite": prerequisite,
                        "stages": [s for s in stages if u16(s, 0) != 0xFFFF]})
    return entries


def og_origin(origin, target):
    """012 renumbered the spawn origin (a new type at 2) and the aim target, and merged some
    pairs: 746 of 817 shared projectiles."""
    new_origin = origin if origin < 2 else origin + 1
    new_target = 3 if target == 2 else target
    if target == 2 and origin == 2:
        new_origin = 11
    if target == 2 and origin in (3, 8):
        new_origin = 12
    if target == 2 and origin == 7:
        new_target = 2
    return new_origin, new_target


def og_magi(data):
    if data[:4] != b"MAGI":
        raise MoveError("magi: no MAGI magic")
    count = u32(data, 8)
    rows = []
    for index in range(count):
        row = data[16 + index * 0x268:16 + (index + 1) * 0x268]
        head = bytearray(row[:0x40])
        origin, target = og_origin(u16(head, 0x1E), u16(head, 0x26))
        put16(head, 0x1E, origin)
        put16(head, 0x26, target)
        values = struct.unpack_from(f"<{3 * MAGI_PARAMS}f", row, 0x40)
        rows.append({"head": head, "params": [tuple(values[3 * p:3 * p + 3]) for p in range(MAGI_PARAMS)],
                     "tail": bytes(2), "indices": None})
    return rows


def og_fepa_row(row):
    if len(row) != 248:
        raise MoveError("FEPA: an original record is 248 bytes")
    out = bytearray(260)
    out[0:0x54] = row[0:0x54]
    out[0x58:0xF8] = row[0x54:0xF4]
    out[0xF8:0xFC] = row[0xF4:0xF8]     # 1 in every original record; +0xFC and +0x100 are 0 in every 012 record
    return out


def og_tracks(data):
    version, tracks = read_tracks(data)
    for track in tracks:
        for ranged, events in ((True, track["ranged"]), (False, track["instant"])):
            for head, args in events:
                want = ARGC_012.get((ranged, event_type(head, ranged)))
                if want is not None and len(args) < want:
                    args.extend([0] * (want - len(args)))
    return version, tracks


def fepa_rows(data, size):
    if data[:4] != b"FEPA":
        raise MoveError("no FEPA magic")
    return records(data, size, head=16, count_at=12)


def write_fepa(version, rows):
    body = b"".join(bytes(r) for r in rows)
    return b"FEPA" + struct.pack("<III", version, 16 + len(body), len(rows)) + body


FEPA_VERSION_012 = 0x20100205


# ---------------------------------------------------------------------------------------------
# Models (Sony GMO): just enough to add motions


GMO_MAGIC = b"OMG.00.1PSP\0\0\0\0\0"
GMO_MODEL, GMO_MOTION = 0x0003, 0x000B


class GmoChunk:
    def __init__(self, data, pos):
        self.type, header, size = struct.unpack_from("<HHI", data, pos)
        self.raw = bytes(data[pos:pos + size])
        self.children = []
        if not self.type & 0x8000:
            child, data_at = struct.unpack_from("<II", data, pos + 8)
            self.head = bytes(data[pos:pos + child])
            cursor = pos + child
            while cursor < pos + size:
                kid = GmoChunk(data, cursor)
                self.children.append(kid)
                cursor += len(kid.raw)

    @property
    def name(self):
        if self.type & 0x8000:
            return None
        header = u16(self.raw, 2)
        return self.raw[16:header].split(b"\0")[0].decode("latin-1")

    def to_bytes(self):
        if self.type & 0x8000 or not self.children:
            return self.raw
        kids = b"".join(kid.to_bytes() for kid in self.children)
        head = bytearray(self.head)
        put32(head, 4, len(head) + len(kids))
        return bytes(head) + kids


def gmo_read(data):
    if data[:16] != GMO_MAGIC:
        raise MoveError("not a GMO model")
    return GmoChunk(data, 16)


def gmo_model(root):
    for chunk in root.children:
        if chunk.type == GMO_MODEL and any(k.type == GMO_MOTION for k in chunk.children):
            return chunk
    raise MoveError("the model has no motions")


def gmo_motions(root):
    return [k for k in gmo_model(root).children if k.type == GMO_MOTION]


def renamed_motion(chunk, name):
    """A copy of a motion chunk under another name (the header grows or shrinks with the name)."""
    raw = chunk.raw
    header, size, child, data_at = u16(raw, 2), u32(raw, 4), u32(raw, 8), u32(raw, 12)
    name_bytes = name.encode("latin-1") + b"\0"
    name_bytes += bytes(-len(name_bytes) % 4)
    shift = 16 + len(name_bytes) - header
    head = bytearray(raw[:16])
    put16(head, 2, header + shift)
    put32(head, 4, size + shift)
    put32(head, 8, child + shift)
    put32(head, 12, data_at + shift)
    copy = GmoChunk.__new__(GmoChunk)
    copy.type, copy.children = chunk.type, []
    copy.raw = bytes(head) + name_bytes + raw[header:]
    return copy


def gmo_append_motions(data, motions):
    """The model with `motions` (chunks) added after its last motion."""
    root = gmo_read(data)
    model = gmo_model(root)
    last = max(i for i, k in enumerate(model.children) if k.type == GMO_MOTION)
    model.children[last + 1:last + 1] = motions
    return GMO_MAGIC + root.to_bytes()


# ---------------------------------------------------------------------------------------------
# A fighter's moveset, decoded


class Fighter:
    """A 012-format moveset archive with its tables decoded."""

    def __init__(self, data):
        self.arc = Arc(data)
        get = self.arc.get
        self.plya_head = get(b"plya")[:8]
        self.attacks = records(get(b"plya"), 0x74)
        self.hits_head = get(b"atkp")[:8]
        self.hits = records(get(b"atkp"), 0x44)
        self.combos = read_comb(get(b"comb"))
        self.magi_version, self.projectiles, self.pool = read_magi(get(b"magi"))
        self.motion_head = get(kind=7)[:8]
        self.motions = read_motions(get(kind=7))
        self.track_version, self.tracks = read_tracks(get(kind=8))
        self.shape_mask, self.shapes = read_shapes(get(kind=5))
        self.brat = read_brat(get(b"brat"))
        self.effe = Mpk(get(b"effe"))
        self.fepa_version = u32(self.effe.get(".feb"), 4)
        self.fepa = fepa_rows(self.effe.get(".feb"), 260)
        self.fep = effect_package.Package(self.effe.get(".fep"))
        self.fxse_version = u32(self.effe.get(".fxseb"), 4)
        self.fxse = fxse_rows(self.effe.get(".fxseb"))
        self.text = get(b"text")
        self.bank = SoundBank(get(kind=14))

    def to_bytes(self):
        self.arc.set(join_records(struct.pack("<II", u32(self.plya_head, 0), len(self.attacks)), self.attacks), b"plya")
        self.hits.sort(key=lambda r: u32(r, 0))
        self.arc.set(join_records(struct.pack("<II", u32(self.hits_head, 0), len(self.hits)), self.hits), b"atkp")
        self.arc.set(write_comb(self.combos), b"comb")
        self.arc.set(write_magi(self.magi_version, self.projectiles, self.pool), b"magi")
        self.arc.set(write_motions(self.motion_head, self.motions), kind=7)
        self.arc.set(write_tracks(self.track_version, self.tracks), kind=8)
        self.arc.set(write_shapes(self.shape_mask, self.shapes), kind=5)
        self.arc.set(write_brat(*self.brat), b"brat")
        self.effe.set(".feb", write_fepa(self.fepa_version, self.fepa))
        self.effe.set(".fep", self.fep.to_bytes())
        self.effe.set(".fxseb", write_fxse(self.fxse_version, self.fxse))
        self.arc.set(self.effe.to_bytes(), b"effe")
        self.arc.set(self.text, b"text")
        self.arc.set(bytes(self.bank.data), kind=14)
        return self.arc.to_bytes()


class Original:
    """The original game's archive of the same fighter, converted table by table to 012's formats."""

    def __init__(self, data, gmo=None):
        self.arc = Arc(data)
        get = self.arc.get
        if u32(get(b"plya"), 0) != 0x14:
            raise MoveError("plya: not the original's version 0x14")
        self.attacks = [og_plya(r) for r in records(get(b"plya"), 0x70)]
        self.hits = [og_atkp(r) for r in records(get(b"atkp"), 0x3C)]
        self.combos = og_comb(get(b"comb"))
        self.projectiles = og_magi(get(b"magi"))
        self.motions = read_motions(get(kind=7))
        self.track_version, self.tracks = og_tracks(get(kind=8))
        self.shape_mask, self.shapes = read_shapes(get(kind=5))
        self.effe = Mpk(get(b"effe"))
        self.fepa = [og_fepa_row(r) for r in fepa_rows(self.effe.get(".feb"), 248)]
        self.fep = effect_package.Package(self.effe.get(".fep"))
        self.fxse = fxse_rows(self.effe.get(".fxseb"))
        self.text = get(b"text")
        self.bank = SoundBank(get(kind=14))
        self.effect_names = dict(enumerate(name for _, name in self.fep.names()))
        self.effect_from = {}           # no stand-ins: effects the 012 owners lack come from this package
        self.sounds = None              # no plan: sounds come from this bank
        self.banners = [table_strings(t) for t in mlng_tables(self.text)]
        self._chunks = gmo_motions(gmo_read(gmo)) if gmo is not None else []

    def anim_name(self, anim):
        return self._chunks[anim].name

    def anim_chunk(self, anim, name):
        return renamed_motion(self._chunks[anim], name)


class EffectSource:
    """Another 012 fighter's effects, sound records and sound bank: where an effect the original
    Bartz borrowed from that fighter is copied from (its 012 version)."""

    def __init__(self, label, data):
        arc = Arc(data)
        effe = Mpk(arc.get(b"effe"))
        self.label = label
        self.fep = effect_package.Package(effe.get(".fep"))
        self.fxse = fxse_rows(effe.get(".fxseb"))
        self.bank = SoundBank(arc.get(kind=14))
        self.by_stem = {effect_stem(name): (i, name) for i, (_, name) in enumerate(self.fep.names())}


def effect_stem(name):
    """An effect's name without its package's prefix (P0041_SOUL_EATER_BODY_LEAF_0 -> SOUL_EATER_BODY_LEAF_0)."""
    return re.sub(r"^P\d{4}_", "", name)


# ---------------------------------------------------------------------------------------------
# Effect names, sound clips and banner strings: what can be matched between the two packages


def fep_names(fep):
    """{effect index: name} of an effect package (FEP). The header's +0x44 points at {u32 n, ptr},
    which points at n x {u32 id, u32 length, ptr name}; offsets are file offsets as stored."""
    if fep[:4] != b"FEP\xcd":
        raise MoveError("not a FEP effect package")
    table = u32(fep, 0x44)
    count, entries = u32(fep, table), u32(fep, table + 4)
    names = {}
    for index in range(count):
        name_at = u32(fep, entries + 12 * index + 8)
        names[index] = fep[name_at:fep.index(b"\0", name_at)].decode("latin-1")
    return names


def sscf_slots(bank):
    """[(kind, clip bytes)] of a sound bank (`SSCF`; tools/sscf_bank.py has the layout)."""
    if bank[:4] != b"SSCF":
        raise MoveError("not an SSCF sound bank")
    out = []
    for slot in range(u32(bank, 12)):
        at = 0x20 + 32 * slot
        kind, offset, size = struct.unpack_from("<III", bank, at)
        out.append((kind, bank[offset:offset + size] if kind == 0 else bank[at:at + 32]))
    return out


def sscf_number(bank):
    return u16(bank, 0x12) or u32(bank, 0x14)


SSCF_EMPTY = 0x00200000
SSCF_DATA_KINDS = (0, 0x00010000)     # slots whose entry gives an offset and a size


class SoundBank:
    """A fighter's sound effect bank (`SSCF`), filled by putting clips into its empty slots: the
    entries keep their places and new clip data goes at the end, so nothing else moves."""

    def __init__(self, data):
        if data[:4] != b"SSCF":
            raise MoveError("not an SSCF sound bank")
        self.data = bytearray(data)

    @property
    def number(self):
        return sscf_number(self.data)

    def entry(self, slot):
        return struct.unpack_from("<8I", self.data, 0x20 + 32 * slot)

    def clip(self, slot):
        kind, offset, size = self.entry(slot)[:3]
        return bytes(self.data[offset:offset + size]) if kind in SSCF_DATA_KINDS else None

    def slots(self):
        return u32(self.data, 12)

    def add(self, entry, clip):
        """Put a clip (with the source slot's entry words) into the first empty slot; its slot."""
        for slot in range(self.slots()):
            if self.entry(slot)[0] == SSCF_EMPTY:
                break
        else:
            raise MoveError("the sound bank has no empty slot left")
        self.data += bytes(-len(self.data) % 16)
        offset = len(self.data)
        self.data += clip
        words = list(entry)
        words[1], words[2] = offset, len(clip)
        struct.pack_into("<8I", self.data, 0x20 + 32 * slot, *words)
        put32(self.data, 8, len(self.data))
        return slot

    def find(self, clip):
        for slot in range(self.slots()):
            if self.clip(slot) == clip:
                return slot
        return None


def bring_sound(bank, source, slot):
    """Slot of `bank` holding `source`'s clip `slot` (an equal clip, or a copy in an empty slot)."""
    clip = source.clip(slot)
    if clip is None:
        return None
    found = bank.find(clip)
    return found if found is not None else bank.add(source.entry(slot), clip)


FXSE_RECORD = 320
FXSE_VERSION = 0x20070422


def fxse_rows(data):
    if data[:4] != b"FXSE":
        raise MoveError("no FXSE magic")
    return records(data, FXSE_RECORD, head=16, count_at=12)


def write_fxse(version, rows):
    body = b"".join(bytes(r) for r in rows)
    return b"FXSE" + struct.pack("<III", version, 16 + len(body), len(rows)) + body


def fxse_moved(row, source_bank, target_bank, own_cue, report):
    """An effect's sound record (16 x {u32 bank x 100, u32 slot, u32, u32, u32}) for another
    fighter: cues on the source fighter's bank are copied into the target's; shared banks stay."""
    row = bytearray(row)
    for k in range(16):
        at = 20 * k
        cue_bank, slot = u32(row, at), u32(row, at + 4)
        if cue_bank == 0 or cue_bank != source_bank.number * 100:
            continue
        new = bring_sound(target_bank, source_bank, slot)
        if new is None:
            report.leave("effect sounds that could not come", f"{source_bank.number}:{slot}")
            row[at:at + 20] = bytes(20)
        else:
            put32(row, at, own_cue)
            put32(row, at + 4, new)
    return row


def mlng_tables(chunk):
    if chunk[:4] != b"mlng":
        raise MoveError("text: not an mlng container")
    count = u32(chunk, 4)
    offsets = struct.unpack_from(f"<{count}I", chunk, 8)
    sizes = struct.unpack_from(f"<{count}I", chunk, 8 + 4 * count)
    return [chunk[o:o + s] for o, s in zip(offsets, sizes)]


def mlng_build(tables):
    """The container as the game's files have it: every table stored, back to back."""
    count = len(tables)
    position = 8 + 8 * count
    offsets = []
    for table in tables:
        offsets.append(position)
        position += len(table)
    sizes = [len(t) for t in tables]
    return b"mlng" + struct.pack(f"<I{count}I{count}I", count, *offsets, *sizes) + b"".join(tables)


def table_strings(table):
    """The strings of one message table, unmasked: [bytes or None]."""
    count = u16(table, 0)
    out = []
    for index in range(count):
        at = u16(table, 2 + 2 * index) * 2
        if not at:
            out.append(None)
            continue
        size = u16(table, at)
        out.append(_mask(table[at + 2:at + 2 + size], index))
    return out


def table_build(strings):
    start = 2 + 2 * len(strings)
    offsets, body = [], bytearray()
    for index, text in enumerate(strings):
        if text is None:
            offsets.append(0)
            continue
        offsets.append((start + len(body)) // 2)
        body += struct.pack("<H", len(text)) + _mask(text, index)
    return struct.pack(f"<H{len(strings)}H", len(strings), *offsets) + bytes(body)


# The game's key for hiding message text (main 0x089D68AA), as in tools/character_text.py.
TEXT_KEY = (0xED98, 0x90AB, 0x5E28, 0x8442, 0x32B1, 0x2DDA, 0xA657, 0xC8BF,
            0x0033, 0x579A, 0xF227, 0x4B4C, 0x36A7, 0x927D, 0x739D, 0x84CE)


def _mask(text, index):
    units = struct.unpack(f"<{len(text) // 2}H", text)
    return struct.pack(f"<{len(units)}H", *(u ^ TEXT_KEY[(index + k) & 15] for k, u in enumerate(units)))


# ---------------------------------------------------------------------------------------------
# Bringing original commands into a 012 fighter

# Event arguments that name things the copy renumbers: (ranged, type) -> [(argument, what)].
EVENT_REFS = {
    (True, 0x00): [(0, "hit"), (1, "shape")], (True, 0x43): [(0, "hit"), (1, "shape"), (2, "projectile")],
    (True, 0x7B): [(0, "hit"), (1, "shape")], (True, 0x12): [(0, "shape")], (True, 0x4E): [(0, "shape")],
    (True, 0x20): [(0, "hit")], (True, 0x78): [(1, "hit")], (True, 0x5B): [(0, "hit")],
    (True, 0x67): [(1, "attack")], (True, 0x79): [(1, "attack")], (True, 0x24): [(0, "projectile")],
    (True, 0x66): [(0, "effect")], (True, 0x35): [(0, "effect")],
    (True, 0x2A): [(2, "key")], (True, 0x5F): [(2, "key")],
    (False, 0x08): [(0, "hit")], (False, 0x04): [(0, "projectile")], (False, 0x29): [(0, "projectile")],
    (False, 0x2D): [(0, "projectile"), (2, "projectile")], (False, 0x33): [(0, "projectile")],
    (False, 0x19): [(0, "effect")], (False, 0x2F): [(0, "effect"), (2, "effect")], (False, 0x30): [(0, "effect")],
    (False, 0x03): [(0, "sound")], (False, 0x11): [(1, "sound")], (False, 0x12): [(1, "sound")],
    (False, 0x1D): [(0, "banner")],
}
# Projectile links (s16, -1 none): emit, terrain x5, hit, fall, clash.
MAGI_LINKS = range(0x0E, 0x1E, 2)
NEW_MOTION_OFFSET = 5000           # the original's attack motions 10000.. become 15000..
# Hitbox groups. A moveset switches groups by id with 64-bit masks; the game makes a group's bit
# with a 64-bit shift (main 0x08837D18 -> 0x089985B0), so an id of 64 or more acts as id - 32
# and would switch a 012 group. 012 Bartz uses ids 0..55 and no fighter uses 63, so the
# original's own groups are stored "parked": record byte 0 = 63 (a bit nothing sets), byte 1 =
# the group's number (64..255; bytes 1-3 are 0 in every retail record), and their events carry
# 63 + 256 x group, whose shift also lands on bit 63. At battle start the port moves the groups
# of the equipped commands onto free ids (port/host/d012_shape_ids.cpp).
PLACED_AS_012 = ("WEP_APPEAR_",)     # effects whose placement is taken from 012 Bartz's own records
KEYED_RECORDS = (9999, 8888)        # effect records a bone_effect event picks by key bit: trails, weapon glows
PARKED_SHAPE = 63
FIRST_VIRTUAL_SHAPE = 64
FIRST_NEW = {"attack": 3100, "hit": 3400, "projectile": 2300, "fepa": 200, "command": 1100}


class Report:
    def __init__(self):
        self.lines = []
        self.left = {}
        self.came = {}

    def note(self, text):
        self.lines.append(text)

    def leave(self, what, item):
        self.left.setdefault(what, set()).add(item)

    def tally(self, what, item):
        self.came.setdefault(what, set()).add(item)


def bring(base, og, base_anims, commands, report, swap=False, sources=()):
    """Add the original's `commands` to `base` (a Fighter; `og` an Original or a Recipe). `commands` maps an
    original command id to the new command id (or, with `swap`, to the 012 command whose stages
    it takes). `base_anims` is the number of motions in the fighter's models. Returns the GMO
    motion chunks to append to every model of the fighter, in order."""
    og_attacks = {u16(r, 0): r for r in og.attacks}
    og_hits = {u32(r, 0): r for r in og.hits}
    og_magi = {u16(r["head"], 0): r for r in og.projectiles}
    og_motion = {u16(r, 0): r for r in og.motions}
    og_track = {t["anim"]: t for t in og.tracks}
    og_combo = {c["command"]: c for c in og.combos}

    # 1. What the commands use: attacks (and the follow-ups their events offer), motions, tracks.
    attacks, order = set(), []
    pending = [u16(stage, 0) for command in commands for stage in og_combo[command]["stages"]]
    motions = []
    while pending:
        attack = pending.pop(0)
        if attack in attacks or attack not in og_attacks:
            continue
        attacks.add(attack)
        order.append(attack)
        row = og_attacks[attack]
        if s16(row, 0x0A) >= 0:
            pending.append(s16(row, 0x0A))
        for motion in (u16(row, 0x1C), s16(row, 0x10)):
            if motion >= 10000 and motion in og_motion and motion not in motions:
                motions.append(motion)
                for ranged, events in ((True, og_track.get(u16(og_motion[motion], 2), {}).get("ranged", [])),
                                       (False, og_track.get(u16(og_motion[motion], 2), {}).get("instant", []))):
                    for head, args in events:
                        for index, what in EVENT_REFS.get((ranged, event_type(head, ranged)), []):
                            if what == "attack" and index < len(args):
                                pending.append(args[index])
    anims = []
    for motion in motions:
        anim = u16(og_motion[motion], 2)
        if anim not in anims:
            anims.append(anim)

    base_motion_ids = {u16(r, 0) for r in base.motions}
    anim_map = {anim: base_anims + k for k, anim in enumerate(anims)}
    motion_map = {}
    for motion in motions:
        new = motion + NEW_MOTION_OFFSET
        if new in base_motion_ids or new > 0xFFFF:
            raise MoveError(f"motion {new} is taken")
        motion_map[motion] = new

    # 2. Hits, projectiles, shapes, effects, sounds, banners the tracks and projectiles use.
    def allocate(kind, used):
        value = max([FIRST_NEW[kind] - 1, *[u for u in used if u < 8000]]) + 1
        used.add(value)
        return value

    used_attacks = {u16(r, 0) for r in base.attacks}
    attack_map = {a: allocate("attack", used_attacks) for a in order}

    base_hits = {u32(r, 0): r for r in base.hits}
    used_hits = set(base_hits)
    hit_map = {}

    def hit(old):
        if old in hit_map:
            return hit_map[old]
        if old not in og_hits:                      # a shared hit the game finds in battle.bin
            hit_map[old] = old
        elif old in base_hits and bytes(base_hits[old]) == bytes(og_hits[old]):
            hit_map[old] = old
        else:
            new = allocate("hit", used_hits)
            row = bytearray(og_hits[old])
            put32(row, 0, new)
            if row[0x0C] != 0xFF and row[0x0C] < 64:   # its contact effect: an effect group bit
                row[0x0C] = group(row[0x0C])
            base.hits.append(row)
            hit_map[old] = new
        return hit_map[old]

    base_groups = {}
    for row in base.shapes:
        base_groups.setdefault(row[0], []).append(bytes(row[1:]))
    by_content = {tuple(sorted(v)): k for k, v in base_groups.items()}
    og_groups = {}
    for row in og.shapes:
        og_groups.setdefault(row[0], []).append(row)
    shape_map = {}
    if any(row[0] == PARKED_SHAPE for row in base.shapes):
        raise MoveError(f"hitbox id {PARKED_SHAPE} is in use; it is needed for the original's groups")
    next_group = [FIRST_VIRTUAL_SHAPE]

    def shape(old):
        """A hitbox group of the original: the 012 id of a group with the same spheres, else a
        group of its own, parked (see PARKED_SHAPE). Returns the value its events carry."""
        if old in shape_map:
            return shape_map[old]
        rows = og_groups.get(old)
        if not rows:
            shape_map[old] = old
            return old
        key = tuple(sorted(bytes(r[1:]) for r in rows))
        if key in by_content:
            shape_map[old] = by_content[key]
        else:
            group = next_group[0]
            if group > 0xFF:
                raise MoveError("more than 192 hitbox groups of the original's")
            next_group[0] += 1
            for row in rows:
                copy = bytearray(row)
                copy[0], copy[1] = PARKED_SHAPE, group
                base.shapes.append(copy)
            by_content[key] = PARKED_SHAPE + 256 * group
            shape_map[old] = by_content[key]
            report.tally("hitbox groups of the original's own (placed by the port at battle start)", group)
        return shape_map[old]

    # Effects: an original placement record (FEPA) is brought when 012's package has an effect of
    # the same name; it gets a new id, and its group bits move to bits 012's package leaves free.
    og_names = og.effect_names
    base_names = {name: index for index, (_, name) in enumerate(base.fep.names())}
    own_cue = base.bank.number * 100

    def ensure_effect(name):
        """The name index of effect `name` in 012 Bartz's package, copying it in when he lacks it:
        from the 012 fighter it was borrowed from (with that fighter's sounds), else from the
        original Bartz's package."""
        if name in base_names:
            return base_names[name]
        if len(base.fxse) != len(base_names):
            raise MoveError("the effect package and its sound records disagree")
        stand_in = og.effect_from.get(name)
        if stand_in is not None:                    # a 012 fighter's effect in place of a Bartz-only one
            source = next(s for s in sources if s.label == stand_in[0])
            index, source_name = source.by_stem[effect_stem(stand_in[1])]
        else:
            source = next((s for s in sources if effect_stem(name) in s.by_stem), None)
            if source is not None:
                index, source_name = source.by_stem[effect_stem(name)]
        if source is not None:
            package, records, bank, label = source.fep, source.fxse, source.bank, source.label
        elif getattr(og, "fep", None) is None:
            raise MoveError(f"effect {name}: no 012 fighter has it and the recipe names no stand-in")
        else:
            index = next(i for i, n in og_names.items() if n == name)
            source_name, package, records, bank, label = name, og.fep, og.fxse, og.bank, "the original"
        new_index, _ = effect_package.copy_effect(base.fep, package, source_name, name)
        base.fxse.append(fxse_moved(records[index], bank, base.bank, own_cue, report))
        base_names[name] = new_index
        report.tally(f"effects from {label}", name)
        return new_index
    used_fepa = {u32(r, 0x24) for r in base.fepa}
    own_rows = [bytes(r) for r in base.fepa]
    used_bits = set()
    for row in base.fepa:
        mask = u32(row, 0x3C) | u32(row, 0x40) << 32
        used_bits |= {b for b in range(64) if mask >> b & 1}
    bit_map, fepa_map, brought_rows, groups_done = {}, {}, set(), set()
    # Weapon trails and glows (records 9999 and 8888): a bone_effect event takes the first record
    # of that id with its key bit (+0x44). 012 Bartz's own records use the same bits and come
    # first, so the original's records get bits his leave free.
    key_used, key_map = {}, {}
    for row in base.fepa:
        if u32(row, 0x24) in KEYED_RECORDS:
            key_used.setdefault(u32(row, 0x24), set()).update(b for b in range(32) if u32(row, 0x44) >> b & 1)

    def key_bit(record, old):
        if (record, old) not in key_map:
            free = [b for b in range(32) if b not in key_used.setdefault(record, set())]
            if not free:
                raise MoveError(f"no free key bit for effect record {record}")
            key_map[(record, old)] = free[0]
            key_used[record].add(free[0])
        return key_map[(record, old)]
    renames = {og.anim_name(anim): og.anim_name(anim) + "_OG" for anim in anims}

    def bit(old):
        if old not in bit_map:
            free = [b for b in range(64) if b not in used_bits]
            if not free:
                raise MoveError("no free effect group bit")
            bit_map[old] = free[0]
            used_bits.add(free[0])
        return bit_map[old]

    def bring_row(index, motion_name=None):
        """Copy original placement record `index`: its effect by name, a new id per original id,
        its group bits moved to bits of their own."""
        if index in brought_rows:
            return True
        old_row = og.fepa[index]
        effect_name = og_names.get(u32(old_row, 0x28))
        if effect_name is None:
            report.leave("placements naming no effect", index)
            return False
        row = bytearray(old_row)
        put32(row, 0x28, ensure_effect(effect_name))
        if effect_stem(effect_name).startswith(PLACED_AS_012):
            # The flash a weapon appears and leaves with: placed and sized as 012 Bartz's own
            # record of it on the same weapon bone (the size alone when he has none on that bone).
            mine = [r for r in own_rows if u32(r, 0x28) == u32(row, 0x28)]
            same_bone = [r for r in mine if r[0xA8:0xC8] == row[0xA8:0xC8]]
            if same_bone:
                row[0x00:0x24] = same_bone[0][0x00:0x24]
            elif mine:
                row[0x18:0x24] = mine[0][0x18:0x24]
        mask = u32(row, 0x3C) | u32(row, 0x40) << 32
        new_mask = 0
        for b in range(64):
            if mask >> b & 1:
                new_mask |= 1 << bit(b)
        put32(row, 0x3C, new_mask & 0xFFFFFFFF)
        put32(row, 0x40, new_mask >> 32)
        bound = bytes(row[0xC8:0xE8]).split(bytes(1))[0].decode("latin-1")
        motion_name = motion_name or renames.get(bound)
        if motion_name is not None:
            row[0xC8:0xE8] = motion_name.encode("latin-1").ljust(32, bytes(1))[:32]
        old_id = u32(old_row, 0x24)
        if old_id in KEYED_RECORDS:
            keys = 0
            for b in range(32):
                if u32(row, 0x44) >> b & 1:
                    keys |= 1 << key_bit(old_id, b)
            put32(row, 0x44, keys)
        if old_id not in fepa_map:
            fepa_map[old_id] = old_id if old_id >= 8000 else allocate("fepa", used_fepa)
        put32(row, 0x24, fepa_map[old_id])
        base.fepa.append(row)
        brought_rows.add(index)
        return True

    def effect(old):
        """The new id of original placement record id `old`, or None when none of its records came."""
        rows = [i for i, r in enumerate(og.fepa) if u32(r, 0x24) == old]
        if not rows:
            report.leave("effect records the original lacks", old)
            return None
        came = [bring_row(i) for i in rows]
        return fepa_map.get(old) if any(came) else None

    def group(old):
        """An original effect group bit (a projectile's look, a hit's contact effect): every record
        with the bit comes along once, and the bit is renumbered."""
        if old not in groups_done:
            groups_done.add(old)
            for i, r in enumerate(og.fepa):
                if (u32(r, 0x3C) | u32(r, 0x40) << 32) >> old & 1 and u32(r, 0x34) == 0:
                    bring_row(i)
        return bit(old)

    bank_base = base.bank.number * 100
    sound_map, sound_slots = {}, set()

    def sound(old):
        """A move's sound: the same slot when 012 Bartz's clip there is the original's, else the
        original's clip put into an empty slot of his bank. A recipe says where each comes from:
        "same" (his own slot) or (012 fighter, slot)."""
        if old in sound_map:
            return sound_map[old]
        slot = old - bank_base
        if og.sounds is not None:
            plan = og.sounds.get(slot)
            if plan is None or plan == "same":
                sound_map[old] = old
            else:
                owner = next(s for s in sources if s.label == plan[0])
                new = bring_sound(base.bank, owner.bank, plan[1])
                if new is None:
                    report.leave("sound slots that could not come", slot)
                else:
                    report.tally(f"move sounds from {plan[0]}", slot)
                sound_map[old] = None if new is None else bank_base + new
            return sound_map[old]
        if 0 <= slot < og.bank.slots():
            sound_slots.add(slot)
        if not 0 <= slot < og.bank.slots():
            sound_map[old] = old                    # a shared bank's sound
        elif slot < base.bank.slots() and base.bank.clip(slot) == og.bank.clip(slot):
            sound_map[old] = old
        else:
            new = bring_sound(base.bank, og.bank, slot)
            if new is None:
                report.leave("sound slots that could not come", slot)
            else:
                report.tally("move sounds from the original", slot)
            sound_map[old] = None if new is None else bank_base + new
        return sound_map[old]

    base_tables = mlng_tables(base.text)
    base_strings = [table_strings(t) for t in base_tables]
    og_strings = og.banners
    banner_map = {}

    def banner(old):
        if old in banner_map:
            return banner_map[old]
        if old == 0xFF or old >= len(og_strings[0]):
            banner_map[old] = old
            return old
        new = len(base_strings[0])
        for k, strings in enumerate(base_strings):
            source = og_strings[k if k < len(og_strings) else 0]
            strings.append(source[old] if old < len(source) else og_strings[0][old])
        banner_map[old] = new
        return new

    used_magi = {u16(r["head"], 0) for r in base.projectiles}
    magi_map = {}

    def projectile(old):
        if old in magi_map:
            return magi_map[old]
        if old not in og_magi:
            report.leave("projectiles the original lacks", old)
            magi_map[old] = old
            return old
        new = allocate("projectile", used_magi)
        magi_map[old] = new
        source = og_magi[old]
        row = {"head": bytearray(source["head"]), "params": source["params"], "tail": bytes(2), "indices": None}
        put16(row["head"], 0, new)
        for at in MAGI_LINKS:
            link = s16(row["head"], at)
            if link >= 0:
                put16(row["head"], at, projectile(link))
        for at in (0x0A, 0x0C):
            if u16(row["head"], at) != 0xFFFF:
                put16(row["head"], at, hit(u16(row["head"], at)))
        look = u16(row["head"], 0x04)
        if look != 0xFFFF and look < 64:
            put16(row["head"], 0x04, group(look))
        if u16(row["head"], 0x1E) == 6:               # starts at an effect record
            target = effect(u16(row["head"], 0x20))
            if target is not None:
                put16(row["head"], 0x20, target)
        base.projectiles.append(row)
        return new

    # 3. Attack records.
    for old in order:
        row = bytearray(og_attacks[old])
        put16(row, 0, attack_map[old])
        if s16(row, 0x0A) >= 0 and s16(row, 0x0A) in attack_map:
            put16(row, 0x0A, attack_map[s16(row, 0x0A)])
        for at in (0x10, 0x1C, 0x1E):
            if s16(row, at) in motion_map:
                put16(row, at, motion_map[s16(row, at)])
        if row[0x21] != 0xFF:
            row[0x21] = banner(row[0x21])
        base.attacks.append(row)

    # 4. Motions, tracks and the model's animations.
    for motion in motions:
        row = bytearray(og_motion[motion])
        put16(row, 0, motion_map[motion])
        put16(row, 2, anim_map[u16(row, 2)])
        base.motions.append(row)
    chunks = []
    for anim in anims:
        og_name = og.anim_name(anim)
        name = og_name + "_OG"
        chunks.append(og.anim_chunk(anim, name))
        for index, old_row in enumerate(og.fepa):   # records that start with the animation itself
            bound = old_row[0xC8:0xE8].split(b"\0")[0].decode("latin-1")
            if bound and bound == og_name:
                bring_row(index, name)
        source = og_track.get(anim)
        if source is None:
            continue
        track = {"anim": anim_map[anim], "ranged": [], "instant": []}
        for ranged, key in ((True, "ranged"), (False, "instant")):
            for head, args in source[key]:
                args = list(args)
                keep = True
                for index, what in EVENT_REFS.get((ranged, event_type(head, ranged)), []):
                    if index >= len(args):
                        continue
                    value = args[index]
                    if what == "hit":
                        args[index] = hit(value)
                    elif what == "shape":
                        args[index] = shape(value)
                    elif what == "attack":
                        args[index] = attack_map.get(value, value)
                    elif what == "projectile":
                        args[index] = projectile(value)
                    elif what == "effect":
                        new = effect(value)
                        keep = keep and new is not None
                        args[index] = new if new is not None else value
                    elif what == "sound":
                        new = sound(value)
                        keep = keep and new is not None
                        args[index] = new if new is not None else value
                    elif what == "banner":
                        args[index] = banner(value)
                    elif what == "key" and args[1] in KEYED_RECORDS:
                        rows = [i for i, r in enumerate(og.fepa) if u32(r, 0x24) == args[1]
                                and u32(r, 0x44) >> value & 1 and u32(r, 0x34) == 0]
                        if all([bring_row(i) for i in rows]) and rows:
                            args[index] = key_bit(args[1], value)
                if keep:
                    track[key].append([bytearray(head), args])
                else:
                    report.leave("events dropped (their effect or sound did not come)", f"anim {anim} type {event_type(head, ranged):#x}")
        base.tracks.append(track)

    # 5. Commands and CPU rows.
    brat_head, brat_rows, brat_pool = base.brat
    by_command = {c["command"]: c for c in base.combos}
    for old, new in commands.items():
        source = og_combo[old]
        stages = []
        for stage in source["stages"]:
            copy = bytearray(stage)
            put16(copy, 0, attack_map.get(u16(stage, 0), u16(stage, 0)))
            stages.append(copy)
        if swap:
            by_command[new]["stages"] = stages
            continue
        follows = FOLLOWS.get(old)
        base.combos.append({"command": new, "alt": -1, "stages": stages,
                            "prerequisite": commands.get(follows, -1) if follows is not None else -1})
    if not swap:
        for old, new in commands.items():
            like = SLOT_012.get(old)
            row = next((r for r in brat_rows if r[0] == like), None)
            if row is None:
                continue
            situations = []
            for offset, count in row[1]:
                situations.append((len(brat_pool), count))
                brat_pool += brat_pool[offset:offset + count]
            brat_rows.append((new, situations))
        base.brat = (brat_head, brat_rows, brat_pool)
    base.text = mlng_build([table_build(s) for s in base_strings])
    report.note(f"{len(order)} attacks, {len(motions)} motions on {len(anims)} new animations, "
                f"{len(magi_map)} projectiles, {sum(1 for k, v in hit_map.items() if k != v)} hits copied")
    # What of the source was used: a recipe keeps exactly this.
    report.used = {"attacks": order, "motions": motions, "anims": anims,
                   "hits": sorted(k for k in hit_map if k in og_hits),
                   "projectiles": sorted(k for k in magi_map if k in og_magi),
                   "shapes": sorted(k for k in shape_map if k in og_groups), "fepa": sorted(brought_rows),
                   "sounds": sorted(sound_slots), "banners": sorted(k for k, v in banner_map.items() if k != v)}
    return chunks


# The original Bartz's commands and the 012 command in the same place of his list.
SLOT_012 = {408: 162, 409: 163, 410: 164, 411: 165, 412: 166, 413: 167, 607: 168,
            414: 169, 415: 170, 416: 171, 604: 172, 417: 173, 418: 174, 419: 175}
# Follow-ups: the original's comb has no prerequisite; its ability table says Flare follows Holy.
FOLLOWS = {604: 416}
# The original Bartz's abilities: command -> English name.
OG_ABILITIES = {408: "Ascendent Lance", 409: "Solid Ascension", 410: "Climbarrel", 411: "Reel Impulse",
                414: "Storm Shot", 415: "Slidehazzard", 416: "Holy", 604: "Flare",
                412: "Soul Eater", 413: "Flood", 417: "Wind Shear", 418: "Paladin Force"}

CODE = "fiv100"
CHAR_ID = 9
FIGHTERS = ("one100", "one200", "two100", "two200", "thr100", "thr200", "for100", "for200", "fiv100", "fiv200",
            "six100", "six200", "sev100", "sev200", "eht100", "eht200", "nin100", "nin200", "ten100", "ten200",
            "gst100", "gst200", "for110", "fiv210", "sev110", "eht110", "ten110", "gst110", "gst120", "gst130",
            "org210")
MODELS = ("1P", "2P", "3P", "ZAKO")
MODEL_BLOCK = 0x264000          # the game's staging block for one model file
FIRST_ABILITY_KEY = 0x20        # ability ids (CHAR_ID << 8) | key; 012's Bartz uses keys 0x01..0x0F
LANGUAGES = ("EN", "FR", "DE", "IT", "ES", "JP")
MAIN_BIN = "general_archive/main/main.bin"
NAMES_ARCHIVE = "general_archive/main/%s/main_lang.bin"
HELP_PACK_012 = "text/%s/mess_pk_customize_ability_0.bin"
HELP_PACK_OG = "text/%s/mess_pk_customize_0.bin"
OG_HELP_ENTRY = 2               # the original's ability help texts: entry 2 of its customize pack


# ---------------------------------------------------------------------------------------------
# Abilities: system.bin's ablt / chrl / amas, the names and the help texts


def message_table(data):
    """[u16 units or None] of a message table, plain (u16 offsets in words) or OFF4 (u32 offsets
    in 4-byte units); the text is hidden with TEXT_KEY by string index."""
    out = []
    off4 = data[:4] == b"OFF4"
    count = u32(data, 4) if off4 else u16(data, 0)
    for index in range(count):
        if off4:
            at = u32(data, 8 + 4 * index) * 4
            size, start = (u32(data, at), at + 4) if at else (0, 0)
        else:
            at = u16(data, 2 + 2 * index) * 2
            size, start = (u16(data, at), at + 2) if at else (0, 0)
        if not at:
            out.append(None)
            continue
        units = struct.unpack_from(f"<{size // 2}H", data, start)
        out.append([u ^ TEXT_KEY[(index + k) & 15] for k, u in enumerate(units)])
    return out, off4


def message_table_bytes(strings, off4=False):
    def hide(units, index):
        return struct.pack(f"<{len(units)}H", *(u ^ TEXT_KEY[(index + k) & 15] for k, u in enumerate(units)))

    if not off4:
        start = 2 + 2 * len(strings)
        offsets, body = [], bytearray()
        for index, units in enumerate(strings):
            if units is None:
                offsets.append(0)
                continue
            position = start + len(body)
            if position // 2 > 0xFFFF:
                return message_table_bytes(strings, off4=True)
            offsets.append(position // 2)
            body += struct.pack("<H", 2 * len(units)) + hide(units, index)
        return struct.pack(f"<H{len(strings)}H", len(strings), *offsets) + bytes(body)
    start = 8 + 4 * len(strings)
    start += -start % 4
    offsets, body = [], bytearray()
    for index, units in enumerate(strings):
        if units is None:
            offsets.append(0)
            continue
        offsets.append((start + len(body)) // 4)
        body += struct.pack("<I", 2 * len(units)) + hide(units, index)
        body += bytes(-len(body) % 4)
    head = b"OFF4" + struct.pack(f"<I{len(strings)}I", len(strings), *offsets)
    return head + bytes(start - len(head)) + bytes(body)


def mess_pak(data):
    if data[:8] != b"mess pak":
        raise MoveError("not a mess pak")
    count = u16(data, 10)
    return [bytes(data[o:o + s]) for o, s in (struct.unpack_from("<II", data, 16 + 8 * i) for i in range(count))]


def mess_pak_bytes(entries):
    head = 16 + 8 * len(entries)
    position = head + (-head % 16)
    table, body = bytearray(), bytearray()
    for entry in entries:
        table += struct.pack("<II", position, len(entry))
        pad = -len(entry) % 16
        body += entry + bytes(pad)
        position += len(entry) + pad
    out = b"mess pak" + struct.pack("<HHI", 1, len(entries), 0) + bytes(table)
    out += bytes(-len(out) % 16)
    return out + bytes(body)


def level_table(data):
    """chrl / amas: header 16 bytes, rows {u32 charId, u32 count, u32 offset, u32 0}, 8-byte entries."""
    head, rows = data[:16], {}
    for index in range(u32(data, 8)):
        char, count, offset, _ = struct.unpack_from("<4I", data, 16 + 16 * index)
        rows[char] = [bytearray(data[offset + 8 * k:offset + 8 * k + 8]) for k in range(count)]
    return head, rows


def level_table_bytes(head, rows):
    out = bytearray(head)
    put32(out, 8, len(rows))
    position = 16 + 16 * len(rows)
    index_rows, body = bytearray(), bytearray()
    for char in sorted(rows):
        index_rows += struct.pack("<4I", char, len(rows[char]), position + len(body), 0)
        for entry in rows[char]:
            body += entry
    return bytes(out) + bytes(index_rows) + bytes(body)


def og_ablt_row(row):
    """An original ability record in 012's layout: 012 put an s16 exclusion group at +0x0A."""
    out = bytearray(40)
    out[0x00:0x0A] = row[0x00:0x0A]
    put16(out, 0x0A, 0xFFFF)
    out[0x0C:0x20] = row[0x0A:0x1E]
    out[0x20:0x28] = row[0x20:0x28]
    return out


# CP of every (OG) ability, and once mastered: what 012's own Bartz abilities cost. The original's
# were 35 (20 or 15 mastered) for Bravery attacks and 45 (25) for HP attacks.
ABILITY_CP, ABILITY_CP_MASTERED = 30, 15
# AP to master (ablt +0x12): what 012 Bartz's own abilities take, 90 for a Bravery attack and 130 for an
# HP attack. The original's took 120 or 180, and 180 or 300 (the owner's choice, 22 November 2026).
HP_ATTACKS, BRAVERY_ATTACK_AP, HP_ATTACK_AP = (412, 413, 417, 418), 90, 130


def original_abilities(og_files):
    """The original Bartz's abilities by command: {command: {row (in 012's layout), names and helps
    ({language: u16 units}), level (None: his from the start), mastery (the commands whose mastery
    unlocks it, or None)}}."""
    og_system = Arc(Mpk(og_files[MAIN_BIN]).get("system.bin"))
    og_ablt = og_system.get(b"ablt")
    og_rows = {u16(r, 0x0A): r for r in (og_ablt[16 + 40 * i:56 + 40 * i] for i in range(u32(og_ablt, 8)))
               if r[0x19] == CHAR_ID}
    names, helps = {}, {}
    for lang in LANGUAGES:
        if NAMES_ARCHIVE % lang in og_files:
            names[lang] = message_table(Mpk(og_files[NAMES_ARCHIVE % lang]).get("ability.bin"))[0]
        if HELP_PACK_OG % lang in og_files:
            helps[lang] = message_table(mess_pak(og_files[HELP_PACK_OG % lang])[OG_HELP_ENTRY])[0]

    def texts(tables, index):
        out = {}
        for lang, table in tables.items():
            units = list(table[index] or [0]) if index < len(table) else [0]
            out[lang] = units[:units.index(0)] if 0 in units else units
        return out
    by_id = {u16(r, 0): command for command, r in og_rows.items()}
    out = {}
    for command, row in og_rows.items():
        if command in OG_ABILITIES:
            row012 = og_ablt_row(row)
            put16(row012, 0x12, HP_ATTACK_AP if command in HP_ATTACKS else BRAVERY_ATTACK_AP)
            out[command] = {"row": bytes(row012), "names": texts(names, u16(row, 0x02)),
                            "helps": texts(helps, u16(row, 0x04)), "level": None, "mastery": None}
    _, levels = level_table(og_system.get(b"chrl"))
    for entry in levels.get(CHAR_ID, []):
        for at in (2, 4, 6):
            command = by_id.get(u16(entry, at))
            if command in out:
                out[command]["level"] = entry[0]
    _, unlocks = level_table(og_system.get(b"amas"))
    for entry in unlocks.get(CHAR_ID, []):
        target, count = by_id.get(u16(entry, 0)), u16(entry, 2)
        if target in out:
            out[target]["mastery"] = [by_id.get(u16(entry, at)) for at in (4, 6)[:count]]
    return out


def abilities(files, originals, commands, report):
    """Give Bartz an "(OG)" ability per original command. `files` holds 012's package files by path
    (main.bin, every language's names and help pack present) and is updated in place for the files
    that change. `originals` is original_abilities()'s result, or a recipe's. Returns
    {ability id: (name, learn level)}."""
    system_main = Mpk(files[MAIN_BIN])
    system = Arc(system_main.get("system.bin"))
    ablt = bytearray(system.get(b"ablt"))
    rows = [bytearray(ablt[16 + 40 * i:56 + 40 * i]) for i in range(u32(ablt, 8))]
    taken = {u16(r, 0) for r in rows}
    sets = chunk_ids_fncs(system.get(b"fncs"))

    langs = [lang for lang in LANGUAGES if NAMES_ARCHIVE % lang in files]
    names = {lang: Mpk(files[NAMES_ARCHIVE % lang]) for lang in langs}
    name_tables = {lang: message_table(names[lang].get("ability.bin")) for lang in langs}
    help_langs = [lang for lang in langs if HELP_PACK_012 % lang in files]
    helps = {lang: mess_pak(files[HELP_PACK_012 % lang]) for lang in help_langs}
    help_tables = {lang: message_table(helps[lang][0]) for lang in help_langs}
    suffix = [ord(c) for c in " (OG)"]

    def pick(texts, lang):
        return list(texts.get(lang) or texts.get("EN") or [])

    new_ids, made = {}, {}
    for key, (old, command) in enumerate(commands.items()):
        source = originals.get(old)
        if source is None:
            raise MoveError(f"the original has no ability for command {old}")
        ability = (CHAR_ID << 8) | (FIRST_ABILITY_KEY + key)
        if ability in taken:
            raise MoveError(f"ability {ability:04X} is taken")
        new_ids[old] = ability
        row = bytearray(source["row"])
        put16(row, 0x00, ability)
        put16(row, 0x0C, command)
        put16(row, 0x08, 0xFFFF)
        put16(row, 0x0E, ABILITY_CP)                  # the owner's choice: 012's costs, not the original's
        put16(row, 0x10, ABILITY_CP_MASTERED)
        if u16(row, 0x14) != 0xFFFF and u16(row, 0x14) not in sets:
            report.note(f"{OG_ABILITIES[old]}: equip bonus set {u16(row, 0x14)} is not in 012's table, left out")
            put16(row, 0x14, 0xFFFF)
        name_index = max(len(t[0]) for t in name_tables.values())
        for lang in langs:
            table = name_tables[lang][0]
            table += [None] * (name_index - len(table)) + [pick(source["names"], lang) + suffix + [0]]
        put16(row, 0x02, name_index)
        if help_langs:
            help_index = max(len(t[0]) for t in help_tables.values())
            for lang in help_langs:
                table = help_tables[lang][0]
                table += [None] * (help_index - len(table)) + [pick(source["helps"], lang) + [0]]
            put16(row, 0x04, help_index)
        rows.append(row)
        made[ability] = OG_ABILITIES[old] + " (OG)"
    for old, follows in FOLLOWS.items():
        if old in new_ids and follows in new_ids:
            row = next(r for r in rows if u16(r, 0) == new_ids[old])
            put16(row, 0x06, new_ids[follows])

    rows.sort(key=lambda r: u16(r, 0))
    put32(ablt, 8, len(rows))
    system.set(bytes(ablt[:16]) + b"".join(bytes(r) for r in rows), b"ablt")
    if len(rows) > 808:
        raise MoveError(f"{len(rows)} ability records; the game's loader holds 808")

    # Learn levels as in the original; abilities it gives from the start are not in its table
    # (the port gives those). Mastery unlocks (Flare after Holy) likewise.
    learn = {ability: 1 for ability in made}
    head, levels = level_table(system.get(b"chrl"))
    for old, ability in sorted(new_ids.items(), key=lambda kv: (originals[kv[0]]["level"] or 0, kv[1])):
        level = originals[old]["level"]
        if level is not None:
            add_level(levels.setdefault(CHAR_ID, []), level, ability)
            learn[ability] = level
    system.set(level_table_bytes(head, levels), b"chrl")
    head, unlocks = level_table(system.get(b"amas"))
    for old, ability in new_ids.items():
        needs = originals[old]["mastery"]
        if needs and all(n in new_ids for n in needs):
            row = bytearray(8)
            struct.pack_into("<HHHH", row, 0, ability, len(needs), *([new_ids[n] for n in needs] + [0xFFFF] * (2 - len(needs))))
            unlocks.setdefault(CHAR_ID, []).append(row)
            learn[ability] = 0
    system.set(level_table_bytes(head, unlocks), b"amas")

    system_main.set("system.bin", system.to_bytes())
    files[MAIN_BIN] = system_main.to_bytes()
    for lang in langs:
        table, off4 = name_tables[lang]
        names[lang].set("ability.bin", message_table_bytes(table, off4))
        files[NAMES_ARCHIVE % lang] = names[lang].to_bytes()
    for lang in help_langs:
        table, off4 = help_tables[lang]
        helps[lang][0] = message_table_bytes(table, off4)
        files[HELP_PACK_012 % lang] = mess_pak_bytes(helps[lang])
    return {ability: (name, learn[ability]) for ability, name in made.items()}


def add_level(entries, level, ability):
    """Put `ability` in the chrl entry of `level` (a free slot of an entry, or a new entry)."""
    for entry in entries:
        if entry[0] == level:
            for at in (2, 4, 6):
                if u16(entry, at) == 0xFFFF:
                    put16(entry, at, ability)
                    return
    entry = bytearray(struct.pack("<BBHHH", level, 0, ability, 0xFFFF, 0xFFFF))
    entries.append(entry)
    entries.sort(key=lambda e: e[0])


def chunk_ids_fncs(data):
    """The effect set ids of 012's system.bin fncs (records {s16 id, s16 n, u32 first} from +0x10)."""
    count = u16(data, 8)
    return {u16(data, 16 + 8 * i) for i in range(count)}


# ---------------------------------------------------------------------------------------------
# The recipe: the original moves' data as text, so that a build needs the 012 disc only

# The abilities built into the port. Solid Ascension, Reel Impulse, Holy and Flare are left out:
# 012 Bartz has the same moves in the same slots.
SHIPPED = (408, 410, 412, 413, 414, 415, 417, 418)
# The 13 animations only the original disc has, each as its owner's 012 animation on Bartz's
# skeleton (tools/move_retarget.py): owner, motion, mirrored, speed, first frame, root scale,
# travel scale. Found by comparing poses (docs/research/original-bartz.md).
RETARGET = {
    "_600A": ("one100", "_600A", False, 1.0, 0, 0.914, 0.939),
    "_621A": ("two100", "_652A", False, 1.0, 0, 0.882, 1.0),
    "_607A": ("eht100", "_606A", True, 1.0, 0, 0.858, 1.0),
    "_701A": ("for100", "_700A", True, 1.0, 0, 0.870, 1.0),
    "_702A": ("for100", "_701A", True, 1.0, 0, 0.862, 1.0),
    "_704A": ("six100", "_700A", False, 1.25, -4, 1.081, 1.0),
    "_630A": ("nin100", "_620A", False, 1.0, 0, 1.266, 1.0),
    "_634A": ("ten100", "_642A", True, 1.0, 0, 1.007, 1.0),
    "_611A": ("sev100", "_611A", False, 1.0, 0, 1.018, 1.0),
    "_612A": ("sev100", "_612A", False, 1.0, 0, 0.946, 1.0),
    "_700A": ("thr100", "_708A", False, 1.0, 0, 1.131, 1.0),
    "_703A": ("for100", "_705A", False, 1.0, 0, 0.874, 1.0),
    "_703B": ("for100", "_705B", False, 1.0, 0, 0.868, 1.0),
}
# Weapon trails the original recoloured for Bartz: the owner's own 012 trail stands in.
EFFECT_FROM = {
    "P0051_NM_TRAIL_NIN100_LEAF_0": ("nin100", "P0091_NM_TRAIL_LEAF_0"),
    "P0051_P_TEN100_KISEKI_LEAF_0": ("ten100", "P0101_KISEKI_DEF_LEAF_0"),
    "P0051_P_ONE100_LOCUS_LEAF_0": ("one100", "P0010_PONE100_LOCUS_LEAF_0"),
    "P0051_P_ONE100_SHIELD_LOCUS_LEAF_0": ("one100", "P0010_PONE100_LOCUS_LEAF_0"),
}
# Move sounds of the original Bartz's bank and where 012 has them: his own bank keeps slots 52,
# 53, 56 and 76 (re-encoded), Cecil's has 88 and 96 byte for byte, and Terra's water clip stands
# in for 78, which 012 dropped.
SOUND_SAME = (52, 53, 56, 76)
SOUND_FROM = {78: ("six100", 93), 88: ("for100", 66), 96: ("for100", 78)}
# Help texts, our own words, laid out as 012's own: lines of about 30 characters (the box does not wrap),
# the equip bonus in the colour the game gives it, and the effects in theirs. A colour is unit 0x0010, two
# units that choose it, the text, then unit 0x001B; 012's help texts use these three. Each (OG) ability
# keeps the original's equip bonus set, which is one 012 Bartz's abilities have too; the bonus line says
# what that set gives, as 012's text for the same set does.


def _stat(text):          # a figure the bonus raises (red)
    return "\u0010\u0141\uff01" + text + "\u001b"


def _bonus(text):         # a boost the bonus gives (purple)
    return "\u0010\u0101\uff81" + text + "\u001b"


def _effects(text):       # the move's additional effects (violet)
    return "\u0010\u0141\uff41" + text + "\u001b"


HELP = {
    408: "[Close] Shield throw, then lance.\nMinor " + _bonus("Physical Shield") + " effect.\n"
         + _effects("Additional Effect: wall rush"),
    410: "[Mid] Slashing charge.\n" + _stat("Chase bravery damage") + " +10%.\n" + _effects("Additional Effect: chase"),
    412: "[Close] Stab with a dark blade.\n" + _stat("Wall rush HP damage") + " +5%.\n"
         + _effects("Additional Effect: wall rush"),
    413: "[Long] Raise a pillar of water.\nMinor " + _bonus("Magic Shield") + " effect.",
    414: "[Mid] Spin in, then strike.\n" + _bonus("Jump Times Boost") + " effect.\n"
         + _effects("Additional Effect: wall rush"),
    415: "[Mid] Charge again and again.\n" + _stat("Wall rush bravery damage") + " +10%.\n"
         + _effects("Additional Effect: wall rush"),
    417: "[Close] Spin and draw foe in.\nMinor " + _bonus("Speed Boost") + " effect.\n"
         + _effects("Effects: wall rush, absorb"),
    418: "[Close] Strike with holy swords.\nMinor " + _bonus("Midair Evasion Boost") + ".\n"
         + _effects("Additional Effect: wall rush"),
}


class Recipe:
    """The original moves as a recipe file holds them: the same tables bring() reads from an
    Original, limited to what the shipped abilities use. Animations, effects and sounds are not in
    it: it names where each comes from on the 012 disc."""

    def __init__(self, data):
        raw = bytes.fromhex
        self.fep = None
        self.attacks = [bytearray(raw(x)) for x in data["attacks"]]
        self.hits = [bytearray(raw(x)) for x in data["hits"]]
        self.combos = [{"command": int(c), "alt": -1, "prerequisite": -1,
                        "stages": [bytearray(raw(x)) for x in stages]} for c, stages in data["commands"].items()]
        self.projectiles = [{"head": bytearray(raw(r["head"])), "params": [tuple(p) for p in r["params"]],
                             "tail": bytes(2), "indices": None} for r in data["projectiles"]]
        self.motions = [bytearray(raw(x)) for x in data["motions"]]
        self.tracks = [{"anim": t["anim"],
                        "ranged": [[bytearray(raw(h)), list(a)] for h, a in t["ranged"]],
                        "instant": [[bytearray(raw(h)), list(a)] for h, a in t["instant"]]} for t in data["tracks"]]
        self.shapes = [bytearray(raw(x)) for x in data["shapes"]]
        self.fepa = [bytearray(raw(x)) for x in data["effect_records"]]
        self.effect_names = {int(k): v for k, v in data["effect_names"].items()}
        self.effect_from = {k: tuple(v) for k, v in data["effect_from"].items()}
        self.sounds = {int(k): v if v == "same" else tuple(v) for k, v in data["sounds"].items()}
        self.banners = [[None if s is None else raw(s) for s in table] for table in data["banners"]]
        self.anims = {int(k): v for k, v in data["animations"].items()}
        self.abilities = {int(k): {"row": raw(v["row"]), "names": v["names"], "helps": v["helps"],
                                   "level": v["level"], "mastery": v["mastery"]}
                          for k, v in data["abilities"].items()}
        self._model = self._read = None
        self._skeletons = {}

    def attach(self, model, read):
        """`model`: 012 Bartz's 1P model; `read(path)`: a file of the 012 package."""
        self._model, self._read = model, read

    def anim_name(self, anim):
        return self.anims[anim]["name"]

    def _skeleton(self, code):
        import gmo_motion
        if code not in self._skeletons:
            data = self._model if code == CODE else self._read(f"obj/P_{code.upper()}_1P.gmo")
            if data is None:
                raise MoveError(f"the disc has no model of {code}")
            self._skeletons[code] = gmo_motion.Skeleton(data)
        return self._skeletons[code]

    def anim_chunk(self, anim, name):
        import gmo_motion
        import move_retarget
        info = self.anims[anim]
        if "owner" not in info:                       # an animation 012 Bartz has himself
            chunk = next((c for c in gmo_motions(gmo_read(self._model)) if c.name == info["name"]), None)
            if chunk is None:
                raise MoveError(f"012 Bartz has no animation {info['name']}")
            return renamed_motion(chunk, name)
        target = self._skeleton(CODE)
        built = move_retarget.retarget(
            self._skeleton(info["owner"]), info["motion"], target, info["frames"], mirror=info["mirror"],
            speed=info["speed"], start=info["start"], root_scale=info["root_scale"],
            trans_scale=info["travel_scale"], name=name, follow=info.get("follow"), held=info.get("held"),
            scales=info.get("scales"))
        return GmoChunk(gmo_motion.chunk_bytes(built), 0)


# ---------------------------------------------------------------------------------------------
# What 012's engine needs changed in the original's data (the recipe holds the changed values)

# Hit record +0x0D, the EX Force a hit throws out. 012 works on a smaller scale: over the hits both
# games share, 50 became 20 or 0, 6 became 0-5 and 2 became 0-3, hit by hit. A hit takes the value
# of the 012 hit at the same moment of the same move (the owner's, or 012 Bartz's own for a shared
# animation); a hit with no such partner takes this table.
ORB_SCALE = {50: 20, 6: 2, 2: 1}
HIT_EVENTS = {(True, 0x00): 0, (True, 0x43): 0, (True, 0x7B): 0, (True, 0x20): 0, (True, 0x5B): 0, (True, 0x78): 1,
              (False, 0x08): 0}
# show_part windows (ranged 0x1B). For the animations both games have, 012 moved a start of tick 20
# to 6 or 4 and brought the ends in by 12 to 16; hit windows stayed. 012's engine draws a part
# later than the original's did.
SHOW_PART, SHOW_START, SHOW_END_EARLIER = 0x1B, {20: 6}, 14


def hit_times(track):
    out = []
    for ranged, key in ((True, "ranged"), (False, "instant")):
        for head, args in track[key]:
            index = HIT_EVENTS.get((ranged, event_type(head, ranged)))
            if index is not None and index < len(args):
                out.append((u16(head, 0), args[index]))
    return out


def orbs_for_012(og, used, base, base_names, owners):
    """{original hit id: EX Force amount on 012's scale}. `owners` gives (Fighter, animation names)
    of a 012 fighter by code; `base_names` are 012 Bartz's animation names."""
    hits = {u32(r, 0): r for r in og.hits}
    tracks = {t["anim"]: t for t in og.tracks}
    out = {}
    for anim in used["anims"]:
        name = og.anim_name(anim)
        if name in RETARGET:
            owner, motion, _mirror, speed, start, _root, _travel = RETARGET[name]
            fighter, names = owners(owner)
        else:
            fighter, names, motion, speed, start = base, base_names, name, 1.0, 0
        if anim not in tracks or motion not in names:
            continue
        theirs = next((t for t in fighter.tracks if t["anim"] == names.index(motion)), None)
        if theirs is None:
            continue
        their_hits = {u32(r, 0): r for r in fighter.hits}
        partners = [(tick, hit) for tick, hit in hit_times(theirs) if hit in their_hits]
        for tick, hit in hit_times(tracks[anim]):
            if hit in out or hit not in hits or not partners:
                continue
            want = 2 * start + tick * speed
            near_tick, near = min(partners, key=lambda p: abs(p[0] - want))
            if abs(near_tick - want) <= 6:
                out[hit] = their_hits[near][0x0D]
    for hit in used["hits"]:
        if hit not in out:
            out[hit] = ORB_SCALE.get(hits[hit][0x0D], min(hits[hit][0x0D], 20))
    return out


def show_windows_for_012(og, used, base, base_names):
    """{animation: {event index among its ranged events: (start, end)}} for the show_part windows."""
    tracks = {t["anim"]: t for t in og.tracks}
    theirs = {base_names[t["anim"]]: t for t in base.tracks if t["anim"] < len(base_names)}
    out = {}
    for anim in used["anims"]:
        track = tracks.get(anim)
        if track is None:
            continue
        same = theirs.get(og.anim_name(anim)) if og.anim_name(anim) not in RETARGET else None
        changes = {}
        for index, (head, args) in enumerate(track["ranged"]):
            if event_type(head, True) != SHOW_PART or not args:
                continue
            start, end = u16(head, 0), u16(head, 2)
            partner = None
            if same is not None:
                partner = next(((u16(h, 0), u16(h, 2)) for h, a in same["ranged"]
                                if event_type(h, True) == SHOW_PART and a and a[0] == args[0]), None)
            if partner is not None:
                new = partner
            else:
                new_start = SHOW_START.get(start, start)
                new_end = end if end == 0xFFFF else max(new_start + 2, end - SHOW_END_EARLIER)
                new = (new_start, new_end)
            if new != (start, end):
                changes[index] = new
        if changes:
            out[anim] = changes
    return out


def scale_tracks(skeleton, name):
    """{bone: [[frame, [x, y, z]], ...]} for the bones an original animation scales (a weapon that
    grows in and out, as Firion's spear does)."""
    motion = skeleton.motion(name)
    out = {}
    for bone, tracks in motion["tracks"].items():
        curve = tracks.get("scale")
        if curve is None:
            continue
        keys = [[key[0], [round(v, 4) for v in key[1][:3]]] for key in curve["keys"]]
        if any(abs(v - 1.0) > 0.01 for k in keys for v in k[1]):   # half floats: 0.9995 is 1
            out[skeleton.bones[bone]["name"]] = keys
    return out


def own_bones(orig, bart, anim, src, motion, mirror, speed, start):
    """How Bartz's own bones sit in an original animation, for the retarget: (follow, held).
    follow {his weapon bone: the owner's weapon bone}: a weapon that moves in the hand the way the
    owner's does. held {bone: {rotate, translate}}: a bone the original keeps still, where that is
    not its rest pose; a weapon that moves like none of the owner's is held at its average grip."""
    import gmo_motion as gm
    import move_retarget
    mo, ms = orig.motion(anim), src.motion(motion)
    end = int(mo["loop"][1])

    def named(bone):
        return (move_retarget.flip(bone["name"]) if mirror else bone["name"]) in src.by_name

    def weapon(bone):
        return bone["name"].startswith(("l_wep", "r_wep"))

    def turn(skeleton, world, index):
        return gm.q_mul(world[index][0], gm.q_conj(skeleton.bind[index][0]))
    candidates = [b for b in src.bones if "wep" in b["name"]]
    follow, held = {}, {}
    for bone in orig.bones:
        if bone["name"] == "star" or (named(bone) and not weapon(bone)):
            continue
        tracks = mo["tracks"].get(bone["index"], {})
        rotate, translate = tracks.get("rotate"), tracks.get("translate")
        rest = bart.bones[bone["index"]]
        if rotate is not None and len(rotate["keys"]) > 1 and weapon(bone):
            errors = {c["name"]: 0.0 for c in candidates}
            for k in range(0, end + 1, 3):
                world = orig.world(orig.pose(mo, k))
                theirs = src.world(src.pose(ms, min(ms["loop"][1], max(0, start + k * speed))))
                mine = turn(orig, world, bone["index"])
                for c in candidates:
                    other = turn(src, theirs, c["index"])
                    errors[c["name"]] += gm.q_angle(mine, gm.mirror_q(other) if mirror else other)
            count = len(range(0, end + 1, 3))
            best = min(errors, key=errors.get) if errors else None
            if best is not None and errors[best] / count < 40:
                follow[bone["name"]] = best
                continue
            total, first = [0.0, 0.0, 0.0, 0.0], gm.sample(rotate, 0)[:4]
            for k in range(end + 1):
                q = gm.sample(rotate, k)[:4]
                if sum(x * y for x, y in zip(q, first)) < 0:
                    q = tuple(-x for x in q)
                total = [x + y for x, y in zip(total, q)]
            held[bone["name"]] = {"rotate": [round(x, 5) for x in gm.q_norm(total)]}
        elif rotate is None or len(rotate["keys"]) == 1:
            rot = tuple(rotate["keys"][0][1][:4]) if rotate else bone["rotate"]
            still = translate is not None and len(translate["keys"]) == 1
            tra = tuple(translate["keys"][0][1][:3]) if still else bone["translate"]
            entry = {}
            if gm.q_angle(rot, rest["rotate"]) > 0.5 or named(bone):
                entry["rotate"] = [round(x, 5) for x in rot]
            if sum((x - y) ** 2 for x, y in zip(tra, rest["translate"])) ** 0.5 > 0.002:
                entry["translate"] = [round(x, 5) for x in tra]
            if entry:
                held[bone["name"]] = entry
    return follow, held


def recipe_data(og, og_gmo, base, used, commands, originals, read, sources=()):
    """The recipe (a JSON-ready dict) of what bring() used of the original: `used` is its
    report.used, `base` 012's fighter as the disc has it."""
    import gmo_motion
    hexs = lambda rows: [bytes(r).hex() for r in rows]                          # noqa: E731
    attacks = {u16(r, 0): r for r in og.attacks}
    hits = {u32(r, 0): r for r in og.hits}
    magi = {u16(r["head"], 0): r for r in og.projectiles}
    motions = {u16(r, 0): r for r in og.motions}
    tracks = {t["anim"]: t for t in og.tracks}
    combos = {c["command"]: c for c in og.combos}
    groups = set(used["shapes"])
    sounds = {}
    for slot in used["sounds"]:
        if slot in SOUND_FROM:
            sounds[slot] = list(SOUND_FROM[slot])
        elif slot in SOUND_SAME or (slot < base.bank.slots() and base.bank.clip(slot) == og.bank.clip(slot)):
            sounds[slot] = "same"
        else:
            raise MoveError(f"sound slot {slot}: no 012 clip is named for it")
    original_names = {u32(og.fepa[i], 0x28) for i in used["fepa"]}
    # Where every effect 012 Bartz lacks comes from: a stand-in, else the first 012 fighter (in
    # FIGHTERS' order) with an effect of the same name. Named here so that a build reads only
    # those fighters.
    own_effects = {name for _, name in base.fep.names()}
    stand_ins = {}
    for index in sorted(original_names):
        name = og.effect_names.get(index)
        if name in EFFECT_FROM:
            stand_ins[name] = list(EFFECT_FROM[name])
        elif name is not None and name not in own_effects:
            source = next((s for s in sources if effect_stem(name) in s.by_stem), None)
            if source is None:
                raise MoveError(f"effect {name}: no 012 fighter has it and no stand-in is named")
            stand_ins[name] = [source.label, source.by_stem[effect_stem(name)][1]]
    skeleton = gmo_motion.Skeleton(og_gmo)
    bart = gmo_motion.Skeleton(base.models["1P"])
    base_motions = {c.name: c.raw for c in gmo_motions(gmo_read(base.models["1P"]))}
    og_motions = {c.name: c.raw for c in gmo_motions(gmo_read(og_gmo))}
    animations = {}
    for anim in used["anims"]:
        name = og.anim_name(anim)
        if name in RETARGET:
            owner, motion, mirror, speed, start, root_scale, travel_scale = RETARGET[name]
            source = gmo_motion.Skeleton(read(f"obj/P_{owner.upper()}_1P.gmo"))
            follow, held = own_bones(skeleton, bart, name, source, motion, mirror, speed, start)
            animations[anim] = {"name": name, "frames": int(skeleton.motion(name)["loop"][1]), "owner": owner,
                                "motion": motion, "mirror": mirror, "speed": speed, "start": start,
                                "root_scale": root_scale, "travel_scale": travel_scale,
                                "follow": follow, "held": held, "scales": scale_tracks(skeleton, name)}
        elif base_motions.get(name) == og_motions[name]:
            animations[anim] = {"name": name}
        else:
            raise MoveError(f"animation {name}: 012 Bartz's differs and no owner is named for it")
    owners_seen = {}

    def owners(code):
        if code not in owners_seen:
            names = [c.name for c in gmo_motions(gmo_read(read(f"obj/P_{code.upper()}_1P.gmo")))]
            owners_seen[code] = (Fighter(read(f"obj/p_{code}.objx")), names)
        return owners_seen[code]
    base_names = [c.name for c in gmo_motions(gmo_read(base.models["1P"]))]
    orbs = orbs_for_012(og, used, base, base_names, owners)
    for hit, amount in orbs.items():
        hits[hit] = bytearray(hits[hit])
        hits[hit][0x0D] = amount
    windows = show_windows_for_012(og, used, base, base_names)
    for anim, changes in windows.items():
        tracks[anim] = {"anim": anim, "instant": tracks[anim]["instant"],
                        "ranged": [[bytearray(h), a] for h, a in tracks[anim]["ranged"]]}
        for index, (start, end) in changes.items():
            put16(tracks[anim]["ranged"][index][0], 0, start)
            put16(tracks[anim]["ranged"][index][0], 2, end)
    for anim in used["anims"]:
        fixes = {k[1:]: v for k, v in KEY_FIXES.items() if k[0] == og.anim_name(anim)}
        if fixes and anim in tracks:
            ranged = [[bytearray(h), list(a)] for h, a in tracks[anim]["ranged"]]
            for head, args in ranged:
                if event_type(head, True) in (0x2A, 0x5F) and len(args) > 2 and (args[1], args[2]) in fixes:
                    args[2] = fixes[(args[1], args[2])]
            tracks[anim] = {"anim": anim, "instant": tracks[anim]["instant"], "ranged": ranged}
    banners = []
    for table in og.banners:
        banners.append([None if (s is None or i not in used["banners"]) else bytes(s).hex() for i, s in enumerate(table)])
    abilities_ = {}
    for old in commands:
        source = originals[old]
        abilities_[old] = {"name": OG_ABILITIES[old], "row": source["row"].hex(), "names": source["names"],
                           "helps": {"EN": [ord(c) for c in HELP[old]]}, "level": source["level"],
                           "mastery": source["mastery"]}
    return {
        "format": 1,
        "about": "Bartz's moves from the first Dissidia, for building his (OG) abilities from the Dissidia 012 disc. "
                 "Written by tools/original_moves.py recipe; records are hex in 012's layouts.",
        "fighter": CODE,
        "commands": {old: hexs(combos[old]["stages"]) for old in commands},
        "attacks": hexs(attacks[a] for a in used["attacks"]),
        "hits": hexs(hits[h] for h in used["hits"]),
        "projectiles": [{"head": bytes(magi[p]["head"]).hex(), "params": [list(t) for t in magi[p]["params"]]}
                        for p in used["projectiles"]],
        "motions": hexs(motions[m] for m in used["motions"]),
        "tracks": [{"anim": anim, "ranged": [[bytes(h).hex(), list(a)] for h, a in tracks[anim]["ranged"]],
                    "instant": [[bytes(h).hex(), list(a)] for h, a in tracks[anim]["instant"]]}
                   for anim in used["anims"] if anim in tracks],
        "shapes": hexs(r for r in og.shapes if r[0] in groups),
        "effect_records": hexs(og.fepa[i] for i in used["fepa"]),
        "effect_names": {index: og.effect_names[index] for index in sorted(original_names) if index in og.effect_names},
        "effect_from": stand_ins,
        "sounds": sounds,
        "banners": banners,
        "animations": animations,
        "abilities": abilities_,
    }


# ---------------------------------------------------------------------------------------------
# Commands

RECIPE = ROOT / "port" / "features" / "bartz-original-moves.json"
# A slip in the original's data, put right. Paladin Force's charge (_703B) asks for the lance's
# trail with key 7, and no trail record has that key: the lance's trail record (bound to _703A, used
# by nothing else) has key 6. Cecil's own Paladin Force draws its trail over the same ticks, 6 to 140.
# (animation, record, key in the event) -> the key meant.
KEY_FIXES = {("_703B", 9999, 7): 6}


def read_base(iso):
    """What a build reads of the 012 disc: Bartz's files, the fighters his moves borrow from, and
    the ability and text files."""
    disc = Archive(iso)
    found = {"base": disc.read(f"obj/p_{CODE}.objx"), "read": disc.read,
             "models": {m: disc.read(f"obj/P_{CODE.upper()}_{m}.gmo") for m in MODELS}}
    if found["base"] is None or None in found["models"].values():
        raise MoveError(f"{iso}: no {CODE} files (is it Dissidia 012?)")
    found["sources"] = []
    for code in FIGHTERS:
        data = disc.read(f"obj/p_{code}.objx") if code != CODE else None
        if data is not None:
            found["sources"].append(EffectSource(code, data))
    found["files"] = {}
    for path in [MAIN_BIN] + [p % lang for lang in LANGUAGES for p in (NAMES_ARCHIVE, HELP_PACK_012)]:
        data = disc.read(path)
        if data is not None:
            found["files"][path] = data
    return found


def read_original(og_iso):
    disc = Archive(og_iso)
    found = {"og": disc.read(f"obj/p_{CODE}.objx"), "og_gmo": disc.read(f"obj/P_{CODE.upper()}_1P.gmo"), "og_files": {}}
    if found["og"] is None or found["og_gmo"] is None:
        raise MoveError(f"{og_iso}: no {CODE} files (is it the original Dissidia?)")
    for path in [MAIN_BIN] + [p % lang for lang in LANGUAGES for p in (NAMES_ARCHIVE, HELP_PACK_OG)]:
        data = disc.read(path)
        if data is not None:
            found["og_files"][path] = data
    return found


def read_discs(og_iso, iso):
    return {**read_base(iso), **read_original(og_iso)}


def model_counts(discs):
    counts = {m: len(gmo_motions(gmo_read(d))) for m, d in discs["models"].items()}
    if len(set(counts.values())) != 1:
        raise MoveError(f"the models have different motion counts: {counts}")
    return counts


def built_files(discs, base, chunks, made):
    """{package path: bytes} of a build: the moveset, the models and the changed ability files."""
    files = {f"obj/p_{CODE}.objx": base.to_bytes()}
    for m, data in discs["models"].items():
        files[f"obj/P_{CODE.upper()}_{m}.gmo"] = gmo_append_motions(data, chunks)
    files.update(made.pop("files", {}))
    return files


def write_out(out, discs, base, chunks, made, report, commands, swap=False):
    package = out / "package"
    files = built_files(discs, base, chunks, made)
    write_manifest(out, made, swap)
    for path, data in files.items():
        target = package / path
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(data)
        over = "  (over the game's model block: needs the port's room for bigger models)" if len(data) > MODEL_BLOCK else ""
        print(f"wrote {path}: {len(data):,} bytes{over}")
    for line in report.lines:
        print(line)
    for what, items in sorted(report.came.items()):
        print(f"copied, {what}: {len(items)}")
    for what, items in sorted(report.left.items()):
        shown = ", ".join(str(i) for i in sorted(items, key=str)[:12])
        print(f"left out, {what}: {len(items)} ({shown}{', ...' if len(items) > 12 else ''})")
    print("commands: " + ", ".join(f"{OG_ABILITIES[o]} {o} -> {n}" for o, n in commands.items()))


def new_abilities(discs, originals, commands, report):
    """{ability: (name, level), "files": {path: bytes}} for the changed ability and text files."""
    texts = dict(discs["files"])
    made = abilities(texts, originals, commands, report)
    report.note("abilities: " + ", ".join(f"{a:04X} {name} (Lv {lv})" for a, (name, lv) in made.items()))
    made["files"] = {path: data for path, data in texts.items() if data != discs["files"][path]}
    return made


def cmd_build(args):
    discs = read_discs(args.og_iso, args.iso)
    base, og = Fighter(discs["base"]), Original(discs["og"], discs["og_gmo"])
    counts = model_counts(discs)
    if args.swap:
        commands = {}
        for pair in args.swap:
            old, new = (int(x, 0) for x in pair.split("="))
            if old not in OG_ABILITIES or new not in {c["command"] for c in base.combos}:
                raise MoveError(f"--swap {pair}: not an original ability command = a 012 command")
            commands[old] = new
    else:
        chosen = [int(x, 0) for x in args.only] if args.only else list(OG_ABILITIES)
        commands = {old: FIRST_NEW["command"] + k for k, old in enumerate(OG_ABILITIES) if old in chosen}
    report = Report()
    chunks = bring(base, og, counts["1P"], commands, report, swap=bool(args.swap), sources=discs["sources"])
    made = {} if args.swap else new_abilities(discs, original_abilities(discs["og_files"]), commands, report)
    write_out(Path(args.out), discs, base, chunks, made, report, commands, bool(args.swap))


def shipped_commands():
    return {old: FIRST_NEW["command"] + k for k, old in enumerate(OG_ABILITIES) if old in SHIPPED}


def cmd_recipe(args):
    """Development: write the recipe from the original disc (and the 012 disc, to see what 012 has)."""
    import json
    discs = read_discs(args.og_iso, args.iso)
    base, og = Fighter(discs["base"]), Original(discs["og"], discs["og_gmo"])
    commands = shipped_commands()
    report = Report()
    bring(base, og, model_counts(discs)["1P"], commands, report, sources=discs["sources"])
    clean = Fighter(discs["base"])
    clean.models = discs["models"]
    data = recipe_data(og, discs["og_gmo"], clean, report.used, commands, original_abilities(discs["og_files"]),
                       discs["read"], discs["sources"])
    data["command_ids"] = {str(old): new for old, new in commands.items()}
    data["check"] = recipe_check(discs, data, commands)
    target = Path(args.recipe)
    target.parent.mkdir(parents=True, exist_ok=True)
    target.write_text(json.dumps(data, indent=1) + "\n", encoding="utf-8", newline="\n")  # LF, as the repository keeps it
    used = report.used
    print(f"wrote {target}: {target.stat().st_size:,} bytes; {len(used['attacks'])} attacks, {len(used['hits'])} hits, "
          f"{len(used['projectiles'])} projectiles, {len(used['anims'])} animations "
          f"({sum(1 for a in data['animations'].values() if 'owner' in a)} from their owners), "
          f"{len(used['fepa'])} effect records, {len(data['sounds'])} sounds")


def recipe_needs(data):
    """(fighters whose animations a recipe's build reads, fighters whose effects and sounds it reads)."""
    anims = sorted({a["owner"] for a in data["animations"].values() if "owner" in a})
    effects = {v[0] for v in data["effect_from"].values()}
    sounds = {v[0] for v in data["sounds"].values() if v != "same"}
    return anims, sorted(effects | sounds)


def recipe_inputs(discs, data):
    """The package paths a build from the recipe reads, in order."""
    anims, others = recipe_needs(data)
    paths = [f"obj/p_{CODE}.objx"] + [f"obj/P_{CODE.upper()}_{m}.gmo" for m in MODELS]
    paths += [f"obj/P_{code.upper()}_1P.gmo" for code in anims]
    paths += [f"obj/p_{code}.objx" for code in others]
    return paths + list(discs["files"])


def recipe_check(discs, data, commands):
    """What the port's builder checks itself with (port/host/d012_original_moves.cpp): the size and
    CRC-32 (zlib's) of every disc file the build reads, and of every file it must come to. A disc
    whose files differ (a disc mod) is left alone, and a build that comes to other bytes is not
    used: online play needs every player's files the same."""
    import copy
    import zlib
    recipe = Recipe(copy.deepcopy(data))
    recipe.attach(discs["models"]["1P"], discs["read"])
    base, report = Fighter(discs["base"]), Report()
    chunks = bring(base, recipe, model_counts(discs)["1P"], commands, report, sources=discs["sources"])
    made = new_abilities(discs, recipe.abilities, commands, report)
    files = built_files(discs, base, chunks, made)
    learned = [[ability, level] for ability, (_, level) in sorted(made.items())]

    def mark(content):
        return [len(content), f"{zlib.crc32(content):08X}"]
    return {"inputs": {path: mark(discs["read"](path)) for path in recipe_inputs(discs, data)},
            "outputs": {path: mark(content) for path, content in sorted(files.items())},
            "abilities": learned}


def cmd_build_012(args):
    """Build the (OG) abilities from the 012 disc and the recipe alone."""
    import json
    discs = read_base(args.iso)
    recipe = Recipe(json.loads(Path(args.recipe).read_text(encoding="utf-8")))
    recipe.attach(discs["models"]["1P"], discs["read"])
    base = Fighter(discs["base"])
    commands = {old: new for old, new in shipped_commands().items() if old in {c["command"] for c in recipe.combos}}
    if args.swap:
        commands = {int(pair.split("=")[0], 0): int(pair.split("=")[1], 0) for pair in args.swap}
    report = Report()
    chunks = bring(base, recipe, model_counts(discs)["1P"], commands, report, swap=bool(args.swap),
                   sources=discs["sources"])
    made = {} if args.swap else new_abilities(discs, recipe.abilities, commands, report)
    write_out(Path(args.out), discs, base, chunks, made, report, commands, bool(args.swap))


def write_manifest(out, made, swap):
    """The mod's mod.json (docs/port/mod-folder.md): its package, and the abilities it adds with the
    level the port gives each at (0: only as the game itself gives it, by mastering another)."""
    import json
    key = re.sub(r"[^a-z0-9._-]", "-", out.name.lower()).strip("-") or "bartz-original-moves"
    manifest = {"format": 1, "id": key,
                "name": "Bartz: original Dissidia moves" + (" (test swap)" if swap else ""),
                "version": "1", "description": "Bartz's moves from the first Dissidia, as abilities named (OG).",
                "package": "package"}
    if made:
        manifest["abilities"] = [{"id": f"{a:04X}", "learn": lv} for a, (_, lv) in sorted(made.items())]
    out.mkdir(parents=True, exist_ok=True)
    (out / "mod.json").write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")


def cmd_check(args):
    """Convert the original fighter and compare its records with 012's where both have them."""
    discs = read_discs(args.og_iso, args.iso)
    base, og = Fighter(discs["base"]), Original(discs["og"])
    for label, ours, theirs, key in (("attacks", og.attacks, base.attacks, lambda r: u16(r, 0)),
                                     ("hits", og.hits, base.hits, lambda r: u32(r, 0))):
        theirs = {key(r): bytes(r) for r in theirs}
        shared = [r for r in ours if key(r) in theirs]
        same = sum(1 for r in shared if bytes(r) == theirs[key(r)])
        print(f"{label}: {len(ours)} converted, {len(shared)} also in 012, {same} of those identical")
    Fighter(discs["base"]).to_bytes()
    print(f"012 tables round-trip: {Fighter(discs['base']).to_bytes() == discs['base']}")


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = parser.add_subparsers(dest="command", required=True)
    for name in ("check", "build", "recipe", "build-012"):
        p = sub.add_parser(name)
        if name != "build-012":
            p.add_argument("--og-iso", required=True, help="the original Dissidia (ULUS10437)")
        p.add_argument("--iso", default=str(ISO_012), help="Dissidia 012 (ULUS10566)")
        if name in ("recipe", "build-012"):
            p.add_argument("--recipe", default=str(RECIPE), help="the recipe file")
        if name in ("build", "build-012"):
            p.add_argument("out", help="the folder to write (its mod.json and package/)")
        if name in ("build", "build-012"):
            p.add_argument("--swap", nargs="*", help="OG=012 command pairs: test an original move in a 012 slot")
        if name == "build":
            p.add_argument("--only", nargs="*", help="original commands to bring (default: all twelve)")
    args = parser.parse_args(argv)
    try:
        {"check": cmd_check, "build": cmd_build, "recipe": cmd_recipe, "build-012": cmd_build_012}[args.command](args)
    except MoveError as error:
        print(f"error: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())

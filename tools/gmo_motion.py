"""Skeletons and animations of the game's GMO models (OMG.00.1PSP): read bones and motions, sample them, pose a
skeleton, and write new motions.

The chunk layout (little endian), as in Dissidia 012's fighter models:
  u16 type, u16 header_size, u32 chunk_size        "half" chunks (type & 0x8000): the payload follows
  ... + u32 child_offset, u32 data_offset, name   other chunks: args [header_size, data_offset), data
                                                  [data_offset, child_offset), children after
Type numbers are Sony's libgmo ones. A fighter file holds a motion model (bones and motions) and a mesh model
(the same bones with the parts and their inverse bind matrices).

Motions: FrameLoop (start, end), FrameRate, FrameRepeat, then Animate commands {bone, command, curve} and the
curves (FCurve: type, dims, key count; keys of (frame, values), stored as half floats in the retail files).
Rotations are quaternions (x, y, z, w) with spherical interpolation.
"""

import math
import struct

MAGIC = b"OMG.00.1PSP\0\0\0\0\0"
KIND = {
    0x0002: "File", 0x0003: "Model", 0x0004: "Bone", 0x0005: "Part", 0x000B: "Motion", 0x000C: "FCurve",
    0x0041: "ParentBone", 0x0044: "BlendBones", 0x0045: "BlendOffsets", 0x0048: "Translate", 0x004B: "RotateQ",
    0x00B1: "FrameLoop", 0x00B2: "FrameRate", 0x00B3: "Animate", 0x00B4: "FrameRepeat",
}
ID = {name: number for number, name in KIND.items()}
COMMAND = {0x48: "translate", 0x4B: "rotate", 0x4D: "scale", 0x4C: "scale1", 0x42: "visibility"}
COMMAND_ID = {"translate": 0x48, "rotate": 0x4B, "scale": 0x4D}
INTERP = {0: "CONSTANT", 1: "LINEAR", 2: "HERMITE", 3: "CUBIC", 4: "SPHERICAL"}
INTERP_ID = {name: number for number, name in INTERP.items()}
PER_DIM = {0: 1, 1: 1, 2: 3, 3: 3, 4: 1}


class Chunk:
    def __init__(self, type_, name=None, args=b"", data=b"", children=None, payload=b""):
        self.type, self.name, self.args, self.data = type_, name, args, data
        self.children = children if children is not None else []
        self.payload = payload
        self.raw_name = None

    @property
    def half(self):
        return bool(self.type & 0x8000)

    @property
    def kind(self):
        return KIND.get(self.type & 0x7FFF, hex(self.type))

    def find(self, kind):
        return [c for c in self.children if c.kind == kind]


def _name_bytes(name):
    raw = name.encode("shift-jis") + b"\0"
    return raw + bytes(-len(raw) % 4)


def _parse(data, pos, end):
    out = []
    while pos + 8 <= end:
        type_, header, size = struct.unpack_from("<HHI", data, pos)
        if size < 8 or pos + size > end:
            raise ValueError(f"bad GMO chunk at {pos:#x}")
        if type_ & 0x8000:
            out.append(Chunk(type_, payload=bytes(data[pos + 8:pos + size])))
        else:
            child, at = struct.unpack_from("<II", data, pos + 8)
            raw = bytes(data[pos + 16:pos + header])
            chunk = Chunk(type_, raw.split(b"\0")[0].decode("shift-jis"), bytes(data[pos + header:pos + at]),
                          bytes(data[pos + at:pos + child]))
            if _name_bytes(chunk.name) != raw:
                chunk.raw_name = raw
            chunk.children = _parse(data, pos + child, pos + size)
            out.append(chunk)
        pos += size
    if pos != end:
        raise ValueError(f"trailing GMO bytes at {pos:#x}")
    return out


def parse(data):
    """The File chunk of a GMO model."""
    if data[:16] != MAGIC:
        raise ValueError("not a GMO model")
    top = _parse(data, 16, len(data))
    if len(top) != 1 or top[0].kind != "File":
        raise ValueError("a GMO model has one File chunk")
    return top[0]


def chunk_bytes(chunk):
    if chunk.half:
        payload = chunk.payload + bytes(-len(chunk.payload) % 4)
        return struct.pack("<HHI", chunk.type, 0, 8 + len(payload)) + payload
    name = chunk.raw_name if chunk.raw_name is not None else _name_bytes(chunk.name)
    kids = b"".join(chunk_bytes(k) for k in chunk.children)
    header = 16 + len(name)
    at = header + len(chunk.args)
    child = at + len(chunk.data)
    return struct.pack("<HHIII", chunk.type, header, child + len(kids), child, at) + name + chunk.args + chunk.data + kids


def to_bytes(root):
    return MAGIC + chunk_bytes(root)


def _ref(word):
    return None if word == 0xFFFFFFFF else {"index": word & 0xFFF, "level": (word >> 12) & 0xF, "type": word >> 16}


def _ref_word(ref):
    return 0xFFFFFFFF if ref is None else (ref["index"] & 0xFFF) | (ref["level"] & 0xF) << 12 | ref["type"] << 16


def floats(chunk, count):
    return list(struct.unpack_from(f"<{count}f", chunk.payload, 0))


# ------------------------------------------------------------------------------------------------ curves


def decode_curve(chunk):
    type_, dims, count, reserved = struct.unpack("<IIII", chunk.args)
    interp = type_ & 0xF
    half = bool(type_ & 0x80)
    per = 1 + dims * PER_DIM[interp]
    body = chunk.data[:count * per * (2 if half else 4)]
    flat = list(struct.unpack(f"<{count * per}{'e' if half else 'f'}", body))
    keys = [(flat[i * per], flat[i * per + 1:(i + 1) * per]) for i in range(count)]
    return {"type": type_, "interp": INTERP[interp], "half": half, "dims": dims, "reserved": reserved, "keys": keys}


def make_curve(interp, dims, keys, half=True):
    flat = [v for frame, values in keys for v in (frame, *values)]
    body = struct.pack(f"<{len(flat)}{'e' if half else 'f'}", *flat)
    body += bytes(-len(body) % 4)
    type_ = INTERP_ID[interp] | (0x80 if half else 0)
    return Chunk(ID["FCurve"], f"fcurve-0", struct.pack("<IIII", type_, dims, len(keys), 0), body)


def sample(curve, frame):
    keys = curve["keys"]
    if len(keys) == 1 or frame <= keys[0][0]:
        return tuple(keys[0][1])
    if frame >= keys[-1][0]:
        return tuple(keys[-1][1])
    lo, hi = 0, len(keys) - 1
    while hi - lo > 1:
        mid = (lo + hi) // 2
        if keys[mid][0] <= frame:
            lo = mid
        else:
            hi = mid
    (t0, v0), (t1, v1) = keys[lo], keys[hi]
    if curve["interp"] == "CONSTANT":
        return tuple(v0)
    t = (frame - t0) / (t1 - t0)
    if curve["interp"] == "SPHERICAL":
        return slerp(tuple(v0), tuple(v1), t)
    return tuple(a + (b - a) * t for a, b in zip(v0, v1))


# ------------------------------------------------------------------------------------------------ maths
# Only add, subtract, multiply, divide and square root on doubles, in a fixed order, so that another
# implementation (the port's, in C++, on any platform) gets the same bits: no maths-library sin or acos,
# no compensated sums.

PI = 3.141592653589793


def dot3(a, b):
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]


def dot4(a, b):
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3]


def distance(a, b):
    x, y, z = a[0] - b[0], a[1] - b[1], a[2] - b[2]
    return math.sqrt(x * x + y * y + z * z)


def atan01(z):
    """atan of 0 <= z <= 1: halve the angle twice, then a fixed series."""
    z = z / (1.0 + math.sqrt(1.0 + z * z))
    z = z / (1.0 + math.sqrt(1.0 + z * z))
    q = z * z
    series = 1.0 / 17.0
    for odd in (15.0, 13.0, 11.0, 9.0, 7.0, 5.0, 3.0, 1.0):
        series = 1.0 / odd - q * series
    return 4.0 * z * series


def acos(x):
    """acos of -1 <= x <= 1."""
    if x < 0.0:
        return PI - acos(-x)
    if x >= 1.0:
        return 0.0
    return 2.0 * atan01(math.sqrt((1.0 - x) / (1.0 + x)))


def sin(x):
    """sin of 0 <= x <= pi, by a fixed series."""
    if x > PI / 2.0:
        x = PI - x
    q = x * x
    series = 1.0
    for n in (20.0, 18.0, 16.0, 14.0, 12.0, 10.0, 8.0, 6.0, 4.0, 2.0):
        series = 1.0 - q * series / (n * (n + 1.0))
    return x * series



def q_mul(a, b):
    ax, ay, az, aw = a
    bx, by, bz, bw = b
    return (aw * bx + ax * bw + ay * bz - az * by, aw * by - ax * bz + ay * bw + az * bx,
            aw * bz + ax * by - ay * bx + az * bw, aw * bw - ax * bx - ay * by - az * bz)


def q_conj(q):
    return (-q[0], -q[1], -q[2], q[3])


def q_norm(q):
    n = math.sqrt(dot4(q, q))
    return (q[0] / n, q[1] / n, q[2] / n, q[3] / n) if n else (0.0, 0.0, 0.0, 1.0)


def q_rot(q, v):
    x, y, z, w = q
    vx, vy, vz = v
    tx, ty, tz = 2 * (y * vz - z * vy), 2 * (z * vx - x * vz), 2 * (x * vy - y * vx)
    return (vx + w * tx + y * tz - z * ty, vy + w * ty + z * tx - x * tz, vz + w * tz + x * ty - y * tx)


def q_arc(a, b):
    """The shortest rotation taking unit vector a to unit vector b."""
    d = dot3(a, b)
    if d < -0.999999:
        return (1.0, 0.0, 0.0, 0.0)
    return q_norm((a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0], 1.0 + d))


def q_angle(a, b):
    d = abs(dot4(q_norm(a), q_norm(b)))
    return 2.0 * acos(min(1.0, d)) * 180.0 / PI


def slerp(a, b, t):
    d = dot4(a, b)
    if d < 0:
        b, d = tuple(-x for x in b), -d
    if d > 0.9995:
        return q_norm(tuple(x + (y - x) * t for x, y in zip(a, b)))
    theta = acos(d)
    s = sin(theta)
    wa, wb = sin((1 - t) * theta) / s, sin(t * theta) / s
    return tuple(x * wa + y * wb for x, y in zip(a, b))


def mirror_q(q):
    """A rotation seen in a mirror across the body's side-to-side plane (x = 0)."""
    return (q[0], -q[1], -q[2], q[3])


def mirror_v(v):
    return (-v[0], v[1], v[2])


def _matrix_quat(m):
    (m00, m01, m02), (m10, m11, m12), (m20, m21, m22) = m
    trace = m00 + m11 + m22
    if trace > 0:
        s = math.sqrt(trace + 1.0) * 2
        return q_norm(((m21 - m12) / s, (m02 - m20) / s, (m10 - m01) / s, 0.25 * s))
    if m00 > m11 and m00 > m22:
        s = math.sqrt(1.0 + m00 - m11 - m22) * 2
        return q_norm((0.25 * s, (m01 + m10) / s, (m02 + m20) / s, (m21 - m12) / s))
    if m11 > m22:
        s = math.sqrt(1.0 + m11 - m00 - m22) * 2
        return q_norm(((m01 + m10) / s, 0.25 * s, (m12 + m21) / s, (m02 - m20) / s))
    s = math.sqrt(1.0 + m22 - m00 - m11) * 2
    return q_norm(((m02 + m20) / s, (m12 + m21) / s, 0.25 * s, (m10 - m01) / s))


def _inverse_bind(m):
    """World (rotation, position) of a bone from its inverse bind matrix (16 floats, column major)."""
    r = [[m[0], m[4], m[8]], [m[1], m[5], m[9]], [m[2], m[6], m[10]]]
    t = (m[12], m[13], m[14])
    (a, b, c), (d, e, f), (g, h, i) = r
    det = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g)
    inv = [[(e * i - f * h) / det, (c * h - b * i) / det, (b * f - c * e) / det],
           [(f * g - d * i) / det, (a * i - c * g) / det, (c * d - a * f) / det],
           [(d * h - e * g) / det, (b * g - a * h) / det, (a * e - b * d) / det]]
    position = tuple(-(inv[k][0] * t[0] + inv[k][1] * t[1] + inv[k][2] * t[2]) for k in range(3))
    return _matrix_quat(inv), position


# ------------------------------------------------------------------------------------------------ skeletons


class Skeleton:
    """A fighter model's bones: names, parents, rest transforms, the bind pose, and its motions."""

    def __init__(self, data):
        self.root = parse(data)
        models = self.root.find("Model")
        self.motion_model = next(m for m in models if m.find("Motion"))
        mesh = next((m for m in models if m.find("Part")), None)
        self.bones = []
        for index, chunk in enumerate(self.motion_model.find("Bone")):
            bone = {"index": index, "name": chunk.name, "parent": None, "translate": (0.0, 0.0, 0.0),
                    "rotate": (0.0, 0.0, 0.0, 1.0)}
            for c in chunk.children:
                if c.kind == "ParentBone":
                    ref = _ref(struct.unpack_from("<I", c.payload, 0)[0])
                    bone["parent"] = ref["index"] if ref else None
                elif c.kind == "Translate":
                    bone["translate"] = tuple(floats(c, 3))
                elif c.kind == "RotateQ":
                    bone["rotate"] = tuple(floats(c, 4))
            self.bones.append(bone)
        self.by_name = {b["name"]: b["index"] for b in self.bones}
        self.order = []
        for i in range(len(self.bones)):
            self._visit(i)
        self.rest = self.world([(b["rotate"], b["translate"]) for b in self.bones])
        bind = {}
        for chunk in (mesh.find("Bone") if mesh else []):
            blend_bones = next((c for c in chunk.children if c.kind == "BlendBones"), None)
            offsets = next((c for c in chunk.children if c.kind == "BlendOffsets"), None)
            if blend_bones is None or offsets is None:
                continue
            count = struct.unpack_from("<I", blend_bones.payload, 0)[0]
            refs = [_ref(w) for w in struct.unpack_from(f"<{count}I", blend_bones.payload, 4)]
            for k, ref in enumerate(refs):
                if ref and ref["index"] not in bind:
                    bind[ref["index"]] = _inverse_bind(struct.unpack_from("<16f", offsets.payload, 4 + 64 * k))
        # Bones no part is skinned to follow their parent's bind pose with their own rest offset.
        self.bind = [None] * len(self.bones)
        for i in self.order:
            if i in bind:
                self.bind[i] = bind[i]
            else:
                b = self.bones[i]
                if b["parent"] is None:
                    self.bind[i] = (b["rotate"], b["translate"])
                else:
                    pr, pt = self.bind[b["parent"]]
                    off = q_rot(pr, b["translate"])
                    self.bind[i] = (q_norm(q_mul(pr, b["rotate"])), tuple(p + o for p, o in zip(pt, off)))
        self.motions = {m.name: m for m in self.motion_model.find("Motion")}

    def _visit(self, i):
        if i in self.order:
            return
        p = self.bones[i]["parent"]
        if p is not None:
            self._visit(p)
        self.order.append(i)

    def world(self, local):
        """World (rotation, position) per bone from local (rotation, translation) pairs."""
        out = [None] * len(self.bones)
        for i in self.order:
            r, t = local[i]
            p = self.bones[i]["parent"]
            if p is None:
                out[i] = (q_norm(r), tuple(t))
            else:
                pr, pt = out[p]
                off = q_rot(pr, t)
                out[i] = (q_norm(q_mul(pr, r)), tuple(a + b for a, b in zip(pt, off)))
        return out

    def motion(self, name):
        return decode_motion(self.motions[name])

    def pose(self, motion, frame):
        """Local (rotation, translation) per bone at a frame."""
        out = []
        for b in self.bones:
            tracks = motion["tracks"].get(b["index"], {})
            r = sample(tracks["rotate"], frame)[:4] if "rotate" in tracks else b["rotate"]
            t = sample(tracks["translate"], frame)[:3] if "translate" in tracks else b["translate"]
            out.append((tuple(r), tuple(t)))
        return out


def decode_motion(chunk):
    """{name, loop, fps, repeat, tracks {bone: {command: curve}}, animates [(bone, command, curve index)]}."""
    curves = [decode_curve(c) for c in chunk.find("FCurve")]
    out = {"name": chunk.name, "loop": None, "fps": None, "repeat": None, "tracks": {}, "animates": []}
    for c in chunk.children:
        if c.kind == "FrameLoop":
            out["loop"] = tuple(floats(c, 2))
        elif c.kind == "FrameRate":
            out["fps"] = floats(c, 1)[0]
        elif c.kind == "FrameRepeat":
            out["repeat"] = struct.unpack_from("<I", c.payload, 0)[0]
        elif c.kind == "Animate":
            block, command, _index, curve = struct.unpack("<IIiI", c.payload)
            bone, curve = _ref(block)["index"], _ref(curve)["index"]
            out["animates"].append((bone, command, curve))
            out["tracks"].setdefault(bone, {})[COMMAND.get(command, hex(command))] = curves[curve]
    return out


def make_motion(name, end_frame, fps, repeat, tracks):
    """A Motion chunk. tracks: [(bone index, "rotate" or "translate", curve chunk)], in the order written. The
    layout follows the retail motions: FrameLoop, FrameRate, FrameRepeat, the Animate commands, the curves."""
    kids = [Chunk(0x8000 | ID["FrameLoop"], payload=struct.pack("<2f", 0.0, float(end_frame))),
            Chunk(0x8000 | ID["FrameRate"], payload=struct.pack("<f", float(fps)))]
    if repeat is not None:
        kids.append(Chunk(0x8000 | ID["FrameRepeat"], payload=struct.pack("<I", repeat)))
    curves = []
    for k, (bone, command, curve) in enumerate(tracks):
        block = {"index": bone, "level": 1, "type": ID["Bone"]}
        ref = {"index": k, "level": 0, "type": ID["FCurve"]}
        kids.append(Chunk(0x8000 | ID["Animate"],
                          payload=struct.pack("<IIiI", _ref_word(block), COMMAND_ID[command], 0, _ref_word(ref))))
        curve.name = f"fcurve-{k}"
        curves.append(curve)
    return Chunk(ID["Motion"], name, children=kids + curves)


def reduce_keys(frames, values, tolerance, spherical):
    """Keys (frame, values) keeping only those needed to stay within `tolerance` (degrees for rotations, units
    for translations) of every sampled frame, by greedy extension of linear or spherical spans."""
    def error(a, b):
        return q_angle(a, b) if spherical else distance(a, b)

    def between(i, j, k):
        t = (frames[k] - frames[i]) / (frames[j] - frames[i])
        if spherical:
            return slerp(values[i], values[j], t)
        return tuple(x + (y - x) * t for x, y in zip(values[i], values[j]))
    kept = [0]
    i = 0
    while i < len(frames) - 1:
        j = i + 1
        while j + 1 < len(frames) and all(error(between(i, j + 1, k), values[k]) <= tolerance for k in range(i + 1, j + 1)):
            j += 1
        kept.append(j)
        i = j
    return [(float(frames[k]), list(values[k])) for k in kept]

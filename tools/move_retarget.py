"""Put one 012 fighter's animation on another 012 fighter's skeleton (tools/gmo_motion.py reads both).

012's skeletons share bone names (hip, waist, chest, l_uparm, r_femur ...), so bones pair by name. The method
works in world space, so different bone axes and rest poses do not matter:
  delta(b, t)  = source world rotation of b at t * inverse(source bind rotation of b)
  C(b)         = shortest rotation from the target bone's bind direction to the source bone's (limbs only)
  world(b, t)  = delta(b, t) * C(b) * target bind rotation of b
  local(b, t)  = inverse(world(parent, t)) * world(b, t)
Mirrored moves (done with the other hand) take the source's opposite bone (l_ <-> r_) and reflect its turn across
the side-to-side plane. Bones the source lacks follow a template motion of the target's own.

Translations: `root` (the body) scales by `root_scale`, `trans_include` (the distance the move covers in play) by
`trans_scale`, the camera bones are copied, a moving prop bone (a thrown shield) keeps its path relative to its
parent, and Bartz's `star` stays at `star_offset` above his head.
"""

import math

import gmo_motion as gm

LIMB_CHILD = {
    "l_femur": "l_tibia", "l_tibia": "l_foot", "l_foot": "l_toe", "r_femur": "r_tibia", "r_tibia": "r_foot",
    "r_foot": "r_toe", "l_uparm": "l_foarm", "l_foarm": "l_hand", "r_uparm": "r_foarm", "r_foarm": "r_hand",
}
COPIED = ("camera_include", "camera2_include", "trans_include")    # world-level bones: their values as they are
SKIPPED = ("star",)


def flip(name):
    if name.startswith("l_"):
        return "r_" + name[2:]
    if name.startswith("r_"):
        return "l_" + name[2:]
    return name


def _unit(v):
    n = math.sqrt(gm.dot3(v, v))
    return (v[0] / n, v[1] / n, v[2] / n) if n else (0.0, 1.0, 0.0)


def _sub(a, b):
    return tuple(x - y for x, y in zip(a, b))


def retarget(source, motion_name, target, frames, mirror=False, speed=1.0, start=0.0, root_scale=1.0,
             trans_scale=1.0, star_offset=(0.0, 0.42, 0.0), template=None, name=None, tolerance=0.25,
             correct=LIMB_CHILD, follow=None, held=None, scales=None):
    """A Motion chunk for `target` (a gm.Skeleton) playing `source`'s motion `motion_name` over frames
    0..`frames`: target frame k shows source frame start + k * speed. `template` is a decoded motion of the
    target's that the bones the source lacks follow (their rest pose without one).
    `follow` {target bone: source bone} pairs bones of different names: the weapon bone a borrowed weapon sits
    on with the owner's weapon bone. `held` {target bone: {"rotate": q, "translate": t}} fixes a bone's local
    values for the whole motion: how a weapon sits in the hand."""
    src = source.motion(motion_name)
    lo, hi = src["loop"]
    pair = {}
    for b in target.bones:
        other = flip(b["name"]) if mirror else b["name"]
        if b["name"] not in SKIPPED and other in source.by_name:
            pair[b["index"]] = source.by_name[other]
    for mine, theirs in (follow or {}).items():
        pair[target.by_name[mine]] = source.by_name[theirs]
    held = {target.by_name[n]: v for n, v in (held or {}).items()}
    for i in held:
        pair.pop(i, None)
    fix = {}
    for name_, child in correct.items():
        if name_ not in target.by_name or child not in target.by_name:
            continue
        s, sc = (flip(name_), flip(child)) if mirror else (name_, child)
        if s not in source.by_name or sc not in source.by_name:
            continue
        dt = _unit(_sub(target.bind[target.by_name[child]][1], target.bind[target.by_name[name_]][1]))
        ds = _unit(_sub(source.bind[source.by_name[sc]][1], source.bind[source.by_name[s]][1]))
        if mirror:
            ds = gm.mirror_v(ds)
        fix[target.by_name[name_]] = gm.q_arc(dt, ds)
    src_moving = {i for i, tr in src["tracks"].items() if "translate" in tr and len(tr["translate"]["keys"]) > 1}
    root_i, head_i = target.by_name.get("root"), target.by_name.get("head")
    star_i = target.by_name.get("star")

    rotations = {i: [] for i in range(len(target.bones))}
    translations = {}
    for k in range(int(frames) + 1):
        t = min(hi, max(lo, start + k * speed))
        local_src = source.pose(src, t)
        world_src = source.world(local_src)
        tpl = target.pose(template, min(k, template["loop"][1])) if template else None
        local, world = [None] * len(target.bones), [None] * len(target.bones)
        for i in target.order:
            b = target.bones[i]
            p = b["parent"]
            parent_rot = world[p][0] if p is not None else (0.0, 0.0, 0.0, 1.0)
            trans = tpl[i][1] if tpl else b["translate"]
            if i in pair and b["name"] in COPIED:
                r, tr = local_src[pair[i]]
                rot = gm.mirror_q(r) if mirror else r
                tr = gm.mirror_v(tr) if mirror else tr
                trans = tuple(x * trans_scale for x in tr) if b["name"] == "trans_include" else tr
            elif i in pair:
                s = pair[i]
                delta = gm.q_mul(world_src[s][0], gm.q_conj(source.bind[s][0]))
                if mirror:
                    delta = gm.mirror_q(delta)
                w = gm.q_norm(gm.q_mul(gm.q_mul(delta, fix.get(i, (0.0, 0.0, 0.0, 1.0))), target.bind[i][0]))
                rot = gm.q_norm(gm.q_mul(gm.q_conj(parent_rot), w))
                if i == root_i:
                    tr = local_src[s][1]
                    trans = tuple(x * root_scale for x in (gm.mirror_v(tr) if mirror else tr))
                elif s in src_moving and source.bones[s]["parent"] is not None:
                    # a prop on the move: its offset from the parent, in world terms, scaled like the body
                    sp = source.bones[s]["parent"]
                    off = gm.q_rot(world_src[sp][0], _sub(local_src[s][1], source.bones[s]["translate"]))
                    if mirror:
                        off = gm.mirror_v(off)
                    back = gm.q_rot(gm.q_conj(parent_rot), off)
                    trans = tuple(r0 + root_scale * d for r0, d in zip(b["translate"], back))
            elif i in held:
                rot = tuple(held[i].get("rotate") or b["rotate"])
                trans = tuple(held[i].get("translate") or b["translate"])
            else:
                rot = tpl[i][0] if tpl else b["rotate"]
            local[i] = (rot, trans)
            if p is None:
                world[i] = (gm.q_norm(rot), tuple(trans))
            else:
                pr, pt = world[p]
                off = gm.q_rot(pr, trans)
                world[i] = (gm.q_norm(gm.q_mul(pr, rot)), tuple(a + c for a, c in zip(pt, off)))
        if star_i is not None and head_i is not None and target.bones[star_i]["parent"] is None:
            local[star_i] = (local[star_i][0], tuple(h + o for h, o in zip(world[head_i][1], star_offset)))
        for i, (rot, trans) in enumerate(local):
            prev = rotations[i][-1] if rotations[i] else None
            if prev is not None and gm.dot4(prev, rot) < 0:
                rot = tuple(-x for x in rot)
            rotations[i].append(tuple(rot))
            translations.setdefault(i, []).append(tuple(trans))

    order = []
    if template:
        order = [(bone, gm.COMMAND.get(command)) for bone, command, _ in template["animates"]]
    order = [(b, c) for b, c in order if c in ("rotate", "translate")]
    have = set(order)
    for i in range(len(target.bones)):
        for command, values in (("rotate", rotations[i]), ("translate", translations[i])):
            rest = target.bones[i]["rotate" if command == "rotate" else "translate"]
            moving = max(gm.q_angle(v, rest) if command == "rotate" else gm.distance(v, rest) for v in values)
            if (i, command) not in have and (moving > 1e-4 or (command == "rotate" and i in pair)):
                order.append((i, command))
                have.add((i, command))
    tracks = []
    keyframes = list(range(int(frames) + 1))
    for bone, command in order:
        values = rotations[bone] if command == "rotate" else translations[bone]
        spherical = command == "rotate"
        keys = gm.reduce_keys(keyframes, values, tolerance if spherical else tolerance / 100.0, spherical)
        if len(keys) == 1 or all((gm.q_angle(v, keys[0][1]) if spherical else gm.distance(v, keys[0][1])) < 1e-5
                                 for _, v in keys):
            curve = gm.make_curve("CONSTANT", 4 if spherical else 3, [(0.0, keys[0][1])])
        else:
            curve = gm.make_curve("SPHERICAL" if spherical else "LINEAR", 4 if spherical else 3, keys)
        tracks.append((bone, command, curve))
    for bone, keys in (scales or {}).items():             # scale keys as they are: a weapon that grows in and out
        tracks.append((target.by_name[bone], "scale", gm.make_curve("LINEAR", 3, [(float(f), list(v)) for f, v in keys])))
    repeat = template["repeat"] if template else 0
    return gm.make_motion(name or motion_name, frames, 30.0, repeat, tracks)


# ------------------------------------------------------------------------------------------------ measures


def limb_pairs(a, b, mirror=False):
    """Bone segments (child, parent) present in both skeletons by name, for comparing poses."""
    pairs = []
    for bone in a.bones:
        p = bone["parent"]
        if p is None or math.sqrt(sum(x * x for x in bone["translate"])) <= 0.02:
            continue
        cn, pn = bone["name"], a.bones[p]["name"]
        if mirror:
            cn, pn = flip(cn), flip(pn)
        if cn in b.by_name and pn in b.by_name and b.bones[b.by_name[cn]]["parent"] == b.by_name[pn]:
            pairs.append(((bone["index"], p), (b.by_name[cn], b.by_name[pn])))
    return pairs


def directions(skeleton, motion, frame, pairs, mirror=False):
    world = skeleton.world(skeleton.pose(motion, frame))
    out = []
    for c, p in pairs:
        v = _unit(_sub(world[c][1], world[p][1]))
        out.append(gm.mirror_v(v) if mirror else v)
    return out


def pose_distance(a, motion_a, b, motion_b, step=1, mirror=False, speed=1.0, start=0.0, pairs=None):
    """Mean angle in degrees between matching limb directions of two motions (a frame k against b frame
    start + k * speed), over the frames of motion_a."""
    pairs = pairs if pairs is not None else limb_pairs(a, b, mirror)
    total, count = 0.0, 0
    end = int(motion_a["loop"][1])
    for k in range(0, end + 1, step):
        da = directions(a, motion_a, k, [x for x, _ in pairs], mirror)
        db = directions(b, motion_b, min(motion_b["loop"][1], start + k * speed), [y for _, y in pairs])
        for u, v in zip(da, db):
            total += math.degrees(math.acos(max(-1.0, min(1.0, sum(x * y for x, y in zip(u, v))))))
            count += 1
    return total / count if count else float("nan")

"""The scene list the port needs to play Story Mode with the Japanese disc's voices.

    python tools/port/jp_story_scenes.py USA.iso JAPAN.iso > port/host/d012_voice_scenes_data.inc

With Japanese voices on, the port plays each voiced story scene whose script differs between the two discs from the
Japanese disc's scene file, with the game's own text (docs/port/japanese-voices.md, "Story scenes"). This tool finds
those scenes and works out, for every voice line a Japanese script plays that the English game has no file name for,
an English name nothing plays with Japanese voices on, to serve that line under. It writes only hashes and file names;
no game data. Both images are opened read-only.

A story scene (talkevent/*.evex) is an ARC archive: "ARC\\x01", u32 chunk count, then from byte 16 one
{u32 kind, 4-byte tag, u32 offset, u32 size} per chunk. The first chunk is the script: 30 track offsets at 0x408 (each
track a u32 count and that many 32-byte commands {u32 op, u16 1, u16 step, u32 args[6]}), and at 0x480 a word, the
offset of the voice table and the count of the 0x40-byte paths at 0x48C. The voice table runs from its offset to the
end of the chunk, 0x40 bytes a path ("voice/jp/event/<folder>/<name>.at3", the language rewritten by the game); command
0x320 plays the next one.
"""

import argparse
import collections
import os
import struct
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
from package_archive import Archive, name_hash  # noqa: E402

VOICE_OP = 0x320
TRACKS = 30


def chunks(archive):
    count = struct.unpack_from("<I", archive, 4)[0]
    return [struct.unpack_from("<I4sII", archive, 16 + 16 * i) for i in range(count)]


def c_string(data):
    return data.split(b"\0", 1)[0].decode("ascii")


def script(archive):
    """(voice paths, number of voices played) of a scene's script chunk, or None when the first chunk is not laid
    out as a script."""
    _, _, offset, size = chunks(archive)[0]
    chunk = archive[offset:offset + size]
    if len(chunk) < 0x48C:
        return None
    played = 0
    for track in struct.unpack_from(f"<{TRACKS}I", chunk, 0x408):
        if track + 4 > len(chunk):
            return None
        count = struct.unpack_from("<I", chunk, track)[0]
        if track + 4 + 32 * count > len(chunk):
            return None
        for k in range(count):
            if struct.unpack_from("<I", chunk, track + 4 + 32 * k)[0] == VOICE_OP:
                played += 1
    voice_at = struct.unpack_from("<I", chunk, 0x484)[0]
    if voice_at > len(chunk):
        return None
    voices = [c_string(chunk[at:at + 0x40]) for at in range(voice_at, len(chunk) - 0x3F, 0x40)]
    return voices, played


def without_text(archive):
    return b"".join(archive[offset:offset + size] for _, tag, offset, size in chunks(archive) if tag != b"text")


def english(path):
    return path.replace("voice/jp/", "voice/en/", 1)


def read(arc, key):
    sector, size = arc.records[key]
    return arc.iso.read(arc.package["extent_lba"] * arc.iso.block + sector * 2048, size)


def scenes(arc):
    for key in sorted(arc.records):
        data = read(arc, key)
        if data[:4] == b"ARC\x01" and b"talkevent" in data:
            yield key, data


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("usa")
    parser.add_argument("japan")
    options = parser.parse_args()
    usa, japan = Archive(options.usa), Archive(options.japan)

    merged = {}  # scene hash: the Japanese script's voice paths
    # English voice names, by the scenes that name them with Japanese voices on (played or not: a name in any
    # scene's voice table is never chosen to serve another line under).
    users = collections.Counter()
    not_scripts = 0
    for key, ours in scenes(usa):
        mine = script(ours)
        if mine is None:
            not_scripts += 1
            continue
        if key in japan.records:
            theirs = read(japan, key)
            other = script(theirs)
            if other is not None and (mine[1] or other[1]) and without_text(ours) != without_text(theirs):
                their_voices, _ = other
                merged[key] = their_voices
                for path in their_voices:
                    users[english(path)] += 1
                continue
        voices, _ = mine
        for path in voices:
            users[english(path)] += 1

    # The English scripts' voice names that exist and that no scene plays with Japanese voices on.
    known = set(usa.records)
    english_names = set()
    for key, ours in scenes(usa):
        mine = script(ours)
        if mine is not None:
            english_names.update(english(p) for p in mine[0])
    spares = sorted(n for n in english_names if name_hash(n) in known and users[n] == 0)

    # The Japanese scripts' voice lines the English game has no file name for.
    problems = []
    needed = set()
    for key in sorted(merged):
        for path in merged[key]:
            if name_hash(english(path)) in known:
                continue
            if name_hash(path) not in japan.records:
                problems.append(f"{key:08x}: {path} is not on the Japanese disc")
                continue
            needed.add(path)
    substitutes = {}
    used = set()
    # A re-recording first: the English game has the same line under its folder in the 013 story.
    for path in sorted(needed):
        folder, file = path.rsplit("/", 2)[-2:]
        if folder != "kaigaiban_add":
            continue
        for other in ("ffst", "ff11", "ff12"):
            candidate = f"voice/en/event/{other}/{file}"
            if name_hash(candidate) in known and users[candidate] == 0 and candidate not in used:
                used.add(candidate)
                substitutes[path] = candidate
                break
    # Then any other: a spare name, of the same folder where there is one.
    for path in sorted(needed - set(substitutes)):
        folder = path.rsplit("/", 2)[-2]
        pool = [n for n in spares if n not in used and n.rsplit("/", 2)[-2] == folder] or \
               [n for n in spares if n not in used]
        if not pool:
            problems.append(f"no English name left for {path}")
            continue
        used.add(pool[0])
        substitutes[path] = pool[0]

    print("// The voiced story scenes whose script differs between the USA and Japanese discs, and the English names")
    print("// the Japanese-only voice lines are served under. Written by tools/port/jp_story_scenes.py from the two")
    print("// discs; do not edit. Hashes and names only.")
    print()
    print(f"// {len(merged)} scenes (talkevent archives, by name hash).")
    print("inline constexpr std::uint32_t kScenes[] = {")
    keys = sorted(merged)
    for i in range(0, len(keys), 8):
        print("    " + " ".join(f"0x{k:08X}u," for k in keys[i:i + 8]))
    print("};")
    print()
    print(f"// {len(substitutes)} voice lines of the Japanese scripts the English game has no file name for: each is")
    print("// served under an English name that no scene plays with Japanese voices on (in a scene's voice table the")
    print("// Japanese path is replaced by that name, with the language folder as the game expects it).")
    print("inline constexpr VoiceName kVoiceNames[] = {")
    for path in sorted(substitutes):
        print(f'    {{"{path}", "{substitutes[path].replace("voice/en/", "voice/jp/", 1)}"}},')
    print("};")
    for p in problems:
        print("// PROBLEM: " + p)
    print(f"{len(merged)} scenes, {len(substitutes)} voice names, {len(spares)} spare names, "
          f"{not_scripts} talkevent archives without a script, {len(problems)} problems", file=sys.stderr)
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main())

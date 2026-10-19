"""Survey the media formats a native runtime must decode.

Reads the retail ISO read-only (tools/inspect_iso.Iso and the PACKAGE.BIN index
reader in tools/package_archive.py). For each loose BGM/movie/system
file and every PACKAGE.BIN entry it reads only the first bytes and records
format fields: magics, RIFF/WAVE format tags, channels, sample rates, PSMF
version and stream ids. No content is copied.

Writes build/scratch/port/media.json.
"""

import argparse
import struct
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import common  # noqa: E402
from inspect_iso import Iso  # noqa: E402
from package_archive import parse_index  # noqa: E402

ISO = common.ROOT / "Dissidia 012 - Duodecim Final Fantasy (USA) (En,Fr,De,Es,It).iso"
WAVE_TAGS = {0x0001: "PCM", 0x0270: "ATRAC3", 0xFFFE: "extensible (ATRAC3plus if GUID matches)"}
AT3PLUS_GUID = bytes.fromhex("BFAA23E958CB7144A119FFFA01E4CE62")


def riff_info(head):
    if head[:4] != b"RIFF" or head[8:12] != b"WAVE":
        return None
    pos = 12
    while pos + 8 <= len(head):
        tag, size = struct.unpack_from("<4sI", head, pos)
        if tag == b"fmt ":
            fmt, channels, rate, byte_rate, block = struct.unpack_from("<HHIIH", head, pos + 8)
            codec = WAVE_TAGS.get(fmt, hex(fmt))
            if fmt == 0xFFFE and head[pos + 8 + 24:pos + 8 + 40] == AT3PLUS_GUID:
                codec = "ATRAC3plus"
            return {"codec": codec, "channels": channels, "rate": rate,
                    "kbps": round(byte_rate * 8 / 1000), "block": block}
        pos += 8 + size + (size & 1)
    return {"codec": "RIFF without fmt in first bytes"}


def psmf_info(head):
    if head[:4] != b"PSMF":
        return None
    version = head[4:8].decode("ascii", "replace")
    streams = struct.unpack_from(">H", head, 0x80)[0]
    ids = []
    width = height = None
    for index in range(min(streams, 16)):
        base = 0x82 + index * 16
        sid, private = head[base], head[base + 1]
        if 0xE0 <= sid <= 0xEF:
            ids.append("AVC video")
            if width is None:
                width, height = head[base + 0x0E] * 16, head[base + 0x0F] * 16
        elif sid == 0xBD:
            ids.append("ATRAC3plus audio" if private < 0x10 else ("LPCM audio" if 0x40 <= private < 0x50 else f"private 0x{private:02X}"))
        else:
            ids.append(f"stream 0x{sid:02X}")
    return {"version": version, "streams": ids, "width": width, "height": height}


def magic_of(head):
    if head[:4] == b"RIFF":
        return "RIFF/WAVE"
    for magic in (b"MIG.00.1PSP", b"OMG.00.1PSP", b"menu pak", b"ARC\x01", b"MPK ", b"SSCF", b"PSMF",
                  b"\x89PNG", b"DES4", b"mlng", b"VOLD", b"mess", b"SEQ ", b"FRR", b"VAGp", b"PSF"):
        if head.startswith(magic):
            return magic.decode("latin-1").strip("\x00\x01\x89")
    return "other"


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--iso", type=Path, default=ISO)
    args = parser.parse_args()
    iso = Iso(args.iso)
    entries = {e["path"].upper(): e for e in iso.files()}
    report = {"loose": {}, "package": {}}
    loose = defaultdict(Counter)
    loose_bytes = Counter()
    for path, entry in sorted(entries.items()):
        if entry["extent_lba"] * iso.block + 512 > iso.size:
            continue
        head = iso.read(entry["extent_lba"] * iso.block, min(2048, entry["size"]))
        folder = path.rsplit("/", 1)[0]
        info = riff_info(head) or psmf_info(head)
        if info is None:
            key = magic_of(head)
        elif "codec" in info:
            key = f"{info['codec']} {info.get('channels')}ch {info.get('rate')} Hz {info.get('kbps')} kbps"
        else:
            # width/height offsets are not validated, so they are kept out of the key
            key = f"PSMF {info['version']} {'+'.join(info['streams'])}"
        loose[folder][key] += 1
        loose_bytes[folder] += entry["size"]
    report["loose"] = {k: {"files": sum(v.values()), "bytes": loose_bytes[k], "formats": dict(v)} for k, v in loose.items()}

    package = entries["/PSP_GAME/USRDIR/DATA/PACKAGE.BIN"]
    index = iso.read(entries["/PSP_GAME/USRDIR/DATA/PACKAGE_INFO.BIN"]["extent_lba"] * iso.block,
                     entries["/PSP_GAME/USRDIR/DATA/PACKAGE_INFO.BIN"]["size"])
    records = parse_index(index)
    magics = Counter()
    magic_bytes = Counter()
    wave = Counter()
    for key, (sector, size) in records.items():
        offset = package["extent_lba"] * iso.block + sector * 2048
        head = iso.read(offset, min(256, size)) if size else b""
        magic = magic_of(head)
        magics[magic] += 1
        magic_bytes[magic] += size
        if magic == "RIFF/WAVE":
            info = riff_info(head)
            wave[f"{info.get('codec')} {info.get('channels')}ch {info.get('rate')} Hz {info.get('kbps')} kbps"] += 1
    report["package"] = {"entries": len(records),
                         "by_magic": {k: {"entries": v, "bytes": magic_bytes[k]} for k, v in magics.most_common()},
                         "riff_wave_formats": dict(wave.most_common())}
    path = common.write_json("media.json", report)
    for folder, data in report["loose"].items():
        print(folder, data["files"], data["bytes"], data["formats"])
    print("PACKAGE:", report["package"]["entries"], "entries")
    for k, v in report["package"]["by_magic"].items():
        print(f"  {k:12s} {v['entries']:6d} {v['bytes']:12d}")
    print("  RIFF formats:", report["package"]["riff_wave_formats"])
    print(f"-> {path}")


if __name__ == "__main__":
    main()

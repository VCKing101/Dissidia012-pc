"""Nintendo stream files: the loop points in a BRSTM, BCSTM or BFSTM header.

These formats (Wii BRSTM; 3DS BCSTM; Wii U BFSTM, big-endian, and Switch
BFSTM, little-endian) carry their loop in the header and no tags, so the mod
maker reads it here, as the port does for "loop": "auto"
(port/host/d012_nintendo_stream.cpp, the same rules). FFmpeg's brstm and bfstm
demuxers decode the audio; FFmpeg's own "loop_start" metadata is in
microseconds, not samples, so it is not used.

The stream info gives a loop flag, the loop start (a sample, counted from 0)
and the stream's sample count, which is also where the loop ends: the loop is
[loop_start, samples). Layouts: the public format pages (BRSTM on the Mario
Kart Wii wiki, BCSTM on 3dbrew, BFSTM on the Mario Kart 8 wiki), read through
references rather than fixed offsets where the format has them.

BWAV and NUS3AUDIO are not read: FFmpeg has no demuxer for either, so the port
could not play them. MIT licence; see port/README.md.
"""

from __future__ import annotations

import os
import struct
from typing import Optional

EXTENSIONS = (".brstm", ".bcstm", ".bfstm")
# Extensions a user may well try, with why not and what to do instead.
UNSUPPORTED = {
    ".bwav": "BWAV files are not supported (FFmpeg cannot decode them); use the BRSTM version of the track",
    ".nus3audio": "NUS3AUDIO files are not supported (FFmpeg cannot decode them); use the BRSTM version of the track",
}
CODECS = {0: "PCM8", 1: "PCM16", 2: "DSP-ADPCM", 3: "IMA-ADPCM"}
# What FFmpeg's brstm/bfstm demuxers decode sample-exactly. They refuse IMA
# ADPCM, and they read PCM8/PCM16 streams wrongly (FFmpeg 7.1: the last block
# is cut to the size DSP-ADPCM would take, and a BFSTM's PCM starts 24 bytes
# early), so only DSP-ADPCM is taken, which is what nearly every file holds.
DECODABLE = (2,)
MAX_HEADER = 1 << 20


class StreamError(ValueError):
    """Not a stream this module (and so the port) can take, with why."""


class _Reader:
    def __init__(self, data: bytes, big: bool, what: str):
        self.data, self.big, self.what = data, big, what

    def _get(self, fmt: str, at: int):
        size = struct.calcsize(fmt)
        if at < 0 or at + size > len(self.data):
            raise StreamError(f"not a valid {self.what} file: its header is cut short")
        return struct.unpack_from((">" if self.big else "<") + fmt, self.data, at)[0]

    def u8(self, at: int) -> int:
        return self._get("B", at)

    def u16(self, at: int) -> int:
        return self._get("H", at)

    def u32(self, at: int) -> int:
        return self._get("I", at)


def kind_of(name: str) -> Optional[str]:
    """"BRSTM", "BCSTM" or "BFSTM" for a file name with one of their extensions, else None."""
    extension = os.path.splitext(name)[1].lower()
    return extension[1:].upper() if extension in EXTENSIONS else None


def _bom(data: bytes, what: str) -> bool:
    """True for big-endian. The byte order mark is the bytes FE FF (big) or FF FE (little)."""
    if data[4:6] == b"\xfe\xff":
        return True
    if data[4:6] == b"\xff\xfe":
        return False
    raise StreamError(f"not a valid {what} file: no byte order mark")


def _check(info: dict, what: str) -> dict:
    if info["channels"] < 1:
        raise StreamError(f"not a valid {what} file: no channels")
    if not 1000 <= info["rate"] <= 384000:
        raise StreamError(f"not a valid {what} file: a sample rate of {info['rate']} Hz")
    if info["samples"] < 1:
        raise StreamError(f"not a valid {what} file: no samples")
    if info["codec"] not in DECODABLE:
        name = CODECS.get(info["codec"], f"codec {info['codec']}")
        why = "cannot decode" if info["codec"] not in (0, 1) else "does not decode correctly"
        raise StreamError(f"this {what} file holds {name} audio, which FFmpeg (and so the port) {why}; only "
                          "DSP-ADPCM streams are supported: use a DSP-ADPCM BRSTM of the track, or a WAV or FLAC "
                          "with the loop set in the mod")
    info["codec_name"] = CODECS[info["codec"]]
    # The loop as the port takes it: from loop_start to the end of the stream.
    # A loop flag with a start at or past the end is a broken loop: the port
    # loops the whole file instead (and says so).
    info["loop_problem"] = None
    if info["loop_flag"] and info["loop_start"] >= info["samples"]:
        info["loop_problem"] = (f"the file's loop starts at sample {info['loop_start']}, at or past its end "
                                f"({info['samples']} samples)")
    info["loop"] = ({"start": info["loop_start"], "end": info["samples"]}
                    if info["loop_flag"] and info["loop_problem"] is None else None)
    blocks = info.get("block_count") or 0
    expected = (blocks - 1) * info["samples_per_block"] + info["last_block_samples"] if blocks else None
    info["block_samples"] = expected
    return info


def _brstm(data: bytes) -> dict:
    what = "BRSTM"
    big = _bom(data, what)
    r = _Reader(data, big, what)
    head = r.u32(0x10)
    if data[head:head + 4] != b"HEAD":
        raise StreamError("not a valid BRSTM file: no HEAD chunk where its header says")
    # HEAD: three references (type, data type, padding, offset from HEAD + 8).
    stream = head + 8 + r.u32(head + 0x0C)
    info = {"format": what, "big_endian": big, "version": f"{r.u8(6)}.{r.u8(7)}",
            "codec": r.u8(stream), "loop_flag": bool(r.u8(stream + 1)), "channels": r.u8(stream + 2),
            # 16 bits at +4, as FFmpeg reads it (the byte at +3 is a high byte in no known file)
            "rate": r.u16(stream + 4), "loop_start": r.u32(stream + 8), "samples": r.u32(stream + 0x0C),
            "block_count": r.u32(stream + 0x14), "samples_per_block": r.u32(stream + 0x1C),
            "last_block_samples": r.u32(stream + 0x24)}
    return _check(info, what)


def _cstm(data: bytes) -> dict:
    what = "BCSTM" if data[:4] == b"CSTM" else "BFSTM"
    big = _bom(data, what)
    r = _Reader(data, big, what)
    version = r.u32(0x08)
    sections = r.u16(0x10)
    info_at = None
    for i in range(min(sections, 16)):
        if r.u16(0x14 + 12 * i) == 0x4000:
            info_at = r.u32(0x14 + 12 * i + 4)
    if info_at is None or data[info_at:info_at + 4] != b"INFO":
        raise StreamError(f"not a valid {what} file: no INFO block where its header says")
    # INFO + 8: a reference (type 0x4100, padding, offset from INFO + 8) to the stream info.
    if r.u16(info_at + 8) != 0x4100:
        raise StreamError(f"not a valid {what} file: its INFO block has no stream info")
    stream = info_at + 8 + r.u32(info_at + 0x0C)
    info = {"format": what, "big_endian": big,
            "version": f"{version >> 24}.{(version >> 16) & 0xFF}.{(version >> 8) & 0xFF}.{version & 0xFF}",
            "codec": r.u8(stream), "loop_flag": bool(r.u8(stream + 1)), "channels": r.u8(stream + 2),
            "rate": r.u32(stream + 4), "loop_start": r.u32(stream + 8), "samples": r.u32(stream + 0x0C),
            "block_count": r.u32(stream + 0x10), "samples_per_block": r.u32(stream + 0x18),
            "last_block_samples": r.u32(stream + 0x20)}
    return _check(info, what)


def parse_header(data: bytes) -> dict:
    """The stream info of a BRSTM, BCSTM or BFSTM file from its first bytes
    (enough to hold its HEAD or INFO block): {"format", "big_endian",
    "version", "codec", "codec_name", "channels", "rate", "loop_flag",
    "loop_start", "samples", "loop": {"start", "end"} or None,
    "loop_problem"}. StreamError when it is not one, or not one FFmpeg decodes."""
    magic = bytes(data[:4])
    if len(data) < 0x40:
        raise StreamError("not a BRSTM, BCSTM or BFSTM file: shorter than a header")
    if magic == b"RSTM":
        return _brstm(bytes(data))
    if magic in (b"CSTM", b"FSTM"):
        return _cstm(bytes(data))
    raise StreamError("not a BRSTM, BCSTM or BFSTM file (it starts " + repr(magic) + ")")


def read_header(path: str) -> dict:
    """parse_header() of a file, reading only as much as its header needs."""
    with open(path, "rb") as handle:
        data = handle.read(0x40)
        if len(data) == 0x40 and data[:4] in (b"RSTM", b"CSTM", b"FSTM"):
            try:
                big = _bom(data, "stream")
                r = _Reader(data, big, "stream")
                if data[:4] == b"RSTM":
                    end = r.u32(0x10) + r.u32(0x14)
                else:
                    end = 0x40
                    for i in range(min(r.u16(0x10), 4)):
                        if r.u16(0x14 + 12 * i) == 0x4000:
                            end = r.u32(0x14 + 12 * i + 4) + r.u32(0x14 + 12 * i + 8)
                data += handle.read(max(0, min(end, MAX_HEADER) - 0x40))
            except StreamError:
                pass
    kind = kind_of(path)
    if kind and data[:4] not in (b"RSTM", b"CSTM", b"FSTM"):
        raise StreamError(f"not a valid {kind} file (it does not start with {kind[1:]!r})")
    return parse_header(data)


def describe(info: dict) -> str:
    """One line for the editor: "BFSTM (Switch, little-endian), DSP-ADPCM, 48000 Hz, 2 channels"."""
    order = "big-endian" if info["big_endian"] else "little-endian"
    platform = {"BRSTM": "Wii", "BCSTM": "3DS"}.get(info["format"]) or ("Wii U" if info["big_endian"] else "Switch")
    return f"{info['format']} ({platform}, {order}), {info['codec_name']}"

"""Synthetic BRSTM, BCSTM and BFSTM files for the tests: test tones only.

FFmpeg has no muxer or encoder for these formats, so the tests write them
here, in the layout Nintendo's tools write (and FFmpeg's brstm and bfstm
demuxers read): header, HEAD or INFO block, ADPC or SEEK block, DATA block,
each 0x20-aligned, channels interleaved block by block.

The audio is DSP-ADPCM from a small encoder: a fixed table of eight
second-order predictors (silence, hold, and resonators for a few pitches) and,
for each 14-sample frame, the predictor and scale with the least error,
quantised in a closed loop with FFmpeg's adpcm_thp arithmetic
(prediction = (c1 * h1 + c2 * h2) >> 11, plus nibble << scale). The encoder
returns what the decoder will make, so a test can compare FFmpeg's output with
it sample for sample. Each block's ADPC/SEEK entry holds the two samples
before it, as FFmpeg's decoder starts each block from them. PCM16 is also
written, and an IMA-ADPCM header for the "cannot decode" case.
MIT licence; see port/README.md.
"""

from __future__ import annotations

import math
import struct
from typing import Optional

FRAME = 14  # samples per 8-byte DSP-ADPCM frame
DEFAULT_BLOCK_SAMPLES = 14336  # 0x2000 bytes of DSP-ADPCM, as Nintendo's files use


def predictors(rate: int) -> list:
    """Eight (c1, c2) pairs, 11-bit fixed point: silence, hold, and x[n] = 2cos(w) x[n-1] - x[n-2] for six pitches."""
    pairs = [(0, 0), (2048, 0)]
    for freq in (110, 330, 660, 1000, 1600, 2600):
        pairs.append((int(round(2 * math.cos(2 * math.pi * freq / rate) * 2048)), -2048))
    return pairs


def _clip(v: int) -> int:
    return -32768 if v < -32768 else 32767 if v > 32767 else v


def dsp_encode(samples: list, coefs: list) -> tuple:
    """(frame bytes, the samples FFmpeg will decode, each frame's header byte) for one channel."""
    out = bytearray()
    decoded: list = []
    headers: list = []
    h1 = h2 = 0
    for at in range(0, len(samples), FRAME):
        x = samples[at:at + FRAME]
        # The predictor whose open-loop residual is smallest, then its scale.
        best_p, best_peak = 0, None
        for p, (c1, c2) in enumerate(coefs):
            a1, a2, peak = h1, h2, 0
            for v in x:
                r = abs(v - ((c1 * a1 + c2 * a2) >> 11))
                if r > peak:
                    peak = r
                a2, a1 = a1, v
            if best_peak is None or peak < best_peak:
                best_p, best_peak = p, peak
        scale = 0
        while scale < 12 and best_peak > 7 * (1 << scale):
            scale += 1
        c1, c2 = coefs[best_p]
        choice = None
        for s in {max(0, scale - 1), scale, min(12, scale + 1)}:
            a1, a2, err, nibbles, ys = h1, h2, 0, [], []
            step = 1 << s
            for v in x:
                pred = (c1 * a1 + c2 * a2) >> 11
                n = int(math.floor((v - pred) / step + 0.5))
                n = -8 if n < -8 else 7 if n > 7 else n
                y = _clip(pred + n * step)
                err += (y - v) ** 2
                nibbles.append(n & 0xF)
                ys.append(y)
                a2, a1 = a1, y
            if choice is None or err < choice[0]:
                choice = (err, s, nibbles, ys)
        _, s, nibbles, ys = choice
        header = (best_p << 4) | s
        headers.append(header)
        nibbles += [0] * (FRAME - len(nibbles))
        out.append(header)
        for i in range(0, FRAME, 2):
            out.append((nibbles[i] << 4) | nibbles[i + 1])
        decoded += ys
        h1 = decoded[-1]
        h2 = decoded[-2] if len(decoded) > 1 else 0
    return bytes(out), decoded, headers


def _align(data: bytearray, to: int = 0x20) -> bytearray:
    data += b"\0" * (-len(data) % to)
    return data


class _Writer:
    def __init__(self, big: bool):
        self.e = ">" if big else "<"

    def pack(self, fmt: str, *values) -> bytes:
        return struct.pack(self.e + fmt, *values)


def _encode(channels: list, rate: int, codec: str, block_samples: int) -> dict:
    """Per channel: block payloads, ADPC/SEEK history, DSP info; and the block sizes."""
    count = len(channels[0])
    if any(len(c) != count for c in channels):
        raise ValueError("every channel needs the same length")
    if block_samples % FRAME:
        raise ValueError("a block holds whole frames")
    blocks = max(1, (count + block_samples - 1) // block_samples)
    last_samples = count - (blocks - 1) * block_samples
    result = {"count": count, "blocks": blocks, "block_samples": block_samples, "last_samples": last_samples,
              "channels": [], "decoded": []}
    for pcm in channels:
        if codec == "dsp":
            coefs = predictors(rate)
            frames, decoded, headers = dsp_encode(pcm, coefs)
            stream = frames
            block_bytes = block_samples // FRAME * 8
            full, rest = divmod(last_samples, FRAME)
            last_used = full * 8 + ((rest + 1) // 2 + 1 if rest else 0)
            history = []
            for b in range(blocks):
                at = b * block_samples
                history.append((decoded[at - 1] if at >= 1 else 0, decoded[at - 2] if at >= 2 else 0))
            result["channels"].append({"stream": stream, "coefs": coefs, "headers": headers, "history": history,
                                       "decoded": decoded})
        elif codec == "pcm16":
            block_bytes = block_samples * 2
            last_used = last_samples * 2
            result["channels"].append({"pcm": pcm, "decoded": list(pcm)})
        elif codec == "ima":
            block_bytes = block_samples // 2
            last_used = (last_samples + 1) // 2
            result["channels"].append({"stream": bytes((count + 1) // 2), "decoded": None})
        else:
            raise ValueError(codec)
        result["decoded"].append(result["channels"][-1]["decoded"])
    result["block_bytes"] = block_bytes
    result["last_used"] = last_used
    result["last_padded"] = last_used + (-last_used % 0x20)
    return result


def _block_data(enc: dict, w: _Writer, codec: str) -> bytes:
    data = bytearray()
    for b in range(enc["blocks"]):
        last = b == enc["blocks"] - 1
        size = enc["last_used"] if last else enc["block_bytes"]
        padded = enc["last_padded"] if last else enc["block_bytes"]
        for ch in enc["channels"]:
            if codec == "pcm16":
                first = b * enc["block_samples"]
                n = enc["last_samples"] if last else enc["block_samples"]
                piece = b"".join(w.pack("h", v) for v in ch["pcm"][first:first + n])
            else:
                at = b * enc["block_bytes"]
                piece = ch["stream"][at:at + size]
            data += piece + b"\0" * (padded - len(piece))
    return bytes(data)


def _dsp_context(ch: dict, w: _Writer, loop_start: Optional[int], cafe: bool) -> bytes:
    """The DSP-ADPCM info after the 16 coefficients: start context, loop context."""
    ps = ch["headers"][0]
    if loop_start is not None and loop_start < len(ch["decoded"]):
        lps = ch["headers"][loop_start // FRAME]
        l1 = ch["decoded"][loop_start - 1] if loop_start >= 1 else 0
        l2 = ch["decoded"][loop_start - 2] if loop_start >= 2 else 0
    else:
        lps = l1 = l2 = 0
    if cafe:  # BCSTM/BFSTM: u8 ps, u8 reserved, s16 yn1, s16 yn2, twice, then padding (0x2E in all)
        return w.pack("BBhhBBhhH", ps, 0, 0, 0, lps, 0, l1, l2, 0)
    # BRSTM: gain, ps, yn1, yn2, loop ps, loop yn1, loop yn2, padding (0x30 in all)
    return w.pack("HHhhHhhH", 0, ps, 0, 0, lps, l1, l2, 0)


CODEC_IDS = {"pcm16": 1, "dsp": 2, "ima": 3}


def brstm(channels: list, rate: int, loop_start: Optional[int] = None, codec: str = "dsp",
          block_samples: int = DEFAULT_BLOCK_SAMPLES, big_endian: bool = True) -> tuple:
    """(file bytes, the samples FFmpeg should decode per channel) of a BRSTM."""
    w = _Writer(big_endian)
    enc = _encode(channels, rate, codec, block_samples)
    n = len(channels)
    # HEAD: three references, then the stream info, track table, channel table.
    head = bytearray(b"HEAD" + b"\0" * 4)
    refs_at = len(head)
    head += b"\0" * 24
    stream_off = len(head) - 8
    head += b"\0" * 0x34  # the stream info, filled once the DATA offset is known
    track_off = len(head) - 8
    head += w.pack("BBH", 1, 1, 0) + w.pack("BBHI", 1, 1, 0, len(head) - 8 + 12)
    head += w.pack("BBHI", 127, 64, 0, 0) + bytes([n]) + bytes(range(n))
    head += b"\0" * (-len(head) % 4)
    chan_off = len(head) - 8
    head += w.pack("BBH", n, 0, 0)
    table_at = len(head)
    head += b"\0" * (8 * n)
    for c, ch in enumerate(enc["channels"]):
        info_at = len(head) - 8
        struct.pack_into(w.e + "BBHI", head, table_at + 8 * c, 1, 0, 0, info_at)
        head += w.pack("BBHI", 1, 0, 0, info_at + 8)
        if codec == "dsp":
            head += b"".join(w.pack("h", v) for pair in ch["coefs"] for v in pair)
            head += _dsp_context(ch, w, loop_start if loop_start is not None else None, cafe=False)
        else:
            head += b"\0" * 0x30
    for i, off in enumerate((stream_off, track_off, chan_off)):
        struct.pack_into(w.e + "BBHI", head, refs_at + 8 * i, 1, 0, 0, off)
    _align(head)
    struct.pack_into(w.e + "I", head, 4, len(head))

    adpc = bytearray(b"ADPC" + b"\0" * 4)
    for b in range(enc["blocks"]):
        for ch in enc["channels"]:
            adpc += w.pack("hh", *ch["history"][b]) if codec == "dsp" else b"\0" * 4
    _align(adpc)
    struct.pack_into(w.e + "I", adpc, 4, len(adpc))

    head_at = 0x40
    adpc_at = head_at + len(head)
    data_at = adpc_at + len(adpc)
    samples = _block_data(enc, w, codec)
    data = bytearray(b"DATA" + w.pack("II", 0, 0x18))
    _align(data)
    data += samples
    _align(data)
    struct.pack_into(w.e + "I", data, 4, len(data))

    loops = loop_start is not None
    struct.pack_into(w.e + "BBBBHHIIIIIIIIIII", head, 8 + stream_off, CODEC_IDS[codec], int(loops), n, 0, rate, 0,
                     loop_start or 0, enc["count"], data_at + 0x20, enc["blocks"], enc["block_bytes"],
                     enc["block_samples"], enc["last_used"], enc["last_samples"], enc["last_padded"],
                     enc["block_samples"], 4)
    total = data_at + len(data)
    header = bytearray(b"RSTM" + (b"\xfe\xff" if big_endian else b"\xff\xfe") + bytes([1, 0]))
    header += w.pack("IHH", total, 0x40, 2)
    header += w.pack("IIIIII", head_at, len(head), adpc_at, len(adpc), data_at, len(data))
    _align(header, 0x40)
    return bytes(header + head + adpc + data), enc["decoded"]


def cstm(channels: list, rate: int, loop_start: Optional[int] = None, codec: str = "dsp", kind: str = "FSTM",
         big_endian: bool = True, block_samples: int = DEFAULT_BLOCK_SAMPLES,
         original_loop: Optional[tuple] = None) -> tuple:
    """(file bytes, the samples FFmpeg should decode per channel) of a BFSTM (kind "FSTM": Wii U
    big-endian, Switch little-endian) or a BCSTM (kind "CSTM", little-endian on the 3DS).
    original_loop: (start, end) for the BFSTM 0.4 "original loop" fields, which players ignore."""
    w = _Writer(big_endian)
    enc = _encode(channels, rate, codec, block_samples)
    n = len(channels)
    info = bytearray(b"INFO" + b"\0" * 4)
    info += b"\0" * 24  # three references
    stream_off = len(info) - 8
    stream_size = 0x38 if kind == "CSTM" else 0x50
    info += b"\0" * stream_size
    # Track info table: one track with every channel.
    track_off = len(info) - 8
    info += w.pack("I", 1)
    info += w.pack("HHI", 0x4101, 0, 12)
    info += w.pack("BBBB", 127, 64, 0, 0) + w.pack("HHI", 0x0100, 0, 12)
    info += w.pack("I", n) + bytes(range(n))
    info += b"\0" * (-len(info) % 4)
    chan_off = len(info) - 8
    table_at = len(info)
    info += w.pack("I", n) + b"\0" * (8 * n)
    infos_at = len(info)
    info += b"\0" * (8 * n)
    for c, ch in enumerate(enc["channels"]):
        adpcm_at = len(info)
        struct.pack_into(w.e + "HHI", info, table_at + 4 + 8 * c, 0x4102, 0, infos_at + 8 * c - table_at)
        if codec == "dsp":
            struct.pack_into(w.e + "HHI", info, infos_at + 8 * c, 0x0300, 0, adpcm_at - (infos_at + 8 * c))
            info += b"".join(w.pack("h", v) for pair in ch["coefs"] for v in pair)
            info += _dsp_context(ch, w, loop_start, cafe=True)
        elif codec == "ima":
            struct.pack_into(w.e + "HHI", info, infos_at + 8 * c, 0x0301, 0, adpcm_at - (infos_at + 8 * c))
            info += b"\0" * 8
        else:
            struct.pack_into(w.e + "HHI", info, infos_at + 8 * c, 0, 0, 0xFFFFFFFF)
    for i, (kind_id, off) in enumerate(((0x4100, stream_off), (0x0101, track_off), (0x0101, chan_off))):
        struct.pack_into(w.e + "HHI", info, 8 + 8 * i, kind_id, 0, off)
    _align(info)
    struct.pack_into(w.e + "I", info, 4, len(info))

    # SEEK: each block's two history samples per channel. FFmpeg reads a
    # big-endian BFSTM's SEEK table as little-endian (as the Wii U's files are).
    seek_w = _Writer(big_endian and kind != "FSTM")
    seek = bytearray(b"SEEK" + b"\0" * 4)
    for b in range(enc["blocks"]):
        for ch in enc["channels"]:
            seek += seek_w.pack("hh", *ch["history"][b]) if codec == "dsp" else b"\0" * 4
    _align(seek)
    struct.pack_into(w.e + "I", seek, 4, len(seek))

    data = bytearray(b"DATA" + b"\0" * 4)
    _align(data)
    data += _block_data(enc, w, codec)
    _align(data)
    struct.pack_into(w.e + "I", data, 4, len(data))

    info_at = 0x40
    seek_at = info_at + len(info)
    data_at = seek_at + len(seek)
    loops = loop_start is not None
    stream = w.pack("BBBBIIIIIIIIIII", CODEC_IDS[codec], int(loops), n, 0, rate, loop_start or 0, enc["count"],
                    enc["blocks"], enc["block_bytes"], enc["block_samples"], enc["last_used"],
                    enc["last_samples"], enc["last_padded"], 4, enc["block_samples"])
    stream += w.pack("HHI", 0x1F00, 0, 0x18)
    if kind != "CSTM":
        # BFSTM: region info size, padding, region reference (none), original loop start and end, padding.
        start, end = original_loop or ((loop_start or 0), enc["count"])
        stream += w.pack("HHHHIIII", 0x100, 0, 0, 0, 0xFFFFFFFF, start, end, 0)
    info[8 + stream_off:8 + stream_off + len(stream)] = stream
    version = 0x02000000 if kind == "CSTM" else (0x00030000 if big_endian else 0x00040000)
    total = data_at + len(data)
    header = bytearray(kind.encode() + (b"\xfe\xff" if big_endian else b"\xff\xfe"))
    header += w.pack("HIIHH", 0x40, version, total, 3, 0)
    for kind_id, at, size in ((0x4000, info_at, len(info)), (0x4001, seek_at, len(seek)), (0x4002, data_at, len(data))):
        header += w.pack("HHII", kind_id, 0, at, size)
    _align(header, 0x40)
    return bytes(header + info + seek + data), enc["decoded"]


def tone(freq: float, count: int, rate: int, level: int = 9000, phase: float = 0.0) -> list:
    return [int(round(level * math.sin(2 * math.pi * freq * i / rate + phase))) for i in range(count)]

"""Decode and encode PS-ADPCM ("VAG") sound, mono, as the game's voice banks store it.

A clip is a run of 16-byte frames. A frame holds 28 samples:

    byte 0      high nibble: predictor 0..4, low nibble: shift 0..12
    byte 1      flags
    bytes 2..15 28 four-bit signed steps, the low nibble of each byte first

A sample is its step scaled by the shift, plus a prediction from the two
samples before it: (step << 12 >> shift) + (s1 * a + s2 * b + 32 >> 6), kept
to 16 bits, with (a, b) one of COEFFICIENTS.

Every one of the 2,886 clips of the game's battle voice banks (tools/sscf_bank.py)
is laid out the same way, and `encode` writes that layout:

    one frame of sixteen zero bytes
    the sound, flags 0 on every frame but the last, which has flags 1
    the end frame: 00 07 and fourteen bytes 77

`decode` gives the sound alone: it leaves out the zero frame at the start and
stops at the end frame, so a clip of N frames gives (N - 2) * 28 samples and
encode(decode(clip)) is as long as clip. Sound is a list of ints, -32768..32767.

    ps_adpcm.py decode CLIP.vag OUT.wav [--rate 16000]
    ps_adpcm.py encode IN.wav CLIP.vag          16-bit mono WAV in
"""

import argparse
import struct
import wave
from pathlib import Path

FRAME_BYTES = 16
FRAME_SAMPLES = 28
COEFFICIENTS = ((0, 0), (60, 0), (115, -52), (98, -55), (122, -60))
MAX_SHIFT = 12
FLAG_LAST = 1
FLAG_END = 7
LEAD_FRAME = bytes(FRAME_BYTES)
END_FRAME = bytes((0, FLAG_END)) + b"\x77" * 14


class AdpcmError(ValueError):
    """The bytes are not a clip, or the sound cannot be written as one."""


def frames(data):
    """[(predictor, shift, flags, steps)] of every frame, the zero frame and the end frame included."""
    if len(data) % FRAME_BYTES:
        raise AdpcmError("A clip is a whole number of 16-byte frames")
    listed = []
    for position in range(0, len(data), FRAME_BYTES):
        steps = []
        for byte in data[position + 2:position + FRAME_BYTES]:
            for nibble in (byte & 15, byte >> 4):
                steps.append(nibble - 16 if nibble >= 8 else nibble)
        listed.append((data[position] >> 4, data[position] & 15, data[position + 1], steps))
    return listed


def sample_count(data):
    """How many samples `decode` gives for a clip laid out as the game's are."""
    return max(0, len(data) // FRAME_BYTES - 2) * FRAME_SAMPLES


def decode(data, lead=False):
    """The clip's sound. With `lead`, the 28 zero samples of a zero frame at the start are kept."""
    listed = frames(data)
    if listed and not lead and data[:FRAME_BYTES] == LEAD_FRAME:
        listed = listed[1:]
    sound = []
    older, old = 0, 0
    for predictor, shift, flags, steps in listed:
        if flags == FLAG_END:
            break
        if predictor >= len(COEFFICIENTS) or shift > MAX_SHIFT:
            raise AdpcmError(f"A frame has predictor {predictor} and shift {shift}")
        first, second = COEFFICIENTS[predictor]
        for step in steps:
            value = ((step << 12) >> shift) + ((old * first + older * second + 32) >> 6)
            value = -32768 if value < -32768 else 32767 if value > 32767 else value
            sound.append(value)
            older, old = old, value
    return sound


def _try(block, predictor, shift, older, old):
    """Encode one frame with a given predictor and shift: (squared error, steps, the last two samples as decoded)."""
    first, second = COEFFICIENTS[predictor]
    scale = 12 - shift
    half = (1 << scale) >> 1
    error = 0
    steps = []
    for wanted in block:
        predicted = (old * first + older * second + 32) >> 6
        step = (wanted - predicted + half) >> scale
        step = -8 if step < -8 else 7 if step > 7 else step
        value = (step << scale) + predicted
        if value > 32767 or value < -32768:
            # Stepping back from the clipped side keeps the decoder inside 16 bits where it can.
            nearer = step - 1 if value > 32767 else step + 1
            if -8 <= nearer <= 7:
                step = nearer
                value = (step << scale) + predicted
            value = -32768 if value < -32768 else 32767 if value > 32767 else value
        steps.append(step)
        error += (wanted - value) * (wanted - value)
        older, old = old, value
    return error, steps, older, old


def _encode_frame(block, older, old):
    """The best (predictor, shift, steps, older, old) for 28 samples after the two decoded before them."""
    best = None
    for predictor, (first, second) in enumerate(COEFFICIENTS):
        # The largest step the frame needs if every sample before were kept exactly: it sets the finest
        # shift worth trying. The frame is then encoded for real at that shift and its two neighbours.
        before_older, before = older, old
        largest = 0
        for wanted in block:
            size = abs(wanted - ((before * first + before_older * second + 32) >> 6))
            if size > largest:
                largest = size
            before_older, before = before, wanted
        shift = MAX_SHIFT
        while shift > 0 and largest > (7 << (12 - shift)) + ((1 << (12 - shift)) >> 1):
            shift -= 1
        for candidate in (shift, shift - 1, shift + 1):
            if not 0 <= candidate <= MAX_SHIFT:
                continue
            error, steps, new_older, new_old = _try(block, predictor, candidate, older, old)
            if best is None or error < best[0]:
                best = (error, predictor, candidate, steps, new_older, new_old)
            if error == 0:
                break
        if best[0] == 0:
            break
    return best[1:]


def encode(sound):
    """The sound as a clip: the zero frame, the frames of sound (the last padded with silence) and the end frame."""
    sound = list(sound)
    if not sound:
        raise AdpcmError("There is no sound to encode")
    for value in sound:
        if not -32768 <= value <= 32767:
            raise AdpcmError(f"Sample {value} is outside 16 bits")
    sound += [0] * (-len(sound) % FRAME_SAMPLES)
    data = bytearray(LEAD_FRAME)
    older, old = 0, 0
    last = len(sound) - FRAME_SAMPLES
    for position in range(0, len(sound), FRAME_SAMPLES):
        predictor, shift, steps, older, old = _encode_frame(sound[position:position + FRAME_SAMPLES], older, old)
        data.append(predictor << 4 | shift)
        data.append(FLAG_LAST if position == last else 0)
        for index in range(0, FRAME_SAMPLES, 2):
            data.append((steps[index] & 15) | (steps[index + 1] & 15) << 4)
    return bytes(data + END_FRAME)


def read_wav(path):
    """(sound, rate) of a 16-bit mono WAV file."""
    with wave.open(str(path), "rb") as source:
        if source.getnchannels() != 1 or source.getsampwidth() != 2:
            raise AdpcmError(f"{path} is not 16-bit mono")
        raw = source.readframes(source.getnframes())
        return list(struct.unpack(f"<{len(raw) // 2}h", raw)), source.getframerate()


def write_wav(path, sound, rate):
    """Write sound as a 16-bit mono WAV file."""
    with wave.open(str(path), "wb") as target:
        target.setnchannels(1)
        target.setsampwidth(2)
        target.setframerate(rate)
        target.writeframes(struct.pack(f"<{len(sound)}h", *sound))


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("action", choices=("decode", "encode"))
    parser.add_argument("source", type=Path)
    parser.add_argument("target", type=Path)
    parser.add_argument("--rate", type=int, default=16000, help="sample rate written to a decoded WAV")
    arguments = parser.parse_args()
    if arguments.action == "decode":
        write_wav(arguments.target, decode(arguments.source.read_bytes()), arguments.rate)
    else:
        arguments.target.write_bytes(encode(read_wav(arguments.source)[0]))


if __name__ == "__main__":
    main()

"""Write the example mod's test tones into music/ next to this script.

    python make_tones.py [folder]

Only generated sine tones: no recorded or real music. The standard library is
enough. Each file shows one part of the mod format (docs/port/mod-folder.md):

  intro_loop.wav      44.1 kHz stereo. 2 s at 440 Hz (the intro), then
                      alternating 660 Hz and 880 Hz, one second each, from 2 s
                      to 6 s (samples 88200 to 264600, the loop in mod.json).
                      Heard in the game: 440 once, then 660/880 for ever.
  chord_48k_mono.wav  48 kHz mono, so the port resamples and doubles it: a
                      C-E chord (523.25 + 659.25 Hz), 3 s, looped whole.
  steps.wav           44.1 kHz stereo: 330 Hz for 1 s, then 495 Hz and 594 Hz
                      a second each, then 1 s of 330 Hz that the loop (1.0 s to
                      3.0 s, in seconds) never reaches again.
  sweep.wav           44.1 kHz stereo, 300 to 1200 Hz over 4 s, played once.
"""

import math
import os
import struct
import sys
import wave

AMPLITUDE = 0.25
FADE = 0.005  # seconds of fade at each segment's ends, so joins do not click


def segment(freqs, seconds, rate):
    """Samples of a sum of sines, faded in and out."""
    count = int(round(seconds * rate))
    fade = int(FADE * rate)
    out = []
    for i in range(count):
        t = i / rate
        value = sum(math.sin(2 * math.pi * f * t) for f in freqs) / len(freqs)
        gain = min(1.0, i / fade if fade else 1.0, (count - 1 - i) / fade if fade else 1.0)
        out.append(AMPLITUDE * gain * value)
    return out


def sweep(start, end, seconds, rate):
    count = int(round(seconds * rate))
    fade = int(FADE * rate)
    out = []
    phase = 0.0
    for i in range(count):
        f = start + (end - start) * i / count
        phase += 2 * math.pi * f / rate
        gain = min(1.0, i / fade, (count - 1 - i) / fade)
        out.append(AMPLITUDE * gain * math.sin(phase))
    return out


def write(path, samples, rate, channels):
    with wave.open(path, "wb") as out:
        out.setnchannels(channels)
        out.setsampwidth(2)
        out.setframerate(rate)
        frames = bytearray()
        for value in samples:
            word = struct.pack("<h", max(-32768, min(32767, int(round(value * 32767)))))
            frames += word * channels
        out.writeframes(bytes(frames))
    print(f"wrote {path} ({len(samples) / rate:.2f} s, {rate} Hz, {channels} channel(s))")


def main():
    folder = sys.argv[1] if len(sys.argv) > 1 else os.path.dirname(os.path.abspath(__file__))
    music = os.path.join(folder, "music")
    os.makedirs(music, exist_ok=True)
    rate = 44100
    intro = segment([440.0], 2.0, rate)
    loop = []
    for _ in range(2):
        loop += segment([660.0], 1.0, rate) + segment([880.0], 1.0, rate)
    write(os.path.join(music, "intro_loop.wav"), intro + loop, rate, 2)
    write(os.path.join(music, "chord_48k_mono.wav"), segment([523.25, 659.25], 3.0, 48000), 48000, 1)
    steps = segment([330.0], 1.0, rate) + segment([495.0], 1.0, rate) + segment([594.0], 1.0, rate)
    write(os.path.join(music, "steps.wav"), steps + segment([330.0], 1.0, rate), rate, 2)
    write(os.path.join(music, "sweep.wav"), sweep(300.0, 1200.0, 4.0, rate), rate, 2)


if __name__ == "__main__":
    main()

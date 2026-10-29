"""End-to-end check: a mod made with the Battle BGM Maker (BgmMaker.exe, its
command line, which runs the window's own code) plays in the port with its loop
exactly where the tool set it, with an MP3 (encoder delay and padding) as the
source. Test tones only.

    python port/tools/bgm_maker/tests/port_check.py            # make, run the port (90 s at most), analyse
    python port/tools/bgm_maker/tests/port_check.py --analyse  # analyse the last run's dump again

1. build/port/data-modmaker-test is made once as a copy of build/port/data
   (muted in its settings.ini; build/port/data itself is never written).
2. A 5 s tone track is written and encoded to MP3 with the port's FFmpeg:
   440 Hz (+571) for 2 s, 660 Hz (+1013) for 1 s, 880 Hz (+1327) for 0.95 s,
   a 50 ms marker of 990 Hz (+1453), then 1320 Hz (+1789) for 1 s. The second
   tone of each part makes the waveform non-periodic, and the marker gives
   every seam a change of its own to align on, so the sample-level match below
   has one answer.
3. With BgmMaker (tools/local/port/build-d012-recomp/bin): --import the MP3
   into a new mod "modmaker-test" (copied into music/), --set-loop from sample
   88,200 (the 660 Hz part) to 176,400 (the end of the marker), --check it, and
   --decode it for the analysis. If the port and the tool counted
   samples differently (an MP3's 1,105-sample delay, say), the game would play
   a little 440 Hz after each jump, or a little 1320 Hz before it.
4. The recompiled port runs (tools/port/run_d012.py, at most 90 s, its own
   process only) to a Quick Battle with D012_TEST_BGM_START on the track and
   D012_AUDIO_DUMP writing its mix.
5. The dump is scanned with Goertzel filters (10 ms windows, 5 ms steps) for
   the tones around every seam, and each seam is located to the sample by
   matching the dump against the tool's own decode of the MP3. The dump is the
   audio device's timeline, which can have short gaps when the game's audio
   thread falls behind, so each seam is aligned on its own; the battle's own
   sounds are mixed over some seams.
MIT licence; see port/README.md.
"""

from __future__ import annotations

import argparse
import json
import math
import os
import re
import shutil
import struct
import subprocess
import sys
import wave

TOOL = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, TOOL)

from bgmmaker import audio  # noqa: E402

ROOT = audio.REPO_ROOT
SOURCE_DATA = os.path.join(ROOT, "build", "port", "data")
DATA = os.path.join(ROOT, "build", "port", "data-modmaker-test")
OUT = os.path.join(ROOT, "build", "port", "modmaker-test")
BUILD = os.path.join("tools", "local", "port", "build-d012-recomp")
BGM_MAKER = os.path.join(ROOT, BUILD, "bin", "BgmMaker.exe" if os.name == "nt" else "BgmMaker")
RATE = 44100
MOD_ID = "modmaker-test"
LOOP_START, LOOP_END = 88200, 176400
PARTS = [(440, 571, 2.0), (660, 1013, 1.0), (880, 1327, 0.95), (990, 1453, 0.05), (1320, 1789, 1.0)]
LABELS = {440: "A 440", 660: "B 660", 880: "C 880", 990: "E 990", 1320: "D 1320"}
ROUTE = ("460:pad a;1080:pad a;1500:pad a;1650:pad a;1900:pad dpdown;2000:pad a;2400:pad a;3200:pad a;"
         "3700:pad a;4400:pad a;4900:pad a;5600:pad dpdown;5750:pad dpright;5900:pad dpup;6050:pad a;"
         "7100:pad a;8000:pad a;8020:speed normal;9700:quit")


def make_data_dir():
    if os.path.isdir(DATA):
        return
    shutil.copytree(SOURCE_DATA, DATA, ignore=shutil.ignore_patterns("mods"))
    settings = os.path.join(DATA, "settings.ini")
    if os.path.isfile(settings):
        with open(settings, encoding="utf-8") as handle:
            text = handle.read()
        text = re.sub(r"^audio\.mute=.*$", "audio.mute=1", text, flags=re.M)
        with open(settings, "w", encoding="utf-8") as handle:
            handle.write(text)
    print(f"made {DATA} (a muted copy of build/port/data)")


def make_tones(ff) -> str:
    source = os.path.join(OUT, "source")
    os.makedirs(source, exist_ok=True)
    wav_path = os.path.join(source, "modmaker_tones.wav")
    frames = bytearray()
    # Phases chosen so the samples on each side of the loop points are at opposite peaks:
    # the intro ends low and the marker ends high, the loop starts high and the part after
    # the loop starts low. The right seam (marker, then loop start) is smooth; a seam one
    # sample off either way plays a sample a whole peak-to-peak away, which the
    # sample-level match below cannot miss even with the battle's sounds over it.
    shapes = {440: ("end", -1), 660: ("start", 1), 880: ("sin", 1), 990: ("end", 1), 1320: ("start", -1)}
    for f1, f2, seconds in PARTS:
        count = int(round(seconds * RATE))
        anchor, sign = shapes[f1]
        for i in range(count):
            t = (i - (count - 1 if anchor == "end" else 0)) / RATE
            wave_ = math.sin if anchor == "sin" else math.cos
            v = int(sign * (9000 * wave_(2 * math.pi * f1 * t) + 4500 * wave_(2 * math.pi * f2 * t)))
            frames += struct.pack("<hh", v, v)
    with wave.open(wav_path, "wb") as out:
        out.setnchannels(2)
        out.setsampwidth(2)
        out.setframerate(RATE)
        out.writeframes(bytes(frames))
    mp3 = os.path.join(source, "modmaker_tones.mp3")
    subprocess.run([ff.ffmpeg, "-hide_banner", "-v", "error", "-y", "-i", wav_path, "-c:a", "libmp3lame", "-b:a",
                    "192k", "-metadata", "title=BGM maker test tones", mp3], check=True)
    return mp3


def bgm_maker(*args) -> str:
    """Runs BgmMaker's command line; its output. A failure stops the check."""
    run = subprocess.run([BGM_MAKER, *args], capture_output=True, timeout=600)
    output = (run.stdout + run.stderr).decode("utf-8", "replace").replace("\r\n", "\n")
    for line in output.splitlines():
        print("  BgmMaker:", line)
    if run.returncode != 0:
        raise RuntimeError(f"BgmMaker {args[0]} failed ({run.returncode})")
    return output


def track_facts(output: str, key: str) -> tuple:
    """(file, samples, rate) of a track in BgmMaker --check's output."""
    match = re.search(rf"^track {re.escape(key)}: (.+), (\d+) samples at (\d+) Hz", output, re.M)
    if not match:
        raise RuntimeError(f"BgmMaker --check says nothing of track {key}")
    return match.group(1), int(match.group(2)), int(match.group(3))


def tool_decode(file: str, wav_path: str):
    """The file as BgmMaker decodes it (the samples its loop points count in), as a WAV."""
    raw = wav_path + ".raw"
    said = bgm_maker("--decode", file, raw)
    frames, rate, channels = map(int, re.search(r"(\d+) frames at (\d+) Hz, (\d) channel", said).groups())
    with open(raw, "rb") as handle:
        data = handle.read()
    os.remove(raw)
    with wave.open(wav_path, "wb") as out:
        out.setnchannels(channels)
        out.setsampwidth(2)
        out.setframerate(rate)
        out.writeframes(data)
    return frames


def make_mod(ff, mp3) -> str:
    mods = os.path.join(DATA, "mods")
    os.makedirs(mods, exist_ok=True)
    folder = os.path.join(mods, MOD_ID)
    if os.path.isdir(folder):
        shutil.rmtree(folder)  # the test's own mod in the test data
    added = bgm_maker("--import", folder, "tones", mp3)
    key, file = re.search(r"^added (\S+): (.+) \(\"", added, re.M).groups()
    bgm_maker("--set-loop", folder, key, str(LOOP_START), str(LOOP_END))
    checked = bgm_maker("--check", folder)
    _, samples, rate = track_facts(checked, key)
    print(f"imported {file} (key {key}): {rate} Hz, {samples} samples after decoding (encoded from "
          f"{int(sum(p[2] for p in PARTS) * RATE)})")
    with open(os.path.join(folder, "mod.json"), encoding="utf-8") as handle:
        print("mod.json loop:", json.load(handle)["music"]["series"][0]["tracks"][0]["loop"])
    tool_decode(os.path.join(folder, *file.split("/")), os.path.join(OUT, "tool_decode.wav"))
    return f"{MOD_ID}:{key}"


def run_port(track_id):
    dump = os.path.join(OUT, "dump.wav")
    if os.path.exists(dump):
        os.remove(dump)
    command = [sys.executable, os.path.join(ROOT, "tools", "port", "run_d012.py"), "--seconds", "90",
               "--build-dir", BUILD, "--data-dir", os.path.relpath(DATA, ROOT), "--log", os.path.join(OUT, "run.log"),
               "--env", "PSPRECOMP_NO_INTERPRETER=1", "--env", "D012_UNTHROTTLED=1",
               "--env", f"D012_TEST_BGM_START={track_id}", "--env", f"D012_AUDIO_DUMP={dump}",
               "--env", f"D012_INPUT_SCRIPT={ROUTE}"]
    print("running the port (at most 90 s)...")
    subprocess.run(command, check=True, timeout=150)
    with open(os.path.join(OUT, "run.log"), encoding="utf-8", errors="replace") as handle:
        for line in handle:
            if MOD_ID in line or "[mods]" in line and ("error" in line or "warning" in line):
                print("  log:", line.rstrip())


# --- Analysis --------------------------------------------------------------

def read_left(path):
    with wave.open(path) as handle:
        channels, rate, n = handle.getnchannels(), handle.getframerate(), handle.getnframes()
        data = handle.readframes(n)
    values = struct.unpack(f"<{n * channels}h", data)
    return list(values[::channels]), rate


def goertzel(x, start, size, freq, rate):
    k = 2 * math.cos(2 * math.pi * freq / rate)
    s1 = s2 = 0.0
    for i in range(start, start + size):
        s0 = x[i] + k * s1 - s2
        s2, s1 = s1, s0
    return s1 * s1 + s2 * s2 - k * s1 * s2


def tone_of(x, start, size, rate):
    """(the strongest test tone in the window, its share of the window's energy); (None, 0) when quiet."""
    energy = sum(v * v for v in x[start:start + size])
    if energy < size * 300 ** 2:
        return None, 0.0
    best, share = None, 0.0
    for freq in LABELS:
        ratio = goertzel(x, start, size, freq, rate) / (energy * size / 2)
        if ratio > share:
            best, share = freq, ratio
    return best, share


def expected_at(x, t, delta=0):
    """Sample t of what the port plays if its samples are the tool's shifted
    by `delta` (delta > 0: the port's decode has `delta` extra samples at the
    front, so its loop points land `delta` samples early in the music)."""
    length = LOOP_END - LOOP_START
    if t < LOOP_END - delta:
        return x[t] if 0 <= t < len(x) else 0
    at = LOOP_START - delta + (t - (LOOP_END - delta)) % length
    return x[at] if 0 <= at < len(x) else 0


def find_onset(marks, hop, size):
    """The dump sample, to about 0.1 s, where the loop first starts: the
    first of three passes of 660 Hz then 880 Hz a second later. `marks` is
    tone_of() every `hop` samples. None when the loop is not found."""
    def is_tone(i, freq):
        return any(0 <= k < len(marks) and marks[k][0] == freq and marks[k][1] > 0.45 for k in (i - 1, i, i + 1))
    for i in range(len(marks) - 60):
        hits = sum(is_tone(i + 20 * r, 660) + is_tone(i + 10 + 20 * r, 880) for r in range(3))
        if hits >= 5:
            first = i
            while first >= 20 and is_tone(first - 20, 660) + is_tone(first - 10, 880) >= 1:
                first -= 20  # back to the first pass of the loop
            return first * hop + size // 2
    return None


def analyse():
    dump, rate = read_left(os.path.join(OUT, "dump.wav"))
    tool, _ = read_left(os.path.join(OUT, "tool_decode.wav"))
    print(f"dump: {len(dump) / rate:.1f} s at {rate} Hz; tool decode: {len(tool)} samples")
    length = LOOP_END - LOOP_START

    # 1. Coarse (0.1 s steps): the loop is 660 Hz and 880 Hz taking turns each second.
    #    The battle's own sounds play over it, so a few hits are enough.
    size, hop = 2048, 4410
    marks = [tone_of(dump, at, size, rate) for at in range(0, len(dump) - size, hop)]

    onset = find_onset(marks, hop, size)
    if onset is None:
        print("FAIL: the loop (660 Hz and 880 Hz taking turns) is not in the dump")
        return False

    # 2. To the sample: match the dump against the tool's decode of the MP3 over the intro.
    def correlation(tau, window=8192, stride=4, at=LOOP_START - 4096):
        sab = saa = sbb = 0.0
        for i in range(0, window, stride):
            a, b = dump[tau + at + i], tool[at + i]
            sab += a * b; saa += a * a; sbb += b * b
        return sab / math.sqrt(saa * sbb) if saa and sbb else -1.0
    guess = onset - LOOP_START
    tau = max(range(guess - 9000, guess + 9001, 2), key=correlation)
    tau = max(range(tau - 4, tau + 5), key=lambda t: correlation(t, 16384, 1))
    score = correlation(tau, 16384, 1)
    span = range(LOOP_START + 4410, LOOP_END - 4410, 3)
    num = sum(dump[tau + i] * tool[i] for i in span)
    den = sum(tool[i] * tool[i] for i in span)
    gain = num / den
    print(f"the track starts at dump sample {tau} ({tau / rate:.4f} s): correlation with the tool's decode "
          f"{score:.4f}, gain {gain:.3f} (the game's music volume)")

    # 3. Each seam on its own. The dump is the audio device's timeline: where the game's
    #    audio thread falls behind, the port restarts its stream a little later, so the
    #    dump can have short gaps between seams. Each seam is aligned from the music just
    #    before it (which no loop error can change), then judged on what follows.
    def fit(tau_j, t0, t1, stride):
        """Correlation and gain of dump[tau_j + t] against the expected stream, t in [t0, t1)."""
        sab = saa = sbb = 0.0
        for t in range(t0, t1, stride):
            a, b = dump[tau_j + t], expected_at(tool, t, 0)
            sab += a * b; saa += a * a; sbb += b * b
        return (sab / math.sqrt(saa * sbb) if saa and sbb else -1.0), (sab / sbb if sbb else 0.0)

    def align(seam_t, around, radius):
        # The last 140 ms before the jump: the 880 Hz chord, then the 50 ms 990 Hz marker.
        # A steady chord alone would match at many shifts; the change between them at one.
        t0, t1 = seam_t - 6000, seam_t - 100
        best = max(range(around - radius, around + radius + 1, 2), key=lambda k: fit(k, t0, t1, 7)[0])
        best = max(range(best - 3, best + 4), key=lambda k: fit(k, t0, t1, 1)[0])
        return best, fit(best, t0, t1, 1)

    def residual(tau_j, gain_j, seam_t, delta, half):
        diff = sig = 0.0
        for t in range(seam_t - half, seam_t + half):
            d = dump[tau_j + t] - gain_j * expected_at(tool, t, delta)
            diff += d * d; sig += dump[tau_j + t] ** 2
        return diff / sig if sig else 9e9

    win, step = 441, 220
    letters = {440: "A", 660: "B", 880: "C", 990: "E", 1320: "D", None: "."}
    text = lambda marks_: "".join(letters[t if s > 0.3 else None] for t, s in marks_)  # noqa: E731
    intro = [tone_of(dump, tau + k, win, rate) for k in range(2205, LOOP_START - 2205, 4410)]
    print(f"\nintro, every 0.1 s: {text(intro)}  (A = 440 Hz)")
    print("seams: Goertzel scan of the 50 ms before and after each jump (10 ms windows, 5 ms steps;"
          " E = the 990 Hz marker that ends the loop, B = 660 Hz starts it; A = 440 or D = 1320 would mean an offset),"
          " then the jump matched to the sample against the tool's decode:")
    results, stray_total, tau_j, skipped, levels = [], 0, tau, 0, []
    for j in range(12):
        seam_t = LOOP_END + j * length
        if tau_j + seam_t + 25000 >= len(dump):
            break
        found_tau, (corr, gain_j) = align(seam_t, tau_j, 4000)
        if corr < 0.6:
            found_tau, (corr, gain_j) = align(seam_t, tau_j, 20000)
        if corr < 0.6:
            # The battle's sounds drown the marker: scan where the seam should be, match nothing.
            seam = tau_j + seam_t
            before = [tone_of(dump, seam - 2205 + k, win, rate) for k in range(0, 2205 - win + 1, step)]
            after = [tone_of(dump, seam + k, win, rate) for k in range(0, 2205 - win + 1, step)]
            print(f"  seam {j + 1:2d} at {seam / rate:8.3f} s: {text(before)} | {text(after)}  (battle sounds over it;"
                  f" alignment {corr:.2f}; not counted)")
            skipped += 1
            continue
        tau_j = found_tau
        seam = tau_j + seam_t
        before = [tone_of(dump, seam - 2205 + k, win, rate) for k in range(0, 2205 - win + 1, step)]
        after = [tone_of(dump, seam + k, win, rate) for k in range(0, 2205 - win + 1, step)]
        stray = sum(1 for t, s in before + after if t in (440, 1320) and s > 0.3)
        stray_total += stray
        level = {d: residual(tau_j, gain_j, seam_t, d, 1000) for d in range(-12, 13)}
        coarse_d = min(range(-1400, 1401, 4), key=lambda d: residual(tau_j, gain_j, seam_t, d, 500))
        if abs(coarse_d) > 12:
            level[coarse_d] = residual(tau_j, gain_j, seam_t, coarse_d, 1000)
        level[1105] = residual(tau_j, gain_j, seam_t, 1105, 1000)
        level[-1105] = residual(tau_j, gain_j, seam_t, -1105, 1000)
        best = min(level, key=level.get)
        # Shifts the dump cannot tell from the best (within 1 % of its residual).
        alike = sorted(d for d, v in level.items() if v <= level[best] * 1.01)
        db = lambda v: 10 * math.log10(max(v, 1e-12))  # noqa: E731
        print(f"  seam {j + 1:2d} at {seam / rate:8.3f} s: {text(before)} | {text(after)}  A/D {stray};"
              f" best shift {best:+d} (residual {db(level[best]):5.1f} dB; as good: {alike[0]:+d}..{alike[-1]:+d});"
              f" at +1105 {db(level[1105]):5.1f} dB, at -1105 {db(level[-1105]):5.1f} dB")
        results.append((best, alike, db(level[best]), db(level[1105]), db(level[-1105])))
        levels.append(level)
    # A real offset would be the same at every seam; the battle's sounds are not. So the
    # residuals of all matched seams are added up, and the shift that explains them all best
    # is the port's offset against the tool.
    shifts = list(range(-12, 13)) + [-1105, 1105]
    joint = {d: sum(level[d] / min(level.values()) for level in levels) for d in shifts} if levels else {}
    joint_best = min(joint, key=joint.get) if joint else None
    exact_seams = [r for r in results if r[2] < -40]
    print(f"\n{len(results)} seams matched, {skipped} drowned by the battle; each seam's best shift: "
          f"{[r[0] for r in results]}")
    if joint:
        ranked = sorted(joint, key=joint.get)
        worse = lambda d: 100 * (joint[d] / joint[joint_best] - 1)  # noqa: E731
        print(f"all matched seams together: best shift {joint_best:+d}; next {ranked[1]:+d} (+{worse(ranked[1]):.1f} %),"
              f" {ranked[2]:+d} (+{worse(ranked[2]):.1f} %); an MP3 delay, +1105: +{worse(1105):.0f} %, -1105:"
              f" +{worse(-1105):.0f} %")
    if exact_seams:
        print(f"seams with the music alone (residual under -40 dB): {[r[0] for r in exact_seams]}")
    no_delay = bool(joint) and min(joint[1105], joint[-1105]) > 1.5 * joint[joint_best]

    # The port's own account: what it decoded, and where it put the loop.
    decoded = None
    log_path = os.path.join(OUT, "run.log")
    if os.path.isfile(log_path):
        with open(log_path, encoding="utf-8", errors="replace") as handle:
            for line in handle:
                found = re.search(r"decoded " + re.escape(MOD_ID) + r":\S+: .*?(\d+) frames at 44100 Hz.*loop (\d+)\.\.(\d+)",
                                  line)
                if found:
                    decoded = tuple(int(v) for v in found.groups())
    print(f"the port's log: {decoded[0] if decoded else '?'} frames decoded (the tool: {len(tool)}), loop "
          f"{decoded[1] if decoded else '?'}..{decoded[2] if decoded else '?'} (frames, end included; the tool set "
          f"{LOOP_START} to {LOOP_END})")
    log_ok = decoded == (len(tool), LOOP_START, LOOP_END - 1)
    passed = bool(results) and joint_best == 0 and no_delay and stray_total == 0 and log_ok and \
        all(r[0] == 0 for r in exact_seams)
    print(f"\n{'PASS' if passed else 'FAIL'}: the loop jumps from sample {LOOP_END} back to {LOOP_START}, where the "
          f"tool set it. 440/1320 Hz at a matched seam: {stray_total} windows; offset of all seams together: "
          f"{joint_best}; the port's frames and loop {'equal' if log_ok else 'differ from'} the tool's")
    return passed


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("--analyse", action="store_true", help="only analyse the last run's dump")
    options = parser.parse_args()
    os.makedirs(OUT, exist_ok=True)
    if not options.analyse:
        ff = audio.find_ffmpeg()
        make_data_dir()
        mp3 = make_tones(ff)
        track_id = make_mod(ff, mp3)
        run_port(track_id)
    return 0 if analyse() else 1


if __name__ == "__main__":
    sys.exit(main())

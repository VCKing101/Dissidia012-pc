"""End-to-end check: a BRSTM imported through the mod maker, saved with
"loop": "auto", loops in the game exactly at the loop start its header gives.
Test tones only (stream_fixtures.py writes the file; FFmpeg cannot).

    python port/tools/mod_maker/tests/stream_port_check.py            # make, run the port (90 s at most), analyse
    python port/tools/mod_maker/tests/stream_port_check.py --analyse  # analyse the last run's dump again

1. build/port/data-brstm-test is made once as a muted copy of build/port/data
   (build/port/data itself is never written).
2. A 4 s stereo 44,100 Hz DSP-ADPCM BRSTM: 440 Hz (+571) up to sample
   88,207 (not a multiple of a 14-sample frame), then the loop: 660 Hz (+1013)
   for 1 s, 880 Hz (+1327) for 0.95 s and a 50 ms 990 Hz (+1453) marker to
   the end of the stream. The header says: loop flag on, loop start 88,207,
   176,407 samples. Phases as in port_check.py, so a seam one sample off is a
   whole peak-to-peak step.
3. Through the tool's API: create the mod "brstm-test", import the BRSTM, see
   the header's loop pre-filled, save it as "auto".
4. A second mod, "brstm-variants", written directly (it has entries the tool
   refuses): the same tones as BCSTM, Wii U BFSTM (big-endian), Switch BFSTM
   (little-endian, 48 kHz), a BRSTM without a loop flag, a BRSTM with a loop
   given in the manifest (which wins over the header's), an IMA-ADPCM BCSTM and
   a BWAV (both refused with a clear error).
5. The recompiled port runs (tools/port/run_d012.py, at most 90 s, its own
   process only) to a Quick Battle with D012_TEST_BGM_START on the BRSTM,
   D012_TEST_MODS_DECODE=1 and D012_AUDIO_DUMP.
6. port_check.py's analysis: a Goertzel scan of every seam (10 ms windows, 5 ms
   steps) and a sample-level match of each seam against the tool's decode,
   which is the encoder's own reconstruction sample for sample
   (test_streams.py), and the port's log of the variants.
MIT licence; see port/README.md.
"""

from __future__ import annotations

import argparse
import json
import math
import os
import re
import shutil
import sys
import threading

HERE = os.path.dirname(os.path.abspath(__file__))
TOOL = os.path.dirname(HERE)
sys.path.insert(0, TOOL)
sys.path.insert(0, HERE)

import port_check as pc  # noqa: E402
import stream_fixtures as sf  # noqa: E402
from modmaker import audio  # noqa: E402
from modmaker.server import App, serve  # noqa: E402

ROOT = audio.REPO_ROOT
DATA = os.path.join(ROOT, "build", "port", "data-brstm-test")
OUT = os.path.join(ROOT, "build", "port", "brstm-test")
MOD_ID = "brstm-test"
VARIANTS_ID = "brstm-variants"
RATE = 44100
LOOP_START = 88207
PARTS = [(440, 571, LOOP_START), (660, 1013, 44100), (880, 1327, 41895), (990, 1453, 2205)]
LOOP_END = sum(p[2] for p in PARTS)  # 176,407: the stream ends where the loop does


def tones(rate=RATE) -> list:
    """The test piece at `rate` (part lengths scaled), one channel."""
    shapes = {440: ("end", -1), 660: ("start", 1), 880: ("sin", 1), 990: ("end", 1)}
    out = []
    for f1, f2, count in PARTS:
        count = round(count * rate / RATE)
        anchor, sign = shapes[f1]
        wave_ = math.sin if anchor == "sin" else math.cos
        for i in range(count):
            t = (i - (count - 1 if anchor == "end" else 0)) / rate
            out.append(int(sign * (9000 * wave_(2 * math.pi * f1 * t) + 4500 * wave_(2 * math.pi * f2 * t))))
    return out


def make_files() -> dict:
    source = os.path.join(OUT, "source")
    os.makedirs(source, exist_ok=True)
    pcm = tones()
    pcm48 = tones(48000)
    loop48 = round(LOOP_START * 48000 / RATE)
    files = {
        "Battle Tones (loop).brstm": sf.brstm([pcm, pcm], RATE, LOOP_START)[0],
        "tones.bcstm": sf.cstm([pcm, pcm], RATE, LOOP_START, kind="CSTM", big_endian=False)[0],
        "tones_wiiu.bfstm": sf.cstm([pcm, pcm], RATE, LOOP_START, kind="FSTM", big_endian=True)[0],
        "tones_switch_48k.bfstm": sf.cstm([pcm48, pcm48], 48000, loop48, kind="FSTM", big_endian=False)[0],
        "tones_noloop.brstm": sf.brstm([pcm, pcm], RATE, None)[0],
        "tones_ima.bcstm": sf.cstm([[0] * 4000], RATE, 0, codec="ima", kind="CSTM", big_endian=False)[0],
        "tones.bwav": b"BWAV\xfe\xff" + b"\0" * 58,
    }
    paths = {}
    for name, data in files.items():
        paths[name] = os.path.join(source, name)
        with open(paths[name], "wb") as handle:
            handle.write(data)
    print(f"wrote {len(files)} test files in {source}; the BRSTM: {LOOP_END} samples, loop from {LOOP_START}")
    return paths


def make_mod(ff, paths) -> str:
    mods = os.path.join(DATA, "mods")
    os.makedirs(mods, exist_ok=True)
    for mod in (MOD_ID, VARIANTS_ID):
        if os.path.isdir(os.path.join(mods, mod)):
            shutil.rmtree(os.path.join(mods, mod))  # the test's own mods in the test data
    app = App(mods, audio.Decoder(ff))
    server = serve(app)
    threading.Thread(target=server.serve_forever, daemon=True).start()
    try:
        api = pc.Client(app)
        assert api.call("/api/mod/create", {"id": MOD_ID, "name": "BRSTM test", "version": "1.0.0",
                                            "author": "Dissidia 012 native port"})["ok"]
        name = "Battle Tones (loop).brstm"
        with open(paths[name], "rb") as handle:
            imported = api.call(f"/api/track/import?folder={MOD_ID}&name={name.replace(' ', '%20')}", raw=handle.read())
        info = api.call("/api/track/info", {"folder": MOD_ID, "file": imported["file"]})
        assert imported["info"]["loop_tags"] == info["loop_tags"]
        print(f"imported {imported['file']} as \"{imported['title']}\": {info['stream']['description']}, "
              f"{info['rate']} Hz, {info['samples']} samples decoded; loop pre-filled {info['loop_tags']}")
        assert info["loop_tags"] == {"start": LOOP_START, "end": LOOP_END, "source": "the BRSTM header"}
        key = api.call("/api/key", {"kind": "track", "text": imported["title"], "taken": [], "mod_id": MOD_ID})["key"]
        model = {"id": MOD_ID, "name": "BRSTM test", "version": "1.0.0", "author": "Dissidia 012 native port",
                 "description": "", "extra": {}, "series": [{"key": "brstm", "name": "BRSTM TEST", "order": 0,
                 "extra": {}, "tracks": [{"key": key, "title": imported["title"], "file": imported["file"],
                                          "extra": {}, "loop": {"mode": "auto"}}]}]}
        saved = api.call("/api/mod/save", {"folder": MOD_ID, "model": model})
        assert saved["ok"], saved["problems"]
        folder = os.path.join(mods, MOD_ID)
        with open(os.path.join(folder, "mod.json"), encoding="utf-8") as handle:
            print("mod.json track:", json.load(handle)["music"]["series"][0]["tracks"][0])
        shutil.copyfile(app.decoder.wav(os.path.join(folder, *imported["file"].split("/"))),
                        os.path.join(OUT, "tool_decode.wav"))
    finally:
        server.shutdown()
        server.server_close()

    # The variants, as a modder might write them by hand.
    folder = os.path.join(mods, VARIANTS_ID, "music")
    os.makedirs(folder)
    for name in paths:
        if name != "Battle Tones (loop).brstm":
            shutil.copyfile(paths[name], os.path.join(folder, name))
    shutil.copyfile(paths["Battle Tones (loop).brstm"], os.path.join(folder, "tones.brstm"))
    tracks = [("bcstm", "tones.bcstm", "auto"), ("bfstm-wiiu", "tones_wiiu.bfstm", "auto"),
              ("bfstm-switch-48k", "tones_switch_48k.bfstm", None), ("brstm-noloop", "tones_noloop.brstm", "auto"),
              ("brstm-manifest-loop", "tones.brstm", {"start": 132300}), ("bcstm-ima", "tones_ima.bcstm", "auto"),
              ("bwav", "tones.bwav", "auto")]
    manifest = {"format": 1, "id": VARIANTS_ID, "name": "BRSTM variants", "version": "1.0.0",
                "music": {"series": [{"key": "variants", "name": "STREAM VARIANTS", "order": 1, "tracks": [
                    dict({"key": k, "title": k, "file": f"music/{f}"}, **({"loop": loop} if loop else {}))
                    for k, f, loop in tracks]}]}}
    with open(os.path.join(mods, VARIANTS_ID, "mod.json"), "w", encoding="utf-8") as handle:
        json.dump(manifest, handle, indent=2)
    return f"{MOD_ID}:{key}"


def run_port(track_id):
    dump = os.path.join(OUT, "dump.wav")
    if os.path.exists(dump):
        os.remove(dump)
    import subprocess
    command = [sys.executable, os.path.join(ROOT, "tools", "port", "run_d012.py"), "--seconds", "90",
               "--build-dir", pc.BUILD, "--data-dir", os.path.relpath(DATA, ROOT), "--log", os.path.join(OUT, "run.log"),
               "--env", "PSPRECOMP_NO_INTERPRETER=1", "--env", "D012_UNTHROTTLED=1",
               "--env", f"D012_TEST_BGM_START={track_id}", "--env", f"D012_AUDIO_DUMP={dump}",
               "--env", "D012_TEST_MODS_DECODE=1", "--env", f"D012_INPUT_SCRIPT={pc.ROUTE}"]
    print("running the port (at most 90 s)...")
    subprocess.run(command, check=True, timeout=150)


def variants_report() -> bool:
    """The port's log for the variants: each loop where its header (or manifest) puts it."""
    expected = {
        "bcstm": (LOOP_START, LOOP_END - 1), "bfstm-wiiu": (LOOP_START, LOOP_END - 1),
        # 48 kHz: the loop sample 96,008 is frame round(96008 * 44100 / 48000) = 88,207 at 44.1 kHz.
        "bfstm-switch-48k": (round(round(LOOP_START * 48000 / RATE) * RATE / 48000), None),
        "brstm-noloop": (0, LOOP_END - 1), "brstm-manifest-loop": (132300, LOOP_END - 1),
    }
    found, errors = {}, []
    with open(os.path.join(OUT, "run.log"), encoding="utf-8", errors="replace") as handle:
        for line in handle:
            if VARIANTS_ID in line or MOD_ID in line:
                if "[mods]" in line or "[host-audio] decoded" in line:
                    print("  log:", line.rstrip()[:240])
            hit = re.search(r"decoded " + re.escape(VARIANTS_ID) + r":(\S+): .*?(\d+) frames at 44100 Hz.*loop (\d+)\.\.(\d+)",
                            line)
            if hit:
                found[hit.group(1)] = (int(hit.group(3)), int(hit.group(4)), int(hit.group(2)))
            if "[mods] error" in line and VARIANTS_ID in line:
                errors.append(line)
    ok = True
    for key, (start, end) in expected.items():
        got = found.get(key)
        good = got is not None and got[0] == start and (end is None or got[1] == end) and \
            (end is not None or got[1] == got[2] - 1)
        ok &= good
        print(f"  {key}: loop {got[0] if got else '?'}..{got[1] if got else '?'} (expected {start}..{end or 'the end'})"
              f" {'ok' if good else 'WRONG'}")
    for key, words in (("bcstm-ima", "IMA-ADPCM"), ("bwav", "not supported")):
        good = any(f'"{key}"' in e and words in e for e in errors)
        ok &= good
        print(f"  {key}: {'refused with a clear error' if good else 'NOT refused as expected'}")
    return ok


def intro_onset(marks, hop, size):
    """port_check.find_onset() for this piece, which the battle's sounds cover
    more: where its 440 Hz intro (at least a second of it in two) turns into
    660 Hz, to about 0.1 s, then to the sample by matching the dump against the
    tool's decode around it (within half a second)."""
    def tone(k, freq):
        return 0 <= k < len(marks) and marks[k][0] == freq and marks[k][1] > 0.3
    dump, rate = pc.read_left(os.path.join(OUT, "dump.wav"))
    tool, _ = pc.read_left(os.path.join(OUT, "tool_decode.wav"))
    for k in range(20, len(marks) - 10):
        if tone(k, 660) and sum(tone(k - j, 440) for j in range(1, 21)) >= 10 and                 sum(tone(k + j, 660) for j in range(10)) >= 5:
            last_a = max(j for j in range(k - 20, k) if tone(j, 440))
            guess = (last_a + k) * hop // 2 + size // 2 - LOOP_START

            def correlation(tau, window=8192, stride=8, at=LOOP_START - 4096):
                sab = saa = sbb = 0.0
                for i in range(0, window, stride):
                    a, b = dump[tau + at + i], tool[at + i]
                    sab += a * b; saa += a * a; sbb += b * b
                return sab / math.sqrt(saa * sbb) if saa and sbb else -1.0
            tau = max(range(guess - 22050, guess + 22051, 4), key=correlation)
            return tau + LOOP_START
    return None


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("--analyse", action="store_true", help="only analyse the last run's dump")
    options = parser.parse_args()
    # port_check.py's analysis, on this test's files and numbers.
    pc.DATA, pc.OUT, pc.MOD_ID, pc.LOOP_START, pc.LOOP_END = DATA, OUT, MOD_ID, LOOP_START, LOOP_END
    pc.PARTS = [(f1, f2, n / RATE) for f1, f2, n in PARTS]
    pc.find_onset = intro_onset
    os.makedirs(OUT, exist_ok=True)
    if not options.analyse:
        ff = audio.find_ffmpeg()
        pc.make_data_dir()
        track_id = make_mod(ff, make_files())
        run_port(track_id)
    print("\nthe variants:")
    variants = variants_report()
    print("\nthe BRSTM in the battle:")
    return 0 if pc.analyse() and variants else 1


if __name__ == "__main__":
    sys.exit(main())

"""Regression check for the BGM playlist's keys: in BGM focus every left/right
press changes the series, game or mod, both ways and across the wraps, and
up/down still roll within a series. See docs/port/bgm-playlist-design.md 11.8.

    python tools/port/check_bgm_series_keys.py [--build-dir DIR] [--base-data DIR]
        [--work DIR] [--mods DIR ...] [--verify] [--keys-only]

Makes a test data directory under --work (default build/port/playlist/series-keys):
the saves and settings of --base-data (default build/port/data, only read), a
generated tone-only mod shaped like a real one (two series of five tracks), and
copies of any --mods folders (e.g. build/port/data-readme-shots/mods/ff-battle-collection).
Then runs the recompiled port twice (tools/port/run_d012.py, at most 90 s each,
PSPRECOMP_NO_INTERPRETER=1):

  keys  from FINAL FANTASY XIII in BGM focus: 21 presses right and 21 left,
        every third one rocked onto up or down for two frames (a D-pad pressed
        off its axis), some deliberate up/down, then menu focus and three
        presses right there (the flat list, unchanged)
  play  starts on a tone track, right and left back to it, down, and confirms
        in BGM focus: the battle must open that track's MD........AT3

and checks the logs:

  - every left/right in BGM focus changes the series to the next or previous
    one in the screen's series list, exactly once, and nothing else changes
    until the next step (a rocked up/down never steps the track);
  - both wraps are crossed and both directions cross game and mod series;
  - a deliberate up/down rolls within the series, wrapping;
  - in menu focus right steps the one flat list as before;
  - no interpreter fallback; with --verify (D012_DECOMP_BGM=1
    D012_DECOMP_VERIFY=1) no DIFFERENT or INCONCLUSIVE comparison.

Exit status 0 when everything holds, 1 otherwise. The tones are generated sine
waves; nothing of the owner's music is used or copied.
"""

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

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
PYTHON = sys.executable
TONE_MOD = "series-keys-test"
TONE_SERIES = [("tones-a", "TONE SERIES A", 0, 330.0), ("tones-b", "TONE SERIES B", 10, 660.0)]
TRACKS_PER_SERIES = 5
START_GAME_ID = 83  # FFXIII - Blinded by Light, in every save's list
# Presses each way: more than the series with the test save, the tone mod and
# the README's mod (19), so that both wraps are crossed from FFXIII.
PRESSES = 21

# The route to the loading screen's selector (design doc, Reproduce).
PREFIX = ("460:pad a;1080:pad a;1500:pad a;1650:pad a;1900:pad dpdown;2000:pad a;2400:pad a;3200:pad a;"
          "3700:pad a;4400:pad a;4900:pad a;5600:pad dpdown;5750:pad dpright;5900:pad dpup;6050:pad a;7100:pad a;")


# ---- Test data -------------------------------------------------------------------

def write_tone(path, frequency, seconds=1.5, rate=44100):
    count = int(seconds * rate)
    fade = int(0.005 * rate)
    frames = bytearray()
    for i in range(count):
        gain = min(1.0, i / fade, (count - 1 - i) / fade)
        frames += struct.pack("<h", int(32767 * 0.25 * gain * math.sin(2 * math.pi * frequency * i / rate)))
    with wave.open(path, "wb") as out:
        out.setnchannels(1)
        out.setsampwidth(2)
        out.setframerate(rate)
        out.writeframes(bytes(frames))


def make_tone_mod(mods_dir):
    folder = os.path.join(mods_dir, TONE_MOD)
    os.makedirs(os.path.join(folder, "music"), exist_ok=True)
    series = []
    for key, name, order, base in TONE_SERIES:
        tracks = []
        for n in range(1, TRACKS_PER_SERIES + 1):
            file = f"music/{key}-{n}.wav"
            write_tone(os.path.join(folder, file), base + 55.0 * (n - 1))
            tracks.append({"key": f"{key[-1]}{n}", "title": f"Tone {key[-1].upper()}{n}", "file": file,
                           "loop": "whole"})
        series.append({"key": key, "name": name, "order": order, "tracks": tracks})
    manifest = {"format": 1, "id": TONE_MOD, "name": "Series keys test tones", "version": "1.0.0",
                "description": "Generated sine tones for tools/port/check_bgm_series_keys.py.",
                "music": {"series": series}}
    with open(os.path.join(folder, "mod.json"), "w", encoding="utf-8") as out:
        json.dump(manifest, out, indent=2)


def make_data(base, data, extra_mods):
    if os.path.isdir(data):
        shutil.rmtree(data)
    os.makedirs(data)
    shutil.copytree(os.path.join(base, "ms0"), os.path.join(data, "ms0"))
    for name in ("settings.ini", "pipeline_cache.bin", "pipeline_keys.bin"):
        if os.path.isfile(os.path.join(base, name)):
            shutil.copy2(os.path.join(base, name), os.path.join(data, name))
    mods = os.path.join(data, "mods")
    os.makedirs(mods)
    make_tone_mod(mods)
    for folder in extra_mods:
        shutil.copytree(folder, os.path.join(mods, os.path.basename(os.path.normpath(folder))))
    labels = set()
    for name in os.listdir(mods):
        manifest = os.path.join(mods, name, "mod.json")
        if os.path.isfile(manifest):
            with open(manifest, encoding="utf-8") as f:
                for s in json.load(f).get("music", {}).get("series", []):
                    labels.add(s["name"])
    return labels


# ---- Runs -------------------------------------------------------------------------

def run(name, work, data, build_dir, steps, env):
    log = os.path.join(work, name + ".log")
    command = [PYTHON, os.path.join(ROOT, "tools", "port", "run_d012.py"), "--seconds", "90",
               "--build-dir", build_dir, "--data-dir", data, "--log", log,
               "--env", "PSPRECOMP_NO_INTERPRETER=1", "--env", "D012_UNTHROTTLED=1",
               "--env", "D012_INPUT_SCRIPT=" + PREFIX + steps]
    for item in env:
        command += ["--env", item]
    print(subprocess.run(command, cwd=ROOT, capture_output=True, text=True).stdout.strip())
    with open(log, encoding="utf-8", errors="replace") as f:
        return f.read().splitlines()


def keys_route():
    """Presses after the selector is up; returns the script steps."""
    steps = ["7240:axis lefttrigger 1", "7250:axis lefttrigger 0"]
    frame = 7270
    for direction in ("dpright", "dpleft"):
        for i in range(PRESSES):
            steps.append(f"{frame}:pad {direction} 6")
            if i % 3 == 1:
                steps.append(f"{frame + 2}:pad {'dpdown' if i % 2 else 'dpup'} 2")  # rocked
            frame += 12
        # Deliberate up/down in the series reached.
        for vertical in ("dpdown", "dpdown", "dpup"):
            steps.append(f"{frame}:pad {vertical} 4")
            frame += 12
    # Menu focus: the one flat list.
    steps += [f"{frame}:axis righttrigger 1", f"{frame + 10}:axis righttrigger 0"]
    frame += 30
    for _ in range(3):
        steps.append(f"{frame}:pad dpright 4")
        frame += 14
    steps.append(f"{frame + 20}:quit")
    return ";".join(steps)


def play_route():
    steps = ["7240:axis lefttrigger 1", "7250:axis lefttrigger 0", "7280:pad dpright 6", "7300:pad dpleft 6",
             "7320:pad dpdown 4", "7360:pad a", "7900:quit"]
    return ";".join(steps)


# ---- The check ------------------------------------------------------------------

CHANGE = re.compile(r"^\[playlist\] (?P<label>.+) (?P<pos>\d+)/(?P<n>\d+): id (?P<id>-?\d+) (?P<file>\S*)")
SERIES = re.compile(r"^\[playlist\] \d+ tracks in \d+ series: (?P<list>.*)$")
START = re.compile(r"starts on (?P<label>.+) (?P<pos>\d+)/(?P<n>\d+): id")
SCRIPT = re.compile(r"^\[script\] frame (?P<frame>\d+): (?P<action>.*)$")


def series_list(text):
    return [(m.group(1), int(m.group(2))) for m in re.finditer(r"(.+?) \((\d+)\)(?: |$)", text)]


class Checker:
    def __init__(self, mod_labels):
        self.mod_labels = mod_labels
        self.failures = []
        self.counts = {}
        self.crossings = set()

    def fail(self, text):
        self.failures.append(text)

    def count(self, what):
        self.counts[what] = self.counts.get(what, 0) + 1

    def check_log(self, name, lines, verify):
        series, state, focus = [], None, "menu"
        steps = []  # (frame, kind, changes)
        last_side = -100
        for line in lines:
            if line.startswith("[interpreter]"):
                self.fail(f"{name}: interpreter fallback: {line}")
            if verify and ("DIFFERENT" in line or "INCONCLUSIVE" in line) and line.startswith("[decomp]"):
                self.fail(f"{name}: {line}")
            if m := SERIES.match(line):
                series = series_list(m.group("list"))
                continue
            if line.startswith("[playlist] ") and (m := START.search(line)):
                state = (m.group("label"), int(m.group("pos")))
                continue
            if line.startswith("[playlist] focus: "):
                focus = "bgm" if line.startswith("[playlist] focus: BGM") else "menu"
                continue
            if m := SCRIPT.match(line):
                frame, action = int(m.group("frame")), m.group("action").split()
                kind = "other"
                if action[:1] == ["pad"] and len(action) > 1:
                    button = action[1]
                    if button in ("dpright", "dpleft"):
                        kind = button[2:]
                        last_side = frame
                    elif button in ("dpdown", "dpup"):
                        kind = "rock" if frame - last_side < 8 else button[2:]
                steps.append({"frame": frame, "kind": kind, "focus": focus, "from": state, "changes": []})
                continue
            if (m := CHANGE.match(line)) and not line.startswith("[playlist] test switch"):
                state = (m.group("label"), int(m.group("pos")))
                if steps:
                    steps[-1]["changes"].append((state, m.group("file")))
        if not any(line.startswith("[script] frame") and line.rstrip().endswith(": quit") for line in lines):
            self.fail(f"{name}: the run ended before its route did (the 90 s limit?)")
        if not series:
            self.fail(f"{name}: no series list in the log")
            return None
        labels = [label for label, _ in series]
        sizes = dict(series)
        # A change the game made a frame late, after the rocked up/down's own
        # step, belongs to the left/right press before it.
        for before, step in zip(steps, steps[1:]):
            if step["kind"] == "rock" and before["kind"] in ("right", "left") and not before["changes"]                     and step["changes"]:
                before["changes"].append(step["changes"].pop(0))
                step["from"] = before["changes"][0][0]
        for step in steps:
            self.check_step(name, step, labels, sizes)
        return steps

    def check_step(self, name, step, labels, sizes):
        kind, changes, before = step["kind"], step["changes"], step["from"]
        where = f"{name}: frame {step['frame']} {kind} ({step['focus']} focus) from {before}"
        if kind == "other" or before is None:
            return
        after = [state for state, _ in changes]
        index = labels.index(before[0]) if before[0] in labels else None
        if index is None:
            self.fail(f"{where}: series {before[0]!r} is not in the list")
            return
        if step["focus"] == "bgm":
            if kind in ("right", "left"):
                self.count(f"bgm {kind}")
                delta = 1 if kind == "right" else -1
                want = labels[(index + delta) % len(labels)]
                if len(after) != 1 or after[0][0] != want:
                    self.fail(f"{where}: wanted one change to {want}, got {after}")
                    return
                if (index == len(labels) - 1 and kind == "right") or (index == 0 and kind == "left"):
                    self.crossings.add(f"wrap {kind}")
                if before[0] in self.mod_labels or want in self.mod_labels:
                    self.crossings.add(f"mod {kind}")
                if before[0] not in self.mod_labels and want not in self.mod_labels:
                    self.crossings.add(f"game {kind}")
            elif kind == "rock":
                self.count("bgm rocked up/down")
                if after:
                    self.fail(f"{where}: an up/down rocked from a left/right press changed {after}")
            elif kind in ("down", "up"):
                self.count(f"bgm {kind}")
                n = sizes[before[0]]
                if n == 1:
                    if after:
                        self.fail(f"{where}: a one-track series changed {after}")
                    return
                delta = 1 if kind == "down" else -1
                want = (before[0], (before[1] - 1 + delta) % n + 1)
                if after != [want]:
                    self.fail(f"{where}: wanted {want}, got {after}")
        else:
            if kind == "right":
                self.count("menu right")
                n = sizes[before[0]]
                want = (before[0], before[1] + 1) if before[1] < n else (labels[(index + 1) % len(labels)], 1)
                if after != [want]:
                    self.fail(f"{where}: wanted {want} (the flat list), got {after}")

    def check_play(self, name, lines, steps):
        chosen = None
        for step in steps or []:
            for state, file in step["changes"]:
                chosen = (state, file)
        if chosen is None or not chosen[1].upper().startswith("MD"):
            self.fail(f"{name}: no mod track chosen ({chosen})")
            return
        opened = [line for line in lines if line.startswith("[io] open ") and
                  line.upper().split()[2].endswith("/BGM/" + chosen[1].upper())]
        if not opened:
            self.fail(f"{name}: the battle did not open {chosen[1]} (chosen {chosen[0]})")
        else:
            self.counts["mod track played"] = f"{chosen[0][0]} {chosen[0][1]}: {opened[0]}"


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("--build-dir", default=os.path.join("tools", "local", "port", "build-d012-recomp"))
    parser.add_argument("--base-data", default=os.path.join("build", "port", "data"))
    parser.add_argument("--work", default=os.path.join("build", "port", "playlist", "series-keys"))
    parser.add_argument("--mods", action="append", default=[], help="another mod folder to add (copied)")
    parser.add_argument("--verify", action="store_true", help="D012_DECOMP_BGM=1 D012_DECOMP_VERIFY=1 too")
    parser.add_argument("--keys-only", action="store_true", help="skip the play run")
    options = parser.parse_args()

    work = os.path.join(ROOT, options.work)
    data = os.path.join(work, "data")
    os.makedirs(work, exist_ok=True)
    mod_labels = make_data(os.path.join(ROOT, options.base_data), data,
                           [os.path.join(ROOT, m) for m in options.mods])
    env = ["D012_TRACE_IO=1"]
    if options.verify:
        env += ["D012_DECOMP_BGM=1", "D012_DECOMP_VERIFY=1", "D012_DECOMP_VERIFY_LIMIT=1000"]

    checker = Checker(mod_labels)
    lines = run("keys", work, data, options.build_dir, keys_route(), env + [f"D012_TEST_BGM_START={START_GAME_ID}"])
    checker.check_log("keys", lines, options.verify)
    for needed in ("wrap right", "wrap left", "mod right", "mod left", "game right", "game left"):
        if needed not in checker.crossings:
            checker.fail(f"keys: the route never covered {needed}")
    if not options.keys_only:
        start = f"{TONE_MOD}:{TONE_SERIES[1][0][-1]}3"
        lines = run("play", work, data, options.build_dir, play_route(), env + [f"D012_TEST_BGM_START={start}"])
        steps = checker.check_log("play", lines, options.verify)
        checker.check_play("play", lines, steps)

    for what, value in sorted(checker.counts.items()):
        print(f"  {what}: {value}")
    print(f"  covered: {', '.join(sorted(checker.crossings))}")
    if checker.failures:
        print(f"FAIL: {len(checker.failures)} problem(s)")
        for text in checker.failures:
            print("  " + text)
        return 1
    print("PASS: every left/right in BGM focus changed the series; up/down rolled within it")
    return 0


if __name__ == "__main__":
    sys.exit(main())

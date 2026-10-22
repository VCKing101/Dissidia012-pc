"""Performance tour: scripted runs of the recompiled port at real time with frame interpolation, one CSV row
per present and per game frame (PortableKit's <prefix>_PERF_CSV), and per-scene frame-time statistics.
docs/port/performance.md describes the method and the results.

    python tools/port/perf_tour.py run [--routes NAME,...] [--label LABEL] [--cold] [--present vsync|immediate]
                                       [--build-dir DIR] [--env NAME=VALUE ...]
    python tools/port/perf_tour.py stats LABEL [LABEL ...]       # tables from runs already made
    python tools/port/perf_tour.py table LABEL [LABEL ...]       # the same as one Markdown table
    python tools/port/perf_tour.py --list

Every run goes through tools/port/run_d012.py (at most 90 s, stopped by its own handle) with
PSPRECOMP_NO_INTERPRETER=1 on the build in --build-dir (default tools/local/port/build-d012-perf), data in
build/port/data-perf, a copy of build/port/data made once (build/port/data itself is never run against or
written). Before each run the copy's ms0/ is restored from build/port/perf/pristine-ms0, and its
settings.ini from build/port/perf/pristine-settings.ini with the present mode set; --cold also deletes the
copy's pipeline_cache.bin and pipeline_keys.bin first (the driver's own shader cache is not touched).
The routes reach their scene unthrottled (scene "fast", left out of the statistics) and then run at real
time (D012_PERF=log, D012_TRACE_INTERPOLATION=1). --data-dir runs against another copy (made the same way, once),
--affinity limits the game to some logical CPUs (run_d012.py --affinity), and --tag names a variant of the same
label's routes (runs/<label>/<route>-<tag>).

Output under build/port/perf/ (never committed): runs/<label>/<route>/ with run.log and frames.csv, and
runs/<label>/stats.json.

Statistics per scene, from the present rows: presents per second (mean), the 1% low (1000 over the 99th
percentile of the time between presents), the mean and 99th percentile of that time and the presents more than
20 ms after the one before, seconds whose present count, as the fps meter counts them, is below
55, presents more than 25 ms after the one before (hitches), the share of presents interpolation blended, and
why the others were not; the emulation thread's CPU time per second (the flip rows' cpu_ms) and, from the log,
the present thread's ([present-thread] lines) and frame interpolation's delay ([interp] ... delay); from the frame rows: pipelines created and their time, textures decoded, key hashing,
snapshots, GPU time.
"""

import argparse
import csv
import json
import os
import re
import shutil
import subprocess
import sys
import threading
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gfx_tour  # noqa: E402  (the routes' menu paths)

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
PYTHON = sys.executable
OUT = os.path.join(ROOT, "build", "port", "perf")
DATA = os.path.join(ROOT, "build", "port", "data-perf")
OWNER_DATA = os.path.join(ROOT, "build", "port", "data")
PRISTINE = os.path.join(OUT, "pristine-ms0")
PRISTINE_SETTINGS = os.path.join(OUT, "pristine-settings.ini")
DEFAULT_BUILD = os.path.join("tools", "local", "port", "build-d012-perf")


def battle_route(name, player_moves, about):
    """Main menu to a Quick Battle, unthrottled up to the versus screen; then at real time: the versus screen,
    the loading screen, the intro, brave and HP attacks, EX Mode and an EX Burst (D012_TEST_EX_FULL)."""
    steps, b = gfx_tour.quick_battle(1, player_moves=player_moves)
    steps = [s for s in steps if not s[1].startswith("shot") and not s[1].startswith("scene")]
    enter = b - 1960 - (25 * (len(player_moves) + 1) if player_moves else 0)
    real = b - 1010  # the versus screen
    steps += [(0, "scene fast"), (real, "speed normal"), (real, "scene versus"), (b - 920, "scene loading"),
              (b - 8, "scene battle_intro"), (b + 260, "scene battle"), (b + 280, "pad b"), (b + 310, "pad b"),
              (b + 340, "pad rightshoulder+x"), (b + 380, "scene ex_mode"), (b + 420, "pad b"), (b + 450, "pad x"),
              (b + 500, "pad x"), (b + 520, "scene ex_burst"), (b + 560, "pad x"), (b + 640, "pad x"),
              (b + 700, "pad x"), (b + 760, "pad b"), (b + 820, "pad x"), (b + 860, "quit")]
    del enter
    env = ["D012_TEST_EX_FULL=1"] + (["D012_TEST_UNLOCK_FIGHTERS=1"] if player_moves else [])
    return {"name": name, "steps": steps, "env": env, "about": about}


def menus_route():
    steps = [(0, "scene fast"), (460, "pad a"), (1080, "pad a"), (1400, "speed normal"), (1400, "scene load_save"),
             (1500, "pad a"), (1650, "pad a"), (1800, "scene main_menu"), (1900, "pad dpdown"), (2000, "pad a"),
             (2000, "scene battle_mode"), (2400, "pad a"), (2400, "scene character_select"), (3200, "pad a"),
             (3700, "pad a"), (3900, "quit")]
    return {"name": "menus", "steps": steps, "about": "Loading the save, the main menu, Battle Mode, character select"}


def worldmap_route():
    steps = [(0, "scene fast"), (460, "pad a"), (1080, "pad a"), (1500, "pad a"), (1650, "pad a"), (2000, "pad a"),
             (2600, "pad a"), (3200, "pad a"), (3700, "speed normal"), (3700, "scene continue_load"),
             (3800, "pad a"), (3880, "scene worldmap"), (4200, "axis lefty -1"), (4500, "axis lefty 0"),
             (4500, "axis leftx 1"), (4800, "axis leftx 0"), (4800, "axis lefty 1"), (5300, "axis lefty 0"),
             (5300, "pad a"), (5400, "pad a"), (5500, "pad a"), (5700, "pad a"), (5790, "scene gate_transition"),
             (5930, "scene gateway_board"), (6100, "pad a"), (6200, "quit")]
    return {"name": "worldmap", "steps": steps,
            "about": "Story Mode Continue (its load), the world map walk, a gateway, its transition, the board"}


def orb_route():
    """The world map's "power of the dead" orb (gfx_triage.py's orb scene): the camera turned left, a walk up and
    right to the glowing orb, cross to absorb it, then standing while its effect plays out."""
    steps = [(0, "scene fast"), (460, "pad a"), (1080, "pad a"), (1500, "pad a"), (1650, "pad a"), (2000, "pad a"),
             (2600, "pad a"), (3200, "pad a"), (3800, "pad a"), (3950, "axis rightx -1"), (3965, "axis rightx 0"),
             (3990, "speed normal"), (3990, "scene orb_walk"), (4000, "axis lefty -0.8"), (4000, "axis leftx 0.45"),
             (4380, "scene orb_absorb"), (4400, "pad a"), (4410, "axis lefty 0"), (4410, "axis leftx 0"),
             (4420, "pad a"), (4700, "scene orb_after"), (5700, "quit")]
    return {"name": "orb", "steps": steps,
            "about": "World map: walking to a power-of-the-dead orb, absorbing it, standing after"}


ROUTES = [
    orb_route(),
    menus_route(),
    worldmap_route(),
    battle_route("qb_ex_burst", (), "Quick Battle, Lightning, Old Chaos Shrine: intro, attacks, EX Mode, EX Burst"),
    battle_route("qb_kuja", ("dpdown", "dpdown") + ("dpright",) * 6,
                 "Quick Battle as Kuja (unlocked by a test switch): his effects, EX Mode, EX Burst"),
]


def prepare_data(cold, present):
    if not os.path.isdir(DATA):
        print(f"copying {OWNER_DATA} to {DATA} (once)")
        shutil.copytree(OWNER_DATA, DATA)
    if os.path.abspath(DATA) == os.path.abspath(OWNER_DATA):
        sys.exit("refusing to run against build/port/data itself")
    os.makedirs(OUT, exist_ok=True)
    if not os.path.isdir(PRISTINE):
        shutil.copytree(os.path.join(OWNER_DATA, "ms0"), PRISTINE)
    if not os.path.exists(PRISTINE_SETTINGS):
        shutil.copyfile(os.path.join(OWNER_DATA, "settings.ini"), PRISTINE_SETTINGS)
    ms0 = os.path.join(DATA, "ms0")
    if os.path.isdir(ms0):
        shutil.rmtree(ms0)
    shutil.copytree(PRISTINE, ms0)
    lines = open(PRISTINE_SETTINGS, encoding="utf-8").read().splitlines()
    lines = [f"video.present_mode={present}" if line.startswith("video.present_mode=") else line for line in lines]
    open(os.path.join(DATA, "settings.ini"), "w", encoding="utf-8").write("\n".join(lines) + "\n")
    if cold:
        for name in ("pipeline_cache.bin", "pipeline_keys.bin"):
            path = os.path.join(DATA, name)
            if os.path.exists(path):
                os.remove(path)


def script_text(steps):
    return ";".join(f"{frame}:{action}" for frame, action in sorted(steps, key=lambda step: step[0]))


def wait_for_quiet(limit_seconds=600):
    """Waits while another copy of the game runs (another build's D012Native.exe on this machine), up to
    `limit_seconds`: two games at once share the CPU and the GPU and spoil the numbers. Never stops one."""
    started = time.monotonic()
    while time.monotonic() - started < limit_seconds:
        listing = subprocess.run(["tasklist", "/FI", "IMAGENAME eq D012Native.exe", "/FO", "CSV", "/NH"],
                                 capture_output=True, text=True).stdout
        if "D012Native.exe" not in listing:
            return time.monotonic() - started
        time.sleep(5)
    return time.monotonic() - started


def run_route(route, options):
    waited = wait_for_quiet()
    if waited > 1:
        print(f"  waited {waited:.0f} s for another game process to end", flush=True)
    folder = route["name"] + (f"-{options.tag}" if options.tag else "")
    out = os.path.join(OUT, "runs", options.label, folder)
    if os.path.isdir(out):
        shutil.rmtree(out)
    os.makedirs(out)
    prepare_data(options.cold, options.present)
    # The log and the CSV are written on a local SSD (--log-root) and copied afterwards: written to the
    # repository's disk, a busy hard drive held the game's own log writes for up to two seconds.
    scratch = os.path.join(options.log_root, options.label, folder)
    os.makedirs(scratch, exist_ok=True)
    frames = os.path.join(scratch, "frames.csv")
    env = ["PSPRECOMP_NO_INTERPRETER=1", "D012_UNTHROTTLED=1", "D012_PERF=log", "D012_TRACE_INTERPOLATION=1",
           "D012_TRACE_AUDIO=1", "D012_TRACE_STALLS=1",
           f"D012_PERF_CSV={frames}", f"D012_INPUT_SCRIPT={script_text(route['steps'])}"] + route.get("env", []) \
        + options.env
    log = os.path.join(scratch, "run.log")
    command = [PYTHON, os.path.join(ROOT, "tools", "port", "run_d012.py"), "--seconds", "90", "--build-dir",
               options.build_dir, "--data-dir", DATA, "--log", log]
    if options.affinity:
        command += ["--affinity", options.affinity]
    for item in env:
        command += ["--env", item]
    started = time.monotonic()
    # Another game process during the run (two D012Native.exe at once, ours included) spoils it.
    overlap = {"seen": False, "done": False}

    def watch():
        while not overlap["done"]:
            listing = subprocess.run(["tasklist", "/FI", "IMAGENAME eq D012Native.exe", "/FO", "CSV", "/NH"],
                                     capture_output=True, text=True).stdout
            if listing.count("D012Native.exe") >= 2:
                overlap["seen"] = True
            time.sleep(3)

    watcher = threading.Thread(target=watch, daemon=True)
    watcher.start()
    result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    overlap["done"] = True
    watcher.join()
    listing = subprocess.run(["tasklist", "/FI", "IMAGENAME eq D012Native.exe", "/FO", "CSV", "/NH"],
                             capture_output=True, text=True).stdout
    if "D012Native.exe" in listing or overlap["seen"]:
        print("  another game process was running at the end of this run: its numbers may be contended", flush=True)
        open(os.path.join(out, "contended"), "w").close()
    for name in ("run.log", "frames.csv"):
        if os.path.exists(os.path.join(scratch, name)):
            shutil.copyfile(os.path.join(scratch, name), os.path.join(out, name))
    print(f"  {options.label}/{folder}: {result.stdout.strip()} ({time.monotonic() - started:.0f} s)",
          flush=True)


# ---------------------------------------------------------------------------------------------------------

def percentile(values, share):
    if not values:
        return 0.0
    ordered = sorted(values)
    index = min(len(ordered) - 1, max(0, int(round(share * (len(ordered) - 1)))))
    return ordered[index]


def number(text):
    try:
        return float(text)
    except (TypeError, ValueError):
        return None


def scene_stats(presents, flips):
    stats = {}
    if len(presents) >= 2:
        start, end = presents[0]["ms"], presents[-1]["ms"]
        seconds = max(1e-3, (end - start) / 1000.0)
        intervals = [p["interval_ms"] for p in presents[1:]]
        kinds = {}
        for p in presents:
            kinds[p["kind"]] = kinds.get(p["kind"], 0) + 1
        bins = {}
        for p in presents:
            bins[int((p["ms"] - start) // 1000)] = bins.get(int((p["ms"] - start) // 1000), 0) + 1
        whole = [count for second, count in bins.items() if (second + 1) * 1000 <= end - start]
        stats.update({
            "seconds": round(seconds, 1),
            "presents": len(presents),
            "fps_mean": round(len(presents) / seconds, 1),
            "low1_fps": round(1000.0 / max(0.1, percentile(intervals, 0.99)), 1),
            "interval_p50_ms": round(percentile(intervals, 0.5), 2),
            "interval_mean_ms": round(sum(intervals) / len(intervals), 2),
            "interval_p99_ms": round(percentile(intervals, 0.99), 2),
            "over_20ms": sum(1 for value in intervals if value > 20.0),
            "late_p99_ms": round(percentile([p["late_ms"] for p in presents], 0.99), 2),
            "interval_max_ms": round(max(intervals), 1),
            "seconds_below_55": sum(1 for count in whole if count < 55),
            "seconds_counted": len(whole),
            "meter_min": min(whole) if whole else None,
            "hitches_over_25ms": sum(1 for value in intervals if value > 25.0),
            "blended_share": round(kinds.get("blended", 0) / len(presents), 3),
            "kinds": kinds,
            "skipped": int(sum(p["skipped"] for p in presents)),
            "late_max_ms": round(max(p["late_ms"] for p in presents), 1),
        })
    if flips:
        def column(name):
            return [f[name] for f in flips if f.get(name) is not None]
        gpu = column("gpu_ms")
        cuts = {}
        for f in flips:
            if f["cut"] and f["cut"] != "none":
                cuts[f["cut"]] = cuts.get(f["cut"], 0) + 1
        stats.update({
            "flips": len(flips),
            "frame_p99_ms": round(percentile(column("interval_ms"), 0.99), 1),
            "guest_mean_ms": round(sum(column("guest_ms")) / len(flips), 2),
            "guest_p99_ms": round(percentile(column("guest_ms"), 0.99), 2),
            "render_mean_ms": round(sum(column("render_ms")) / len(flips), 2),
            "render_p99_ms": round(percentile(column("render_ms"), 0.99), 2),
            "gpu_mean_ms": round(sum(gpu) / len(gpu), 2) if gpu else None,
            "gpu_p99_ms": round(percentile(gpu, 0.99), 2) if gpu else None,
            "pipelines": int(sum(column("pipelines"))),
            "pipeline_ms": round(sum(column("pipeline_ms")), 1),
            "pipeline_max_ms": round(max(column("pipeline_max_ms") or [0.0]), 1),
            "prewarmed": int(sum(column("prewarmed"))),
            "textures": int(sum(column("textures"))),
            "texture_ms": round(sum(column("texture_ms")), 1),
            "texture_max_frame_ms": round(max(column("texture_ms") or [0.0]), 2),
            "key_ms_per_frame": round(sum(column("key_ms")) / len(flips), 3),
            "snapshots_per_frame": round(sum(column("snapshots")) / len(flips), 2),
            "snapshot_kib_per_frame": round(sum(column("snapshot_kib")) / len(flips), 0),
            "blocked": int(sum(column("blocked"))),
            "emu_cpu_ms_per_s": round(sum(column("cpu_ms")) / max(1e-3, sum(column("interval_ms")) / 1000.0), 1),
            "over_budget": int(sum(column("over_budget"))),
            "cuts": cuts,
        })
    return stats


def read_csv(path):
    presents, flips = [], []
    with open(path, newline="", encoding="utf-8", errors="replace") as handle:
        for row in csv.DictReader(handle):
            if row.get("scene") is None:
                continue  # a last line cut off at the time limit
            if row["type"] == "present":
                presents.append({"ms": number(row["ms"]), "interval_ms": number(row["interval_ms"]),
                                 "kind": row["kind"], "t": number(row["t"]), "skipped": number(row["skipped"]) or 0,
                                 "late_ms": number(row["late_ms"]) or 0.0, "rate": number(row["rate"]),
                                 "scene": row["scene"]})
            elif row["type"] == "flip":
                item = {"scene": row["scene"], "cut": row["cut"]}
                for name in ("ms", "interval_ms", "guest_ms", "render_ms", "wait_ms", "pacing_ms", "gpu_ms",
                             "pipelines", "pipeline_ms", "pipeline_max_ms", "prewarmed", "textures", "texture_ms",
                             "key_ms", "snapshots", "snapshot_kib", "blocked", "over_budget", "cpu_ms"):
                    item[name] = number(row[name])
                flips.append(item)
    return [p for p in presents if None not in (p["ms"], p["interval_ms"])], flips


def analyse(label):
    base = os.path.join(OUT, "runs", label)
    result = {}
    for route in sorted(os.listdir(base)):
        path = os.path.join(base, route, "frames.csv")
        if not os.path.exists(path):
            continue
        presents, flips = read_csv(path)
        scenes = []
        for p in presents:
            if p["scene"] != "fast" and p["scene"] not in scenes:
                scenes.append(p["scene"])
        per_scene = {}
        for scene in scenes:
            per_scene[scene] = scene_stats([p for p in presents if p["scene"] == scene],
                                           [f for f in flips if f["scene"] == scene])
        real = [p for p in presents if p["scene"] != "fast"]
        per_scene["ALL"] = scene_stats(real, [f for f in flips if f["scene"] != "fast"])
        log = os.path.join(base, route, "run.log")
        text = open(log, encoding="utf-8", errors="replace").read() if os.path.exists(log) else ""
        per_scene["rate_changes"] = re.findall(r"\[interp\] frame rate (\d+ -> \d+: [^(]+)", text)
        per_scene["pipeline_lines"] = re.findall(r"\[render\] (\d+ (?:new pipelines?|pipelines of earlier)[^\n]*)",
                                                 text)
        # [audio] lines (D012_TRACE_AUDIO), one a second: frames the device took that the game had not
        # written (silent: an underrun while music plays), attributed to the scene the log last named.
        scene, audio = "start", {}
        for line in text.splitlines():
            named = re.match(r"\[script\] frame \d+: scene (\S+)", line)
            if named:
                scene = named.group(1)
            heard = re.match(r"\[audio\] \d+s frames=(\d+) peak=(\d+) rms=[\d.]+ silent=(\d+) dropped=(\d+)", line)
            if heard and scene != "fast":
                item = audio.setdefault(scene, {"seconds": 0, "silent_frames": 0, "seconds_with_gaps": 0,
                                                "dropped_frames": 0})
                item["seconds"] += 1
                item["silent_frames"] += int(heard.group(3))
                item["dropped_frames"] += int(heard.group(4))
                item["seconds_with_gaps"] += 1 if 0 < int(heard.group(3)) < int(heard.group(1)) else 0
        for name, item in audio.items():
            if name in per_scene and isinstance(per_scene[name], dict):
                per_scene[name]["audio"] = item
        per_scene["slow_frames"] = len(re.findall(r"\[slow-frame\]", text))
        # The present thread's lines and interpolation's delay, over the real-time part (after "speed normal").
        real_text = text.split("[script] game speed normal", 1)[-1]
        threads = re.findall(r"\[present-thread\] ([\d.]+) s: (\d+) presents, cpu ([\d.]+) ms \([\d.]+% of a core\); "
                             r"waited for the renderer ([\d.]+) ms \(longest ([\d.]+)\), for an image ([\d.]+) ms "
                             r"\(longest ([\d.]+)\), (\d+) not made; latest ([\d.-]+) ms late; the emulation thread "
                             r"waited ([\d.]+) ms", real_text)
        if threads:
            seconds = sum(float(t[0]) for t in threads)
            per_scene["present_thread"] = {
                "seconds": round(seconds, 1),
                "cpu_ms_per_s": round(sum(float(t[2]) for t in threads) / seconds, 1),
                "lock_wait_ms_per_s": round(sum(float(t[3]) for t in threads) / seconds, 2),
                "lock_wait_max_ms": round(max(float(t[4]) for t in threads), 2),
                "acquire_ms_per_s": round(sum(float(t[5]) for t in threads) / seconds, 2),
                "acquire_max_ms": round(max(float(t[6]) for t in threads), 2),
                "not_made": sum(int(t[7]) for t in threads),
                "late_max_ms": round(max(float(t[8]) for t in threads), 2),
                "emu_wait_ms_per_s": round(sum(float(t[9]) for t in threads) / seconds, 2),
            }
        delays = [float(d) for d in re.findall(r"; delay ([\d.]+) ms \(code", real_text)]
        if delays:
            per_scene["delay_ms"] = {"median": round(percentile(delays, 0.5), 1), "min": min(delays),
                                     "max": max(delays)}
        per_scene["display_stalls"] = re.findall(r"\[present\] the display has taken no image[^\n]*", text)
        per_scene["display"] = (re.findall(r"\| (FIFO|MAILBOX|IMMEDIATE)[^|]*\|", text) or [""])[0]
        result[route] = per_scene
    with open(os.path.join(base, "stats.json"), "w", encoding="utf-8") as handle:
        json.dump(result, handle, indent=1)
    return result


def print_table(labels):
    keys = ["fps_mean", "low1_fps", "seconds_below_55", "meter_min", "hitches_over_25ms", "blended_share",
            "pipelines", "pipeline_ms", "textures", "texture_ms", "gpu_mean_ms", "render_p99_ms", "guest_p99_ms",
            "frame_p99_ms", "snapshots_per_frame", "audio"]
    print("label | route | scene | " + " | ".join(keys))
    for label in labels:
        stats = analyse(label)
        for route, scenes in stats.items():
            for scene, values in scenes.items():
                if not isinstance(values, dict) or "fps_mean" not in values:
                    continue
                print(f"{label} | {route} | {scene} | " + " | ".join(str(values.get(k)) for k in keys))
            if scenes.get("rate_changes"):
                print(f"{label} | {route} | rate changes: {scenes['rate_changes']}")


def markdown(labels):
    """One Markdown table: per route and scene, each label's mean fps, 1% low, seconds under 55, presents more
    than 25 ms apart, the 99th percentile of the time between presents and the share of blended presents."""
    stats = {label: analyse(label) for label in labels}
    print("| Route | Scene | " + " | ".join(f"{label}: mean / 1% low / s<55 / >25 ms / p99 ms / blended"
                                          for label in labels) + " |")
    print("| --- | --- | " + " | ".join("---" for _ in labels) + " |")
    routes = []
    for label in labels:
        for route in stats[label]:
            if route not in routes:
                routes.append(route)
    for route in routes:
        scenes = []
        for label in labels:
            for scene, values in stats[label].get(route, {}).items():
                if isinstance(values, dict) and "fps_mean" in values and scene not in scenes:
                    scenes.append(scene)
        for scene in scenes:
            cells = []
            for label in labels:
                v = stats[label].get(route, {}).get(scene)
                if not isinstance(v, dict) or "fps_mean" not in v:
                    cells.append("")
                    continue
                cells.append(f"{v['fps_mean']:.1f} / {v['low1_fps']:.1f} / {v['seconds_below_55']} / "
                             f"{v['hitches_over_25ms']} / {1000.0 / v['low1_fps']:.1f} / {v['blended_share']:.2f}")
            print(f"| {route} | {scene} | " + " | ".join(cells) + " |")


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("command", nargs="?", default="run", choices=["run", "stats", "table"])
    parser.add_argument("labels", nargs="*")
    parser.add_argument("--routes", default="all")
    parser.add_argument("--label", default="probe")
    parser.add_argument("--cold", action="store_true")
    parser.add_argument("--present", default="vsync", choices=["vsync", "immediate", "mailbox"])
    parser.add_argument("--build-dir", default=DEFAULT_BUILD)
    parser.add_argument("--list", action="store_true")
    parser.add_argument("--log-root", default=os.path.join(os.environ.get("LOCALAPPDATA", OUT), "Temp", "d012-perf"),
                        help="where the run writes its log and CSV (a local SSD), copied to build/port/perf after")
    parser.add_argument("--env", action="append", default=[], metavar="NAME=VALUE")
    parser.add_argument("--data-dir", default=None, help="the data copy to run against (default build/port/data-perf)")
    parser.add_argument("--affinity", default=None, help="CPU mask for the game (run_d012.py --affinity)")
    parser.add_argument("--tag", default="", help="a suffix for the routes' run folders")
    options = parser.parse_args()
    if options.data_dir:
        global DATA
        DATA = os.path.join(ROOT, options.data_dir)
    if options.list:
        for route in ROUTES:
            print(f"{route['name']}: {route['about']}")
        return 0
    if options.command == "stats":
        print_table(options.labels or [options.label])
        return 0
    if options.command == "table":
        markdown(options.labels or [options.label])
        return 0
    names = [route["name"] for route in ROUTES] if options.routes == "all" else options.routes.split(",")
    for name in names:
        route = next((r for r in ROUTES if r["name"] == name), None)
        if route is None:
            sys.exit(f"no route {name}; --list shows them")
        # A run another game process overlapped is made again, twice at most.
        for _ in range(3):
            run_route(route, options)
            folder = route["name"] + (f"-{options.tag}" if options.tag else "")
            if not os.path.exists(os.path.join(OUT, "runs", options.label, folder, "contended")):
                break
    print_table([options.label])
    return 0


if __name__ == "__main__":
    sys.exit(main())

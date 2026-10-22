"""Graphics tour: scripted runs of the recompiled port through the game's effect-heavy scenes, with captures
at fixed frames and the renderer's GE census (D012_GE_CENSUS) for each scene, compared with a local baseline.
docs/port/graphics-census.md describes the scenes and what the census found.

    python tools/port/gfx_tour.py [--routes NAME,...] [--variants base,nointerp,1x] [--save-baseline]
                                  [--build-dir DIR] [--seconds S] [--list] [--script "f:action;..."]
                                  [--env NAME=VALUE ...]

Every run goes through tools/port/run_d012.py (at most 90 s, stopped by its own handle) with
PSPRECOMP_NO_INTERPRETER=1 on the build in --build-dir (default tools/local/port/build-d012-tour), data in
build/port/data-gfx-tour, a copy of build/port/data made once (build/port/data itself is never run against
or written). The memory stick ms0/ of that copy is restored before each run from
build/port/gfx-tour/pristine-ms0, so autosaves and test switches of one run never reach the next. Routes
may set the profile's test switches (D012_TEST_EX_FULL, D012_TEST_UNLOCK_FIGHTERS,
port/host/d012_test_switches.cpp).

Variants (each route is run once per variant):
  base      the data copy's own settings (the owner's: internal scale 4, 60 fps with frame interpolation),
            with D012_CHECK_REPLAY: every 150 flips the older frame is drawn again from its recording and
            compared with the frame as drawn; a run where the colour of more than 1% of the pixels differs
            is a problem (the check's own count includes the alpha bits, which hold the stencil)
  nointerp  D012_FRAME_RATE=30: no frame interpolation
  1x        D012_FRAME_RATE=30 and D012_INTERNAL_SCALE=1

Output under build/port/gfx-tour/ (never committed):
  runs/<route>/<variant>/        captures (BMP, png/, sheet.png), census.json, run.log
  baseline/<route>/<variant>/    the stored captures, one per timeline seen (NAME.png, NAME.alt1.png, ...)
  report.json                    per capture: the difference to the nearest baseline picture, and for nointerp
                                 to the nearest 1x picture
  census_summary.json            the census of every base run merged: per feature the count, the scenes, an example
  replay_checks.json             the replay check's share of differing pixels, per base run

A difference is the mean absolute difference (0-255) and the share of pixels differing by more than 40,
both after scaling the two captures to 480x272; a flag (mean over 8 or more than 6% over 40) means "look at
this pair". The guest clock follows the wall clock, so menus repeat exactly but a battle or a walk falls
into one of a few timelines (another stage variant, a moogle met on the way). The baseline keeps one picture
per timeline: --save-baseline adds a capture that matches none of the stored ones, so after a few runs a
flag is a real change. nointerp against 1x differs by the internal scale alone when the timelines agree.

--script runs one ad hoc route (for finding new routes, with --name); --list prints the routes.
"""

import argparse
import json
import re
import os
import shutil
import subprocess
import sys
import time

from PIL import Image, ImageChops, ImageDraw, ImageStat

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
PYTHON = sys.executable
TOUR = os.path.join(ROOT, "build", "port", "gfx-tour")
DATA = os.path.join(ROOT, "build", "port", "data-gfx-tour")
OWNER_DATA = os.path.join(ROOT, "build", "port", "data")
PRISTINE = os.path.join(TOUR, "pristine-ms0")
# Where runs, the report, the census summary and the replay checks go (--out); the baseline stays in TOUR.
RUNS = TOUR
DEFAULT_BUILD = os.path.join("tools", "local", "port", "build-d012-tour")

VARIANTS = {
    "base": ["D012_CHECK_REPLAY=1"],
    "nointerp": ["D012_FRAME_RATE=30", "D012_FRAME_RATE_AUTO=0"],
    "1x": ["D012_FRAME_RATE=30", "D012_FRAME_RATE_AUTO=0", "D012_INTERNAL_SCALE=1"],
}

# ---------------------------------------------------------------------------------------------------------
# Routes. Frames are the input script's: one per game frame (and per interface frame while a port screen is
# up). From the owner's save: the notice, LOAD GAME, the slot and Yes, then the main menu at about 1800.

BOOT = [(400, "shot title"), (460, "pad a"), (1080, "pad a"), (1500, "pad a"), (1650, "pad a")]


def quick_battle(stage_rights, rules_rights=0, player_moves=()):
    """Main menu to a Quick Battle (1-on-1): Lightning against a CPU Lightning at minimal strength.
    `stage_rights` is the number of right presses on the STAGE row from Random (1/15): 1 Old Chaos Shrine,
    2 Pandaemonium, 3 World of Darkness, 4 Lunar Subterrane, 5 Interdimensional Rift, 6 Crystal Tower,
    7 Planet's Core, 8 Ultimecia's Castle, 9 Crystal World, 10 Dream's End, 11 Order's Sanctuary,
    12 Empyreal Paradox, 13 Fortress, 14 Orphan's Cradle (the save's 15 stages). `rules_rights` picks the
    RULES row: 0 None, 1 Official, 2 Off (Skill), 3 EX-Centric, 4 AST-Centric, 5 BRV-Centric. Returns the
    steps and the frame the battle is on screen from. `player_moves` are D-pad presses on the player grid
    before the choice (with D012_TEST_UNLOCK_FIGHTERS the villains are the third row: dpdown, dpdown from
    Lightning is the Cloud of Darkness, six dpright more Kuja); every later step moves on by their time."""
    steps = [(1850, "scene menus"), (1900, "pad dpdown"), (2000, "pad a"), (2400, "pad a"),
             (3150, "shot player_select"), (3200, "pad a"), (3700, "pad a"), (4400, "pad a"), (4900, "pad a"),
             (5600, "pad dpdown")]
    frame = 5650
    for _ in range(stage_rights):
        steps.append((frame, "pad dpright"))
        frame += 20
    frame = max(frame, 5800)
    steps.append((frame, "pad dpdown"))
    for _ in range(4):
        frame += 50
        steps.append((frame, "pad dpleft"))
    ups = 2
    if rules_rights:
        for _ in range(3):
            frame += 40
            steps.append((frame, "pad dpdown"))
        for _ in range(rules_rights):
            frame += 30
            steps.append((frame, "pad dpright"))
        ups = 5
    for _ in range(ups):
        frame += 50
        steps.append((frame, "pad dpup"))
    frame -= 50
    steps += [(frame + 90, "shot battle_setup")]
    enter = max(frame + 150, 6200)
    steps += [(enter, "pad a"), (enter + 950, "shot versus"), (enter + 1050, "pad a"), (enter + 1950, "pad a")]
    if not player_moves:
        return steps, enter + 1960
    shift = 25 * (len(player_moves) + 1)
    steps = [(f + shift if f >= 3200 else f, action) for f, action in steps]
    steps += [(3200 + 25 * i, f"pad {move}") for i, move in enumerate(player_moves)]
    steps.append((3200 + shift - 10, "shot player_choice"))
    return steps, enter + 1960 + shift


def battle_fight(b):
    """A short fight from battle frame `b`: the intro at real time, then brave attacks, a dash, HP attacks and
    an assist; the CPU at minimal strength mostly stands."""
    return [(b - 8, "scene battle_intro"), (b, "speed normal"), (b + 40, "shot intro1"), (b + 140, "shot intro2"),
            (b + 260, "scene battle"), (b + 290, "shot fight1"),
            (b + 310, "pad b"), (b + 340, "pad b"), (b + 370, "pad b"), (b + 400, "shot fight2"), (b + 420, "pad y"),
            (b + 450, "pad b"), (b + 480, "pad x"), (b + 520, "shot fight3"), (b + 540, "pad a"), (b + 560, "pad b"),
            (b + 600, "shot fight4"), (b + 640, "pad x"), (b + 700, "shot fight5"), (b + 740, "pad rightshoulder+b"),
            (b + 800, "shot fight6"), (b + 860, "quit")]


def battle_route(name, stage_rights, about):
    steps, b = quick_battle(stage_rights)
    return {"name": name, "steps": BOOT + steps + battle_fight(b), "seconds": 90, "about": about}


def ex_results_route():
    """EX-Centric rules on Old Chaos Shrine: the EX gauges fill fast and the CPU (minimal, level 1) falls
    within the 90 s on most timelines (without D012_TEST_EX_FULL, which keeps Lightning in EX Mode and slowed the
    win). Attacks unthrottled with R+square between them, Bravery Break, the stage's change, the
    win, the results screen and its menu."""
    steps, b = quick_battle(1, rules_rights=3)
    steps = BOOT + steps + [(b - 8, "scene battle")]
    pattern = ["pad b", "pad b", "pad b", "pad x", "pad rightshoulder+x", "pad b", "pad b", "pad x",
               "pad rightshoulder+x", "pad y"]
    frame, i = b + 100, 0
    while frame < b + 1500:
        steps.append((frame, pattern[i % len(pattern)]))
        frame += 25
        i += 1
    for k, at in enumerate(range(b + 100, b + 1500, 200)):
        steps.append((at + 7, f"shot ex_fight{k + 1}"))
    steps += [(b + 1500, "scene results"), (b + 1560, "shot results1"), (b + 1680, "shot results2"),
              (b + 1800, "shot results3"), (b + 1920, "shot results4"), (b + 2040, "shot results_menu"),
              (b + 2120, "quit")]
    return {"name": "qb_ex_results", "steps": steps, "seconds": 90,
            "about": "Quick Battle, EX-Centric rules: fight unthrottled to the win and the results screen"}


def ex_burst_route(name, player_moves, about):
    """D012_TEST_EX_FULL (port/host/d012_test_switches.cpp) keeps the player's EX gauge full: R+square enters
    EX Mode and the next HP attack that lands starts the EX Burst. Unthrottled to the battle, then real time."""
    steps, b = quick_battle(1, player_moves=player_moves)
    steps = BOOT + steps + [(b - 8, "scene battle_intro"), (b, "speed normal"), (b + 140, "shot intro"),
                            (b + 260, "scene battle"), (b + 280, "pad b"), (b + 310, "pad b"),
                            (b + 340, "pad rightshoulder+x"), (b + 380, "scene ex_mode"), (b + 390, "shot ex_mode1"),
                            (b + 420, "pad b"), (b + 450, "pad x"), (b + 470, "shot ex_mode2"), (b + 500, "pad x"),
                            (b + 520, "scene ex_burst"), (b + 540, "shot ex_burst1"), (b + 560, "pad x"),
                            (b + 600, "shot ex_burst2"), (b + 640, "pad x"), (b + 660, "shot ex_burst3"),
                            (b + 700, "pad x"), (b + 720, "shot ex_burst4"), (b + 760, "pad b"), (b + 780, "shot ex_burst5"),
                            (b + 820, "pad x"), (b + 840, "shot after1"), (b + 850, "quit")]
    env = ["D012_TEST_EX_FULL=1"] + (["D012_TEST_UNLOCK_FIGHTERS=1"] if player_moves else [])
    return {"name": name, "steps": steps, "seconds": 90, "about": about, "env": env}


def ex_revenge_route():
    """EX-Centric rules with D012_TEST_EX_FULL: R+square while the CPU attacks is EX Revenge, whose greyscale reads
    the framebuffer through three CLUT16 index shifts (graphics-census.md, finding 1); unthrottled."""
    steps, b = quick_battle(1, rules_rights=3)
    steps = BOOT + steps + [(b - 8, "scene battle")]
    pattern = ["pad b", "pad b", "pad b", "pad x", "pad rightshoulder+x", "pad b", "pad b", "pad x",
               "pad rightshoulder+x", "pad y"]
    frame, i = b + 100, 0
    while frame < b + 1100:
        steps.append((frame, pattern[i % len(pattern)]))
        frame += 25
        i += 1
    for k, at in enumerate(range(b + 100, b + 1100, 100)):
        steps.append((at + 7, f"shot revenge{k + 1}"))
    steps.append((b + 1110, "quit"))
    return {"name": "qb_ex_revenge", "steps": steps, "seconds": 90, "env": ["D012_TEST_EX_FULL=1"],
            "about": "Quick Battle, EX-Centric rules, a full EX gauge: EX Mode under attack (EX Revenge's greyscale)"}


def water_route():
    """Lightning's Water (graphics-triage.md, "Water"): R while holding L changes her paradigm to RAV
    (Ravager), then a jump and circle cast Water, a sphere that refracts a half-size copy of the previous
    frame (0x041A6000). Its captures came out as a dark colour negative before the fix."""
    steps, b = quick_battle(1)
    steps = BOOT + steps + [(b - 8, "scene battle_intro"), (b, "speed normal"), (b + 260, "scene battle"),
                            (b + 270, "shot com"), (b + 280, "pad leftshoulder 30"), (b + 288, "pad rightshoulder 6"),
                            (b + 330, "shot rav"), (b + 350, "scene water")]
    # Run away from the CPU first (cast at point-blank range, the sphere bursts at once), then jump and
    # circle; three casts, since a jump made while a hit still holds her is lost on some timelines.
    for cast, (at, way) in enumerate(((b + 420, -1), (b + 560, 1), (b + 700, -1))):
        steps += [(at - 70, f"axis leftx {way}"), (at - 10, "axis leftx 0"), (at, "pad a"), (at + 12, "pad b")]
        steps += [(at + 12 + 6 * k, f"shot water{cast + 1}_{k}") for k in range(2, 10)]
    steps.append((b + 780, "quit"))
    return {"name": "qb_water", "steps": steps, "seconds": 90,
            "about": "Quick Battle, Lightning in RAV: Water (an aerial circle), a sphere refracting the last frame"}


def story_route():
    """Story Mode, Continue: Lightning on the world map (chapter 12-1-2), a walk up, right and down to the
    Gateway of Fools and Hope (its portal glow), into it (the white transition and the title card) and onto
    the gateway board (the beam, Lightning's lines)."""
    steps = [(400, "shot title"), (460, "pad a"), (1080, "pad a"), (1500, "pad a"), (1650, "pad a"),
             (1800, "scene menus"), (2000, "pad a"), (2350, "shot story_mode"), (2600, "pad a"), (3200, "pad a"),
             (3700, "shot continue"), (3800, "pad a"), (3880, "scene worldmap"), (4000, "shot worldmap"),
             (4200, "axis lefty -1"), (4400, "shot walk_up"), (4500, "axis lefty 0"), (4500, "axis leftx 1"),
             (4700, "shot walk_right"), (4800, "axis leftx 0"), (4800, "axis lefty 1"), (4980, "shot walk_down"),
             # Down to the gateway and confirm: its card, then into it. Timeline-dependent: on some runs the
             # moogle that follows Lightning is nearer and the confirm starts its lines instead.
             (5100, "shot gateway_ahead"), (5300, "axis lefty 0"), (5300, "pad a"), (5400, "pad a"), (5500, "pad a"),
             (5580, "shot gateway_info"), (5700, "pad a"), (5760, "shot portal_glow"),
             (5790, "scene gate_transition"), (5820, "shot gate_flash"), (5880, "shot gate_title"),
             (5930, "scene gateway_board"), (6000, "shot board_title"), (6060, "shot board_beam"),
             (6100, "pad a"), (6200, "shot board_dialog"), (6400, "pad a"), (6700, "pad a"), (6780, "shot board"),
             (6900, "quit")]
    return {"name": "story_gateway", "steps": steps, "seconds": 85,
            "about": "Story Mode: world map walk, a gateway's portal, the gate transition, the gateway board"}


ROUTES = [
    {"name": "menus", "seconds": 60, "about": "Title, LOAD GAME, the main menu and its pages, Battle Mode, Quick Battle setup",
     "steps": [(0, "scene boot"), (380, "scene title"), (400, "shot title"), (460, "pad a"), (1000, "shot load_game"),
               (1080, "pad a"), (1400, "shot load_list"), (1500, "pad a"), (1650, "pad a"),
               (1800, "scene main_menu"), (1850, "shot main_menu"), (1900, "pad dpdown"), (1980, "shot battle_mode_row"),
               (2000, "pad a"), (2350, "shot battle_mode"), (2400, "pad a"), (3150, "shot player_select"),
               (3200, "pad a"), (3650, "shot player_confirm"), (3700, "pad a"), (4350, "shot cpu_select"),
               (4400, "pad a"), (4850, "shot cpu_confirm"), (4900, "pad a"), (5550, "shot battle_setup"),
               (5600, "quit")]},
    story_route(),
    battle_route("qb_chaos_shrine", 1, "Quick Battle, Old Chaos Shrine: intro, brave and HP attacks, assist"),
    battle_route("qb_lunar_subterrane", 4, "Quick Battle, Lunar Subterrane (FFIV)"),
    battle_route("qb_orders_sanctuary", 11, "Quick Battle, Order's Sanctuary"),
    battle_route("qb_empyreal_paradox", 12, "Quick Battle, Empyreal Paradox"),
    battle_route("qb_orphans_cradle", 14, "Quick Battle, Orphan's Cradle"),
    ex_burst_route("qb_ex_burst", (), "Quick Battle, Lightning with a full EX gauge: EX Mode and EX Burst"),
    ex_burst_route("qb_kuja", ("dpdown", "dpdown") + ("dpright",) * 6,
                   "Quick Battle as Kuja (unlocked by a test switch) with a full EX gauge: his attacks and EX Burst"),
    ex_revenge_route(),
    ex_results_route(),
    water_route(),
]


def route_named(name):
    for route in ROUTES:
        if route["name"] == name:
            return route
    sys.exit(f"no route {name}; --list shows them")


def script_text(steps, quit_frame=None):
    items = sorted(steps, key=lambda step: step[0])
    text = ";".join(f"{frame}:{action}" for frame, action in items)
    if quit_frame is not None and not any(action == "quit" for _, action in items):
        text += f";{quit_frame}:quit"
    return text


# ---------------------------------------------------------------------------------------------------------

def prepare_data():
    if not os.path.isdir(DATA):
        print(f"copying {OWNER_DATA} to {DATA} (once)")
        shutil.copytree(OWNER_DATA, DATA)
    if not os.path.isdir(PRISTINE):
        shutil.copytree(os.path.join(OWNER_DATA, "ms0"), PRISTINE)
    ms0 = os.path.join(DATA, "ms0")
    if os.path.isdir(ms0):
        shutil.rmtree(ms0)
    shutil.copytree(PRISTINE, ms0)


def run_route(route, variant, options):
    out = os.path.join(RUNS, "runs", route["name"], variant)
    if os.path.isdir(out):
        shutil.rmtree(out)
    os.makedirs(out)
    prepare_data()
    census = os.path.join(out, "census.json")
    env = ["PSPRECOMP_NO_INTERPRETER=1", "D012_UNTHROTTLED=1", f"D012_GE_CENSUS={census}",
           f"D012_GE_CENSUS_SCENE=start", f"D012_SCREENSHOT_DIR={out}", "D012_SCREENSHOT_EVERY=100000000",
           f"D012_INPUT_SCRIPT={script_text(route['steps'])}"] + route.get("env", []) + VARIANTS[variant] + options.env
    log = os.path.join(out, "run.log")
    command = [PYTHON, os.path.join(ROOT, "tools", "port", "run_d012.py"), "--seconds",
               str(min(route.get("seconds", 90), options.seconds, 90)), "--build-dir", options.build_dir,
               "--data-dir", DATA, "--log", log]
    for item in env:
        command += ["--env", item]
    started = time.monotonic()
    result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    text = open(log, encoding="utf-8", errors="replace").read() if os.path.exists(log) else ""
    problems = []
    if "No recompiled function registered" in text:
        problems.append("interpreter fallback")
    if "[script] unknown action" in text:
        problems.append("unknown script action")
    shots = convert(out)
    # D012_CHECK_REPLAY (base variant): every 150 flips the older frame is drawn again from its recording and
    # compared with the frame as drawn; any difference is frame interpolation drawing something else.
    # The check compares whole RGBA words, and the alpha bits of a 5551 framebuffer hold the stencil, which
    # the replay does not rebuild; the older/again images it writes (RGB) say whether a difference shows.
    replay = []
    for m in re.finditer(r"\[interp\] replay check at frame (\d+): (\d+) of (\d+) pixels differ", text):
        rgba = 100.0 * int(m.group(2)) / int(m.group(3))
        older = os.path.join(out, f"replay_{m.group(1)}_older.bmp")
        again = os.path.join(out, f"replay_{m.group(1)}_again.bmp")
        rgb = rgba
        if os.path.exists(older) and os.path.exists(again):
            diff = ImageChops.difference(Image.open(older).convert("RGB"), Image.open(again).convert("RGB"))
            histogram = diff.convert("L").histogram()
            rgb = 100.0 * (sum(histogram) - histogram[0]) / float(sum(histogram))
        replay.append({"frame": int(m.group(1)), "rgba_pct": round(rgba, 3), "rgb_pct": round(rgb, 3)})
    if replay and max(check["rgb_pct"] for check in replay) > 1.0:
        problems.append(f"replay check: up to {max(c['rgb_pct'] for c in replay):.1f}% of pixels differ")
    print(f"  {route['name']}/{variant}: {result.stdout.strip()} {len(shots)} captures"
          f"{' PROBLEMS: ' + ', '.join(problems) if problems else ''} ({time.monotonic() - started:.0f} s)", flush=True)
    return {"route": route["name"], "variant": variant, "shots": shots, "problems": problems,
            "census": census if os.path.exists(census) else None, "replay_checks": replay}


def convert(directory):
    png = os.path.join(directory, "png")
    os.makedirs(png, exist_ok=True)
    names = []
    for name in sorted(os.listdir(directory)):
        if not name.endswith(".bmp") or name.startswith("replay_"):
            continue
        Image.open(os.path.join(directory, name)).convert("RGB").save(os.path.join(png, name[:-4] + ".png"))
        names.append(name[:-4])
    return names


def difference(a_path, b_path):
    a = Image.open(a_path).convert("RGB").resize((480, 272), Image.BOX)
    b = Image.open(b_path).convert("RGB").resize((480, 272), Image.BOX)
    diff = ImageChops.difference(a, b).convert("L")
    mean = ImageStat.Stat(diff).mean[0]
    histogram = diff.histogram()
    over = sum(histogram[41:]) / float(480 * 272)
    return round(mean, 2), round(100.0 * over, 2)


def flagged(mean, over):
    return mean > 8.0 or over > 6.0


def alternatives(directory, shot):
    """A shot's stored pictures: NAME.png and NAME.altN.png, one per timeline seen (see compare)."""
    if not os.path.isdir(directory):
        return []
    return sorted(os.path.join(directory, name) for name in os.listdir(directory)
                  if name == shot + ".png" or (name.startswith(shot + ".alt") and name.endswith(".png")))


def nearest(path, candidates):
    best = None
    for candidate in candidates:
        mean, over = difference(path, candidate)
        if best is None or (mean, over) < (best["mean"], best["over40_pct"]):
            best = {"mean": mean, "over40_pct": over, "flag": flagged(mean, over),
                    "match": os.path.basename(candidate), "of": len(candidates)}
    return best


def compare(results, options):
    """Each capture against the stored baseline of its route and variant. The guest clock follows the wall
    clock, so a battle or a walk falls into one of a few timelines (another stage variant, a moogle met on
    the way); the baseline keeps one picture per timeline seen and a capture is compared with the nearest.
    --save-baseline adds a capture that matches none of them as another timeline. nointerp captures are
    also compared with the nearest 1x picture (this run's or the baseline's), which differs by the internal
    scale alone when the timelines agree."""
    report = []
    for result in results:
        route, variant = result["route"], result["variant"]
        run_dir = os.path.join(RUNS, "runs", route, variant, "png")
        base_dir = os.path.join(TOUR, "baseline", route, variant)
        for shot in result["shots"]:
            path = os.path.join(run_dir, shot + ".png")
            entry = {"route": route, "variant": variant, "shot": shot}
            stored = alternatives(base_dir, shot)
            if stored:
                entry["baseline"] = nearest(path, stored)
            if variant == "nointerp":
                ones = alternatives(os.path.join(TOUR, "baseline", route, "1x"), shot)
                current = os.path.join(RUNS, "runs", route, "1x", "png", shot + ".png")
                if any(r["route"] == route and r["variant"] == "1x" for r in results) and os.path.exists(current):
                    ones.append(current)
                if ones:
                    entry["vs_1x"] = nearest(path, ones)
            report.append(entry)
            if options.save_baseline and (not stored or entry["baseline"]["flag"]):
                os.makedirs(base_dir, exist_ok=True)
                name = shot + ".png" if not stored else f"{shot}.alt{len(stored)}.png"
                shutil.copy2(path, os.path.join(base_dir, name))
                entry["saved_as"] = name
    return report


def merge_census(results):
    features = {}
    scenes = {}
    for result in results:
        if result["variant"] != "base" or not result["census"]:
            continue
        data = json.load(open(result["census"], encoding="utf-8"))
        for scene in data["scenes"]:
            if scene["draws"] == 0:
                continue
            key = f"{result['route']}/{scene['name']}"
            scenes[key] = {"frames": scene["frames"], "draws": scene["draws"], "features": scene["features"]}
        for feature in data["features"]:
            entry = features.setdefault(feature["feature"], {
                "feature": feature["feature"], "category": feature["category"], "impact": feature["impact"],
                "description": feature["description"], "count": 0, "scenes": {}, "variants": {},
                "first": None, "example": None})
            entry["count"] += feature["count"]
            for scene, count in feature["scenes"].items():
                entry["scenes"][f"{result['route']}/{scene}"] = count
            for variant, count in feature["variants"].items():
                entry["variants"][variant] = entry["variants"].get(variant, 0) + count
            if entry["example"] is None:
                entry["first"] = {"route": result["route"], "scene": feature["first_scene"],
                                  "frame": feature["first_frame"]}
                entry["example"] = feature["example"]
    rank = {"high": 0, "medium": 1, "low": 2, "info": 3, "": 4}
    ordered = sorted(features.values(), key=lambda f: (rank.get(f["impact"], 4), -f["count"]))
    return {"features": ordered, "scenes": scenes}


def contact_sheet(results):
    for result in results:
        directory = os.path.join(RUNS, "runs", result["route"], result["variant"], "png")
        names = result["shots"]
        if not names:
            continue
        width, height, columns = 320, 182, 4
        rows = (len(names) + columns - 1) // columns
        sheet = Image.new("RGB", (columns * width, rows * (height + 14)), (40, 40, 40))
        draw = ImageDraw.Draw(sheet)
        for i, name in enumerate(names):
            image = Image.open(os.path.join(directory, name + ".png")).resize((width, height))
            x, y = (i % columns) * width, (i // columns) * (height + 14)
            sheet.paste(image, (x, y + 14))
            draw.text((x + 2, y + 1), name, fill=(255, 255, 0))
        sheet.save(os.path.join(RUNS, "runs", result["route"], result["variant"], "sheet.png"))


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("--routes", default="all")
    parser.add_argument("--variants", default="base")
    parser.add_argument("--save-baseline", action="store_true")
    parser.add_argument("--build-dir", default=DEFAULT_BUILD)
    parser.add_argument("--seconds", type=float, default=90.0)
    parser.add_argument("--script", help="an ad hoc route: 'frame:action;...' (scene and shot steps included)")
    parser.add_argument("--name", default="probe", help="the ad hoc route's name")
    parser.add_argument("--list", action="store_true")
    parser.add_argument("--env", action="append", default=[], metavar="NAME=VALUE")
    parser.add_argument("--data-dir", default=None, help="the data copy to run against (default build/port/data-gfx-tour)")
    parser.add_argument("--out", default=None, help="where runs and reports go (default build/port/gfx-tour)")
    options = parser.parse_args()
    global DATA, RUNS
    if options.data_dir:
        DATA = os.path.join(ROOT, options.data_dir)
    if options.out:
        RUNS = os.path.join(ROOT, options.out)
    if os.path.abspath(DATA) == os.path.abspath(OWNER_DATA):
        sys.exit("refusing to run against build/port/data")
    if options.list:
        for route in ROUTES:
            print(f"{route['name']:24} {route['about']}")
        return 0
    if options.script:
        steps = []
        for item in options.script.split(";"):
            if ":" in item:
                frame, action = item.split(":", 1)
                steps.append((int(frame), action.strip()))
        routes = [{"name": options.name, "steps": steps, "seconds": options.seconds, "about": "ad hoc"}]
    elif options.routes == "all":
        routes = ROUTES
    else:
        routes = [route_named(name) for name in options.routes.split(",")]
    variants = options.variants.split(",")
    for variant in variants:
        if variant not in VARIANTS:
            sys.exit(f"unknown variant {variant}")
    results = []
    for route in routes:
        for variant in variants:
            results.append(run_route(route, variant, options))
    contact_sheet(results)
    report = compare(results, options)
    with open(os.path.join(RUNS, "report.json"), "w", encoding="utf-8") as file:
        json.dump(report, file, indent=1)
    flags = [entry for entry in report if any(isinstance(v, dict) and v.get("flag") for v in entry.values())]
    for entry in flags:
        parts = [f"{key} {value['mean']}/{value['over40_pct']}%" for key, value in entry.items()
                 if isinstance(value, dict) and value.get("flag")]
        print(f"  FLAG {entry['route']}/{entry['variant']}/{entry['shot']}: {', '.join(parts)}")
    summary = merge_census(results)
    if summary["features"]:
        with open(os.path.join(RUNS, "census_summary.json"), "w", encoding="utf-8") as file:
            json.dump(summary, file, indent=1)
        for feature in summary["features"]:
            print(f"  {feature['impact']:6} {feature['count']:>9} {feature['feature']:34} "
                  f"{len(feature['scenes'])} scenes")
    for result in results:
        if result["replay_checks"]:
            checks = result["replay_checks"]
            rgb = [check["rgb_pct"] for check in checks]
            rgba = [check["rgba_pct"] for check in checks]
            print(f"  replay check {result['route']}/{result['variant']}: {len(checks)} checks, colour differs in "
                  f"up to {max(rgb):.2f}% of pixels (with alpha {max(rgba):.2f}%)")
    with open(os.path.join(RUNS, "replay_checks.json"), "w", encoding="utf-8") as file:
        json.dump({f"{r['route']}/{r['variant']}": r["replay_checks"] for r in results if r["replay_checks"]}, file)
    problems = [r for r in results if r["problems"]]
    print(f"{len(results)} runs, {len(report)} captures, {len(flags)} flagged, {len(problems)} with problems")
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main())

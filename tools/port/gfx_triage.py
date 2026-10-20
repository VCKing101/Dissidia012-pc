"""Graphics triage: run one scripted scene of the recompiled port with A/B switches.

    python tools/port/gfx_triage.py SCENE LABEL [--env NAME=VALUE ...] [--build-dir DIR] [--seconds S]
    python tools/port/gfx_triage.py --list

Runs tools/port/run_d012.py on the recompiled build (default
tools/local/port/build-d012-gfx) with a copy of the owner's data,
build/port/data-gfx-triage (made once with `Copy-Item -Recurse build/port/data
build/port/data-gfx-triage`; build/port/data is never run against), with
PSPRECOMP_NO_INTERPRETER=1, the scene's input script and its own extra
switches, then converts the captures to PNG. Captures go to
build/port/gfx-triage/SCENE/LABEL/, the log to build/port/gfx-triage/logs/.
Every run is bounded (90 s at most, by run_d012.py).

Captures: the scene's `shot` steps. With --env D012_CAPTURE_BLENDED=1 each shot
waits for an in-between present that frame interpolation blended, which is
where interpolation faults show; --env D012_FRAME_RATE=30 turns interpolation
off for the A side.

The scenes (docs/port/graphics-triage.md): the routes start from the owner's
Story Mode save (Chapter 12-1, Lightning on the world map) and from Quick
Battle reached through its menus, with the test switches of port/host
(D012_TEST_UNLOCK_FIGHTERS for Kuja, D012_TEST_EX_FULL for the EX Burst).
"""

import argparse
import os
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
PYTHON = sys.executable

# Boot, title, LOAD GAME, the save, main menu.
BOOT = "460:pad a;1080:pad a;1500:pad a;1650:pad a;"
# Story Mode, Continue, Yes: the world map from about frame 3900.
WORLDMAP = BOOT + "2000:pad a;2600:pad a;3200:pad a;3800:pad a;4100:speed normal;"


def shots(first, count, step, prefix="s"):
    return "".join(f"{first + i * step}:shot {prefix}{first + i * step};" for i in range(count))


SCENES = {
    # Standing on the world map: mountains against the sky (the glow), the
    # moogle, gateways in the distance.
    "worldmap": WORLDMAP + shots(4300, 6, 20) + "4460:quit",
    # The same, turning the camera with the right stick while the shots are taken.
    "worldmap_turn": WORLDMAP + "4280:axis rightx 0.6;" + shots(4300, 6, 10) + "4370:axis rightx 0;4420:quit",
    # Walking forwards (up the screen) towards the gateway.
    "worldmap_walk": WORLDMAP + "4250:axis lefty -1;" + shots(4300, 8, 15) + "4430:axis lefty 0;4460:quit",
    # The camera turned left: a red gateway ahead with its swirling portal,
    # a glowing orb to its right. Standing still.
    "gate_view": WORLDMAP.replace("4100:speed normal;", "") + "3950:axis rightx -1;3965:axis rightx 0;"
                 "4000:speed normal;" + shots(4100, 12, 10) + "4230:quit",
    # Walking up to the gateway (its panel shows) and entering it with cross:
    # the transition into it.
    "gate_enter": WORLDMAP.replace("4100:speed normal;", "") + "3950:axis rightx -1;3965:axis rightx 0;"
                  "3990:speed normal;4000:axis lefty -1;4080:axis lefty 0;4100:pad a;" + shots(4110, 40, 8) + "4440:quit",
    # Walking up and right (the camera follows round) to a glowing orb, then
    # cross to take it.
    "orb": WORLDMAP.replace("4100:speed normal;", "") + "3950:axis rightx -1;3965:axis rightx 0;"
           "3990:speed normal;4000:axis lefty -0.8;4000:axis leftx 0.45;4410:axis lefty 0;4410:axis leftx 0;"
           "4400:pad a;4420:pad a;" + shots(4390, 26, 8) + "4610:quit",
}


def quick_battle(player_moves, stage, fight, quit_at, normal_after=20):
    """Quick Battle (1-on-1) against a CPU Lightning: the player's cursor moves
    from Lightning, the stage is `stage` presses right from Random (1 Old Chaos
    Shrine, 4 Lunar Subterrane, 11 Order's Sanctuary), then the steps `fight`,
    which start once the battle runs at real time (about frame 8250)."""
    script = "460:pad a;1080:pad a;1500:pad a;1900:pad dpdown;2000:pad a;2400:pad a;"
    frame = 3100
    for move in player_moves:
        script += f"{frame}:pad {move};"
        frame += 50
    script += "3600:pad a;4100:pad a;4800:pad a;5300:pad a;5900:pad dpdown;"
    frame = 6000
    for _ in range(stage):
        script += f"{frame}:pad dpright;"
        frame += 60
    script += f"{frame}:pad dpup;{frame + 100}:pad a;{frame + 1150}:pad a;{frame + 2050}:pad a;"
    script += f"{frame + 2050 + normal_after}:speed normal;"
    return script + fight + f"{quit_at}:quit"


KUJA = ["dpdown", "dpdown"] + ["dpright"] * 6
# Kuja's attacks: circle (bravery) and square (HP) in turn, shots in between.
KUJA_FIGHT = "".join(f"{f}:pad {'b' if i % 2 == 0 else 'x'};" for i, f in enumerate(range(8700, 9300, 45))) +     shots(8720, 14, 40)
SCENES["kuja_lunar"] = quick_battle(KUJA, 4, KUJA_FIGHT, 9300)
SCENES["kuja_order"] = quick_battle(KUJA, 11, KUJA_FIGHT, 9300)
# Lightning against Lightning on Old Chaos Shrine with D012_TEST_EX_FULL=1:
# R + square enters EX Mode, then walk in and attack (square) until the EX
# Burst starts; shots through it.
EX_FIGHT = "".join(f"{f}:pad rightshoulder 14;{f + 5}:pad x 4;" for f in (8760, 8960)) + "".join(
    f"{f}:axis lefty -1;{f + 20}:axis lefty 0;{f + 25}:pad x;" for f in range(8800, 9300, 50)
    if f not in (8950, 9000)) + shots(9250, 20, 25)
# Unthrottled up to the EX Burst (about frame 9280), real time from 9240.
SCENES["lightning_ex"] = quick_battle([], 1, EX_FIGHT, 9760, normal_after=1130)


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("scene", nargs="?")
    parser.add_argument("label", nargs="?")
    parser.add_argument("--env", action="append", default=[], metavar="NAME=VALUE")
    parser.add_argument("--build-dir", default=os.path.join("tools", "local", "port", "build-d012-gfx"))
    parser.add_argument("--data-dir", default=os.path.join("build", "port", "data-gfx-triage"))
    parser.add_argument("--seconds", type=float, default=88.0)
    parser.add_argument("--script", help="replace the scene's input script")
    parser.add_argument("--list", action="store_true")
    options = parser.parse_args()
    if options.list or not options.scene:
        for name, script in SCENES.items():
            print(f"{name}: {script}")
        return 0
    if options.data_dir.replace("\\", "/").rstrip("/").endswith("build/port/data"):
        sys.exit("never run against build/port/data; use a copy")
    script = options.script or SCENES[options.scene]
    out = os.path.join("build", "port", "gfx-triage", options.scene, options.label)
    log = os.path.join(ROOT, "build", "port", "gfx-triage", "logs", f"{options.scene}_{options.label}.log")
    env = ["PSPRECOMP_NO_INTERPRETER=1", "D012_UNTHROTTLED=1", "D012_MSGDIALOG_ANSWERS=ok",
           f"D012_SCREENSHOT_DIR={out}", "D012_SCREENSHOT_EVERY=100000", f"D012_INPUT_SCRIPT={script}"]
    env += options.env
    command = [PYTHON, os.path.join(ROOT, "tools", "port", "run_d012.py"), "--seconds", str(options.seconds),
               "--build-dir", options.build_dir, "--data-dir", options.data_dir, "--log", log]
    for item in env:
        command += ["--env", item]
    subprocess.run(command, cwd=ROOT, check=False)
    shots_dir = os.path.join(ROOT, out)
    if os.path.isdir(shots_dir):
        subprocess.run([PYTHON, os.path.join(ROOT, "tools", "port", "shots_to_png.py"), shots_dir], cwd=ROOT,
                       check=False, stdout=subprocess.DEVNULL)
    return 0


if __name__ == "__main__":
    sys.exit(main())

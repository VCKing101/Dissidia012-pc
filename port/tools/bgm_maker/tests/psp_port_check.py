"""End-to-end check of "Make a PSP / PPSSPP ISO": a mod's track, added to a
copy of the game's ISO by the Battle BGM Maker (BgmMaker --psp-iso), plays
from that ISO with its loop exactly where the tool set it. Test tones only.

    python port/tools/bgm_maker/tests/psp_port_check.py            # make, run the port (90 s at most), analyse
    python port/tools/bgm_maker/tests/psp_port_check.py --analyse  # analyse the last run's dump again

1. The tone track and the mod of port_check.py (an MP3, loop 88,200 to
   176,400), made the same way, in build/port/data-modmaker-test/mods.
2. That mod alone, in a mods folder of its own, goes into a copy of
   build/port/game/disc.iso (never written) with BgmMaker --psp-iso: the
   original is checked, the track encoded to ATRAC3 with atracdenc and added
   to the game's list as its first new row (id 0x4000).
3. The recompiled port runs on the made ISO (D012_GAME_DIR: a folder with
   EBOOT.ELF and the ISO) with an empty mods folder, so the track can only
   come from the ISO's own table, its selector on 0x4000, D012_AUDIO_DUMP
   writing its mix; the log must show the game opening DATA/BGM/MD00000000.AT3.
4. port_check.py's analysis: every seam matched to the sample against the
   tool's decode of the MP3. The ATRAC3 encoding makes every residual larger
   (about -2 dB where the port's own PCM gives -40), so the seams are judged
   together: the best shift must be 0, which also checks the encoder delay
   the export writes, and an MP3 delay's shift at least 20 % worse.
MIT licence; see port/README.md.
"""

from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import port_check as pc  # noqa: E402

ROOT = pc.ROOT
ORIGINAL = os.path.join(ROOT, "build", "port", "game", "disc.iso")
EBOOT = os.path.join(ROOT, "build", "port", "game", "EBOOT.ELF")
OUT = os.path.join(ROOT, "build", "port", "psp-iso-test")
TRACK_ID = "16384"  # 0x4000: the first row the export adds


def make_iso() -> str:
    mods = os.path.join(OUT, "mods")
    shutil.rmtree(mods, ignore_errors=True)
    shutil.copytree(os.path.join(pc.DATA, "mods", pc.MOD_ID), os.path.join(mods, pc.MOD_ID))
    iso = os.path.join(OUT, "made.iso")
    said = pc.bgm_maker("--psp-iso", ORIGINAL, iso, mods)
    if "1 track(s) added" not in said:
        raise RuntimeError("BgmMaker --psp-iso did not add the one track")
    return iso


def run_port(iso: str):
    game = os.path.join(OUT, "game")
    os.makedirs(game, exist_ok=True)
    shutil.copyfile(EBOOT, os.path.join(game, "EBOOT.ELF"))
    disc = os.path.join(game, "disc.iso")
    if os.path.exists(disc):
        os.remove(disc)
    try:
        os.link(iso, disc)
    except OSError:
        shutil.copyfile(iso, disc)
    no_mods = os.path.join(OUT, "no-mods")
    os.makedirs(no_mods, exist_ok=True)
    dump = os.path.join(OUT, "dump.wav")
    if os.path.exists(dump):
        os.remove(dump)
    log = os.path.join(OUT, "run.log")
    command = [sys.executable, os.path.join(ROOT, "tools", "port", "run_d012.py"), "--seconds", "90",
               "--build-dir", pc.BUILD, "--data-dir", os.path.relpath(pc.DATA, ROOT), "--log", log,
               "--env", f"D012_GAME_DIR={game}", "--env", f"D012_MODS_DIR={no_mods}",
               "--env", "PSPRECOMP_NO_INTERPRETER=1", "--env", "D012_UNTHROTTLED=1", "--env", "D012_TRACE_IO=1",
               "--env", f"D012_TEST_BGM_START={TRACK_ID}", "--env", f"D012_AUDIO_DUMP={dump}",
               "--env", f"D012_INPUT_SCRIPT={pc.ROUTE}"]
    print("running the port on the made ISO (at most 90 s)...")
    subprocess.run(command, check=True, timeout=150)


def opened_from_iso() -> bool:
    with open(os.path.join(OUT, "run.log"), encoding="utf-8", errors="replace") as handle:
        lines = [line.rstrip() for line in handle if "MD00000000.AT3" in line]
    for line in lines[:3]:
        print("  log:", line[:160])
    return any("open DISC0:/PSP_GAME/USRDIR/DATA/BGM/MD00000000.AT3" in line for line in lines)


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("--analyse", action="store_true", help="only analyse the last run's dump")
    options = parser.parse_args()
    os.makedirs(OUT, exist_ok=True)
    if not options.analyse:
        for path in (ORIGINAL, EBOOT):
            if not os.path.isfile(path):
                print(f"FAIL: {path} is not there (set the game up first)")
                return 1
        os.makedirs(pc.OUT, exist_ok=True)
        ff = pc.audio.find_ffmpeg()
        pc.make_data_dir()
        pc.make_mod(ff, pc.make_tones(ff))
        shutil.copyfile(os.path.join(pc.OUT, "tool_decode.wav"), os.path.join(OUT, "tool_decode.wav"))
        run_port(make_iso())
        for made in (os.path.join(OUT, "made.iso"), os.path.join(OUT, "game", "disc.iso")):
            os.remove(made)  # 1.8 GB each when the link could not be made
    from_iso = opened_from_iso()
    if not from_iso:
        print("FAIL: the game did not open the added track from the ISO")
    passed = pc.analyse(OUT, check_log=False, margin=1.2) and from_iso
    return 0 if passed else 1


if __name__ == "__main__":
    sys.exit(main())

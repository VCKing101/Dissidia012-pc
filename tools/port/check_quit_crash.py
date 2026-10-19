"""Shutdown check: run the recompiled port to a Quick Battle and quit, N times,
and count the runs that crash on the way out. See docs/port/quit-crash.md.

    python tools/port/check_quit_crash.py [--runs N] [--camera on|off|both] [--quit script|window|menu]
        [--work DIR] [--data-dir DIR] [--seconds S] [--env NAME=VALUE ...]

Each run goes through tools/port/run_d012.py (at most 90 s, stopped by its own
handle), on the recompiled build with PSPRECOMP_NO_INTERPRETER=1 and
D012_UNTHROTTLED=1, data in --data-dir (default build/port/data-quit-test, a
copy of build/port/data made once if it is missing; build/port/data itself is
never written). The route is the right-stick camera's (right-stick-camera.md,
section 7): title, Continue, the save, Quick Battle, Lightning against
Lightning; in battle the right stick is pushed both ways, the left stick walks,
then the run quits:

  script  the input script's `quit` (an SDL quit event, as the window's close
          button sends)
  window  a WM_CLOSE to the port's own window, found by its process id, --close-after seconds from the start
  menu    Esc opens the port's menu, System, Quit game, Quit

--camera off adds D012_NO_RIGHT_STICK_CAMERA=1; both alternates on and off.
With D012_CRASH_DUMP (added by default, into <work>/dumps) a crash leaves a
minidump and a symbolised stack in the run's log.

A run is "clean" when the log says "Runtime stopped" and the process exits with
0 (or 4, the old exit code of a quit); "crash" when it exits with anything else
(an NTSTATUS such as 0xC0000005); "timeout" when the time limit came first. Exit status 0 when no run crashed or timed out.
"""

import argparse
import ctypes
import os
import re
import shutil
import subprocess
import sys
import threading
import time

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
PYTHON = sys.executable
BUILD = os.path.join("tools", "local", "port", "build-d012-recomp")

ROUTE = ("460:pad a;1080:pad a;1500:pad a;1650:pad a;1900:pad dpdown;2000:pad a;2400:pad a;3200:pad a;3700:pad a;"
         "4400:pad a;4900:pad a;5600:pad dpdown;5750:pad dpright;5800:pad dpdown;5850:pad dpleft;5900:pad dpleft;"
         "5950:pad dpleft;6000:pad dpleft;6050:pad dpup;6100:pad dpup;6200:pad a;7250:pad a;8150:pad a;"
         "8160:speed normal;8500:axis rightx 1;8560:axis rightx 0;8580:axis righty -1;8620:axis righty 0;"
         "8640:pad rightstick;8660:axis leftx 1;8700:axis leftx 0;8720:axis rightx -1;8780:axis rightx 0;")
QUIT_FRAME = 8820
# Story Mode, Continue: Lightning on the world map (right-stick-camera.md, section 7), where the
# earlier crashes were seen.
WORLDMAP = ("460:pad a;1080:pad a;1500:pad a;1650:pad a;2000:pad a;2600:pad a;3200:pad a;3800:pad a;"
            "4100:speed normal;4310:axis rightx 1;4370:axis rightx 0;4420:pad leftshoulder 30;")
WORLDMAP_QUIT = 4560
# The port's menu: Esc, W four times to the System page, five rows down to "Quit game", Return, and Return
# on the confirmation's Quit (Left: the focus starts on Cancel). The menu pauses the game; its frames still count.
MENU_KEYS = ";".join(f"{{q{offset}}}:{step}" for offset, step in
                     [(0, "key Escape"), (40, "key W"), (60, "key W"), (80, "key W"), (100, "key W"),
                      (130, "key Down"), (150, "key Down"), (170, "key Down"), (190, "key Down"), (210, "key Down"),
                      (240, "key Return"), (280, "key Left"), (300, "shot menu_confirm"), (320, "key Return")])


def close_window(pid):
    """Posts WM_CLOSE to the top-level windows of process `pid` only."""
    user32 = ctypes.windll.user32
    found = []

    @ctypes.WINFUNCTYPE(ctypes.c_bool, ctypes.c_void_p, ctypes.c_void_p)
    def each(hwnd, _):
        owner = ctypes.c_ulong()
        user32.GetWindowThreadProcessId(ctypes.c_void_p(hwnd), ctypes.byref(owner))
        if owner.value == pid and user32.IsWindowVisible(ctypes.c_void_p(hwnd)):
            found.append(hwnd)
        return True

    user32.EnumWindows(each, None)
    for hwnd in found:
        user32.PostMessageW(ctypes.c_void_p(hwnd), 0x0010, 0, 0)  # WM_CLOSE
    return len(found)


def run_once(index, camera, options, work):
    name = f"{options.label}{index:02d}_{'cam' if camera else 'nocam'}"
    log = os.path.join(work, "logs", name + ".log")
    route, quit_frame = (WORLDMAP, WORLDMAP_QUIT) if options.route == "worldmap" else (ROUTE, QUIT_FRAME)
    if options.quit == "script":
        route += f"{quit_frame}:quit"
    elif options.quit == "menu":
        route += re.sub(r"\{q(\d+)\}", lambda m: str(quit_frame + int(m.group(1))), options.menu_keys).replace(
            "@", str(quit_frame))
    else:
        route += f"{quit_frame + 3000}:pad a"  # no quit step: the window is closed from outside
    env = ["PSPRECOMP_NO_INTERPRETER=1", "D012_UNTHROTTLED=1", f"D012_INPUT_SCRIPT={route}",
           f"D012_CRASH_DUMP={os.path.join(work, 'dumps')}"] + options.env
    if not camera:
        env.append("D012_NO_RIGHT_STICK_CAMERA=1")
    command = [PYTHON, os.path.join(ROOT, "tools", "port", "run_d012.py"), "--seconds", str(options.seconds),
               "--build-dir", BUILD, "--data-dir", options.data_dir, "--log", log]
    for item in env:
        command += ["--env", item]
    started = time.monotonic()
    runner = subprocess.Popen(command, cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    if options.quit == "window":
        # Wait for the battle (the log's QUIT_FRAME script step has no quit) then close the window.
        threading.Thread(target=watch_and_close, args=(runner, options.close_after), daemon=True).start()
    output = runner.communicate()[0]
    elapsed = time.monotonic() - started
    match = re.search(r"pid (\d+) (exited with (-?\d+)|stopped at the time limit)", output)
    text = open(log, "r", encoding="utf-8", errors="replace").read() if os.path.exists(log) else ""
    stopped = re.search(r"Runtime stopped: (.*)", text)
    code = int(match.group(3)) if match and match.group(3) is not None else None
    if code is not None and code < 0:
        code &= 0xFFFFFFFF
    if code is None:
        outcome = "timeout"
    elif code not in (0, 4):
        outcome = "crash"  # stdout is block-buffered: a crash can lose the "Runtime stopped" line
    elif stopped:
        outcome = "clean"
    else:
        outcome = "died"  # exited cleanly without the runtime stopping
    crash = re.search(r"\[crash\] unhandled exception (0x[0-9a-f]+)", text)
    frames = re.findall(r"  #0\d (\S+)", text)
    detail = f"exit {code:#x}" if code is not None else "no exit"
    if stopped:
        detail += f", stopped: {stopped.group(1).strip()}"
    if crash:
        detail += f", exception {crash.group(1)} " + " < ".join(frames[:4])
    print(f"{name}: {outcome} ({detail}) {elapsed:.0f} s", flush=True)
    return outcome


def watch_and_close(runner, seconds):
    """Closes the window of the D012Native.exe that `runner` started, `seconds` after the start. The log is
    block-buffered, so the time and not the log says where the game is."""
    time.sleep(seconds)
    if runner.poll() is not None:
        return
    pid = find_child_pid(runner.pid)
    if pid:
        print(f"  WM_CLOSE to {close_window(pid)} window(s) of pid {pid}", flush=True)
    else:
        print("  no D012Native.exe child found", flush=True)


def find_child_pid(parent):
    """The D012Native.exe that run_d012.py started: a descendant of `parent` (a venv's python.exe starts
    the real interpreter as a child of its own)."""
    out = subprocess.run(["powershell", "-NoProfile", "-Command",
                          "Get-CimInstance Win32_Process | ForEach-Object "
                          "{ \"$($_.ProcessId) $($_.ParentProcessId) $($_.Name)\" }"],
                         capture_output=True, text=True).stdout
    children = {}
    for line in out.splitlines():
        parts = line.split(" ", 2)
        if len(parts) == 3:
            children.setdefault(int(parts[1]), []).append((int(parts[0]), parts[2]))
    queue = [parent]
    while queue:
        for pid, name in children.get(queue.pop(), []):
            if name == "D012Native.exe":
                return pid
            queue.append(pid)
    return None


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("--runs", type=int, default=10)
    parser.add_argument("--camera", choices=("on", "off", "both"), default="both")
    parser.add_argument("--quit", choices=("script", "window", "menu"), default="script")
    parser.add_argument("--work", default=os.path.join("build", "port", "quit-test"))
    parser.add_argument("--data-dir", default=os.path.join("build", "port", "data-quit-test"))
    parser.add_argument("--seconds", type=float, default=90.0)
    parser.add_argument("--label", default="r")
    parser.add_argument("--close-after", type=float, default=32.0,
                        help="--quit window: seconds from the start (32: the world map, 50: the battle)")
    parser.add_argument("--route", choices=("battle", "worldmap"), default="battle")
    parser.add_argument("--menu-keys", default=MENU_KEYS, help="script steps for --quit menu; @ is the quit frame")
    parser.add_argument("--env", action="append", default=[], metavar="NAME=VALUE")
    options = parser.parse_args()
    options.seconds = min(options.seconds, 90.0)
    work = os.path.join(ROOT, options.work)
    os.makedirs(os.path.join(work, "logs"), exist_ok=True)
    data = os.path.join(ROOT, options.data_dir)
    if os.path.abspath(data) == os.path.abspath(os.path.join(ROOT, "build", "port", "data")):
        sys.exit("refusing to run against build/port/data")
    if not os.path.isdir(data):
        shutil.copytree(os.path.join(ROOT, "build", "port", "data"), data)
    counts = {}
    for index in range(options.runs):
        camera = options.camera == "on" or (options.camera == "both" and index % 2 == 0)
        outcome = run_once(index + 1, camera, options, work)
        key = ("cam" if camera else "nocam", outcome)
        counts[key] = counts.get(key, 0) + 1
    for (camera, outcome), count in sorted(counts.items()):
        print(f"{camera:5} {outcome:7} {count}")
    return 0 if all(outcome == "clean" for (_, outcome) in counts) else 1


if __name__ == "__main__":
    sys.exit(main())

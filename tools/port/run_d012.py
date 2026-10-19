"""Run the port for a bounded time and keep its output.

    python tools/port/run_d012.py [--seconds N] [--headless] [--log FILE] [--env NAME=VALUE ...]
                                  [--build-dir DIR] [--data-dir DIR] [-- args]

Starts tools/local/port/build-d012/bin/D012Native.exe with D012_GAME_DIR set to
build/port/game and D012_DATA_DIR to build/port/data, writes stdout and stderr
to a log (default build/port/logs/run.log), and stops the process it started,
by its own handle, when the time is up (at most 90 s). Nothing else is touched.
The memory stick is ms0/ in the data directory (D012_MEMORY_STICK_DIR), so the
game's saves stay in the port's own data directory, never in build/port/game.

--build-dir picks another build directory (default tools/local/port/build-d012;
the recompiled build is tools/local/port/build-d012-recomp) and --data-dir
another data directory (default build/port/data).

--headless sets D012_NO_RENDER=1 and D012_NO_AUDIO=1. --env adds or overrides
any variable, e.g. --env D012_TRACE_IO=1 --env D012_SCREENSHOT_DIR=build/port/shots.
"""

import argparse
import os
import subprocess
import sys
import time

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BUILD_DIR = os.path.join(ROOT, "tools", "local", "port", "build-d012")
MAX_SECONDS = 90


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("--seconds", type=float, default=40.0)
    parser.add_argument("--headless", action="store_true")
    parser.add_argument("--log", default=os.path.join(ROOT, "build", "port", "logs", "run.log"))
    parser.add_argument("--env", action="append", default=[], metavar="NAME=VALUE")
    parser.add_argument("--build-dir", default=BUILD_DIR)
    parser.add_argument("--data-dir", default=os.path.join(ROOT, "build", "port", "data"))
    parser.add_argument("args", nargs="*")
    options = parser.parse_args()
    seconds = min(options.seconds, MAX_SECONDS)

    env = dict(os.environ)
    env["D012_GAME_DIR"] = os.path.join(ROOT, "build", "port", "game")
    env["D012_DATA_DIR"] = os.path.join(ROOT, options.data_dir)
    os.makedirs(env["D012_DATA_DIR"], exist_ok=True)
    env["D012_MEMORY_STICK_DIR"] = os.path.join(env["D012_DATA_DIR"], "ms0")
    exe = os.path.join(ROOT, options.build_dir, "bin", "D012Native.exe")
    if options.headless:
        env["D012_NO_RENDER"] = "1"
        env["D012_NO_AUDIO"] = "1"
    for item in options.env:
        name, _, value = item.partition("=")
        if value and not os.path.isabs(value) and ("/" in value or "\\" in value):
            value = os.path.join(ROOT, value)
            os.makedirs(value, exist_ok=True) if name.endswith("_DIR") else None
        env[name] = value

    os.makedirs(os.path.dirname(os.path.abspath(options.log)), exist_ok=True)
    started = time.monotonic()
    with open(options.log, "wb") as log:
        process = subprocess.Popen([exe] + options.args, cwd=os.path.dirname(exe), env=env,
                                   stdout=log, stderr=subprocess.STDOUT)
        try:
            code = process.wait(timeout=seconds)
            outcome = f"exited with {code}"
        except subprocess.TimeoutExpired:
            process.kill()  # this process only, by its handle
            process.wait()
            outcome = "stopped at the time limit"
    print(f"pid {process.pid} {outcome} after {time.monotonic() - started:.1f} s; log {options.log}")
    return 0


if __name__ == "__main__":
    sys.exit(main())

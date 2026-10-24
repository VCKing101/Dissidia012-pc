"""Console output, questions, state and process helpers for the port's setup (setup.ps1)."""

import json
import os
import re
import subprocess
import sys
import time
from collections import deque
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
STATE_FILE = ROOT / "build" / "port" / "setup" / "state.json"
LOG_DIR = ROOT / "build" / "port" / "logs"
VSENV = ROOT / "tools" / "port" / "setup" / "vsenv.cmd"
PK_CMAKE = ROOT / "tools" / "port" / "pk_cmake.cmd"

INTERACTIVE = True  # set by d012_setup.main()


class SetupError(Exception):
    """A problem the user can fix; the message says what to do."""


def step(number, total, title):
    print()
    print(f"[{number}/{total}] {title}")


def info(text=""):
    print(f"    {text}" if text else "")


def warn(text):
    print(f"    WARNING: {text}")


def ask(question, default=""):
    """A line from the user (quotes from a dragged-in path removed); the default on Enter."""
    if not INTERACTIVE:
        raise SetupError(f"{question} (no answer possible with -NonInteractive)")
    suffix = f" [{default}]" if default else ""
    try:
        answer = input(f"    {question}{suffix}: ").strip()
    except EOFError:
        raise SetupError("no answer (input closed)")
    answer = answer.strip().strip('"').strip("'").strip()
    if answer.startswith("& "):  # PowerShell's drag-and-drop form: & 'C:\path'
        answer = answer[2:].strip().strip("'").strip('"')
    return answer or default


def confirm(question, default=True):
    if not INTERACTIVE:
        return default
    hint = "Y/n" if default else "y/N"
    while True:
        answer = ask(f"{question} ({hint})").lower()
        if not answer:
            return default
        if answer in ("y", "yes"):
            return True
        if answer in ("n", "no"):
            return False


def load_state():
    try:
        return json.loads(STATE_FILE.read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return {}


def save_state(state):
    STATE_FILE.parent.mkdir(parents=True, exist_ok=True)
    temporary = STATE_FILE.with_suffix(".tmp")
    temporary.write_text(json.dumps(state, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    os.replace(temporary, STATE_FILE)


def duration(seconds):
    seconds = int(seconds)
    if seconds < 60:
        return f"{seconds} s"
    return f"{seconds // 60} min {seconds % 60:02d} s"


def _quote(argument):
    argument = str(argument)
    if not argument or any(c in argument for c in ' \t&()^=;,'):
        return f'"{argument}"'
    return argument


def cmd_line(script, args):
    """A cmd.exe command line that runs a batch file with arguments, spaces in paths included."""
    return 'cmd /d /s /c "' + " ".join(_quote(a) for a in [script, *args]) + '"'


def run(args, log_name=None, cwd=ROOT, progress=None, check=True, shell_line=None, tail=40):
    """Run a process, keeping its output in build/port/logs/<log_name> (printed only on failure).

    progress=(label, expected) parses Ninja's "[done/total]" lines into one updating line.
    Returns (exit code, list of the last output lines)."""
    LOG_DIR.mkdir(parents=True, exist_ok=True)
    log_path = LOG_DIR / log_name if log_name else None
    command = shell_line if shell_line is not None else [str(a) for a in args]
    started = time.monotonic()
    last = deque(maxlen=tail)
    process = subprocess.Popen(command, cwd=str(cwd), stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                               stdin=subprocess.DEVNULL)
    log = open(log_path, "w", encoding="utf-8", errors="replace") if log_path else None
    tty = sys.stdout.isatty()
    shown = -1
    pattern = re.compile(rb"^\[(\d+)/(\d+)\]")
    try:
        for raw in process.stdout:
            line = raw.decode("utf-8", errors="replace").rstrip("\r\n")
            if log:
                log.write(line + "\n")
            last.append(line)
            if progress:
                match = pattern.match(raw)
                if match:
                    done, total = int(match.group(1)), int(match.group(2))
                    percent = done * 100 // max(total, 1)
                    elapsed = time.monotonic() - started
                    left = ""
                    if done >= max(5, total // 10) and done < total:
                        left = f", about {duration(elapsed / done * (total - done))} left"
                    text = f"    {progress}: {done}/{total} steps ({percent}%), {duration(elapsed)}{left}"
                    if tty:
                        sys.stdout.write("\r" + text.ljust(100)[:118])
                        sys.stdout.flush()
                    elif percent // 10 != shown:
                        shown = percent // 10
                        print(text, flush=True)
        code = process.wait()
    except KeyboardInterrupt:
        process.terminate()  # the process this function started
        process.wait()
        raise
    finally:
        if log:
            log.close()
        if progress and tty:
            sys.stdout.write("\n")
    if code != 0 and check:
        print()
        for line in last:
            print(f"      | {line}")
        where = f" (full output: {log_path})" if log_path else ""
        raise SetupError(f"{Path(str(args[0] if args else 'command')).name} failed with exit code {code}{where}")
    return code, list(last)


def run_vs(script, args, log_name, progress=None, check=True):
    """Run a batch file (pk_cmake.cmd, vsenv.cmd) through cmd.exe."""
    return run([script], log_name=log_name, progress=progress, check=check, shell_line=cmd_line(script, args))

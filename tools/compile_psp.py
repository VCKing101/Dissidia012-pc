"""Run the pinned historical PSP compiler through wibo in Ubuntu WSL."""

import os
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def compiler_command(executable, arguments):
    guest_command = ["tools/local/wibo", Path(executable).as_posix(), *arguments]
    if os.name == "nt":
        linux_root = "/mnt/" + ROOT.drive[0].lower() + ROOT.as_posix()[2:]
        return ["wsl", "-d", "Ubuntu-24.04", "--cd", linux_root, "--", *guest_command]
    return guest_command


def compile_object(executable, flags, source, output):
    if "psp-gcc-" in executable:
        compiler_name = Path(executable).parts[2]
        guest_command = ["bash", "tools/probe_legacy_gcc.sh", compiler_name, *flags,
                         Path(source).as_posix(), Path(output).as_posix()]
        if os.name == "nt":
            linux_root = "/mnt/" + ROOT.drive[0].lower() + ROOT.as_posix()[2:]
            command = ["wsl", "-d", "Ubuntu-24.04", "--cd", linux_root, "--", *guest_command]
        else:
            command = guest_command
        return subprocess.run(command, cwd=ROOT, capture_output=True, text=True, timeout=45)
    includes = ["-Iinclude"] if Path(executable).name.lower() == "pspsnc.exe" else ["-nostdinc", "-cwd", "include", "-i", "include"]
    args = [*flags, *includes, "-c", Path(source).as_posix(), "-o", Path(output).as_posix()]
    return subprocess.run(compiler_command(executable, args), cwd=ROOT, capture_output=True, text=True, timeout=45)

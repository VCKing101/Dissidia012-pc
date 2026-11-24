"""Checks the Linux setup's AppDir before it is packed, with the Windows setup's rules.

    python3 tools/port/linux_check_setup.py [--program <file>] <AppDir> [<machine path> ...]

Run by tools/port/linux_setup.sh, and by tools/port/linux_bgm_maker.sh with --program BgmMaker (the program
checked for the game's code; payload/D012Native unless named). Refuses (exit 1) when the AppDir holds:
- a file named like the game's, or one that is the game's by its contents (package_installer.py's FORBIDDEN_NAMES
  and is_game_content: a PSP ELF, a PBP, an encrypted PSP executable, a PARAM.SFO, an ISO 9660 image);
- a program bigger than one without the game's code, or naming a recompiled unit;
- a path of this machine (the checkout, the home folder, the work folder, and each path given) in the port's own
  files. The toolchain (LLVM's release files and the SDK image's sysroot) is not searched for paths.
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from package_installer import FORBIDDEN_NAMES, MAX_PROGRAM_BYTES, is_game_content  # noqa: E402

ROOT = Path(__file__).resolve().parents[2]


def main():
    args = sys.argv[1:]
    program_name = "payload/D012Native"
    if args[:1] == ["--program"] and len(args) >= 2:
        program_name = args[1]
        args = args[2:]
    if not args:
        sys.exit(__doc__)
    appdir = Path(args[0])
    program = appdir / program_name
    needles = set()
    for base in [ROOT, Path.home(), *map(Path, args[1:])]:
        needles.update({str(base).encode(), base.as_posix().encode()})
    needles.discard(b"/")
    problems = []
    program_bytes = 0
    files = [p for p in sorted(appdir.rglob("*")) if p.is_file() and not p.is_symlink()]
    for path in files:
        relative = path.relative_to(appdir)
        if FORBIDDEN_NAMES.search(path.name):
            problems.append(f"{relative}: named like a file of the game")
        with open(path, "rb") as stream:
            head = stream.read(40000)
        found = is_game_content(head)
        if found:
            problems.append(f"{relative}: {found}")
        if relative.parts[:2] == ("payload", "toolchain"):
            continue
        data = path.read_bytes()
        for needle in needles:
            if needle in data:
                problems.append(f"{relative}: contains a path of this machine ({needle[:60]!r})")
                break
        if path == program:
            program_bytes = len(data)
            if b"recomp_unit_0" in data or b"register_recomp_unit" in data:
                problems.append(f"{relative}: names recompiled units: the game's code is linked in")
    if program_bytes == 0:
        problems.append(f"{program_name} is missing")
    elif program_bytes > MAX_PROGRAM_BYTES:
        problems.append(f"{program_name} is {program_bytes:,} bytes: more than a program without the game's code")
    if problems:
        sys.exit("the setup's files are not fit to ship:\n  " + "\n  ".join(problems))
    total = sum(p.stat().st_size for p in files)
    print(f"  checked {len(files)} files, {total / 2**20:.0f} MB: no game files, no paths of this machine, "
          "a program without the game's code")


if __name__ == "__main__":
    main()

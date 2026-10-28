"""Assemble the self-contained Windows package of the port, for the Steam Deck (Proton).

    python tools/port/package_deck.py [--build] [--build-dir DIR] [--out DIR] [--game-dir DIR] [--copy-iso]

Makes build/port/deck-package/Dissidia012/ from the release build, configured with
PORTABLEKIT_RELEASE=ON: tools/local/port/build-d012-release-clang (llvm-mingw, game code
about three times faster) when it is configured, else tools/local/port/build-d012-release
(MSVC, what setup.ps1 -Deck makes; docs/port/steam-deck.md):

    Dissidia012/D012Native.exe        the program
    Dissidia012/*.dll                 SDL3 and FFmpeg (and libc++, libunwind from clang)
    Dissidia012/overlays/*.dll        the recompiled overlay libraries
    Dissidia012/licenses/             FFmpeg's, SDL's and PortableKit's licence texts
    Dissidia012/dissidia012.ico       the launcher icon, if build/port/launcher has it (for Steam)
    Dissidia012/game/EBOOT.ELF        copied from build/port/game (SHA-256 checked)
    Dissidia012/game/disc.iso         hard-linked from build/port/game when on the same volume, else copied
    Dissidia012/data/settings.ini     written only if absent: fullscreen, 2x, vsync, 60 fps

The program finds game/ and data/ next to itself when no D012_* variables are set
(port/host/d012_portable.cpp) and runs only its recompiled code.

Files of the program that the build no longer makes are removed from the package;
data/ is never emptied or overwritten. No saves and no mods are ever put in it: the
script refuses to go on if data/ holds ms0/ or mods/ (a run from the package folder
would have made them), so the package stays clean for deploy_deck.py.

--build first builds the release target with tools/port/pk_cmake.cmd, or pk_cmake_mingw.cmd
for a clang build. --copy-iso copies
the disc image instead of hard-linking it. The package holds game code and data: it
stays in the ignored build/ and goes nowhere but the owner's Deck.
"""

import argparse
import hashlib
import os
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
MSVC_BUILD_DIR = ROOT / "tools/local/port/build-d012-release"
CLANG_BUILD_DIR = ROOT / "tools/local/port/build-d012-release-clang"
BUILD_DIR = CLANG_BUILD_DIR if (CLANG_BUILD_DIR / "CMakeCache.txt").exists() else MSVC_BUILD_DIR
OUT_DIR = ROOT / "build/port/deck-package"
GAME_DIR = ROOT / "build/port/game"
PACKAGE_NAME = "Dissidia012"
EXE_NAME = "D012Native.exe"
# port/host/d012_profile.cpp, executable_sha256.
EBOOT_SHA256 = "0bc8e779aad2814a3d1bd77346f69ca5e16665845ed053cd0ad44a9cbfaefe6d"
ICON = ROOT / "build/port/launcher/dissidia012.ico"
LICENSES = {
    "PortableKit-LICENSE.txt": ROOT / "tools/local/port/PortableKit/LICENSE",
    "SDL3-LICENSE.txt": ROOT / "tools/local/port-deps/sdl3/SDL3-3.4.18/LICENSE.txt",
}

# Deck-friendly first-run settings (keys and values from PortableKit's host/settings/settings.cpp).
DECK_SETTINGS = """\
# Steam Deck defaults, written by tools/port/package_deck.py. D012Native rewrites
# this file when settings change in its menu (L3+R3 on the pad, or Esc).
video.fullscreen=1
video.internal_scale=2
video.present_mode=vsync
video.frame_rate=60
video.frame_rate_auto=1
video.aspect=original
"""


def sha256_file(path, chunk=1 << 20):
    digest = hashlib.sha256()
    with open(path, "rb") as stream:
        while block := stream.read(chunk):
            digest.update(block)
    return digest.hexdigest()


def same_file(a, b):
    try:
        return os.path.samefile(a, b)
    except OSError:
        return False


def copy_if_changed(source, target):
    """Copy when the target is missing or differs in size or time; returns True if copied."""
    target.parent.mkdir(parents=True, exist_ok=True)
    if target.exists():
        s, t = source.stat(), target.stat()
        if s.st_size == t.st_size and int(s.st_mtime) == int(t.st_mtime):
            return False
    shutil.copy2(source, target)
    return True


def place_iso(source, target, copy):
    target.parent.mkdir(parents=True, exist_ok=True)
    if target.exists():
        if same_file(source, target):
            return "hard link (unchanged)"
        s, t = source.stat(), target.stat()
        if s.st_size == t.st_size and int(s.st_mtime) == int(t.st_mtime):
            return "copy (unchanged)"
        target.unlink()
    if not copy:
        try:
            os.link(source, target)
            return "hard link"
        except OSError as error:
            print(f"package: cannot hard-link the disc image ({error}); copying it")
    shutil.copy2(source, target)
    return "copy"


def is_clang_build(build_dir):
    cache = (build_dir / "CMakeCache.txt").read_text(encoding="utf-8", errors="replace")
    return any(line.startswith("CMAKE_CXX_COMPILER:") and "clang" in line for line in cache.splitlines())


def build_release(build_dir):
    wrapper = "tools/port/pk_cmake_mingw.cmd" if is_clang_build(build_dir) else "tools/port/pk_cmake.cmd"
    command = ["cmd", "/c", str(ROOT / wrapper), "--build", str(build_dir), "-j4"]
    print("package: building", build_dir)
    result = subprocess.run(command, cwd=ROOT)
    if result.returncode != 0:
        raise SystemExit(f"package: the release build failed ({result.returncode})")


def check_release_build(build_dir):
    cache = build_dir / "CMakeCache.txt"
    if not cache.exists():
        raise SystemExit(f"package: {build_dir} is not configured (see docs/port/steam-deck.md)")
    if "PORTABLEKIT_RELEASE:BOOL=ON" not in cache.read_text(encoding="utf-8", errors="replace"):
        raise SystemExit(f"package: {build_dir} is not a release build (PORTABLEKIT_RELEASE=ON)")


def assemble(build_dir, out_dir, game_dir, copy_iso):
    bin_dir = build_dir / "bin"
    exe = bin_dir / EXE_NAME
    if not exe.exists():
        raise SystemExit(f"package: not built: {exe}")
    overlays = sorted((bin_dir / "overlays").glob("*.dll"))
    if not overlays:
        raise SystemExit(f"package: no overlay libraries in {bin_dir / 'overlays'}; the build has no corpus")
    package = out_dir / PACKAGE_NAME
    data = package / "data"
    for name in ("ms0", "mods"):
        if (data / name).exists():
            raise SystemExit(f"package: {data / name} exists (a run from the package folder?); "
                             "move it out first, the package must hold no saves or mods")

    wanted = {}  # path relative to the package -> source
    wanted[EXE_NAME] = exe
    for dll in sorted(bin_dir.glob("*.dll")):
        wanted[dll.name] = dll
    for dll in overlays:
        wanted[f"overlays/{dll.name}"] = dll
    for name in ("FFmpeg-LICENSE.txt", "FFmpeg-SOURCE.txt"):
        if (bin_dir / name).exists():
            wanted[f"licenses/{name}"] = bin_dir / name
    for name, source in LICENSES.items():
        if source.exists():
            wanted[f"licenses/{name}"] = source
    if ICON.exists():
        wanted["dissidia012.ico"] = ICON

    eboot = game_dir / "EBOOT.ELF"
    iso = game_dir / "disc.iso"
    for path in (eboot, iso):
        if not path.is_file():
            raise SystemExit(f"package: missing {path} (run tools/port/prepare_game_dir.py)")
    if sha256_file(eboot) != EBOOT_SHA256:
        raise SystemExit(f"package: {eboot} is not the executable the profile knows")

    copied = 0
    for relative, source in wanted.items():
        copied += copy_if_changed(source, package / relative)
    copied += copy_if_changed(eboot, package / "game/EBOOT.ELF")
    iso_mode = place_iso(iso, package / "game/disc.iso", copy_iso)

    # Program files the build no longer makes. game/ and data/ are left alone.
    removed = []
    for path in sorted(package.rglob("*")):
        if not path.is_file():
            continue
        relative = path.relative_to(package).as_posix()
        if relative.startswith(("game/", "data/")) or relative.startswith("."):
            continue
        if relative not in wanted:
            path.unlink()
            removed.append(relative)

    settings = data / "settings.ini"
    data.mkdir(parents=True, exist_ok=True)
    wrote_settings = not settings.exists()
    if wrote_settings:
        settings.write_text(DECK_SETTINGS, encoding="utf-8", newline="\n")

    total = sum(p.stat().st_size for p in package.rglob("*") if p.is_file())
    print(f"package: {package}")
    print(f"package: {len(wanted)} program files ({copied} copied), {len(overlays)} overlays, "
          f"EBOOT.ELF, disc.iso ({iso_mode}), {total / (1 << 30):.2f} GiB in all")
    if removed:
        print("package: removed", ", ".join(removed))
    print("package: data/settings.ini", "written (Deck defaults)" if wrote_settings else "kept as it is")
    return package


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("--build", action="store_true", help="build the release target first")
    parser.add_argument("--build-dir", type=Path, default=BUILD_DIR)
    parser.add_argument("--out", type=Path, default=OUT_DIR)
    parser.add_argument("--game-dir", type=Path, default=GAME_DIR)
    parser.add_argument("--copy-iso", action="store_true", help="copy the disc image instead of hard-linking it")
    options = parser.parse_args()
    build_dir = options.build_dir if options.build_dir.is_absolute() else ROOT / options.build_dir
    out_dir = options.out if options.out.is_absolute() else ROOT / options.out
    game_dir = options.game_dir if options.game_dir.is_absolute() else ROOT / options.game_dir
    check_release_build(build_dir)
    if options.build:
        build_release(build_dir)
    assemble(build_dir, out_dir, game_dir, options.copy_iso)
    return 0


if __name__ == "__main__":
    sys.exit(main())

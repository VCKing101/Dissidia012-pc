"""Builds the Windows installer, build/port/installer/dist/Dissidia012-PC-Setup-<version>.exe.

    python tools/port/package_installer.py --version 0.3.0 [--portablekit DIR] [--no-build]

1. Builds the installer build (port/README.md, "Installer build") in tools/local/port/build-d012-installer with
   llvm-mingw (tools/port/pk_cmake_mingw.cmd): the program without the game's code, the Battle BGM Maker
   (BgmMaker.exe), the recompiler and sdk/; and, when it is not built yet, atracdenc, the ATRAC3 encoder the BGM
   Maker's "Make a PSP / PPSSPP ISO" runs (tools/port/build_atracdenc.cmd).
2. Stages build/port/installer/stage: the programs, their DLLs, psp_recomp.exe, sdk/, llvm-mingw trimmed to what
   compiling the game's code needs (as PortableKit's scripts/package_desktop_windows.ps1 trims it), atracdenc.exe,
   README.txt, LICENSE.txt and licenses/ (with atracdenc's source, as its LGPL asks).
3. Checks the staged folder and refuses to package when it finds:
   - a file named like the game's (an executable, disc image, overlay, PRX, PARAM.SFO, save, generated code);
   - a file that is one by its contents (a PSP ELF, a PBP, an encrypted ~PSP executable, a PSF, an ISO 9660
     image);
   - a program bigger than one without the game's code, or naming a recompiled unit;
   - a path of this machine (the checkout, the user's home folder) in the port's own files.
4. Compiles tools/port/installer/dissidia012.iss with Inno Setup (tools/local/innosetup/ISCC.exe) and writes
   SHA256SUMS beside the setup.

Nothing of the game is ever needed or read: the setup is made from the build and the repository alone.
"""

import argparse
import hashlib
import io
import os
import re
import shutil
import subprocess
import sys
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
BUILD = ROOT / "tools" / "local" / "port" / "build-d012-installer"
LLVM = ROOT / "tools" / "local" / "llvm-mingw-20260922-ucrt-x86_64"
ISCC = ROOT / "tools" / "local" / "innosetup" / "ISCC.exe"
WRAPPER = ROOT / "tools" / "port" / "pk_cmake_mingw.cmd"
INSTALLER = ROOT / "tools" / "port" / "installer"
OUT = ROOT / "build" / "port" / "installer"
STAGE = OUT / "stage"
DIST = OUT / "dist"
SDL3_LICENSE = ROOT / "tools" / "local" / "port-deps" / "sdl3" / "SDL3-3.4.18" / "LICENSE.txt"
# atracdenc (LGPL-2.1, with libgha: BSD-2, and KISS FFT: BSD-3), built from its source at the commit
# build_atracdenc.cmd names; shipped unmodified beside BgmMaker.exe, which runs it as a program of its own.
ATRACDENC_SCRIPT = ROOT / "tools" / "port" / "build_atracdenc.cmd"
ATRACDENC_SRC = ROOT / "build" / "external" / "atracdenc-src" / "atracdenc"
ATRACDENC_EXE = ROOT / "tools" / "local" / "port" / "build-atracdenc" / "src" / "atracdenc.exe"
ATRACDENC_URL = "https://github.com/dcherednik/atracdenc"

PROGRAM_FILES = ["D012Native.exe", "BgmMaker.exe", "psp_recomp.exe", "SDL3.dll", "avcodec-61.dll",
                 "avformat-61.dll", "avutil-59.dll", "swresample-5.dll", "libc++.dll", "libunwind.dll"]
TOOLCHAIN_BIN = ["clang.exe", "clang++.exe", "clang-target-wrapper.exe", "ld.lld.exe", "x86_64-w64-mingw32-clang.exe",
                 "x86_64-w64-mingw32-clang++.exe", "libc++.dll", "libunwind.dll", "libwinpthread-1.dll",
                 "libclang-cpp.dll", "mingw32-common.cfg", "x86_64-w64-windows-gnu.cfg"]
# A program without the game's code is about 8 MB; with the main corpus linked in it was 120 MB.
MAX_PROGRAM_BYTES = 30 * 1024 * 1024
FORBIDDEN_NAMES = re.compile(r"(^eboot|\.iso$|\.cso$|\.zso$|\.pbp$|\.prx$|\.elf$|\.ovl$|^param\.sfo$|^data\.bin$|"
                             r"^umd_data|^generated_unit|^generated_registry|^ovl0[0-9a-f]{7}_|ulus10566|"
                             r"^game-code|^dissidia\.bin$)", re.I)


class PackagingError(Exception):
    pass


def run(args, **kwargs):
    print("  $ " + " ".join(str(a) for a in args), flush=True)
    result = subprocess.run([str(a) for a in args], **kwargs)
    if result.returncode != 0:
        raise PackagingError(f"{Path(str(args[0])).name} failed with exit code {result.returncode}")


def build(portablekit):
    if not (LLVM / "bin" / "clang++.exe").is_file():
        raise PackagingError(f"llvm-mingw 20260922 is not in {LLVM}")
    cache = BUILD / "CMakeCache.txt"
    if not cache.is_file():
        run(["cmd", "/c", WRAPPER, "-S", ROOT / "port", "-B", BUILD, "-DD012_INSTALLER_BUILD=ON",
             "-DPORTABLEKIT_RELEASE=ON", f"-DD012_PORTABLEKIT_DIR={portablekit.as_posix()}"])
    run(["cmd", "/c", WRAPPER, "--build", BUILD, "-j", str(max(2, (os.cpu_count() or 4) // 2))])
    build_atracdenc()


def atracdenc_commit():
    return re.search(r'^set "COMMIT=([0-9a-f]{40})"', ATRACDENC_SCRIPT.read_text(), re.M).group(1)


def git(folder, *args):
    result = subprocess.run(["git", "-C", str(folder), *args], capture_output=True)
    if result.returncode != 0:
        raise PackagingError(f"git {' '.join(args)} in {folder} failed: {result.stderr.decode(errors='replace')}")
    return result.stdout


def build_atracdenc():
    if not ATRACDENC_EXE.is_file():
        run(["cmd", "/c", ATRACDENC_SCRIPT])


def check_atracdenc():
    """The encoder is built, from the source checkout at the pinned commit, unmodified. Returns the commits of
    atracdenc and of its libgha submodule."""
    commit = atracdenc_commit()
    if not ATRACDENC_EXE.is_file():
        raise PackagingError(f"{ATRACDENC_EXE} is missing; run tools/port/build_atracdenc.cmd")
    head = git(ATRACDENC_SRC, "rev-parse", "HEAD").decode().strip()
    if head != commit:
        raise PackagingError(f"{ATRACDENC_SRC} is at {head}, not {commit}; run tools/port/build_atracdenc.cmd")
    if git(ATRACDENC_SRC, "status", "--porcelain", "--ignore-submodules=none").strip():
        raise PackagingError(f"{ATRACDENC_SRC} has local changes: the shipped encoder must be built from the "
                             "unmodified source")
    libgha = git(ATRACDENC_SRC / "src" / "lib" / "libgha", "rev-parse", "HEAD").decode().strip()
    return commit, libgha


def atracdenc_source(target, commit, libgha):
    """atracdenc's source as built, with its libgha submodule, in one zip (git archive of each)."""
    with zipfile.ZipFile(target, "w", zipfile.ZIP_DEFLATED) as out:
        for prefix, folder, revision in (("atracdenc/", ATRACDENC_SRC, commit),
                                         ("atracdenc/src/lib/libgha/", ATRACDENC_SRC / "src" / "lib" / "libgha",
                                          libgha)):
            data = git(folder, "archive", "--format=zip", f"--prefix={prefix}", revision)
            with zipfile.ZipFile(io.BytesIO(data)) as part:
                for entry in part.infolist():
                    if not entry.is_dir():
                        out.writestr(entry, part.read(entry))


def check_build():
    cache = (BUILD / "CMakeCache.txt").read_text(encoding="utf-8", errors="replace")
    for wanted in ("D012_INSTALLER_BUILD:BOOL=ON", "PORTABLEKIT_RELEASE:BOOL=ON"):
        if wanted not in cache:
            raise PackagingError(f"{BUILD} is not configured with {wanted}")
    # The cache may name the compiler as found on PATH; CMake's compiler file has the full path.
    compilers = [m.group(1) for f in (BUILD / "CMakeFiles").glob("*/CMakeCXXCompiler.cmake")
                 for m in [re.search(r'set\(CMAKE_CXX_COMPILER "([^"]+)"', f.read_text(errors="replace"))] if m]
    if not compilers or not all("llvm-mingw" in c for c in compilers):
        raise PackagingError(f"{BUILD} is not built with llvm-mingw ({', '.join(compilers) or '?'})")
    portablekit = Path(re.search(r"^D012_PORTABLEKIT_DIR:\w+=(.+)$", cache, re.M).group(1))
    return portablekit


def copy(source, target):
    target.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, target)


def stage(portablekit, atracdenc):
    if STAGE.exists():
        shutil.rmtree(STAGE)
    STAGE.mkdir(parents=True)
    bin_dir = BUILD / "bin"
    for name in PROGRAM_FILES:
        if not (bin_dir / name).is_file():
            raise PackagingError(f"{bin_dir / name} is missing; build the installer build first")
        copy(bin_dir / name, STAGE / name)
    shutil.copytree(bin_dir / "sdk", STAGE / "sdk")

    # The toolchain, cut to what compiling the game's code for x86_64 needs.
    tool = STAGE / "toolchain"
    for name in TOOLCHAIN_BIN:
        if not (LLVM / "bin" / name).is_file():
            raise PackagingError(f"{LLVM / 'bin' / name} is missing from the toolchain")
        copy(LLVM / "bin" / name, tool / "bin" / name)
    versioned = [p for p in (LLVM / "bin").iterdir() if re.fullmatch(r"clang-\d+\.exe|libLLVM-\d+\.dll", p.name)]
    if len(versioned) < 2:
        raise PackagingError(f"no clang-<version>.exe and libLLVM-<version>.dll in {LLVM / 'bin'}")
    for path in versioned:
        copy(path, tool / "bin" / path.name)
    copy(LLVM / "LICENSE.TXT", tool / "LICENSE.TXT")
    shutil.copytree(LLVM / "include", tool / "include")
    shutil.copytree(LLVM / "lib" / "clang", tool / "lib" / "clang")
    # Of clang's runtime libraries, only Windows x86_64's.
    runtimes = [d for version in (tool / "lib" / "clang").iterdir() if (version / "lib").is_dir()
                for d in (version / "lib").iterdir() if d.is_dir()]
    for runtime in runtimes:
        if "x86_64" in runtime.name and "windows" in runtime.name:
            continue
        if runtime.name != "windows":
            shutil.rmtree(runtime)
            continue
        for path in runtime.iterdir():
            if path.is_file() and "x86_64" not in path.name:
                path.unlink()
    if not list((tool / "lib" / "clang").rglob("libclang_rt.builtins*x86_64*")):
        raise PackagingError("the trimmed toolchain lost clang's x86_64 runtime")
    shutil.copytree(LLVM / "x86_64-w64-mingw32", tool / "x86_64-w64-mingw32")

    # The Battle BGM Maker's encoder, beside it.
    copy(ATRACDENC_EXE, STAGE / "atracdenc.exe")

    copy(INSTALLER / "README.txt", STAGE / "README.txt")
    copy(ROOT / "LICENSE", STAGE / "LICENSE.txt")
    licenses = STAGE / "licenses"
    copy(INSTALLER / "THIRD_PARTY_NOTICES.md", licenses / "THIRD_PARTY_NOTICES.md")
    copy(portablekit / "LICENSE", licenses / "PortableKit-LICENSE.txt")
    copy(SDL3_LICENSE, licenses / "SDL3-LICENSE.txt")
    for name in ("FFmpeg-LICENSE.txt", "FFmpeg-SOURCE.txt"):
        copy(bin_dir / name, licenses / name)
    copy(portablekit / "third_party" / "imgui" / "LICENSE.txt", licenses / "DearImGui-LICENSE.txt")
    copy(portablekit / "third_party" / "tiny_aes" / "UNLICENSE", licenses / "tiny-AES-c-UNLICENSE.txt")
    copy(portablekit / "third_party" / "xxhash" / "LICENSE", licenses / "xxHash-LICENSE.txt")
    copy(LLVM / "LICENSE.TXT", licenses / "llvm-mingw-LICENSE.txt")
    commit, libgha = atracdenc
    copy(ATRACDENC_SRC / "LICENSE", licenses / "atracdenc-LICENSE.txt")
    copy(ATRACDENC_SRC / "src" / "lib" / "libgha" / "LICENSE", licenses / "libgha-LICENSE.txt")
    copy(ATRACDENC_SRC / "src" / "lib" / "libgha" / "src" / "3rd" / "kissfft" / "LICENSES" / "BSD-3-Clause",
         licenses / "KISS-FFT-LICENSE.txt")
    atracdenc_source(licenses / "atracdenc-source.zip", commit, libgha)
    (licenses / "atracdenc-SOURCE.txt").write_text(
        "atracdenc.exe is atracdenc, an ATRAC encoder by Daniil Cherednik, licensed under the\n"
        "GNU LGPL version 2.1 (atracdenc-LICENSE.txt), unmodified. Its libgha (BSD 2-Clause,\n"
        "libgha-LICENSE.txt) and the KISS FFT code in both (copyright Mark Borgerding, BSD 3-Clause,\n"
        "KISS-FFT-LICENSE.txt) are compiled into it. The Battle BGM Maker runs it as a program of its\n"
        "own for \"Make a PSP / PPSSPP ISO\"; you may replace it with your own build.\n\n"
        f"Source:  {ATRACDENC_URL}, commit {commit}\n"
        f"         (its libgha submodule at commit {libgha}),\n"
        "         and the same source in atracdenc-source.zip beside this file.\n"
        "Built:   with MSVC and the CMake options -DATRACDENC_PCM_IO_BACKEND=mediafoundation\n"
        "         -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded (tools/port/build_atracdenc.cmd in\n"
        "         https://github.com/VCKing101/Dissidia012-pc).\n", encoding="ascii", newline="\r\n")
    notices = LLVM / "x86_64-w64-mingw32" / "share" / "mingw32"
    if notices.is_dir():
        for path in notices.glob("COPYING*"):
            copy(path, licenses / ("mingw-w64-" + path.name + ("" if path.suffix else ".txt")))


def is_game_content(head):
    """A file that is the game's, by its first bytes (and, for an ISO, its volume descriptor)."""
    if head[:4] == b"\x7fELF" and len(head) >= 20 and head[18:20] == b"\x08\x00":
        return "a PSP (MIPS) ELF"
    if head[:4] == b"\x00PBP":
        return "a PBP"
    if head[:4] == b"~PSP":
        return "an encrypted PSP executable"
    if head[:4] == b"\x00PSF":
        return "a PARAM.SFO"
    if len(head) > 32774 and head[32769:32774] == b"CD001":
        return "an ISO 9660 image"
    return None


def check_stage(portablekit):
    problems = []
    machine_paths = set()
    # The checkout and the folders the build read, as they are on this machine (tools/local may be a link).
    for base in {ROOT, ROOT.resolve(), Path.home(), BUILD.resolve(), portablekit, portablekit.resolve()}:
        for form in (str(base), base.as_posix()):
            machine_paths.update({form.encode(), form.lower().encode(), form.encode("utf-16-le")})
    program_bytes = 0
    for path in sorted(STAGE.rglob("*")):
        if not path.is_file():
            continue
        relative = path.relative_to(STAGE)
        if FORBIDDEN_NAMES.search(path.name):
            problems.append(f"{relative}: named like a file of the game")
        with open(path, "rb") as stream:
            head = stream.read(40000)
        found = is_game_content(head)
        if found:
            problems.append(f"{relative}: {found}")
        if relative.parts[0] == "toolchain":
            continue  # llvm-mingw's own release files
        data = path.read_bytes()
        for needle in machine_paths:
            if needle in data:
                problems.append(f"{relative}: contains a path of this machine ({needle[:60]!r})")
                break
        if path.name == "D012Native.exe":
            program_bytes = len(data)
            if b"recomp_unit_0" in data or b"register_recomp_unit" in data:
                problems.append(f"{relative}: names recompiled units: the game's code is linked in")
    if program_bytes == 0:
        problems.append("D012Native.exe is missing")
    elif program_bytes > MAX_PROGRAM_BYTES:
        problems.append(f"D012Native.exe is {program_bytes:,} bytes: more than a program without the game's code")
    if problems:
        raise PackagingError("the staged folder is not fit to ship:\n  " + "\n  ".join(problems))
    total = sum(p.stat().st_size for p in STAGE.rglob("*") if p.is_file())
    print(f"  checked {sum(1 for p in STAGE.rglob('*') if p.is_file())} files, {total / 2**20:.0f} MB: "
          "no game files, no paths of this machine, a program without the game's code")


def compile_setup(version):
    if not ISCC.is_file():
        raise PackagingError(f"Inno Setup's compiler is not at {ISCC}")
    DIST.mkdir(parents=True, exist_ok=True)
    run([ISCC, f"/DAppVersion={version}", f"/DStage={STAGE}", f"/DOutputDir={DIST}", "/Q",
         INSTALLER / "dissidia012.iss"])
    setup = DIST / f"Dissidia012-PC-Setup-{version}.exe"
    digest = hashlib.sha256(setup.read_bytes()).hexdigest()
    (DIST / "SHA256SUMS").write_text(f"{digest}  {setup.name}\n", encoding="ascii", newline="\n")
    print(f"  {setup} ({setup.stat().st_size / 2**20:.0f} MB), SHA-256 {digest}")


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("--version", required=True, help="the release's version, e.g. 0.3.0")
    parser.add_argument("--portablekit", type=Path, default=ROOT / "tools" / "local" / "port" / "PortableKit",
                        help="the PortableKit checkout of a first configure (later: the build's own)")
    parser.add_argument("--no-build", action="store_true", help="package the build as it is")
    options = parser.parse_args()
    if not re.fullmatch(r"\d+\.\d+\.\d+", options.version):
        parser.error("--version must look like 0.3.0")
    try:
        print("1. building the installer build")
        if not options.no_build:
            build(options.portablekit)
        portablekit = check_build()
        atracdenc = check_atracdenc()
        print("2. staging")
        stage(portablekit, atracdenc)
        print("3. checking")
        check_stage(portablekit)
        print("4. compiling the setup")
        compile_setup(options.version)
    except PackagingError as error:
        print(f"package_installer: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())

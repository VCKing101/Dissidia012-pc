"""The recompiled build, as in port/README.md ("Recompiled build"):

    0. only when the decrypted EBOOT is not there yet: configure and build D012Native with no game
       code (bootstrap()), which setup runs as `D012Native --install` to decrypt it from the ISO;
    1. configure tools/local/port/build-d012-recomp (D012_WORK_DIR=build/port/recomp,
       PSPRECOMP_GENERATED_JOBS = one per 4 GiB of memory, PortableKit's own rule);
    2. build psp_recomp;
    3. generate the main corpus (with the main rows of port/config/dispatch-calls.txt) and the overlay
       and PRX corpora (tools/port/overlay_meta.py --recompile, which passes the overlay rows);
    4. build everything (D012Native and the Battle BGM Maker, BgmMaker.exe);
    5. build atracdenc, the encoder of the BGM Maker's "Make a PSP / PPSSPP ISO", from its source
       (tools/port/build_atracdenc.cmd); optional: setup goes on without it.

The corpora are generated into a staging folder and only files whose bytes changed are moved into
build/port/recomp, so regenerating with the same result recompiles nothing. They are regenerated only
when psp_recomp, the EBOOT, the overlays, dispatch-calls.txt or overlay_meta.py changed.
"""

import ctypes
import filecmp
import hashlib
import os
import re
import shutil
import subprocess
import sys
import time
from pathlib import Path

from ui import PK_CMAKE, ROOT, SetupError, duration, info, run, run_vs, warn

sys.path.insert(0, str(ROOT / "tools" / "port"))
import overlay_meta  # noqa: E402

PK_DIR = ROOT / "tools" / "local" / "port" / "PortableKit"
BUILD = ROOT / "tools" / "local" / "port" / "build-d012-recomp"
RELEASE = ROOT / "tools" / "local" / "port" / "build-d012-release"
WORK = ROOT / "build" / "port" / "recomp"
STAGING = ROOT / "build" / "port" / "recomp-staging"  # outside WORK, where the build's globs look
EBOOT = ROOT / "build" / "port" / "game" / "EBOOT.ELF"
LIBRARIES = [ROOT / "build/port/ovl/OVL_MENU_APP.ELF", ROOT / "build/port/ovl/OVL_BATTLE_APP.ELF",
             ROOT / "build/port/ovl/OVL_EXTRA_APP.ELF", ROOT / "build/port/prx/LIBSUPPREACC.PRX"]
MAIN_BASE = "0x08804000"
ATRACDENC_SCRIPT = ROOT / "tools" / "port" / "build_atracdenc.cmd"
ATRACDENC = ROOT / "tools" / "local" / "port" / "build-atracdenc" / "src" / "atracdenc.exe"


class MemoryStatus(ctypes.Structure):
    _fields_ = [("dwLength", ctypes.c_ulong), ("dwMemoryLoad", ctypes.c_ulong),
                ("ullTotalPhys", ctypes.c_ulonglong), ("ullAvailPhys", ctypes.c_ulonglong),
                ("ullTotalPageFile", ctypes.c_ulonglong), ("ullAvailPageFile", ctypes.c_ulonglong),
                ("ullTotalVirtual", ctypes.c_ulonglong), ("ullAvailVirtual", ctypes.c_ulonglong),
                ("ullAvailExtendedVirtual", ctypes.c_ulonglong)]


def memory_gib():
    status = MemoryStatus()
    status.dwLength = ctypes.sizeof(MemoryStatus)
    if not ctypes.windll.kernel32.GlobalMemoryStatusEx(ctypes.byref(status)):
        return 8.0
    return status.ullTotalPhys / (1 << 30)


def jobs():
    """(generated-code jobs, Ninja jobs)."""
    cpus = os.cpu_count() or 4
    return max(1, min(cpus, int(memory_gib() // 4))), cpus


def posix(path):
    return str(path).replace("\\", "/")


def cache_values(build_dir):
    values = {}
    cache = build_dir / "CMakeCache.txt"
    if cache.is_file():
        for line in cache.read_text(encoding="utf-8", errors="replace").splitlines():
            name, sep, value = line.partition("=")
            if sep and ":" in name and not line.startswith(("//", "#")):
                values[name.split(":", 1)[0]] = value
    return values


def configure(build_dir, generated_jobs, release=False):
    wanted = {"D012_PORTABLEKIT_DIR": posix(PK_DIR), "D012_WORK_DIR": posix(WORK),
              "PSPRECOMP_GENERATED_JOBS": str(generated_jobs), "CMAKE_GENERATOR": "Ninja"}
    if release:
        wanted["PORTABLEKIT_RELEASE"] = "ON"
    values = cache_values(build_dir)
    if (build_dir / "build.ninja").is_file() and all(
            values.get(k, "").replace("\\", "/").rstrip("/").lower() == v.rstrip("/").lower()
            for k, v in wanted.items()):
        info(f"{build_dir.relative_to(ROOT)}: already configured")
        return
    info(f"configuring {build_dir.relative_to(ROOT)} (the first time about 1-2 minutes: "
         "PortableKit fetches and unpacks its FFmpeg build)")
    started = time.monotonic()
    args = ["-S", posix(ROOT / "port"), "-B", posix(build_dir), "-G", "Ninja",
            f"-DD012_PORTABLEKIT_DIR={posix(PK_DIR)}", f"-DD012_WORK_DIR={posix(WORK)}",
            f"-DPSPRECOMP_GENERATED_JOBS={generated_jobs}"]
    if release:
        args.append("-DPORTABLEKIT_RELEASE=ON")
    log = "setup-configure-release.log" if release else "setup-configure.log"
    _code, last = run_vs(PK_CMAKE, args, log)
    text = (ROOT / "build" / "port" / "logs" / log).read_text(encoding="utf-8", errors="replace")
    if "Vulkan renderer enabled" not in text:
        raise SetupError(f"the configure did not report \"D012Native: Vulkan renderer enabled\" "
                         f"(see build/port/logs/{log})")
    info(f"configured in {duration(time.monotonic() - started)} (Vulkan renderer enabled)")


def build(build_dir, ninja_jobs, label, log, target=None):
    args = ["--build", posix(build_dir), "-j", str(ninja_jobs)]
    if target:
        args += ["--target", target]
    started = time.monotonic()
    _code, last = run_vs(PK_CMAKE, args, log, progress=label)
    if any("ninja: no work to do" in line for line in last):
        info(f"{label}: nothing to do, already up to date")
    else:
        info(f"{label}: done in {duration(time.monotonic() - started)}")


def sha256_file(path):
    digest = hashlib.sha256()
    with open(path, "rb") as stream:
        for block in iter(lambda: stream.read(1 << 20), b""):
            digest.update(block)
    return digest.hexdigest()


def corpus_digest(recompiler):
    digest = hashlib.sha256()
    for path in [recompiler, EBOOT, *LIBRARIES, ROOT / "port/config/dispatch-calls.txt",
                 ROOT / "tools/port/overlay_meta.py"]:
        digest.update(path.name.encode() + b"\0" + sha256_file(path).encode() + b"\n")
    return digest.hexdigest()


def corpus_present():
    generated = WORK / "generated"
    overlays = WORK / "overlays"
    return (generated.is_dir() and any(generated.glob("*.cpp"))
            and overlays.is_dir() and len(list(overlays.glob("*/meta.txt"))) == len(LIBRARIES))


def sync(source, target):
    """Make target hold source's files, touching only those whose bytes differ. Returns (changed, removed)."""
    changed = removed = 0
    wanted = set()
    for path in source.rglob("*"):
        if not path.is_file():
            continue
        relative = path.relative_to(source)
        wanted.add(relative)
        destination = target / relative
        if destination.is_file() and destination.stat().st_size == path.stat().st_size and \
                filecmp.cmp(path, destination, shallow=False):
            continue
        destination.parent.mkdir(parents=True, exist_ok=True)
        os.replace(path, destination)
        changed += 1
    if target.is_dir():
        for path in sorted(target.rglob("*"), reverse=True):
            if path.is_file() and path.relative_to(target) not in wanted:
                path.unlink()
                removed += 1
            elif path.is_dir() and not any(path.iterdir()):
                path.rmdir()
    return changed, removed


def generate(state, force=False):
    recompiler = BUILD / "portablekit" / "psp_recomp.exe"
    if not recompiler.is_file():
        raise SetupError(f"psp_recomp was not built: {recompiler}")
    digest = corpus_digest(recompiler)
    if not force and state.get("corpus") == digest and corpus_present():
        info("corpora: up to date (same psp_recomp, EBOOT, overlays and dispatch calls)")
        return
    started = time.monotonic()
    info("generating the C++ corpora from the EBOOT, the three overlays and the disc PRX (about a minute)")
    if STAGING.exists():
        shutil.rmtree(STAGING)
    STAGING.mkdir(parents=True)
    main_args = [recompiler, EBOOT, "--auto", STAGING / "generated", MAIN_BASE] + overlay_meta.dispatch_calls("main")
    run(main_args, "setup-corpus-main.log")
    run([sys.executable, ROOT / "tools/port/overlay_meta.py", "--recompile", "--recompiler", recompiler,
         "--out", STAGING / "overlays", *LIBRARIES], "setup-corpus-overlays.log")
    changed = removed = 0
    for name in ("generated", "overlays"):
        c, r = sync(STAGING / name, WORK / name)
        changed, removed = changed + c, removed + r
    shutil.rmtree(STAGING, ignore_errors=True)
    state["corpus"] = digest
    info(f"corpora generated in {duration(time.monotonic() - started)}: {changed} files new or changed, "
         f"{removed} removed")


def check_outputs(build_dir):
    exe = build_dir / "bin" / "D012Native.exe"
    overlays = list((build_dir / "bin" / "overlays").glob("*.dll"))
    if not exe.is_file():
        raise SetupError(f"the build finished but {exe} is missing")
    if len(overlays) < len(LIBRARIES):
        raise SetupError(f"the build has {len(overlays)} overlay libraries in {build_dir / 'bin/overlays'}, "
                         f"expected {len(LIBRARIES)}")
    return exe


def bootstrap():
    """The program with no game code yet (PortableKit's bootstrap stage), to decrypt the EBOOT with
    --install. It compiles only the host code, which the full build then reuses, and links PortableKit's
    empty corpus stub while build/port/recomp/generated has no units. Returns the program's path."""
    generated_jobs, ninja_jobs = jobs()
    configure(BUILD, generated_jobs)
    info("compiling the program without the game code first (it decrypts the EBOOT from your ISO); "
         "the full build reuses this")
    build(BUILD, ninja_jobs, "bootstrap", "setup-build-bootstrap.log", target="D012Native")
    exe = BUILD / "bin" / "D012Native.exe"
    if not exe.is_file():
        raise SetupError(f"the bootstrap build finished but {exe} is missing")
    return exe


def ensure(state, force_corpus=False):
    generated_jobs, ninja_jobs = jobs()
    info(f"{os.cpu_count()} logical CPUs, {memory_gib():.0f} GiB of memory: {generated_jobs} generated units "
         f"at a time (one per 4 GiB), {ninja_jobs} jobs otherwise")
    configure(BUILD, generated_jobs)
    build(BUILD, ninja_jobs, "psp_recomp", "setup-build-psp_recomp.log", target="psp_recomp")
    generate(state, force_corpus)
    if not state.get("built"):
        info("compiling the port. The first time this is the long part: about 5 minutes on a recent 8-core PC, "
             "up to 30 minutes or more on 4 cores and 8 GB. Later runs rebuild only what changed.")
    build(BUILD, ninja_jobs, "build", "setup-build.log")
    return check_outputs(BUILD)


def ensure_encoder(state):
    commit = re.search(r'^set "COMMIT=([0-9a-f]{40})"', ATRACDENC_SCRIPT.read_text(), re.M).group(1)
    if ATRACDENC.is_file() and state.get("atracdenc") == commit:
        info("atracdenc (the encoder for the Battle BGM Maker's PSP / PPSSPP ISO): up to date")
        return
    info("building atracdenc, the encoder for the Battle BGM Maker's PSP / PPSSPP ISO "
         "(the first time it clones its source from GitHub)")
    started = time.monotonic()
    code, _last = run_vs(ATRACDENC_SCRIPT, [], "setup-build-atracdenc.log", check=False)
    if code == 0 and ATRACDENC.is_file():
        state["atracdenc"] = commit
        info(f"atracdenc: done in {duration(time.monotonic() - started)}")
    else:
        warn("atracdenc could not be built (build/port/logs/setup-build-atracdenc.log). The port and the Battle "
             "BGM Maker work without it; only its \"Make a PSP / PPSSPP ISO\" needs it. Run setup again to retry.")


def ensure_release(state):
    generated_jobs, ninja_jobs = jobs()
    configure(RELEASE, generated_jobs, release=True)
    info("compiling the release build for the Steam Deck (the first time as long as the main build)")
    build(RELEASE, ninja_jobs, "release build", "setup-build-release.log")
    check_outputs(RELEASE)
    run([sys.executable, ROOT / "tools/port/package_deck.py", "--build-dir", RELEASE], "setup-package-deck.log")
    log = (ROOT / "build/port/logs/setup-package-deck.log").read_text(encoding="utf-8", errors="replace")
    for line in log.splitlines():
        if line.startswith("package:"):
            info(line)

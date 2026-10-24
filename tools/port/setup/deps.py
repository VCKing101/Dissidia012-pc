"""Build dependencies: SDL3, glslang and the Vulkan headers, downloaded and SHA-256 checked into
tools/local/port-deps in the layout tools/port/pk_cmake.cmd expects, and vulkan-1.lib made from the
driver's System32\\vulkan-1.dll (so no Vulkan SDK and no administrator rights are needed).

    tools/local/port-deps/dl/<sub>/<zip>     the downloads, kept for re-runs
    tools/local/port-deps/sdl3/SDL3-3.4.18/  SDL3's Visual C++ development package
    tools/local/port-deps/glslang/bin/       glslang.exe
    tools/local/port-deps/vkheaders/Vulkan-Headers-vulkan-sdk-1.4.363.0/include
    tools/local/port-deps/vulkan-lib/vulkan-1.lib
    tools/local/port-deps/dl/ffmpeg/         PortableKit's own configure fetches FFmpeg here, checking its pinned hash
"""

import hashlib
import os
import re
import shutil
import sys
import urllib.request
import zipfile
from pathlib import Path

from ui import ROOT, VSENV, SetupError, info, run_vs, warn

DEPS_DIR = ROOT / "tools" / "local" / "port-deps"
DL_DIR = DEPS_DIR / "dl"
MARKER = ".d012-source-sha256"

DEPENDENCIES = [
    {
        "name": "SDL3 3.4.18",
        "sub": "sdl3",
        "file": "SDL3-devel-3.4.18-VC.zip",
        "url": "https://github.com/libsdl-org/SDL/releases/download/release-3.4.18/SDL3-devel-3.4.18-VC.zip",
        "sha256": "78d84602ae616cfe26b33a73b7d3b9b6b56e8a6650e53d92c9c6ca938f747acd",
        "check": "SDL3-3.4.18/cmake/SDL3Config.cmake",
    },
    {
        "name": "glslang 16.6.0",
        "sub": "glslang",
        "file": "glslang-16.6.0-windows-x86_64-release.zip",
        "url": "https://github.com/KhronosGroup/glslang/releases/download/16.6.0/"
               "glslang-16.6.0-windows-x86_64-release.zip",
        "sha256": "82bf434e69b9bb4829de7e2b4bc2c5e7a7861e53d66cf75e5cc70f5f694a8d9b",
        "check": "bin/glslang.exe",
    },
    {
        # A source archive GitHub makes from the tag (commit 6802bb4733b63ed5efd3adb308a6c885ef180ea1).
        # GitHub does not promise such archives stay byte-identical, so when the zip's own hash differs,
        # the hash of its contents (every file's path and SHA-256) is checked instead.
        "name": "Vulkan-Headers vulkan-sdk-1.4.363.0",
        "sub": "vkheaders",
        "file": "Vulkan-Headers-vulkan-sdk-1.4.363.0.zip",
        "url": "https://github.com/KhronosGroup/Vulkan-Headers/archive/refs/tags/vulkan-sdk-1.4.363.0.zip",
        "sha256": "f4be95220ff0ee0b1c620301ff4fe9ae48592d71f4c30f2d5c571351a153ae81",
        "content_sha256": "a32e435c1d3b3155da7bd05c713bd82efacc8c5269fb38213a53aa4918efb163",
        "check": "Vulkan-Headers-vulkan-sdk-1.4.363.0/include/vulkan/vulkan.h",
    },
]
VULKAN_DLL = Path(os.environ.get("SystemRoot", r"C:\Windows")) / "System32" / "vulkan-1.dll"


def sha256_file(path):
    digest = hashlib.sha256()
    with open(path, "rb") as stream:
        for block in iter(lambda: stream.read(1 << 20), b""):
            digest.update(block)
    return digest.hexdigest()


def zip_content_sha256(path):
    """SHA-256 over every file's "path NUL sha256 LF", sorted by path: the same for any re-packing."""
    digest = hashlib.sha256()
    with zipfile.ZipFile(path) as archive:
        names = sorted(item.filename for item in archive.infolist() if not item.is_dir())
        for name in names:
            digest.update(name.encode("utf-8") + b"\0" + hashlib.sha256(archive.read(name)).hexdigest().encode()
                          + b"\n")
    return digest.hexdigest()


def verify(dep, path):
    """None when the file is the expected one, else the reason it is not."""
    actual = sha256_file(path)
    if actual == dep["sha256"]:
        return None
    if dep.get("content_sha256"):
        try:
            if zip_content_sha256(path) == dep["content_sha256"]:
                info(f"{dep['file']}: GitHub re-packed the archive (zip SHA-256 {actual[:16]}...), "
                     "but its contents match the pinned ones")
                return None
        except (zipfile.BadZipFile, OSError):
            pass
    return f"SHA-256 {actual}, expected {dep['sha256']}"


def download(url, target):
    request = urllib.request.Request(url, headers={"User-Agent": "dissidia012-port-setup"})
    partial = target.with_name(target.name + ".part")
    with urllib.request.urlopen(request, timeout=60) as response, open(partial, "wb") as out:
        total = int(response.headers.get("Content-Length") or 0)
        done = 0
        shown = -1
        while True:
            block = response.read(1 << 16)
            if not block:
                break
            out.write(block)
            done += len(block)
            if total and sys.stdout.isatty():
                tenth = done * 10 // total
                if tenth != shown:
                    shown = tenth
                    print(f"\r    downloading {target.name}: {done / 1e6:.1f} of {total / 1e6:.1f} MB", end="",
                          flush=True)
    if sys.stdout.isatty():
        print()
    info(f"downloaded {target.name} ({done / 1e6:.1f} MB)")
    os.replace(partial, target)


def reject(path, reason, source):
    bad = path.with_name(path.name + ".rejected")
    if bad.exists():
        bad.unlink()
    os.replace(path, bad)
    raise SetupError(
        f"{path.name} from {source} is not the expected file: {reason}.\n"
        f"    It was moved aside to {bad}. Nothing was extracted.\n"
        "    A damaged download: run setup again. If it fails again, the file changed upstream; "
        "please report it (do not use the file).")


def fetch(dep, cache):
    """The verified zip in tools/local/port-deps/dl/<sub>/."""
    target = DL_DIR / dep["sub"] / dep["file"]
    target.parent.mkdir(parents=True, exist_ok=True)
    if target.exists():
        reason = verify(dep, target)
        if reason is None:
            return target
        warn(f"{target} is damaged ({reason}); fetching it again")
        target.unlink()
    if cache:
        for candidate in (Path(cache) / dep["sub"] / dep["file"], Path(cache) / dep["file"]):
            if candidate.is_file():
                info(f"copying {dep['file']} from the cache {cache}")
                shutil.copyfile(candidate, target)
                reason = verify(dep, target)
                if reason:
                    reject(target, reason, f"the cache ({candidate})")
                return target
    info(f"downloading {dep['name']} from {dep['url']}")
    try:
        download(dep["url"], target)
    except OSError as error:
        raise SetupError(f"cannot download {dep['url']}: {error}\n"
                         f"    Check the internet connection, or put the file in {target.parent} by hand.")
    reason = verify(dep, target)
    if reason:
        reject(target, reason, dep["url"])
    return target


def safe_extract(archive_path, dest):
    dest.mkdir(parents=True, exist_ok=True)
    root = dest.resolve()
    with zipfile.ZipFile(archive_path) as archive:
        for item in archive.infolist():
            target = (dest / item.filename).resolve()
            if root != target and root not in target.parents:
                raise SetupError(f"{archive_path.name}: unsafe path in the archive: {item.filename}")
        archive.extractall(dest)


def extracted_matches(archive_path, dest):
    """True when every file of the zip is already in dest with the same bytes."""
    with zipfile.ZipFile(archive_path) as archive:
        for item in archive.infolist():
            if item.is_dir():
                continue
            target = dest / item.filename
            if not target.is_file() or target.stat().st_size != item.file_size:
                return False
            if archive.read(item.filename) != target.read_bytes():
                return False
    return True


def extract(dep, archive_path):
    dest = DEPS_DIR / dep["sub"]
    marker = dest / MARKER
    expected = sha256_file(archive_path)
    if (dest / dep["check"]).exists():
        if marker.exists() and marker.read_text().strip() == expected:
            return "already in place"
        if extracted_matches(archive_path, dest):
            marker.write_text(expected + "\n")
            return "already in place (checked file by file)"
    safe_extract(archive_path, dest)
    if not (dest / dep["check"]).exists():
        raise SetupError(f"{archive_path.name} did not contain {dep['check']}")
    marker.write_text(expected + "\n")
    return "extracted"


def seed_ffmpeg(cache):
    """With a cache, put PortableKit's FFmpeg build where its configure looks first (it checks the hash)."""
    if not cache:
        return
    target_dir = DL_DIR / "ffmpeg"
    for folder in (Path(cache) / "ffmpeg", Path(cache)):
        for candidate in sorted(folder.glob("ffmpeg-*.zip")) if folder.is_dir() else []:
            target = target_dir / candidate.name
            if not target.exists():
                target_dir.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(candidate, target)
                info(f"copied {candidate.name} from the cache (PortableKit's configure checks its hash)")
        if target_dir.is_dir() and any(target_dir.glob("ffmpeg-*.zip")):
            return


def make_vulkan_lib():
    """vulkan-1.lib from the driver's vulkan-1.dll: dumpbin /exports, then lib /def."""
    out = DEPS_DIR / "vulkan-lib"
    lib = out / "vulkan-1.lib"
    if lib.exists():
        return "already in place"
    if not VULKAN_DLL.exists():
        raise SetupError(f"{VULKAN_DLL} is missing: install or update your graphics driver (Vulkan support)")
    out.mkdir(parents=True, exist_ok=True)
    exports = out / "exports.txt"
    _code, _ = run_vs(VSENV, ["dumpbin", "/nologo", "/exports", str(VULKAN_DLL), f"/out:{exports}"],
                      "setup-vulkan-dumpbin.log")
    names = []
    for line in exports.read_text(encoding="utf-8", errors="replace").splitlines():
        match = re.match(r"^\s+\d+\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f]{8}\s+(vk\w+)", line)
        if match:
            names.append(match.group(1))
    if len(names) < 100 or "vkCreateInstance" not in names:
        raise SetupError(f"could not read the exports of {VULKAN_DLL} (found {len(names)}); see {exports}")
    definition = out / "vulkan-1.def"
    definition.write_text("LIBRARY vulkan-1\nEXPORTS\n" + "".join(f"    {name}\n" for name in names),
                          encoding="ascii")
    run_vs(VSENV, ["lib", "/nologo", f"/def:{definition}", "/machine:x64", f"/out:{lib}"], "setup-vulkan-lib.log")
    if not lib.exists():
        raise SetupError(f"lib.exe did not make {lib}")
    return f"made from {VULKAN_DLL} ({len(names)} functions)"


def ensure(cache=None):
    for dep in DEPENDENCIES:
        archive_path = fetch(dep, cache)
        info(f"{dep['name']}: verified, {extract(dep, archive_path)}")
    seed_ffmpeg(cache)
    info(f"vulkan-1.lib: {make_vulkan_lib()}")

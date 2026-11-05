#!/usr/bin/env bash
# Builds the Linux setup, Dissidia012-Setup-<version>-x86_64.AppImage, from WSL:
#
#   wsl -d Ubuntu-24.04 -- bash tools/port/linux_setup.sh [--version X.Y.Z] [--no-build] [--jobs N]
#
# The version is tools/port/release-version.txt's unless --version gives
# another, as for the Windows setup (tools/port/package_installer.py).
#
# 1. The installer build in the SDK container and its staged files
#    (tools/port/linux_build.sh --installer: ~/d012-linux/setup/payload),
#    unless --no-build.
# 2. The toolchain (tools/port/linux_toolchain.sh: ~/d012-linux/toolchain),
#    when it is not made yet.
# 3. The AppDir: tools/port/linux_setup/AppRun (the wizard), its desktop file
#    and icon, VERSION, and payload/ (what is installed) with toolchain/.
# 4. tools/port/linux_check_setup.py: no game files, no paths of this machine,
#    a program without the game's code.
# 5. appimagetool with the pinned type-2 runtime, and SHA256SUMS. The result
#    goes to ~/d012-linux/setup/dist and the main checkout's
#    build/port/installer-linux/dist; the release's unstripped programs, for
#    reading a player's crash report, to build/port/installer-linux/symbols/<version>
#    (never shipped), as package_installer.py keeps the Windows ones.
#
# Nothing of the game is ever needed or read: the setup is made from the build
# and the repository alone, as the Windows one (tools/port/package_installer.py).
set -euo pipefail

repo="$(cd "$(dirname "$0")/../.." && pwd)"
main_repo="${D012_MAIN_REPO:-${repo%%/.claude/worktrees/*}}"
root="${D012_LINUX_WORK:-$HOME/d012-linux}"
sources="$root/sources"
version="$(tr -d '[:space:]' < "$repo/tools/port/release-version.txt")"
build=1
jobs=8
while [[ $# -gt 0 ]]; do
    case "$1" in
        --version) version="${2:?--version needs a value}"; shift 2 ;;
        --no-build) build=0; shift ;;
        --jobs) jobs="${2:?--jobs needs a value}"; shift 2 ;;
        *) echo "unknown option $1" >&2; exit 2 ;;
    esac
done
[[ "$version" =~ ^[0-9]+\.[0-9]+\.[0-9]+(-[a-z0-9.]+)?$ ]] || { echo "--version must look like 0.5.4" >&2; exit 2; }

step() { printf '\n=== %s\n' "$*"; }

# appimagetool 1.9.1 and the type-2 runtime 20251108 (AppImage project,
# GitHub releases), pinned.
appimagetool="$sources/appimagetool-x86_64.AppImage"
appimagetool_url=https://github.com/AppImage/appimagetool/releases/download/1.9.1/appimagetool-x86_64.AppImage
appimagetool_sha256=ed4ce84f0d9caff66f50bcca6ff6f35aae54ce8135408b3fa33abfc3cb384eb0
runtime="$sources/runtime-x86_64"
runtime_url=https://github.com/AppImage/type2-runtime/releases/download/20251108/runtime-x86_64
runtime_sha256=2fca8b443c92510f1483a883f60061ad09b46b978b2631c807cd873a47ec260d
fetch() {  # <file> <url> <sha256>
    if [[ -f "$1" ]] && echo "$3  $1" | sha256sum -c --status; then return; fi
    curl -fsSL -o "$1.part" "$2"
    echo "$3  $1.part" | sha256sum -c --status || { echo "error: $2 does not match its pinned SHA-256" >&2; exit 1; }
    mv "$1.part" "$1"
}
mkdir -p "$sources"
fetch "$appimagetool" "$appimagetool_url" "$appimagetool_sha256"
fetch "$runtime" "$runtime_url" "$runtime_sha256"
chmod +x "$appimagetool"

if [[ "$build" -eq 1 ]]; then
    bash "$repo/tools/port/linux_build.sh" --installer --jobs "$jobs"
fi
[[ -x "$root/setup/payload/D012Native" ]] || { echo "error: no installer build in $root/setup/payload" >&2; exit 1; }
if [[ ! -x "$root/toolchain/bin/clang++" ]]; then
    bash "$repo/tools/port/linux_toolchain.sh"
fi

step "The AppDir"
appdir="$root/setup/AppDir"
rm -rf "$appdir"
mkdir -p "$appdir"
install -m 755 "$repo/tools/port/linux_setup/AppRun" "$appdir/AppRun"
cp "$repo/tools/port/linux_setup/dissidia012-setup.desktop" "$repo/tools/port/linux_setup/dissidia012-setup.svg" "$appdir/"
ln -s dissidia012-setup.svg "$appdir/.DirIcon"
echo "$version" > "$appdir/VERSION"
cp -a "$root/setup/payload" "$appdir/payload"
cp -a "$root/toolchain" "$appdir/payload/toolchain"
cp "$root/toolchain/licenses/"* "$appdir/payload/licenses/"

step "Checking it"
python3 "$repo/tools/port/linux_check_setup.py" "$appdir" "$root" "$main_repo" "/mnt/g"

step "Packing the AppImage"
dist="$root/setup/dist"
mkdir -p "$dist"
out="$dist/Dissidia012-Setup-$version-x86_64.AppImage"
rm -f "$out"
# appimagetool is an AppImage itself; WSL has no FUSE, so it runs unpacked.
ARCH=x86_64 "$appimagetool" --appimage-extract-and-run --no-appstream --runtime-file "$runtime" \
    --comp zstd "$appdir" "$out"
(cd "$dist" && sha256sum "$(basename "$out")" > SHA256SUMS)
mkdir -p "$main_repo/build/port/installer-linux/dist"
cp "$out" "$dist/SHA256SUMS" "$main_repo/build/port/installer-linux/dist/"
symbols="$main_repo/build/port/installer-linux/symbols/$version"
mkdir -p "$symbols"
cp "$root/build-installer/bin/D012Native" "$root/build-installer/bin/psp_recomp" "$symbols/"
ls -l "$out"
cat "$dist/SHA256SUMS"

#!/usr/bin/env bash
# Builds the Battle BGM Maker for Linux as one file that runs where it is,
# with nothing installed: Dissidia012-BgmMaker-<version>-x86_64.AppImage, from WSL:
#
#   wsl -d Ubuntu-24.04 -- bash tools/port/linux_bgm_maker.sh [--version X.Y.Z] [--jobs N]
#
# The version is tools/port/release-version.txt's unless --version gives
# another. Put in a game's folder (beside D012Native), the AppImage works on
# that game's mods and starts it for "Try in game"; elsewhere it opens the
# mods folder opened in it last time, else the game the Linux setup installed
# (port/bgm_maker/mods_folder.cpp).
#
# 1. This checkout's port/ and the PortableKit working tree copied into WSL
#    (tools/port/linux_build.sh --sync-only --installer: no game code).
# 2. atracdenc's source at the commit tools/port/build_atracdenc.cmd names,
#    from its clone in the main checkout's build/external/atracdenc-src (git
#    archive, with its libgha submodule): built from it, and shipped with it.
# 3. In the SDK container: tools/port/linux_bgm_maker_in_sdk.sh (BgmMaker,
#    atracdenc, the AppDir), through linux_build_in_sdk.sh.
# 4. tools/port/linux_check_setup.py on the AppDir, then appimagetool with the
#    pinned type-2 runtime (the pins of tools/port/linux_setup.sh), and
#    SHA256SUMS. The result goes to ~/d012-linux/bgm-maker/dist and the main
#    checkout's build/port/bgm-maker-linux/dist.
set -euo pipefail

repo="$(cd "$(dirname "$0")/../.." && pwd)"
main_repo="${D012_MAIN_REPO:-${repo%%/.claude/worktrees/*}}"
root="${D012_LINUX_WORK:-$HOME/d012-linux}"
sources="$root/sources"
version="$(tr -d '[:space:]' < "$repo/tools/port/release-version.txt")"
jobs=8
while [[ $# -gt 0 ]]; do
    case "$1" in
        --version) version="${2:?--version needs a value}"; shift 2 ;;
        --jobs) jobs="${2:?--jobs needs a value}"; shift 2 ;;
        *) echo "unknown option $1" >&2; exit 2 ;;
    esac
done
[[ "$version" =~ ^[0-9]+\.[0-9]+\.[0-9]+(-[a-z0-9.]+)?$ ]] || { echo "--version must look like 0.5.4" >&2; exit 2; }

step() { printf '\n=== %s\n' "$*"; }

# appimagetool 1.9.1 and the type-2 runtime 20251108 (AppImage project,
# GitHub releases), pinned as tools/port/linux_setup.sh pins them.
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

bash "$repo/tools/port/linux_build.sh" --sync-only --installer

step "atracdenc's source"
atracdenc_commit="$(tr -d '\r' < "$repo/tools/port/build_atracdenc.cmd" | sed -n 's/^set "COMMIT=\([0-9a-f]\{40\}\)"$/\1/p')"
[[ "$atracdenc_commit" =~ ^[0-9a-f]{40}$ ]] || { echo "error: no atracdenc commit in build_atracdenc.cmd" >&2; exit 1; }
atracdenc_clone="$main_repo/build/external/atracdenc-src/atracdenc"
[[ -d "$atracdenc_clone" ]] ||
    { echo "error: no atracdenc clone in $atracdenc_clone: run tools/port/build_atracdenc.cmd on Windows" >&2; exit 1; }
# A Windows clone, read from WSL.
atrac_git() { git -c safe.directory='*' -C "$1" "${@:2}"; }
libgha_clone="$atracdenc_clone/src/lib/libgha"
libgha_commit="$(atrac_git "$atracdenc_clone" ls-tree "$atracdenc_commit" src/lib/libgha | awk '{ print $3 }')"
[[ "$libgha_commit" =~ ^[0-9a-f]{40}$ ]] || { echo "error: atracdenc $atracdenc_commit has no libgha submodule" >&2; exit 1; }
archive="$sources/atracdenc-$atracdenc_commit.tar.gz"
if [[ ! -s "$archive" ]]; then
    work="$(mktemp -d)"
    atrac_git "$atracdenc_clone" archive --format=tar --prefix=atracdenc/ "$atracdenc_commit" | tar -C "$work" -xf -
    atrac_git "$libgha_clone" archive --format=tar --prefix=atracdenc/src/lib/libgha/ "$libgha_commit" | tar -C "$work" -xf -
    tar -C "$work" --sort=name --owner=0 --group=0 --numeric-owner --mtime='2000-01-01 00:00Z' -czf "$archive.part" atracdenc
    rm -rf "$work"
    mv "$archive.part" "$archive"
fi
echo "$libgha_commit" > "$sources/atracdenc-$atracdenc_commit.libgha"
echo "atracdenc $atracdenc_commit, libgha $libgha_commit"

# What linux_build.sh copies for a build of its own, and the BGM Maker's files.
cp "$repo/tools/port/linux_build_in_sdk.sh" "$repo/tools/port/linux_bgm_maker_in_sdk.sh" "$repo/LICENSE" "$root/src/"
rm -rf "$root/src/linux_bgm_maker"
cp -r "$repo/tools/port/linux_bgm_maker" "$root/src/linux_bgm_maker"
mkdir -p "$root/src/tools/port"
cp "$repo/tools/port/release-version.txt" "$root/src/tools/port/"
rm -f "$root/src/tools/port/update-signing-key.txt"
[[ -f "$repo/tools/port/update-signing-key.txt" ]] && cp "$repo/tools/port/update-signing-key.txt" "$root/src/tools/port/"

# shellcheck source=/dev/null
source "$root/src/PortableKit/packaging/sources.sh"
step "Building in $SDK_IMAGE"
mkdir -p "$root/home"
docker run --rm --user "$(id -u):$(id -g)" -v "$root:$root" -w "$root" \
    -e HOME="$root/home" -e D012_ROOT="$root" -e D012_JOBS="$jobs" -e D012_BGM_MAKER=1 \
    -e D012_ATRACDENC_COMMIT="$atracdenc_commit" \
    "$SDK_IMAGE" bash "$root/src/linux_build_in_sdk.sh"

appdir="$root/bgm-maker/AppDir"
step "Checking it"
python3 "$repo/tools/port/linux_check_setup.py" --program BgmMaker "$appdir" "$root" "$main_repo" "/mnt/g"

step "Packing the AppImage"
dist="$root/bgm-maker/dist"
mkdir -p "$dist"
out="$dist/Dissidia012-BgmMaker-$version-x86_64.AppImage"
rm -f "$out"
# appimagetool is an AppImage itself; WSL has no FUSE, so it runs unpacked.
ARCH=x86_64 "$appimagetool" --appimage-extract-and-run --no-appstream --runtime-file "$runtime" \
    --comp zstd "$appdir" "$out"
(cd "$dist" && sha256sum "$(basename "$out")" > SHA256SUMS)
mkdir -p "$main_repo/build/port/bgm-maker-linux/dist"
cp "$out" "$dist/SHA256SUMS" "$main_repo/build/port/bgm-maker-linux/dist/"
ls -l "$out"
cat "$dist/SHA256SUMS"

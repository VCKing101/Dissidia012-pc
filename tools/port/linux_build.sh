#!/usr/bin/env bash
# Build the native Linux port inside the Steam Runtime 3 "sniper" SDK
# container (the image PortableKit's release_linux.sh pins), from WSL.
#
#   wsl -d Ubuntu-24.04 -- bash tools/port/linux_build.sh [--sync-only] [--installer] [--jobs N]
#
# Run from the checkout (on G:, through /mnt/g). It copies into WSL's own disk
# (~/d012-linux by default, D012_LINUX_WORK): src/port from this checkout's
# port/, src/PortableKit from the PortableKit clone's "dissidia" branch, and
# work/ from the recompiled build's corpus (generated/, overlays/, config/).
# The corpus is game code: it stays in WSL and in the build, never in git.
# Then it builds SDL3 from its pinned source and D012Native in the container.
# --installer builds the installer build instead (no corpus, build-installer/),
# what the Linux setup ships (tools/port/linux_setup.sh).
set -euo pipefail

repo="$(cd "$(dirname "$0")/../.." && pwd)"
# The main checkout holds the ignored tools/local and build/. From a worktree
# under .claude/worktrees, it is the checkout around it (WSL's git cannot
# follow a worktree's Windows path back to it).
main_repo="${D012_MAIN_REPO:-${repo%%/.claude/worktrees/*}}"
# A PortableKit working tree, copied as it is (uncommitted changes included).
kit_tree="${D012_PORTABLEKIT_DIR:-$main_repo/tools/local/port/PortableKit}"
corpus="${D012_CORPUS:-$main_repo/build/port/recomp}"
root="${D012_LINUX_WORK:-$HOME/d012-linux}"
jobs=8
sync_only=0
installer=0
while [[ $# -gt 0 ]]; do
    case "$1" in
        --sync-only) sync_only=1; shift ;;
        --installer) installer=1; shift ;;
        --jobs) jobs="${2:?--jobs needs a value}"; shift 2 ;;
        *) echo "unknown option $1" >&2; exit 2 ;;
    esac
done

step() { printf '\n=== %s\n' "$*"; }

step "Copying sources into $root"
mkdir -p "$root/src" "$root/work"
mkdir -p "$root/src/port"
tar -C "$repo/port" -cf - . | tar -C "$root/src/port" -xpf -
# Copied over what is there: tar keeps each file's time, so the build
# recompiles only what changed.
mkdir -p "$root/src/PortableKit"
tar -C "$kit_tree" --exclude=./.git -cf - . | tar -C "$root/src/PortableKit" -xpf -
echo "PortableKit from $kit_tree"
for part in generated overlays config; do
    if [[ "$installer" -eq 0 && ! -d "$root/work/$part" ]]; then
        cp -r "$corpus/$part" "$root/work/$part"
    fi
done
[[ "$sync_only" -eq 1 ]] && exit 0

# shellcheck source=/dev/null
source "$root/src/PortableKit/packaging/sources.sh"
step "Building in $SDK_IMAGE"
mkdir -p "$root/home"
cp "$repo/tools/port/linux_build_in_sdk.sh" "$repo/LICENSE" "$root/src/"
# The launcher icon, if the main checkout's build/port/launcher has it.
icon="$main_repo/build/port/launcher/dissidia012.ico"
[[ -f "$icon" ]] && cp "$icon" "$root/src/"
docker run --rm --user "$(id -u):$(id -g)" -v "$root:$root" -w "$root" \
    -e HOME="$root/home" -e D012_ROOT="$root" -e D012_JOBS="$jobs" -e D012_INSTALLER="$installer" \
    "$SDK_IMAGE" bash "$root/src/linux_build_in_sdk.sh"

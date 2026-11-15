#!/usr/bin/env bash
# Builds a developer's Android app of the port (arm64-v8a, Android 10 and up)
# in WSL, with the Android NDK, for the developer's own test device:
#
#   wsl -d Ubuntu-24.04 -- bash tools/port/android_build.sh [--player] [--sync-only] [--jobs N] [--gen-jobs N]
#
# This app carries the game's code (libmain.so and the overlay libraries,
# compiled from the recompiled build's corpus), so it is for the developer's
# own device only and never shared. The player's app carries none of it:
# --player builds that one (the installer build, D012_INSTALLER_BUILD, in
# build-player/, app id io.github.vcking101.dissidia012), which compiles the
# game's code on the phone from the player's disc with the clang it carries
# (host/d012_device_compile.cpp; run tools/port/android_runtime.sh and
# tools/port/android_compiler.sh first) and loads it from its own storage
# (game-code/, host/d012_game_code.cpp).
#
# Run from the checkout (on G:, through /mnt/g). Like tools/port/linux_build.sh
# it copies into WSL's own disk (~/d012-android by default, D012_ANDROID_WORK):
# src/port from this checkout, src/PortableKit from a PortableKit working tree
# (D012_PORTABLEKIT_DIR, default the main checkout's clone), and work/ from the
# recompiled build's corpus. Then it builds SDL3 for Android, the port with
# PortableKit's Android app option, and packs the APK with PortableKit's
# packaging/android/build_apk.sh into ~/d012-android/dist.
#
# Tools it expects in ~/d012-android: ndk/ (NDK r28 or newer: its libraries
# are aligned for 16 KiB pages), sdk/build-tools/<v>/ and sdk/platforms/
# android-<n>/, jdk/, tools/usr/bin/zip and unzip. CMake, Ninja and
# glslangValidator come from the Linux build's tools (D012_HOSTTOOLS, default
# ~/d012-linux/hosttools/bin); the SDL3 and FFmpeg sources and the CJK font
# from its sources/ (D012_SOURCES, default ~/d012-linux/sources).
#
# The app is signed with a key of the developer's own, made once in
# ~/d012-android/keys: later builds install over earlier ones and keep the
# app's data (saves, settings, the installed disc). Keep that folder.
set -euo pipefail

repo="$(cd "$(dirname "$0")/../.." && pwd)"
main_repo="${D012_MAIN_REPO:-${repo%%/.claude/worktrees/*}}"
kit_tree="${D012_PORTABLEKIT_DIR:-$main_repo/tools/local/port/PortableKit}"
corpus="${D012_CORPUS:-$main_repo/build/port/recomp}"
root="${D012_ANDROID_WORK:-$HOME/d012-android}"
hosttools="${D012_HOSTTOOLS:-$HOME/d012-linux/hosttools/bin}"
sources="${D012_SOURCES:-$HOME/d012-linux/sources}"
app_id="${D012_APP_ID:-io.github.vcking101.dissidia012.dev}"
app_name="${D012_APP_NAME:-Dissidia 012 dev}"
api=29
jobs=12
# The recompiled units take over a gigabyte each to compile.
gen_jobs=6
# The setups compile the game's code at -O1 (host/d012_compile_game.cpp).
gen_opt=1
sync_only=0
player=0
while [[ $# -gt 0 ]]; do
    case "$1" in
        --player) player=1; shift ;;
        --sync-only) sync_only=1; shift ;;
        --jobs) jobs="${2:?--jobs needs a value}"; shift 2 ;;
        --gen-jobs) gen_jobs="${2:?--gen-jobs needs a value}"; shift 2 ;;
        *) echo "unknown option $1" >&2; exit 2 ;;
    esac
done

if [[ "$player" -eq 1 ]]; then
    app_id="${D012_APP_ID_PLAYER:-io.github.vcking101.dissidia012}"
    app_name="${D012_APP_NAME_PLAYER:-Dissidia 012}"
fi

step() { printf '\n=== %s\n' "$*"; }
fail() { echo "error: $*" >&2; exit 1; }

ndk="$root/ndk"
sdk="$root/sdk"
[[ -f "$ndk/build/cmake/android.toolchain.cmake" ]] || fail "no NDK in $ndk"
[[ -d "$sdk/build-tools" && -d "$sdk/platforms" ]] || fail "no SDK build-tools and platform in $sdk"
[[ -x "$root/jdk/bin/javac" ]] || fail "no JDK in $root/jdk"
[[ -x "$root/tools/usr/bin/zip" ]] || fail "no zip in $root/tools/usr/bin"
for tool in cmake ninja glslangValidator; do
    [[ -x "$hosttools/$tool" ]] || fail "no $tool in $hosttools (build the Linux port once: tools/port/linux_build.sh)"
done
# Only the tools named here, not the Linux build's pkg-config: FFmpeg's
# configure must not find the build machine's libraries.
mkdir -p "$root/bin"
for tool in cmake ninja glslangValidator; do ln -sfn "$hosttools/$tool" "$root/bin/$tool"; done
export ANDROID_HOME="$sdk" ANDROID_NDK="$ndk" JAVA_HOME="$root/jdk"
export PATH="$root/bin:$root/jdk/bin:$root/tools/usr/bin:$PATH"

step "Copying sources into $root"
mkdir -p "$root/src/port" "$root/src/PortableKit" "$root/src/tools/port" "$root/work"
# Copied over what is there: tar keeps each file's time, so the build
# recompiles only what changed. A removed file would stay, so start clean when
# the PortableKit tree changes.
kit_stamp="$root/src/.portablekit-tree"
if [[ "$(cat "$kit_stamp" 2>/dev/null)" != "$kit_tree" ]]; then
    rm -rf "$root/src/PortableKit"; mkdir -p "$root/src/PortableKit"
fi
tar -C "$repo/port" -cf - . | tar -C "$root/src/port" -xpf -
tar -C "$kit_tree" --exclude=./.git -cf - . | tar -C "$root/src/PortableKit" -xpf -
echo "$kit_tree" > "$kit_stamp"
echo "PortableKit from $kit_tree"
cp "$repo/tools/port/release-version.txt" "$root/src/tools/port/"
cp "$repo/LICENSE" "$root/src/"
for part in generated overlays config; do
    [[ "$player" -eq 1 ]] && break
    if [[ ! -d "$root/work/$part" ]]; then
        [[ -d "$corpus/$part" ]] || fail "no corpus in $corpus (the recompiled build's generated/ and overlays/)"
        cp -r "$corpus/$part" "$root/work/$part"
    fi
done
[[ "$sync_only" -eq 1 ]] && exit 0

toolchain=(-DCMAKE_TOOLCHAIN_FILE="$ndk/build/cmake/android.toolchain.cmake"
           -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM="android-$api")

# shellcheck source=/dev/null
source "$root/src/PortableKit/packaging/sources.sh"
sdl_archive="$sources/SDL3-$SDL3_VERSION.tar.gz"
[[ -f "$sdl_archive" ]] || fail "no $sdl_archive (tools/port/linux_build.sh fetches it)"
echo "$SDL3_SHA256  $sdl_archive" | sha256sum -c --quiet
deps="$root/deps"
if [[ "$(cat "$deps/.sdl3.stamp" 2>/dev/null)" != "$SDL3_VERSION $SDL3_SHA256 android-$api" ]]; then
    step "Building SDL3 $SDL3_VERSION for Android"
    rm -rf "$root/deps-build" "$deps"
    mkdir -p "$root/deps-build"
    tar -xzf "$sdl_archive" -C "$root/deps-build"
    cmake -S "$root/deps-build/SDL3-$SDL3_VERSION" -B "$root/deps-build/sdl3-build" -G Ninja \
        "${toolchain[@]}" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$deps" \
        -DSDL_SHARED=ON -DSDL_STATIC=OFF -DSDL_TESTS=OFF -DSDL_EXAMPLES=OFF
    cmake --build "$root/deps-build/sdl3-build" -j "$jobs"
    cmake --install "$root/deps-build/sdl3-build"
    echo "$SDL3_VERSION $SDL3_SHA256 android-$api" > "$deps/.sdl3.stamp"
fi
sdl_lib="$(find "$deps" -name libSDL3.so | head -1)"
sdl_cmake="$(dirname "$(find "$deps" -name SDL3Config.cmake | head -1)")"
[[ -f "$sdl_lib" && -d "$sdl_cmake" ]] || fail "SDL3 for Android is missing in $deps"

step "Configuring the port"
build="$root/build"
player_flags=()
if [[ "$player" -eq 1 ]]; then
    build="$root/build-player"
    player_flags=(-DD012_INSTALLER_BUILD=ON)
fi
cmake -S "$root/src/port" -B "$build" -G Ninja "${toolchain[@]}" "${player_flags[@]}" \
    -DCMAKE_BUILD_TYPE=Release \
    -DPORTABLEKIT_ANDROID_APP=ON \
    -DPORTABLEKIT_RELEASE=ON \
    -DPORTABLEKIT_FFMPEG=bundled \
    -DPORTABLEKIT_FFMPEG_DOWNLOAD_DIR="$sources" \
    -DSDL3_DIR="$sdl_cmake" \
    -DD012_PORTABLEKIT_DIR="$root/src/PortableKit" \
    -DD012_WORK_DIR="$root/work" \
    -DPSPRECOMP_GENERATED_OPT_LEVEL="$gen_opt" \
    -DPSPRECOMP_GENERATED_JOBS="$gen_jobs" 2>&1 | tee "$root/configure.log"

step "Building"
set +e
build_targets=(D012Native)
[[ "$player" -eq 1 ]] && build_targets+=(D012Native_android_files)
cmake --build "$build" -j "$jobs" --target "${build_targets[@]}" -- -k 0 2>&1 | tee "$root/build.log"
status=${PIPESTATUS[0]}
set -e
if [[ "$status" -ne 0 ]]; then
    echo; echo "=== The build failed. First errors:"
    grep -n -m 8 -A4 -E 'error:|FAILED:|undefined (reference|symbol)' "$root/build.log" | cut -c1-240
    exit "$status"
fi
# The overlay libraries are targets of their own.
overlay_targets=($(cmake --build "$build" --target help | sed -n 's/^\(overlay_[^:]*\):.*/\1/p'))
if [[ ${#overlay_targets[@]} -gt 0 ]]; then
    cmake --build "$build" -j "$jobs" --target "${overlay_targets[@]}" 2>&1 | tee -a "$root/build.log"
fi

step "Packing the APK"
# The app's resources: port/android/res, renamed for a developer's build.
res="$root/res"
rm -rf "$res"
cp -r "$root/src/port/android/res" "$res"
sed -i "s|<string name=\"app_name\">[^<]*</string>|<string name=\"app_name\">$app_name</string>|" \
    "$res/values/strings.xml"
mkdir -p "$root/keys" "$root/dist"
keystore="$root/keys/dev.keystore"
if [[ ! -f "$keystore" ]]; then
    keytool -genkeypair -keystore "$keystore" -storepass d012dev -keypass d012dev -alias d012dev \
        -keyalg RSA -keysize 2048 -validity 10000 -dname "CN=Dissidia 012 developer build" > /dev/null
    echo "made the signing key $keystore: keep it, or the next build will not install over this one"
fi
version="$(tr -d '[:space:]' < "$root/src/tools/port/release-version.txt")"
apk="$root/dist/Dissidia012-dev.apk"
version_name="$version-dev"
extra=()
if [[ "$player" -eq 1 ]]; then
    apk="$root/dist/Dissidia012.apk"
    version_name="$version"
    # The phone's compiler (host/d012_device_compile.cpp): clang, lld and the
    # recompiler as libraries, and the rest as one asset, toolchain.tar, in the
    # layout the app unpacks: lib/clang/<N>/ (clang's headers and the
    # builtins), android/sysroot/, sdk/, licenses/. Made by
    # tools/port/android_runtime.sh and tools/port/android_compiler.sh.
    rt="${D012_ANDROID_RT_WORK:-$root/rt}/out"
    [[ -x "$rt/android-compiler/bin/clang" && -d "$rt/android-runtime/sysroot" ]] ||
        fail "no compiler for the phone in $rt: run tools/port/android_runtime.sh, then tools/port/android_compiler.sh"
    programs="$build/apk-programs"
    tree="$build/apk-toolchain"
    assets="$build/apk-assets"
    rm -rf "$programs" "$tree" "$assets"
    mkdir -p "$programs" "$tree/lib/clang" "$tree/android" "$tree/licenses" "$assets"
    cp "$rt/android-compiler/bin/clang" "$programs/libclang.so"
    cp "$rt/android-compiler/bin/lld" "$programs/liblld.so"
    cp "$build/bin/android-programs/libpsp_recomp.so" "$programs/"
    cp -r "$rt/android-compiler/lib/clang/." "$tree/lib/clang/"
    major="$(ls "$tree/lib/clang")"
    cp -r "$rt/android-runtime/clang/lib" "$tree/lib/clang/$major/"
    cp -r "$rt/android-runtime/sysroot" "$tree/android/"
    cp -r "$build/bin/android-toolchain/sdk" "$tree/"
    cp "$rt/android-runtime/licenses/"* "$rt/android-runtime/SOURCES.txt" "$tree/licenses/"
    # The same files give the same tar: names sorted, no times or owners.
    tar --format=ustar --sort=name --mtime=@0 --owner=0 --group=0 --numeric-owner -C "$tree" -cf "$assets/toolchain.tar" .
    sha256sum "$assets/toolchain.tar" | cut -d' ' -f1 > "$assets/toolchain.sha256"
    echo "toolchain.tar $(du -h "$assets/toolchain.tar" | cut -f1), $(cat "$assets/toolchain.sha256")"
    extra=(EXTRA_LIBS="$programs" EXTRA_ASSETS="$assets")
fi
env "${extra[@]}" \
APP_ID="$app_id" GAME_RES="$res" GAME_REPO="$repo" FONT_DIR="$sources" \
KEYSTORE="$keystore" KEYSTORE_PASS=d012dev KEY_ALIAS=d012dev \
VERSION_NAME="$version_name" VERSION_CODE="$(date +%s)" DEBUGGABLE=1 \
    bash "$root/src/PortableKit/packaging/android/build_apk.sh" "$build" \
        "$root/deps-build/SDL3-$SDL3_VERSION" "$sdl_lib" "$apk"
sha256sum "$apk"

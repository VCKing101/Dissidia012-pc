#!/usr/bin/env bash
# The in-container half of tools/port/linux_build.sh: builds SDL3 from the
# source PortableKit pins, then the port (D012Native, its overlay libraries,
# BgmMaker and d012_tests) as a release build, and the portable folder
# package/Dissidia012 with its .tar.xz. Resumable: SDL3 is kept once
# built, and the build directory is reused.
set -euo pipefail

root="${D012_ROOT:?}"
jobs="${D012_JOBS:-8}"
kit="$root/src/PortableKit"
# shellcheck source=/dev/null
source "$kit/packaging/sources.sh"
sources="$root/sources"
deps="$root/deps"
deps_build="$root/deps-build"
build="$root/build"
mkdir -p "$sources" "$deps" "$deps_build"

export CC=gcc-14 CXX=g++-14
export PKG_CONFIG_PATH="$deps/lib/pkgconfig"

step() { printf '\n=== %s\n' "$*"; }

# fetch <file name> <url> <sha256>
fetch() {
    local target="$sources/$1"
    if [[ -f "$target" ]] && echo "$3  $target" | sha256sum -c --status; then return; fi
    curl -fsSL -o "$target.part" "$2"
    echo "$3  $target.part" | sha256sum -c --status ||
        { echo "error: $2 does not match its pinned SHA-256" >&2; exit 1; }
    mv "$target.part" "$target"
}

step "Fetching pinned sources"
fetch "SDL3-$SDL3_VERSION.tar.gz" "$SDL3_URL" "$SDL3_SHA256"
fetch NotoSansCJKjp-Regular.otf "$NOTO_CJK_URL" "$NOTO_CJK_SHA256"
fetch NotoSansCJK-LICENSE.txt "$NOTO_CJK_LICENSE_URL" "$NOTO_CJK_LICENSE_SHA256"

if [[ "$(cat "$deps/.sdl3.stamp" 2>/dev/null)" != "$SDL3_VERSION $SDL3_SHA256" ]]; then
    step "Building SDL3 $SDL3_VERSION"
    rm -rf "$deps_build/SDL3-$SDL3_VERSION" "$deps_build/sdl3-build"
    tar -xzf "$sources/SDL3-$SDL3_VERSION.tar.gz" -C "$deps_build"
    cmake -S "$deps_build/SDL3-$SDL3_VERSION" -B "$deps_build/sdl3-build" -G Ninja \
        -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$deps" -DCMAKE_INSTALL_LIBDIR=lib \
        -DSDL_SHARED=ON -DSDL_STATIC=OFF -DSDL_TESTS=OFF -DSDL_EXAMPLES=OFF
    cmake --build "$deps_build/sdl3-build" -j "$jobs"
    cmake --install "$deps_build/sdl3-build"
    echo "$SDL3_VERSION $SDL3_SHA256" > "$deps/.sdl3.stamp"
fi

if [[ "${D012_INSTALLER:-0}" -eq 1 ]]; then
    # The installer build (port/CMakeLists.txt, D012_INSTALLER_BUILD): the
    # program without the game's code, with psp_recomp and sdk/ beside it, for
    # the Linux setup (tools/port/linux_setup.sh). It reads no corpus.
    build="$root/build-installer"
    step "Configuring the installer build"
    cmake -S "$root/src/port" -B "$build" -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DPORTABLEKIT_RELEASE=ON \
        -DD012_INSTALLER_BUILD=ON \
        -DPORTABLEKIT_FFMPEG=bundled \
        -DPORTABLEKIT_FFMPEG_DOWNLOAD_DIR="$sources" \
        -DCMAKE_PREFIX_PATH="$deps" \
        -DD012_PORTABLEKIT_DIR="$kit" 2>&1 | tee "$root/configure-installer.log"
    step "Building the installer build"
    cmake --build "$build" -j "$jobs" --target D012Native BgmMaker D012Native_installer_files -- -k 0 2>&1 |
        tee "$root/build-installer.log"

    # What the setup installs, as the Windows setup's stage
    # (tools/port/package_installer.py): the program, psp_recomp, lib/,
    # sdk/, fonts/ and licenses/. tools/port/linux_setup.sh adds the
    # toolchain and makes the AppImage around it. No BgmMaker yet: its
    # "Make a PSP / PPSSPP ISO" needs an atracdenc built for Linux.
    step "Staging the setup's files"
    stage="$root/setup/payload"
    rm -rf "$stage"
    mkdir -p "$stage/lib" "$stage/fonts" "$stage/licenses"
    for program in D012Native psp_recomp; do
        install -m 755 "$build/bin/$program" "$stage/$program"
        strip --strip-unneeded "$stage/$program"
    done
    cp -r "$build/bin/sdk" "$stage/sdk"
    needed() { objdump -p "$1" | awk '$1 == "NEEDED" { print $2 }'; }
    pending=("$stage/D012Native" "$stage/psp_recomp")
    while [[ ${#pending[@]} -gt 0 ]]; do
        current="${pending[0]}"
        pending=("${pending[@]:1}")
        for soname in $(needed "$current"); do
            [[ -e "$stage/lib/$soname" ]] && continue
            for dir in "$build/bin/lib" "$deps/lib"; do
                [[ -e "$dir/$soname" ]] || continue
                cp -L "$dir/$soname" "$stage/lib/$soname"
                chmod 644 "$stage/lib/$soname"
                strip --strip-unneeded "$stage/lib/$soname"
                pending+=("$stage/lib/$soname")
                break
            done
        done
    done
    cp "$sources/NotoSansCJKjp-Regular.otf" "$stage/fonts/"
    cp "$root/src/LICENSE" "$stage/licenses/Dissidia012-LICENSE.txt"
    cp "$kit/LICENSE" "$stage/licenses/PortableKit-LICENSE.txt"
    cp "$deps_build/SDL3-$SDL3_VERSION/LICENSE.txt" "$stage/licenses/SDL3-LICENSE.txt"
    cp "$build/bin/lib/FFmpeg-COPYING.LGPLv2.1.txt" "$build/bin/lib/FFmpeg-SOURCE.txt" "$stage/licenses/"
    cp "$kit/third_party/imgui/LICENSE.txt" "$stage/licenses/DearImGui-LICENSE.txt"
    cp "$kit/third_party/tiny_aes/UNLICENSE" "$stage/licenses/tiny-AES-c-UNLICENSE.txt"
    cp "$kit/third_party/xxhash/LICENSE" "$stage/licenses/xxHash-LICENSE.txt"
    cp "$root/src/port/fonts/GoudyBookletter1911-OFL.txt" "$stage/licenses/"
    cp "$sources/NotoSansCJK-LICENSE.txt" "$stage/licenses/NotoSansCJK-OFL.txt"

    step "Checking the setup's files"
    for program in D012Native psp_recomp; do
        missing="$(LD_LIBRARY_PATH='' ldd "$stage/$program" | grep 'not found' || true)"
        if [[ -n "$missing" ]]; then
            echo "error: $program has unresolved libraries:" >&2
            echo "$missing" >&2
            exit 1
        fi
    done
    if objdump -p "$stage/D012Native" "$stage/psp_recomp" | grep -q 'NEEDED.*libstdc++'; then
        echo "error: a staged program needs libstdc++.so" >&2
        exit 1
    fi
    glibc_floor="$(objdump -T "$stage/D012Native" "$stage/psp_recomp" "$stage/lib/"*.so* |
        grep -o 'GLIBC_[0-9.]*' | sort -uV | tail -1)"
    echo "Newest glibc symbol version needed: $glibc_floor"
    du -sh "$stage"
    exit 0
fi

step "Configuring the port"
cmake -S "$root/src/port" -B "$build" -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DPORTABLEKIT_RELEASE=ON \
    -DPORTABLEKIT_FFMPEG=bundled \
    -DPORTABLEKIT_FFMPEG_DOWNLOAD_DIR="$sources" \
    -DCMAKE_PREFIX_PATH="$deps" \
    -DD012_PORTABLEKIT_DIR="$kit" \
    -DD012_WORK_DIR="$root/work" \
    -DPSPRECOMP_GENERATED_JOBS="$jobs" 2>&1 | tee "$root/configure.log"

step "Building"
cmake --build "$build" -j "$jobs" -- -k 0 2>&1 | tee "$root/build.log"

# The portable folder, as PortableKit's packaging/linux/build_in_sdk.sh stages
# its own: the program, overlays/, lib/ (SDL3 and FFmpeg, found through the
# program's $ORIGIN/lib), fonts/ (the Japanese text) and licenses/. No game/
# and no data/: the disc's EBOOT.ELF and image go in game/ beside the program
# where it is installed, and data/ is made on the first run.
step "Staging the portable folder"
stage="$root/package/Dissidia012"
rm -rf "$stage"
mkdir -p "$stage/lib" "$stage/overlays" "$stage/fonts" "$stage/licenses"
install -m 755 "$build/bin/D012Native" "$stage/D012Native"
strip --strip-unneeded "$stage/D012Native"
cp "$build/bin/overlays/"*.so "$stage/overlays/"
strip --strip-unneeded "$stage/overlays/"*.so

needed() { objdump -p "$1" | awk '$1 == "NEEDED" { print $2 }'; }
pending=("$stage/D012Native")
while [[ ${#pending[@]} -gt 0 ]]; do
    current="${pending[0]}"
    pending=("${pending[@]:1}")
    for soname in $(needed "$current"); do
        [[ -e "$stage/lib/$soname" ]] && continue
        for dir in "$build/bin/lib" "$deps/lib"; do
            [[ -e "$dir/$soname" ]] || continue
            cp -L "$dir/$soname" "$stage/lib/$soname"
            chmod 644 "$stage/lib/$soname"
            strip --strip-unneeded "$stage/lib/$soname"
            pending+=("$stage/lib/$soname")
            break
        done
    done
done

cp "$sources/NotoSansCJKjp-Regular.otf" "$stage/fonts/"
cp "$root/src/LICENSE" "$stage/licenses/Dissidia012-LICENSE.txt"
cp "$kit/LICENSE" "$stage/licenses/PortableKit-LICENSE.txt"
cp "$deps_build/SDL3-$SDL3_VERSION/LICENSE.txt" "$stage/licenses/SDL3-LICENSE.txt"
cp "$build/bin/lib/FFmpeg-COPYING.LGPLv2.1.txt" "$build/bin/lib/FFmpeg-SOURCE.txt" "$stage/licenses/"
cp "$kit/third_party/imgui/LICENSE.txt" "$stage/licenses/DearImGui-LICENSE.txt"
cp "$kit/third_party/tiny_aes/UNLICENSE" "$stage/licenses/tiny-AES-c-UNLICENSE.txt"
cp "$kit/third_party/xxhash/LICENSE" "$stage/licenses/xxHash-LICENSE.txt"
cp "$root/src/port/fonts/GoudyBookletter1911-OFL.txt" "$stage/licenses/"
cp "$sources/NotoSansCJK-LICENSE.txt" "$stage/licenses/NotoSansCJK-OFL.txt"
[[ -f "$root/src/dissidia012.ico" ]] && cp "$root/src/dissidia012.ico" "$stage/"

step "Checking the portable folder"
# Everything resolves from lib/ or from libraries every desktop has.
missing="$(LD_LIBRARY_PATH='' ldd "$stage/D012Native" | grep 'not found' || true)"
if [[ -n "$missing" ]]; then
    echo "error: unresolved libraries:" >&2
    echo "$missing" >&2
    exit 1
fi
ldd "$stage/D012Native" | grep "$stage/lib" || { echo "error: bundled libraries not used" >&2; exit 1; }
if objdump -p "$stage/D012Native" "$stage/overlays/"*.so | grep -q 'NEEDED.*libstdc++'; then
    echo "error: a staged binary needs libstdc++.so" >&2
    exit 1
fi
glibc_floor="$(objdump -T "$stage/D012Native" "$stage/lib/"*.so* "$stage/overlays/"*.so |
    grep -o 'GLIBC_[0-9.]*' | sort -uV | tail -1)"
echo "Newest glibc symbol version needed: $glibc_floor"
tar -C "$root/package" -cJf "$root/package/Dissidia012-linux-x86_64.tar.xz" Dissidia012
ls -l "$root/package/Dissidia012-linux-x86_64.tar.xz"

#!/usr/bin/env bash
# The in-container half of tools/port/linux_bgm_maker.sh, sourced by
# linux_build_in_sdk.sh once SDL3 is built and the pinned sources are fetched
# (root, jobs, kit, sources, deps, deps_build and step are set there):
#
# 1. BgmMaker as the installer build makes it (no corpus), in build-bgm-maker/.
# 2. atracdenc, the ATRAC3 encoder "Make a PSP / PPSSPP ISO" runs, from the
#    source tools/port/linux_bgm_maker.sh exported (the commit
#    tools/port/build_atracdenc.cmd names), unmodified, reading WAV with
#    libsndfile (the Windows build uses Media Foundation).
# 3. The AppDir, bgm-maker/AppDir: BgmMaker (AppRun is a link to it), atracdenc,
#    lib/ (SDL3, FFmpeg, and libsndfile with the libraries it needs, from the
#    SDK image), fonts/ (the Japanese face, for systems without one),
#    licenses/, the desktop entry and the icon.

atracdenc_commit="${D012_ATRACDENC_COMMIT:?}"
build="$root/build-bgm-maker"
out="$root/bgm-maker"

step "Configuring the Battle BGM Maker"
cmake -S "$root/src/port" -B "$build" -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DPORTABLEKIT_RELEASE=ON \
    -DD012_INSTALLER_BUILD=ON \
    -DPORTABLEKIT_FFMPEG=bundled \
    -DPORTABLEKIT_FFMPEG_DOWNLOAD_DIR="$sources" \
    -DCMAKE_PREFIX_PATH="$deps" \
    -DD012_PORTABLEKIT_DIR="$kit" 2>&1 | tee "$root/configure-bgm-maker.log"
step "Building the Battle BGM Maker"
cmake --build "$build" -j "$jobs" --target BgmMaker 2>&1 | tee "$root/build-bgm-maker.log"

atrac_src="$deps_build/atracdenc-$atracdenc_commit"
atrac_build="$deps_build/atracdenc-build"
if [[ "$(cat "$atrac_build/.atracdenc.stamp" 2>/dev/null)" != "$atracdenc_commit" ]]; then
    step "Building atracdenc $atracdenc_commit"
    rm -rf "$atrac_src" "$atrac_build"
    mkdir -p "$atrac_src"
    tar -xzf "$sources/atracdenc-$atracdenc_commit.tar.gz" -C "$atrac_src" --strip-components=1
    # The C++ runtime linked in, as the port's own programs; DT_RPATH (not
    # RUNPATH) so libsndfile finds the libraries it needs in lib/ too.
    cmake -S "$atrac_src" -B "$atrac_build" -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DATRACDENC_PCM_IO_BACKEND=libsndfile \
        -DCMAKE_EXE_LINKER_FLAGS="-static-libstdc++ -static-libgcc -Wl,--disable-new-dtags" \
        -DCMAKE_BUILD_WITH_INSTALL_RPATH=ON \
        '-DCMAKE_INSTALL_RPATH=$ORIGIN/lib'
    cmake --build "$atrac_build" -j "$jobs" --target atracdenc
    echo "$atracdenc_commit" > "$atrac_build/.atracdenc.stamp"
fi

step "The AppDir"
appdir="$out/AppDir"
rm -rf "$appdir"
mkdir -p "$appdir/lib" "$appdir/fonts" "$appdir/licenses"
install -m 755 "$build/bin/BgmMaker" "$appdir/BgmMaker"
install -m 755 "$atrac_build/src/atracdenc" "$appdir/atracdenc"
strip --strip-unneeded "$appdir/BgmMaker" "$appdir/atracdenc"
ln -s BgmMaker "$appdir/AppRun"

# Every library the two need that a desktop may lack: what the build made
# (lib/ and deps/), and libsndfile with its codecs from the SDK image (the
# Steam Runtime's Debian packages). Everything else (glibc, Vulkan's loader)
# is the system's.
from_sdk=" libsndfile.so.1 libFLAC.so.8 libogg.so.0 libvorbis.so.0 libvorbisenc.so.2 libopus.so.0 "
needed() { objdump -p "$1" | awk '$1 == "NEEDED" { print $2 }'; }
pending=("$appdir/BgmMaker" "$appdir/atracdenc")
while [[ ${#pending[@]} -gt 0 ]]; do
    current="${pending[0]}"
    pending=("${pending[@]:1}")
    for soname in $(needed "$current"); do
        [[ -e "$appdir/lib/$soname" ]] && continue
        dirs=("$build/bin/lib" "$deps/lib")
        [[ "$from_sdk" == *" $soname "* ]] && dirs+=(/usr/lib/x86_64-linux-gnu)
        for dir in "${dirs[@]}"; do
            [[ -e "$dir/$soname" ]] || continue
            cp -L "$dir/$soname" "$appdir/lib/$soname"
            chmod 644 "$appdir/lib/$soname"
            strip --strip-unneeded "$appdir/lib/$soname"
            pending+=("$appdir/lib/$soname")
            break
        done
    done
done

cp "$sources/NotoSansCJKjp-Regular.otf" "$appdir/fonts/"
cp "$root/src/linux_bgm_maker/dissidia012-bgm-maker.desktop" "$root/src/linux_bgm_maker/dissidia012-bgm-maker.svg" "$appdir/"
ln -s dissidia012-bgm-maker.svg "$appdir/.DirIcon"

licenses="$appdir/licenses"
cp "$root/src/LICENSE" "$licenses/Dissidia012-LICENSE.txt"
cp "$kit/LICENSE" "$licenses/PortableKit-LICENSE.txt"
cp "$deps_build/SDL3-$SDL3_VERSION/LICENSE.txt" "$licenses/SDL3-LICENSE.txt"
cp "$build/bin/lib/FFmpeg-COPYING.LGPLv2.1.txt" "$build/bin/lib/FFmpeg-SOURCE.txt" "$licenses/"
cp "$kit/third_party/imgui/LICENSE.txt" "$licenses/DearImGui-LICENSE.txt"
cp "$sources/NotoSansCJK-LICENSE.txt" "$licenses/NotoSansCJK-OFL.txt"
cp "$atrac_src/LICENSE" "$licenses/atracdenc-LICENSE.txt"
cp "$atrac_src/src/lib/libgha/LICENSE" "$licenses/libgha-LICENSE.txt"
cp "$atrac_src/src/lib/libgha/src/3rd/kissfft/LICENSES/BSD-3-Clause" "$licenses/KISS-FFT-LICENSE.txt"
cp "$sources/atracdenc-$atracdenc_commit.tar.gz" "$licenses/atracdenc-source.tar.gz"
libgha_commit="$(cat "$sources/atracdenc-$atracdenc_commit.libgha")"
cat > "$licenses/atracdenc-SOURCE.txt" <<EOF
atracdenc is atracdenc, an ATRAC encoder by Daniil Cherednik, licensed under the
GNU LGPL version 2.1 (atracdenc-LICENSE.txt), unmodified. Its libgha (BSD 2-Clause,
libgha-LICENSE.txt) and the KISS FFT code in both (copyright Mark Borgerding, BSD 3-Clause,
KISS-FFT-LICENSE.txt) are compiled into it. The Battle BGM Maker runs it as a program of its
own for "Make a PSP / PPSSPP ISO"; you may replace it with your own build.

Source:  https://github.com/dcherednik/atracdenc, commit $atracdenc_commit
         (its libgha submodule at commit $libgha_commit),
         and the same source in atracdenc-source.tar.gz beside this file.
Built:   with GCC in the Steam Runtime 3 "sniper" SDK and the CMake options
         -DATRACDENC_PCM_IO_BACKEND=libsndfile -DCMAKE_BUILD_TYPE=Release
         (tools/port/linux_bgm_maker_in_sdk.sh in https://github.com/VCKing101/Dissidia012-pc).
EOF
# libsndfile and its codecs, unmodified, as the SDK image has them.
packages="libsndfile1 libflac8 libogg0 libvorbis0a libvorbisenc2 libopus0"
{
    echo "lib/ holds these libraries, unmodified, from the Steam Runtime 3 \"sniper\" SDK image"
    echo "(Debian 11 packages), for atracdenc to read WAV files:"
    echo
    for package in $packages; do
        printf '  %-16s %s\n' "$package" "$(dpkg-query -W -f '${Version}' "$package:amd64")"
    done
    echo
    echo "libsndfile is licensed under the GNU LGPL version 2.1 or later; FLAC, Ogg, Vorbis and Opus"
    echo "under BSD-style licences. Each package's copyright file is beside this one."
    echo "Source: each package's Debian source, as https://sources.debian.org/src/<source>/<version>/"
    echo "(sources libsndfile, flac, libogg, libvorbis, opus), and the Steam Runtime's source"
    echo "packages at https://repo.steampowered.com/steamrt3/images/."
} > "$licenses/libsndfile-SOURCE.txt"
for package in $packages; do
    cp -L "/usr/share/doc/$package/copyright" "$licenses/$package-copyright.txt"
done
# Files from the Windows checkout come with every permission bit set.
chmod 644 "$appdir/"*.desktop "$appdir/"*.svg "$appdir/fonts/"* "$licenses/"*

step "Checking the AppDir"
for program in BgmMaker atracdenc; do
    missing="$(LD_LIBRARY_PATH='' ldd "$appdir/$program" | grep 'not found' || true)"
    if [[ -n "$missing" ]]; then
        echo "error: $program has unresolved libraries:" >&2
        echo "$missing" >&2
        exit 1
    fi
done
# The SDK image has these libraries too: each must be found in lib/.
for soname in $from_sdk libSDL3.so.0 libavcodec.so.61 libavformat.so.61 libavutil.so.59 libswresample.so.5; do
    [[ -e "$appdir/lib/$soname" ]] || { echo "error: lib/$soname was not bundled" >&2; exit 1; }
done
stray="$(LD_LIBRARY_PATH='' ldd "$appdir/BgmMaker" "$appdir/atracdenc" |
    grep -E 'lib(sndfile|FLAC|ogg|vorbis|vorbisenc|opus|SDL3|avcodec|avformat|avutil|swresample)\.so' |
    grep -v "=> $appdir/lib/" || true)"
if [[ -n "$stray" ]]; then
    echo "error: libraries found outside lib/:" >&2
    echo "$stray" >&2
    exit 1
fi
if objdump -p "$appdir/BgmMaker" "$appdir/atracdenc" | grep -q 'NEEDED.*libstdc++'; then
    echo "error: a program needs libstdc++.so" >&2
    exit 1
fi
glibc_floor="$(objdump -T "$appdir/BgmMaker" "$appdir/atracdenc" "$appdir/lib/"*.so* |
    grep -o 'GLIBC_[0-9.]*' | sort -uV | tail -1)"
echo "Newest glibc symbol version needed: $glibc_floor"
du -sh "$appdir"

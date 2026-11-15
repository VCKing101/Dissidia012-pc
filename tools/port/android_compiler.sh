#!/usr/bin/env bash
# clang and lld that run on the phone, for the Android app's on-device compile
# of the game's code from the player's disc (`--compile-game` inside the app,
# host/d012_compile_game.cpp). Built from LLVM's source release, against the
# Android parts tools/port/android_runtime.sh makes (run that first): the
# same sysroot the compiler then compiles with.
#
#   wsl -d Ubuntu-24.04 -- bash tools/port/android_compiler.sh [--jobs N]
#
# Makes, in ~/d012-android/rt (D012_ANDROID_RT_WORK):
#
#   out/android-compiler/
#     bin/clang                  clang 23.1.2, arm64 Android 10+, AArch64 only,
#                                its default target aarch64-linux-android29
#     bin/lld                    lld (run as `lld -flavor gnu`)
#     lib/clang/23/include/      clang's own headers
#     licenses/LLVM-LICENSE.txt
#
# The app ships both programs in its native library folder (as libclang.so
# and liblld.so: Android runs no program an app wrote into its own storage).
# Nothing of the game is in them.
#
# The same sources and build clang give the same bytes: no VCS revision, no
# build paths (-ffile-prefix-map), lld without build ids. The keys of the
# compile (key.txt) hash the compiler, so an app update that ships the same
# compiler compiles nothing again.
set -euo pipefail

work="${D012_ANDROID_RT_WORK:-$HOME/d012-android/rt}"
jobs=$(( $(nproc) / 2 ))
while [ $# -gt 0 ]; do
    case "$1" in
        --jobs) jobs="$2"; shift 2 ;;
        *) echo "usage: android_compiler.sh [--jobs N]" >&2; exit 2 ;;
    esac
done

api=29
clang_triple=aarch64-unknown-linux-android$api
llvm_version=23.1.2
llvm_major=23
build_llvm_version=23.1.4

src="$work/src"
llvm="$src/llvm-project-$llvm_version.src"
bin="$work/tools/LLVM-$build_llvm_version/bin"
runtime="$work/out/android-runtime"
sysroot="$runtime/sysroot"
build="$work/build-compiler"
out="$work/out/android-compiler"

step() { printf '\n=== %s\n' "$*"; }
die() { echo "error: $*" >&2; exit 1; }

[ -x "$bin/clang" ] && [ -d "$sysroot/usr/include/c++/v1" ] ||
    die "run tools/port/android_runtime.sh first (it fetches the sources and makes the sysroot)"
cmake="$(command -v cmake || echo "$HOME/d012-android/bin/cmake")"
ninja="$(command -v ninja || echo "$HOME/d012-android/bin/ninja")"
command -v g++ >/dev/null || die "g++ is needed for LLVM's build-time tools (apt install g++)"

top="llvm-project-$llvm_version.src"
if [ ! -f "$llvm/.unpacked-compiler" ]; then
    step "Unpacking LLVM, clang and lld from llvm-project-$llvm_version.src.tar.xz"
    tar -xJf "$src/llvm-project-$llvm_version.src.tar.xz" -C "$src" "$top/llvm" "$top/clang" "$top/lld" "$top/cmake" \
        "$top/third-party"
    touch "$llvm/.unpacked-compiler"
fi

# clang finds the builtins for linking the programs in its own folder: a copy
# of the build clang's with the Android builtins added.
resource="$work/tools/resource-android"
rm -rf "$resource"
mkdir -p "$resource/lib"
cp -r "$bin/../lib/clang/$llvm_major/include" "$resource/"
cp -r "$runtime/clang/lib/." "$resource/lib/"

flags="-O2 -ffunction-sections -fdata-sections -resource-dir=$resource -ffile-prefix-map=$llvm=llvm -ffile-prefix-map=$build=build"
step "Configuring clang and lld for $clang_triple"
mkdir -p "$build"
"$cmake" -G Ninja -S "$llvm/llvm" -B "$build" -DCMAKE_MAKE_PROGRAM="$ninja" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_SYSTEM_NAME=Linux -DANDROID=ON -DCMAKE_SYSTEM_PROCESSOR=aarch64 -DCMAKE_CROSSCOMPILING=ON \
    -DCMAKE_C_COMPILER="$bin/clang" -DCMAKE_CXX_COMPILER="$bin/clang++" -DCMAKE_ASM_COMPILER="$bin/clang" \
    -DCMAKE_C_COMPILER_TARGET="$clang_triple" -DCMAKE_CXX_COMPILER_TARGET="$clang_triple" \
    -DCMAKE_ASM_COMPILER_TARGET="$clang_triple" -DCMAKE_SYSROOT="$sysroot" \
    -DCMAKE_AR="$bin/llvm-ar" -DCMAKE_RANLIB="$bin/llvm-ranlib" -DCMAKE_NM="$bin/llvm-nm" \
    -DCMAKE_C_FLAGS="$flags" -DCMAKE_CXX_FLAGS="$flags" \
    -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld -static-libstdc++ -resource-dir=$resource -Wl,--gc-sections -Wl,--build-id=none" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCROSS_TOOLCHAIN_FLAGS_NATIVE="-DCMAKE_C_COMPILER=gcc;-DCMAKE_CXX_COMPILER=g++;-DCMAKE_MAKE_PROGRAM=$ninja" \
    -DLLVM_ENABLE_PROJECTS="clang;lld" -DLLVM_TARGETS_TO_BUILD=AArch64 \
    -DLLVM_HOST_TRIPLE="$clang_triple" -DLLVM_DEFAULT_TARGET_TRIPLE="$clang_triple" \
    -DLLVM_APPEND_VC_REV=OFF -DLLVM_ENABLE_ASSERTIONS=OFF -DLLVM_ENABLE_BACKTRACES=OFF \
    -DLLVM_ENABLE_ZLIB=OFF -DLLVM_ENABLE_ZSTD=OFF -DLLVM_ENABLE_LIBXML2=OFF -DLLVM_ENABLE_TERMINFO=OFF \
    -DLLVM_ENABLE_LIBEDIT=OFF -DLLVM_ENABLE_LIBPFM=OFF -DLLVM_ENABLE_THREADS=ON \
    -DLLVM_INCLUDE_TESTS=OFF -DLLVM_INCLUDE_EXAMPLES=OFF -DLLVM_INCLUDE_BENCHMARKS=OFF -DLLVM_INCLUDE_DOCS=OFF \
    -DLLVM_BUILD_TOOLS=OFF -DLLVM_INCLUDE_UTILS=OFF \
    -DCLANG_ENABLE_STATIC_ANALYZER=OFF -DCLANG_ENABLE_ARCMT=OFF -DCLANG_PLUGIN_SUPPORT=OFF \
    -DCLANG_BUILD_TOOLS=OFF -DCLANG_INCLUDE_TESTS=OFF -DCLANG_INCLUDE_DOCS=OFF -DCLANG_ENABLE_OBJC_REWRITER=OFF \
    -DCLANG_TOOL_CLANG_BUILD=ON -DCLANG_DEFAULT_CXX_STDLIB=libc++ -DCLANG_DEFAULT_RTLIB=compiler-rt \
    -DCLANG_DEFAULT_LINKER=lld \
    -DLLD_BUILD_TOOLS=ON \
    >"$build/configure.log" || { tail -40 "$build/configure.log"; die "configuring failed"; }

step "Building clang and lld ($jobs jobs)"
"$ninja" -C "$build" -j "$jobs" clang lld >"$build/build.log" ||
    { grep -m5 -B2 -A8 "error" "$build/build.log" | head -60; die "building failed"; }

rm -rf "$out"
mkdir -p "$out/bin" "$out/lib/clang/$llvm_major" "$out/licenses"
"$bin/llvm-strip" --strip-all -o "$out/bin/clang" "$build/bin/clang-$llvm_major"
"$bin/llvm-strip" --strip-all -o "$out/bin/lld" "$build/bin/lld"
cp -r "$build/lib/clang/$llvm_major/include" "$out/lib/clang/$llvm_major/"
cp "$llvm/llvm/LICENSE.TXT" "$out/licenses/LLVM-LICENSE.txt"
"$bin/llvm-readelf" -h -d "$out/bin/clang" | grep -E "Machine|Type:|NEEDED"
ls -la "$out/bin"
sha256sum "$out/bin/clang" "$out/bin/lld"

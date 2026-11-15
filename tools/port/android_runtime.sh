#!/usr/bin/env bash
# The Android parts clang needs to compile the game's code for the Android
# app, which compiles it on the phone from the player's disc
# (host/d012_device_compile.cpp, with the clang of
# tools/port/android_compiler.sh), built from open-source code at pinned
# versions. Nothing comes from the Android NDK download.
#
#   wsl -d Ubuntu-24.04 -- bash tools/port/android_runtime.sh
#
# Makes, in ~/d012-android/rt (D012_ANDROID_RT_WORK):
#
#   out/android-runtime/
#     sysroot/usr/include/           bionic's C library and Linux kernel headers
#                                    (AOSP), libc++'s headers (LLVM)
#     sysroot/usr/lib/aarch64-linux-android/29/
#                                    libc.so, libm.so, libdl.so: link stubs for
#                                    Android 10 (API 29), made from bionic's
#                                    symbol lists; crtbegin_so.o, crtend_so.o (libraries),
#                                    crtbegin_dynamic.o, crtend_android.o (programs)
#     sysroot/usr/lib/aarch64-linux-android/
#                                    libc++.a (with libc++abi), libunwind.a
#     clang/lib/aarch64-unknown-linux-android29/libclang_rt.builtins.a
#                                    compiler-rt's builtins, which go in clang's
#                                    own folder (lib/clang/<N>/)
#     licenses/                      bionic's NOTICE files, LLVM's licence
#     SOURCES.txt                    what each part was made from
#   dist/android-runtime-<llvm>-<aosp>.zip
#                                    the same, with fixed dates and order, so
#                                    the same sources give the same zip
#
# Sources, each checked against its pinned hash:
#   AOSP platform/bionic and platform/build/soong at $aosp_tag (git commits);
#     soong's ndkstubgen makes the stubs and is not shipped
#   LLVM's llvm-project-$llvm_version source release
#   LLVM-$build_llvm_version-Linux-X64, the clang that builds them (the one the
#     Linux setup ships, tools/port/linux_toolchain.sh)
#
# Kept to 23.x: the runtimes are built from the same LLVM as the Windows
# setup's clang (llvm-mingw 20260922, clang 23.1.2), and clang looks for the
# builtins under its major version.
set -euo pipefail

work="${D012_ANDROID_RT_WORK:-$HOME/d012-android/rt}"
api=29
triple=aarch64-linux-android
clang_triple=aarch64-unknown-linux-android$api

aosp_tag=android-16.0.0_r4
bionic_commit=f22516cbc67e81c13cf943ce02f01b8929a98726
soong_commit=f389fa2a2a768a93bc99957e2288f3fbee032bff

llvm_version=23.1.2
llvm_archive="llvm-project-$llvm_version.src.tar.xz"
llvm_url="https://github.com/llvm/llvm-project/releases/download/llvmorg-$llvm_version/$llvm_archive"
llvm_sha256=c98bbef08a2b4c2613cd50e9aa9ae7b69b1fe6c16b2c40373bc0ab6116fdf78a

build_llvm_version=23.1.4
build_llvm_major=23
build_archive="LLVM-$build_llvm_version-Linux-X64.tar.xz"
build_url="https://github.com/llvm/llvm-project/releases/download/llvmorg-$build_llvm_version/$build_archive"
build_sha256=25d814fda50d5fb0084b42be09605d8b1595d49d5e2e03a3345d7e112ff6f9f3

repo="$(cd "$(dirname "$0")/../.." && pwd)"
src="$work/src"
tools="$work/tools"
out="$work/out/android-runtime"
sysroot="$out/sysroot"
build="$work/build"

step() { printf '\n=== %s\n' "$*"; }
die() { echo "error: $*" >&2; exit 1; }

# A file downloaded once and kept while it matches its hash.
fetch() {
    local url=$1 file=$2 sha=$3
    if ! echo "$sha  $file" | sha256sum -c --status 2>/dev/null; then
        step "Downloading $(basename "$file")"
        curl -fL -o "$file.part" "$url"
        echo "$sha  $file.part" | sha256sum -c --status || die "$url does not match its pinned SHA-256"
        mv "$file.part" "$file"
    fi
}

# An AOSP project at the pinned tag, checked to be the pinned commit.
fetch_git() {
    local project=$1 dir=$2 commit=$3
    if [ "$(git -C "$dir" rev-parse HEAD 2>/dev/null)" != "$commit" ]; then
        step "Fetching $project at $aosp_tag"
        rm -rf "$dir"
        git -c advice.detachedHead=false clone -q --depth 1 --branch "$aosp_tag" \
            "https://android.googlesource.com/$project" "$dir"
    fi
    [ "$(git -C "$dir" rev-parse HEAD)" = "$commit" ] || die "$project at $aosp_tag is not commit $commit"
    [ -z "$(git -C "$dir" status --porcelain)" ] || die "$dir has local changes"
}

for tool in curl git python3 tar xz sha256sum; do
    command -v "$tool" >/dev/null || die "$tool is needed (apt install $tool)"
done
cmake="$(command -v cmake || echo "$HOME/d012-android/bin/cmake")"
ninja="$(command -v ninja || echo "$HOME/d012-android/bin/ninja")"
[ -x "$cmake" ] && [ -x "$ninja" ] || die "cmake and ninja are needed"

mkdir -p "$src" "$tools"
fetch_git platform/bionic "$src/bionic" "$bionic_commit"
fetch_git platform/build/soong "$src/soong" "$soong_commit"
fetch "$llvm_url" "$src/$llvm_archive" "$llvm_sha256"
fetch "$build_url" "$src/$build_archive" "$build_sha256"

llvm="$src/llvm-project-$llvm_version.src"
if [ ! -f "$llvm/.unpacked" ]; then
    step "Unpacking the runtimes' sources from $llvm_archive"
    rm -rf "$llvm"
    top="llvm-project-$llvm_version.src"
    tar -xJf "$src/$llvm_archive" -C "$src" \
        "$top/cmake" "$top/llvm/cmake" "$top/llvm/utils/llvm-lit" "$top/llvm/LICENSE.TXT" "$top/runtimes" \
        "$top/libcxx" "$top/libcxxabi" "$top/libunwind" "$top/compiler-rt" "$top/third-party" "$top/libc"
    touch "$llvm/.unpacked"
fi

bin="$tools/LLVM-$build_llvm_version/bin"
if [ ! -x "$bin/clang" ]; then
    step "Unpacking clang $build_llvm_version to build with"
    rm -rf "$tools/LLVM-$build_llvm_version"
    mkdir -p "$tools/LLVM-$build_llvm_version"
    top="LLVM-$build_llvm_version-Linux-X64"
    tar -xJf "$src/$build_archive" -C "$tools/LLVM-$build_llvm_version" --strip-components=1 \
        "$top/bin/clang-$build_llvm_major" "$top/bin/clang" "$top/bin/clang++" "$top/bin/lld" "$top/bin/ld.lld" \
        "$top/bin/llvm-ar" "$top/bin/llvm-ranlib" "$top/bin/llvm-nm" "$top/bin/llvm-readobj" \
        "$top/bin/llvm-readelf" "$top/bin/llvm-objcopy" "$top/bin/llvm-strip" \
        "$top/lib/clang/$build_llvm_major/include"
fi
cc=("$bin/clang" "--target=$clang_triple" "--sysroot=$sysroot")

rm -rf "$out" "$build"
mkdir -p "$sysroot/usr/include" "$sysroot/usr/lib/$triple/$api" "$out/licenses" "$build"
lib="$sysroot/usr/lib/$triple"

step "Headers from bionic"
# As bionic's ndk_headers rules lay them out (libc/Android.bp).
bionic="$src/bionic"
cp -r "$bionic/libc/include/." "$sysroot/usr/include/"
for dir in asm-generic drm linux misc mtd rdma scsi sound video xen; do
    cp -r "$bionic/libc/kernel/uapi/$dir" "$sysroot/usr/include/"
done
cp -r "$bionic/libc/kernel/android/uapi/linux/." "$sysroot/usr/include/linux/"
cp -r "$bionic/libc/kernel/android/scsi/scsi/." "$sysroot/usr/include/scsi/"
mkdir -p "$sysroot/usr/include/$triple"
cp -r "$bionic/libc/kernel/uapi/asm-arm64/." "$sysroot/usr/include/$triple/"
# Only headers: the folders also hold build files and notes.
find "$sysroot/usr/include" -type f ! -name '*.h' -delete
# The kernel's firewall (netfilter) headers have names that differ only in
# case (xt_CONNMARK.h, xt_connmark.h), which a Windows disk cannot hold. The
# game's code uses none of them.
rm -rf "$sysroot/usr/include/linux/netfilter" "$sysroot/usr/include/linux/netfilter_ipv4" \
    "$sysroot/usr/include/linux/netfilter_ipv6"
same_name=$(cd "$sysroot" && find . | tr '[:upper:]' '[:lower:]' | sort | uniq -d)
[ -z "$same_name" ] || die "names that differ only in case: $same_name"

step "Link stubs for Android $api: libc, libm, libdl"
printf '{"G": 9, "I": 14, "J": 16, "J-MR1": 17, "J-MR2": 18, "K": 19, "L": 21, "L-MR1": 22, "M": 23, "N": 24, "N-MR1": 25, "O": 26, "O-MR1": 27, "P": 28, "Q": 29, "R": 30, "S": 31, "S-V2": 32, "Tiramisu": 33, "UpsideDownCake": 34, "VanillaIceCream": 35, "Baklava": 36}\n' \
    >"$build/api_levels.json"
for name in libc libm libdl; do
    case $name in libc) map="$bionic/libc/libc.map.txt" ;; *) map="$bionic/${name}/${name}.map.txt" ;; esac
    python3 -I -c 'import sys; sys.path.insert(0, sys.argv[1]); sys.argv = sys.argv[1:]; import ndkstubgen; ndkstubgen.main()' \
        "$src/soong/cc" --arch arm64 --api "$api" --api-map "$build/api_levels.json" \
        "$map" "$build/$name.c" "$build/$name.map" "$build/$name.abi.txt"
    # The flags soong's ndk_library gives stubs (cc/ndk_library.go).
    "${cc[@]}" -c -O2 -fPIC -fno-builtin -fno-unwind-tables -fvisibility=default \
        -Wno-incompatible-library-redeclaration -Wno-incomplete-setjmp-declaration \
        -Wno-builtin-requires-header -Wno-invalid-noreturn -w "$build/$name.c" -o "$build/$name.o"
    "${cc[@]}" -shared -nostdlib -fuse-ld=lld -Wl,--hash-style=both -Wl,-soname,"$name.so" \
        -Wl,--version-script,"$build/$name.map" -Wl,-z,max-page-size=16384 \
        "$build/$name.o" -o "$lib/$api/$name.so"
done

step "crtbegin_so.o, crtend_so.o"
crt=("${cc[@]}" -c -O2 -fPIC -I"$bionic/libc" -I"$bionic/libc/bionic" -I"$bionic/libc/private"
    -DPLATFORM_SDK_VERSION="$api" -Wno-gcc-compat)
common="$bionic/libc/arch-common/bionic"
"${crt[@]}" "$common/crtbrand.S" -o "$build/crtbrand.o"
"${crt[@]}" "$common/crtbegin_so.c" -o "$build/crtbegin_so_c.o"
"$bin/ld.lld" -r "$build/crtbegin_so_c.o" "$build/crtbrand.o" -o "$lib/$api/crtbegin_so.o"
"${crt[@]}" "$common/crtend_so.S" -o "$lib/$api/crtend_so.o"
# For programs (the compiler the app runs, tools/port/android_compiler.sh).
"${crt[@]}" "$common/crtbegin.c" -o "$build/crtbegin_dynamic_c.o"
"$bin/ld.lld" -r "$build/crtbegin_dynamic_c.o" "$build/crtbrand.o" -o "$lib/$api/crtbegin_dynamic.o"
"${crt[@]}" "$common/crtend.S" -o "$lib/$api/crtend_android.o"

step "compiler-rt builtins, libc++, libc++abi, libunwind from LLVM $llvm_version"
flags="-O2 -fPIC -ffunction-sections -fdata-sections"
"$cmake" -G Ninja -S "$llvm/runtimes" -B "$build/runtimes" -DCMAKE_MAKE_PROGRAM="$ninja" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_SYSTEM_NAME=Linux -DANDROID=ON -DCMAKE_SYSTEM_PROCESSOR=aarch64 \
    -DCMAKE_C_COMPILER="$bin/clang" -DCMAKE_CXX_COMPILER="$bin/clang++" -DCMAKE_ASM_COMPILER="$bin/clang" \
    -DCMAKE_C_COMPILER_TARGET="$clang_triple" -DCMAKE_CXX_COMPILER_TARGET="$clang_triple" \
    -DCMAKE_ASM_COMPILER_TARGET="$clang_triple" -DCMAKE_SYSROOT="$sysroot" \
    -DCMAKE_AR="$bin/llvm-ar" -DCMAKE_RANLIB="$bin/llvm-ranlib" -DCMAKE_NM="$bin/llvm-nm" \
    -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
    -DCMAKE_C_FLAGS="$flags" -DCMAKE_CXX_FLAGS="$flags" -DCMAKE_ASM_FLAGS="$flags" \
    -DCMAKE_INSTALL_PREFIX="$build/install" \
    -DLLVM_ENABLE_RUNTIMES="compiler-rt;libcxx;libcxxabi;libunwind" \
    -DLLVM_ENABLE_PER_TARGET_RUNTIME_DIR=ON -DLLVM_DEFAULT_TARGET_TRIPLE="$clang_triple" \
    -DLLVM_INCLUDE_TESTS=OFF -DLLVM_INCLUDE_DOCS=OFF \
    -DCOMPILER_RT_DEFAULT_TARGET_ONLY=ON -DCOMPILER_RT_BUILD_BUILTINS=ON \
    -DCOMPILER_RT_BUILD_CRT=OFF -DCOMPILER_RT_BUILD_SANITIZERS=OFF -DCOMPILER_RT_BUILD_XRAY=OFF \
    -DCOMPILER_RT_BUILD_LIBFUZZER=OFF -DCOMPILER_RT_BUILD_PROFILE=OFF -DCOMPILER_RT_BUILD_MEMPROF=OFF \
    -DCOMPILER_RT_BUILD_ORC=OFF -DCOMPILER_RT_BUILD_CTX_PROFILE=OFF -DCOMPILER_RT_INCLUDE_TESTS=OFF \
    -DLIBCXX_ENABLE_SHARED=OFF -DLIBCXXABI_ENABLE_SHARED=OFF -DLIBUNWIND_ENABLE_SHARED=OFF \
    -DLIBCXX_CXX_ABI=libcxxabi -DLIBCXX_ENABLE_STATIC_ABI_LIBRARY=ON \
    -DLIBCXXABI_USE_LLVM_UNWINDER=ON -DLIBCXXABI_ENABLE_STATIC_UNWINDER=OFF \
    -DLIBCXX_USE_COMPILER_RT=ON -DLIBCXXABI_USE_COMPILER_RT=ON -DLIBUNWIND_USE_COMPILER_RT=ON \
    -DLIBCXX_HERMETIC_STATIC_LIBRARY=ON -DLIBCXXABI_HERMETIC_STATIC_LIBRARY=ON -DLIBUNWIND_HIDE_SYMBOLS=ON \
    -DLIBCXX_INCLUDE_TESTS=OFF -DLIBCXX_INCLUDE_BENCHMARKS=OFF -DLIBCXXABI_INCLUDE_TESTS=OFF \
    -DLIBUNWIND_INCLUDE_TESTS=OFF -DLIBCXX_INSTALL_MODULES=OFF \
    -DLIBUNWIND_HAS_PTHREAD_LIB=OFF -DLIBCXXABI_HAS_PTHREAD_LIB=OFF -DLIBCXX_HAS_PTHREAD_LIB=OFF \
    -DLIBCXX_HAS_RT_LIB=OFF -DLIBCXX_HAS_ATOMIC_LIB=OFF \
    >"$build/runtimes-configure.log" || { tail -40 "$build/runtimes-configure.log"; die "configuring the runtimes failed"; }
"$ninja" -C "$build/runtimes" install >"$build/runtimes-build.log" ||
    { tail -40 "$build/runtimes-build.log"; die "building the runtimes failed"; }

i="$build/install"
cp -r "$i/include/c++" "$sysroot/usr/include/"
[ -d "$i/include/$clang_triple/c++" ] && cp -r "$i/include/$clang_triple/c++/v1/." "$sysroot/usr/include/c++/v1/"
cp "$i/lib/$clang_triple/libc++.a" "$i/lib/$clang_triple/libunwind.a" "$lib/"
builtins=$(find "$i/lib" -name 'libclang_rt.builtins*.a' | head -1)
[ -n "$builtins" ] || die "no builtins library was made"
mkdir -p "$out/clang/lib/$clang_triple"
cp "$builtins" "$out/clang/lib/$clang_triple/libclang_rt.builtins.a"
"$bin/llvm-strip" --strip-debug "$lib/libc++.a" "$lib/libunwind.a" "$out/clang/lib/$clang_triple/libclang_rt.builtins.a"
# Android has pthreads in libc: an archive asking the linker for libpthread
# (or librt, libatomic) would fail to link.
for archive in "$lib/libc++.a" "$lib/libunwind.a" "$out/clang/lib/$clang_triple/libclang_rt.builtins.a"; do
    wants=$("$bin/llvm-readelf" -p .deplibs "$archive" 2>/dev/null | sed -n 's/^ *\[ *[0-9a-f]*\] //p' | sort -u |
        grep -v -x -e c -e m -e dl || true)
    [ -z "$wants" ] || die "$(basename "$archive") asks the linker for: $wants"
done

step "Licences"
cp "$bionic/libc/NOTICE" "$out/licenses/bionic-libc-NOTICE.txt"
cp "$bionic/libm/NOTICE" "$out/licenses/bionic-libm-NOTICE.txt"
cp "$bionic/libdl/NOTICE" "$out/licenses/bionic-libdl-NOTICE.txt"
cp "$llvm/llvm/LICENSE.TXT" "$out/licenses/LLVM-LICENSE.txt"
for project in libcxx libcxxabi libunwind compiler-rt; do
    [ -f "$llvm/$project/LICENSE.TXT" ] && cp "$llvm/$project/LICENSE.TXT" "$out/licenses/$project-LICENSE.txt"
done
cat >"$out/SOURCES.txt" <<EOF
Android parts for compiling the game's code for the Android app, API $api, arm64.
Built by tools/port/android_runtime.sh from:

  bionic (headers, libc/libm/libdl stubs, crt objects)
    https://android.googlesource.com/platform/bionic  $aosp_tag  $bionic_commit
  ndkstubgen, which made the stubs (not included)
    https://android.googlesource.com/platform/build/soong  $aosp_tag  $soong_commit
  libc++, libc++abi, libunwind, compiler-rt builtins (and libc++'s headers)
    $llvm_url
    SHA-256 $llvm_sha256
  compiled by clang $build_llvm_version
    $build_url
    SHA-256 $build_sha256

Licences: licenses/. bionic: BSD-style and Apache 2.0 (its NOTICE files);
the Linux kernel headers: GPL-2.0 WITH Linux-syscall-note (in the NOTICE);
LLVM: Apache 2.0 with LLVM exceptions.

clang/ goes in clang's own folder, lib/clang/<major>/.
EOF

step "The zip"
mkdir -p "$work/dist"
zip="$work/dist/android-runtime-$llvm_version-$aosp_tag.zip"
python3 -I - "$out" "$zip" <<'PY'
import os, sys, zipfile
root, target = sys.argv[1], sys.argv[2]
files = []
for dirpath, dirnames, filenames in os.walk(root):
    for name in filenames:
        full = os.path.join(dirpath, name)
        files.append((os.path.relpath(full, root).replace(os.sep, "/"), full))
files.sort()
with zipfile.ZipFile(target + ".part", "w", zipfile.ZIP_DEFLATED, compresslevel=9) as z:
    for name, full in files:
        info = zipfile.ZipInfo("android-runtime/" + name, date_time=(2026, 1, 1, 0, 0, 0))
        info.compress_type = zipfile.ZIP_DEFLATED
        info.external_attr = 0o644 << 16
        with open(full, "rb") as f:
            z.writestr(info, f.read())
os.replace(target + ".part", target)
PY
du -sh "$out"
ls -la "$zip"
sha256sum "$zip"

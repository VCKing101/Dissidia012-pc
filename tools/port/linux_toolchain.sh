#!/usr/bin/env bash
# The compiler the Linux setup ships, for `D012Native --compile-game` on the
# player's machine (port/host/d012_compile_game.cpp): clang and lld from an
# official LLVM release, and a sysroot from the Steam Runtime 3 "sniper" SDK
# image the program is built in, so the game's code needs what the program
# needs (glibc 2.31) and links its own copy of the same libstdc++ (GCC 14).
#
#   wsl -d Ubuntu-24.04 -- bash tools/port/linux_toolchain.sh
#
# Makes ~/d012-linux/toolchain (D012_LINUX_WORK):
#
#   bin/clang, clang++, ld.lld   clang-<N> and lld, stripped
#   bin/clang++.cfg              --target and --sysroot, read by clang++ itself
#   lib/clang/<N>/include        clang's own headers
#   sysroot/                     glibc's, Linux's and libcrypt's headers and
#                                link files; GCC 14's libgcc and libstdc++ (static)
#                                and its headers, in the layout clang looks for
#   licenses/                    LLVM's licence, and where the sysroot is from
#
# Nothing of the game is in it. SteamOS has no development headers, which is
# why the sysroot comes along.
set -euo pipefail

root="${D012_LINUX_WORK:-$HOME/d012-linux}"
sources="$root/sources"
llvm_version=23.1.4
llvm_major=23
# The .tar.xz: neither WSL nor the SDK image has zstd for the .tar.zst.
llvm_archive="LLVM-$llvm_version-Linux-X64.tar.xz"
llvm_url="https://github.com/llvm/llvm-project/releases/download/llvmorg-$llvm_version/$llvm_archive"
llvm_sha256=25d814fda50d5fb0084b42be09605d8b1595d49d5e2e03a3345d7e112ff6f9f3
# The release's archive has no licence file: LLVM's own, from the same tag.
llvm_license_url="https://raw.githubusercontent.com/llvm/llvm-project/llvmorg-$llvm_version/llvm/LICENSE.TXT"
llvm_license_sha256=8d85c1057d742e597985c7d4e6320b015a9139385cff4cbae06ffc0ebe89afee
out="$root/toolchain"
repo="$(cd "$(dirname "$0")/../.." && pwd)"

step() { printf '\n=== %s\n' "$*"; }

mkdir -p "$sources"
if ! echo "$llvm_sha256  $sources/$llvm_archive" | sha256sum -c --status 2>/dev/null; then
    step "Downloading $llvm_archive"
    curl -fL -o "$sources/$llvm_archive.part" "$llvm_url"
    echo "$llvm_sha256  $sources/$llvm_archive.part" | sha256sum -c --status ||
        { echo "error: $llvm_url does not match its pinned SHA-256" >&2; exit 1; }
    mv "$sources/$llvm_archive.part" "$sources/$llvm_archive"
fi
llvm_license="$sources/LLVM-$llvm_version-LICENSE.TXT"
if ! echo "$llvm_license_sha256  $llvm_license" | sha256sum -c --status 2>/dev/null; then
    curl -fsSL -o "$llvm_license.part" "$llvm_license_url"
    echo "$llvm_license_sha256  $llvm_license.part" | sha256sum -c --status ||
        { echo "error: $llvm_license_url does not match its pinned SHA-256" >&2; exit 1; }
    mv "$llvm_license.part" "$llvm_license"
fi

rm -rf "$out"
mkdir -p "$out/bin" "$out/lib/clang" "$out/sysroot" "$out/licenses"

step "clang and lld from LLVM $llvm_version"
top="LLVM-$llvm_version-Linux-X64"
tar -xJf "$sources/$llvm_archive" -C "$out" \
    "$top/bin/clang-$llvm_major" "$top/bin/lld" "$top/lib/clang/$llvm_major/include"
mv "$out/$top/bin/clang-$llvm_major" "$out/$top/bin/lld" "$out/bin/"
mv "$out/$top/lib/clang/$llvm_major" "$out/lib/clang/"
cp "$llvm_license" "$out/licenses/LLVM-LICENSE.txt"
rm -rf "${out:?}/$top"
strip --strip-unneeded "$out/bin/clang-$llvm_major" "$out/bin/lld"
ln -s "clang-$llvm_major" "$out/bin/clang"
ln -s "clang-$llvm_major" "$out/bin/clang++"
ln -s lld "$out/bin/ld.lld"
# clang++ reads clang++.cfg beside it (clang's "default configuration file");
# <CFGDIR> is that file's directory, so the folder can be moved.
cat > "$out/bin/clang++.cfg" <<'EOF'
--target=x86_64-linux-gnu
--sysroot=<CFGDIR>/../sysroot
EOF

step "The sysroot, from the SDK image"
# shellcheck source=/dev/null
source "$root/src/PortableKit/packaging/sources.sh"
# The packages' own files (not /usr/share), and GCC 14's from its prefix
# (/usr/lib/gcc-14 in this image) moved to where clang finds a GCC.
docker run --rm --user "$(id -u):$(id -g)" -v "$out/sysroot:/out" "$SDK_IMAGE" bash -euo pipefail -c '
    for p in libc6:amd64 libc6-dev:amd64 linux-libc-dev:amd64 libcrypt-dev:amd64 libcrypt1:amd64 libgcc-s1:amd64; do
        dpkg -L "$p"
    done | grep -E "^/(usr/)?(lib|lib64|include)/|^/usr/include/" | grep -vE "/gconv/|/audit/|\.a$" |
        while read -r f; do [[ -f "$f" || -L "$f" ]] && echo "${f#/}"; done | sort -u > /tmp/files
    # The static archives a link can name: libc_nonshared.a (libc.so names
    # it) and none of the rest.
    echo usr/lib/x86_64-linux-gnu/libc_nonshared.a >> /tmp/files
    tar -C / -cf - -T /tmp/files | tar -C /out -xf -
    gcc=usr/lib/gcc-14/lib/gcc/x86_64-linux-gnu/14
    mkdir -p /out/usr/lib/gcc/x86_64-linux-gnu/14 /out/usr/include/c++
    # This GCC (configured --disable-shared) has its unwinder in libgcc.a and
    # no libgcc_eh.a, which clang names for -static-libgcc: an empty archive
    # stands in for it.
    for f in crtbegin.o crtbeginS.o crtend.o crtendS.o libgcc.a libstdc++.a libsupc++.a; do
        cp "/$gcc/$f" /out/usr/lib/gcc/x86_64-linux-gnu/14/
    done
    if [[ -f "/$gcc/libgcc_eh.a" ]]; then
        cp "/$gcc/libgcc_eh.a" /out/usr/lib/gcc/x86_64-linux-gnu/14/
    else
        printf "!<arch>\n" > /out/usr/lib/gcc/x86_64-linux-gnu/14/libgcc_eh.a
    fi
    cp -r "/$gcc/include" /out/usr/lib/gcc/x86_64-linux-gnu/14/
    cp -r /usr/lib/gcc-14/x86_64-linux-gnu/include/c++/14 /out/usr/include/c++/14
    # libgcc_s for -shared links that ask for it (libgcc_s.so is a linker script in GCC).
    ln -sf /lib/x86_64-linux-gnu/libgcc_s.so.1 /out/usr/lib/gcc/x86_64-linux-gnu/14/libgcc_s.so
    # Links to absolute paths (/lib64/ld-linux-x86-64.so.2 is one) made
    # relative, so they stay inside the sysroot wherever it is.
    find /out -type l | while read -r link; do
        target="$(readlink "$link")"
        if [[ "$target" == /* ]]; then ln -sfr "/out$target" "$link"; fi
    done
    dpkg-query -W -f="\${Package} \${Version}\n" libc6:amd64 libc6-dev:amd64 linux-libc-dev:amd64 \
        libcrypt-dev:amd64 libgcc-s1:amd64 libgcc-14-dev:amd64 libstdc++-14-dev:amd64 > /out/PACKAGES.txt
'
cat > "$out/licenses/sysroot-SOURCE.txt" <<EOF
The sysroot/ folder holds files of these Debian packages, unmodified, from the
Steam Runtime 3 "sniper" SDK image $SDK_IMAGE
(source packages: https://repo.steampowered.com/steamrt3/images/ and
https://repo.steampowered.com/steamrt3/apt/), for compiling the game's code:

$(cat "$out/sysroot/PACKAGES.txt")

glibc and libcrypt are LGPL-2.1-or-later; the Linux headers are GPL-2.0 with the
syscall note; libgcc and libstdc++ are GPL-3.0 with the GCC Runtime Library
Exception. Their licences are in /usr/share/doc/<package>/copyright of that image.
EOF
mv "$out/sysroot/PACKAGES.txt" "$out/licenses/sysroot-PACKAGES.txt"

step "Checking it"
cat > "$root/toolchain-test.cpp" <<'EOF'
#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>
extern "C" __attribute__((visibility("default"))) int d012_toolchain_test(int n) {
    std::vector<std::string> words(n, "x");
    try {
        if (n < 0) throw std::runtime_error("negative");
    } catch (const std::exception &) {
        return -1;
    }
    std::printf("%zu %f\n", words.size(), std::sin(0.5));
    return static_cast<int>(words.size());
}
EOF
"$out/bin/clang++" -std=c++20 -O1 -fPIC -fvisibility=hidden -c "$root/toolchain-test.cpp" -o "$root/toolchain-test.o"
"$out/bin/clang++" -shared -fuse-ld=lld -static-libgcc -static-libstdc++ -Wl,--exclude-libs,ALL \
    -o "$root/toolchain-test.so" "$root/toolchain-test.o"
objdump -p "$root/toolchain-test.so" | awk '$1 == "NEEDED"'
objdump -T "$root/toolchain-test.so" | grep -o 'GLIBC_[0-9.]*' | sort -uV | tail -1
du -sh "$out" "$out/bin" "$out/sysroot" "$out/lib"
echo "toolchain in $out"

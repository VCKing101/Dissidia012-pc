# Third-party notices (Android app)

The Dissidia 012 port is distributed under the MIT License (`Game-LICENSE.txt`, the port; `PortableKit-LICENSE.txt`,
the framework it is built on, with the port's patches, which are MIT too). The app also contains the third-party
software listed here, each under its own license. The license texts are in the app's `assets/licenses` folder.

The app does not include the game, its assets or anything made from them. You provide the disc image of a copy you
own; what the app makes from it on your phone (the decrypted executable, the recompiled code and the libraries
compiled from it) stays in the app's own storage and is for your own use only. The program contains the PSP's
published fixed keys, as PortableKit's ports do, and the published key for the game's executable, so it can read
your disc.

## Compiled into the app

### PortableKit

The recompiler (`libpsp_recomp.so`), the runtime and the PSP system around the game. MIT License:
`PortableKit-LICENSE.txt`. <https://github.com/TeamGDB/PortableKit>, with the patches in the port's repository
(`port/portablekit-patches`).

### Dear ImGui

The port's menus and touch controls. Copyright (c) 2014-2026 Omar Cornut. MIT License: `DearImGui-LICENSE.txt`.
<https://github.com/ocornut/imgui>

### tiny-AES-c

AES for reading the disc's executable and for save data. Released into the public domain under the Unlicense:
`tiny-AES-c-UNLICENSE.txt`. <https://github.com/kokke/tiny-AES-c>

### stb_truetype, stb_image and stb_image_write

Text drawing and the PNG images of texture packs. Copyright (c) 2017 Sean Barrett. Used under the public-domain
dedication they offer as an alternative to the MIT License. <https://github.com/nothings/stb>

### xxHash

Texture hashes. Copyright (c) 2012-2021 Yann Collet. BSD 2-Clause License: `xxHash-LICENSE.txt`.
<https://github.com/Cyan4973/xxHash>

### Goudy Bookletter 1911

The serif of the titles on the save, load and keyboard screens. Copyright (c) 2010 Barry Schwartz. SIL Open Font
License 1.1: `GoudyBookletter1911-OFL.txt`. <https://fonts.google.com/specimen/Goudy+Bookletter+1911>

## Shipped as separate libraries

### SDL3 3.4.16

The app's window, input, audio output and Android activity. Copyright (C) 1997-2026 Sam Lantinga. zlib License:
`SDL3-LICENSE.txt`. `libSDL3.so` is built unmodified from the 3.4.16 source release,
<https://github.com/libsdl-org/SDL/releases/tag/release-3.4.16>.

### FFmpeg 7.1.5

Decodes the game's ATRAC3plus audio and movies, and the music files of battle music mods. `libavcodec.so`,
`libavformat.so`, `libavutil.so` and `libswresample.so` are built unmodified from FFmpeg's source as LGPL shared
libraries, licensed under the GNU Lesser General Public License, version 2.1 or later: `FFmpeg-COPYING.LGPLv2.1.txt`.
Where the source is and how it was built: `FFmpeg-SOURCE.txt`. <https://ffmpeg.org/>

### Noto Sans CJK JP

The fallback font for text the game's own fonts do not cover. Copyright 2014-2021 Adobe. SIL Open Font License 1.1:
`NotoSansCJK-OFL.txt`. <https://github.com/notofonts/noto-cjk>

## The compiler the app compiles the game's code with

The app makes the game's code on your phone from your disc, with a compiler it carries. All of it is built from
source at fixed versions (where from: `SOURCES.txt`); none of it is taken from Google's Android NDK.

### LLVM 23.1.2: clang, lld, libc++, libc++abi, libunwind and compiler-rt

`libclang.so` and `liblld.so` are clang and lld, built to run on Android, and the compiler's files in the app's
`toolchain.tar` hold clang's headers, libc++ (with libc++abi), libunwind and compiler-rt's builtins. Apache License 2.0
with LLVM Exceptions: `LLVM-LICENSE.txt`, `libcxx-LICENSE.txt`, `libcxxabi-LICENSE.txt`, `libunwind-LICENSE.txt`,
`compiler-rt-LICENSE.txt`. <https://llvm.org/>

### bionic, Android's C library

Its headers, the link stubs of `libc.so`, `libm.so` and `libdl.so`, and `crtbegin`/`crtend`, from the Android Open
Source Project (in `toolchain.tar`). BSD-style licenses and the Apache License 2.0; the Linux kernel headers in it are
GPL-2.0 with the Linux syscall note: `bionic-libc-NOTICE.txt`, `bionic-libm-NOTICE.txt`, `bionic-libdl-NOTICE.txt`.
<https://android.googlesource.com/platform/bionic>

## Not shipped

Vulkan and the C library the app runs on are the phone's own.

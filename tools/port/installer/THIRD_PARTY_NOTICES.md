# Third-party notices

The Dissidia 012 PC port is distributed under the MIT License (`LICENSE.txt`, the port; `PortableKit-LICENSE.txt`,
the framework it is built on, with the port's patches, which are MIT too). The installed folder also contains the
third-party software listed here, each under its own license. The license texts are in this `licenses` folder.

The port does not include the game, its assets or anything made from them. You provide the disc image of a copy you
own; what Setup makes from it (the decrypted executable, the recompiled code and the libraries compiled from it)
stays on your PC and is for your own use only. The program contains the PSP's published fixed keys, as PortableKit's
ports do, and the published key for the game's executable, so it can read your disc.

## Compiled into the program

### PortableKit

The recompiler (`psp_recomp.exe`), the runtime and the PSP system around the game. MIT License:
`PortableKit-LICENSE.txt`. <https://github.com/TeamGDB/PortableKit>, with the patches in the port's repository
(`port/portablekit-patches`).

### Dear ImGui 1.92.9b

The port's menus. Copyright (c) 2014-2026 Omar Cornut. MIT License: `DearImGui-LICENSE.txt`.
<https://github.com/ocornut/imgui>

### tiny-AES-c

AES for reading the disc's executable and for save data. Released into the public domain under the Unlicense:
`tiny-AES-c-UNLICENSE.txt`. <https://github.com/kokke/tiny-AES-c>

### stb_truetype 1.26, stb_image 2.30 and stb_image_write 1.16

Text drawing and the PNG images of texture packs. Copyright (c) 2017 Sean Barrett. Used under the public-domain
dedication they offer as an alternative to the MIT License. <https://github.com/nothings/stb>

### xxHash 0.8.3

Texture hashes. Copyright (c) 2012-2021 Yann Collet. BSD 2-Clause License: `xxHash-LICENSE.txt`.
<https://github.com/Cyan4973/xxHash>

## Shipped as separate libraries and programs

Dynamically linked and unmodified: you may replace them with your own builds of the same or a compatible version.

### SDL3 3.4.18

Window, input and audio output. Copyright (C) 1997-2026 Sam Lantinga. zlib License: `SDL3-LICENSE.txt`.
`SDL3.dll` is the unmodified release build from `SDL3-devel-3.4.18-VC.zip`,
<https://github.com/libsdl-org/SDL/releases/tag/release-3.4.18>.

### FFmpeg 7.1.5

Decodes the game's ATRAC3plus audio and movies, and the music files of battle music mods. `avcodec-61.dll`,
`avformat-61.dll`, `avutil-59.dll` and `swresample-5.dll` are an unmodified LGPL shared build from
BtbN/FFmpeg-Builds, licensed under the GNU Lesser General Public License, version 3 or later:
`FFmpeg-LICENSE.txt`. Where the build and its source are: `FFmpeg-SOURCE.txt`. <https://ffmpeg.org/>

With the optional Battle BGM Maker, also FFmpeg's programs `ffmpeg.exe` and `ffprobe.exe` and the libraries
`avdevice-61.dll`, `avfilter-10.dll` and `swscale-8.dll` they need, from the same unmodified LGPL build.

### Python 3.14.8 (with the optional Battle BGM Maker only)

Runs the Battle BGM Maker, in `bgm-maker\python\`: the unmodified Windows embeddable package from python.org
(<https://www.python.org/downloads/release/python-3148/>). Copyright (c) 2001 Python Software Foundation, under the
PSF License Agreement, with the licenses of the components it includes: `Python-LICENSE.txt`.

### llvm-mingw 20260922

The compiler Setup compiles the game's code with, in `toolchain\`, trimmed to what that needs: clang and lld from
LLVM 23.1.2, libc++, libunwind and the mingw-w64 headers and runtime. `libc++.dll` and `libunwind.dll` beside the
program are from it too. LLVM, clang, lld, libc++ and libunwind: Apache License 2.0 with LLVM Exceptions,
`llvm-mingw-LICENSE.txt`. mingw-w64: the licenses in `mingw-w64-COPYING*.txt` (public domain, ZPL and other
permissive licenses). <https://github.com/mstorsjo/llvm-mingw/releases/tag/20260922>

## Not shipped

Vulkan is the system's own `vulkan-1.dll`, installed with the graphics driver.

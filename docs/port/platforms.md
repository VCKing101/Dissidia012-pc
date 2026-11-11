# More platforms: macOS, Linux, Android, Switch

> Written 5 November 2026. A study only: nothing here is built or run. It sizes the work of taking the native port
> beyond Windows (and the Deck under Proton) to native Linux, macOS, Android and a homebrew Switch build.

Conventions as in the other port notes: "V:" verified here (by the code or a search of it), "I:" inference, "D:"
taken from PortableKit's or Yakumo's documents without being re-checked.

## Summary

| Platform | Framework support | Our own work | Main blocker | Rough size |
| --- | --- | --- | --- | --- |
| **Linux (native)** | Ready: release script, Steam Runtime container, Flatpak | Small | The installer must ship a clang + sysroot (SteamOS has no compiler) | 1-2 weeks |
| **macOS** | Ready: `.app` + MoltenVK release script, the framework's own test Mac | Small to medium | Needs an Apple Silicon Mac to build and test on; signing | 2-3 weeks, with a Mac |
| **Android** | Partly: APK packaging, touch overlay, Steam-Deck-style motion | Medium | Where the game's code is compiled (not on the phone); real-phone GPU drivers | 4-8 weeks |
| **Switch (homebrew)** | None | Large | No Vulkan on Horizon homebrew: a second renderer; no `dlopen` | Months |

Recommended order: **Linux, then macOS, then Android.** Switch only as a long project of its own, if ever.

## What already carries over

- V: **No x86-only code.** No SSE/AVX intrinsics in `port/`, PortableKit's `host/` or `src/`. AVX is only an MSVC
  compiler switch (`PSPRECOMP_HOST_AVX`), which clang builds do not use.
- V: **No memory tricks.** The guest memory is a plain allocation: no 4 GiB reservation, no `VirtualAlloc`/`mmap`
  fast-memory mapping. Nothing needs page-level OS features.
- V: **The renderer asks for nothing unusual.** The Vulkan renderer (about 17,800 lines in `host/gpu/`) uses no
  geometry shaders, logic ops, depth clamp, wide lines or storage images, and already handles
  `VK_KHR_portability_subset`, the MoltenVK extension.
- V: **Our own code is mostly portable already.** 19,200 lines in `port/host` and `port/bgm_maker`; 22
  platform `#if`s in 11 files, all `_WIN32` with POSIX `#else` branches already written in most
  (`d012_compile_game.cpp` already names `clang++` without `.exe` and reads memory size on POSIX). Five of the
  67 PortableKit patches touch Windows code (0010 save dialog, 0025 host audio, 0045 perf CSV, 0050 present thread,
  0061 background disc read) and need their non-Windows halves checked.
- D: **PortableKit ships release scripts for macOS, Linux and Android** (`scripts/release_macos.sh`,
  `scripts/release_linux.sh`, `packaging/android/`), and Yakumo (MHP3rd) ships on Windows, Linux/Deck, macOS 13+
  (Apple Silicon) and Android 10+ (arm64). Yakumo's own README lists only macOS, the Deck and Windows 11 as tested.

## The real question on every platform: where the game's code is compiled

The release holds no game code: `setup.exe` makes it from the player's disc and compiles it with the shipped
llvm-mingw ([installer-design.md](installer-design.md)). That rule is what decides each platform's cost.

| Platform | Compiler on the machine? | Plan |
| --- | --- | --- |
| Linux desktop | Usually (`clang++`/`g++`) | Use it, or ship one |
| Steam Deck | No: read-only root, no compiler (D: `DESKTOP_APP.md`) | Ship clang + lld + a glibc sysroot, about 100-150 MB compressed (D) |
| macOS | Apple's Command Line Tools, a free 1.7 GB install the player makes (D) | PortableKit's desktop app already does this through `xcrun` |
| Android | No | Compile on the player's PC and make the APK there (below) |
| Switch | No | Compile on the player's PC with devkitA64 and make the `.nro` there |

## Linux (native)

What it buys over Proton: no Proton layer, the Deck's gyro through PortableKit's `steam_deck_imu.cpp` (hidraw),
and a release for desktop Linux players.

Work:
1. Build the port on Linux (WSL2 or the Steam Runtime "sniper" container that `release_linux.sh` uses). I: expect
   a day or two of compile fixes in our `_WIN32` branches; the framework already builds there (D).
2. A Linux `setup`: the wizard is a Windows program today. Port it to SDL3/ImGui like the BGM Maker, or a small
   script; ship the trimmed clang toolchain and sysroot inside the tarball.
3. The BGM Maker's file dialogs and launcher (`bgm_maker/launcher.cpp`, `psp_export.cpp`, `cli.cpp`): SDL3's file
   dialog covers them.
4. Fonts: the game's text uses MS Gothic from Windows; the Linux build needs a bundled or system Japanese font (the
   Deck under Proton has the same risk, noted in [steam-deck.md](steam-deck.md)).
5. `deploy_deck.py` and `package_deck.py` learn a native package beside the Proton one.

Test bed: the owner's Deck (only on the owner's request; see the Deck notes) and WSL2/a VM for desktop.

## macOS

Work:
1. **A Mac is needed.** Apple Silicon, macOS 13 SDK. Cross-building from Windows is not practical (the SDK cannot
   be redistributed and the release script checks imports against it).
2. Our installer on macOS: PortableKit's desktop app already compiles a game through Apple's Command Line Tools
   (`xcrun clang++`, `-bundle -bundle_loader`), and finds overlays inside an `.app`. Our `--compile-game` path
   needs the same Apple branch (`d012_compile_game.cpp` has Windows and generic POSIX branches only).
3. Data beside the app: a signed `.app` must not be written into, so data goes in a folder beside it (PortableKit
   uses `<Name> Data/`) and App Translocation has to be handled (D: the desktop app does this).
4. Signing: ad hoc signing works for the owner and testers; a notarized build needs an Apple Developer account
   ($99 a year) and the `disable-library-validation` entitlement to load the locally compiled game code (D: listed
   as untested in `DESKTOP_APP.md`).
5. Online play and ad hoc: check the local-network permission prompt macOS shows.
6. MoltenVK: the many renderer patches since PortableKit's last Mac test (stencil, render-target snapshots,
   readback, bloom, depth) are untested on it. I: expect a few graphics fixes.

## Android

Work:
1. **Compiling the game.** I: Android 10+ apps (target API 29+) cannot execute or `dlopen` files they wrote into
   their own data folder, so compiling on the phone would need workarounds. The simpler plan is a "Make Android
   app" step in the PC setup: recompile for arm64, compile with an Android clang (the NDK; the player downloads it
   under Google's licence, or a clang with an Android sysroot that we may ship; licence to check), pack and sign
   the APK with a key made on the player's PC. PortableKit's `build_apk.sh` already packs and signs without Gradle.
   D: Yakumo's APK is 0.8 GB and needs only the ISO, which suggests it ships its game code; that does not fit our
   rule of distributing no game-derived code.
2. **Overlays.** D: PortableKit's Android work (merged 7 October 2026) was described as "for games without
   overlays"; Dissidia has three overlays plus `LIBSUPPREACC.PRX`. The manifest extracts native libraries so the
   overlay loader can list them; to check against our three overlay libraries, or link them in statically.
3. **Disc access.** The ISO comes through Android's document picker and is copied into the app (D: Yakumo, about
   1.3 GB). Our DLC, mods folder, texture packs and Japanese voices disc need the same picker route.
4. **Controls.** PortableKit has a touch overlay; Dissidia needs a layout for its buttons (and the right-stick
   camera on touch), or a gamepad requirement for a first release.
5. **Performance and drivers.** The real unknown. Vulkan 1.1 on Adreno/Mali/Xclipse, a 30 Hz game at -O1 game
   code. D: Yakumo lists "Android on real phones: device testing, graphics drivers and performance" as open.
   The off-screen render targets, readback and stencil paths are where mobile drivers differ most.
6. Things to leave out at first: the BGM Maker (a desktop tool), split-screen versus, online play.

## Switch (homebrew only)

The only way onto a Switch is homebrew on a modified console (Atmosphère, libnx, devkitA64). There is no official
route for a fan port of a Square Enix game, and a Switch release raises a public-profile risk the PC releases do not.

Technical gaps:
1. **No Vulkan.** Homebrew has deko3d (a low-level API for the Tegra X1) and switch-mesa (OpenGL). No homebrew
   Vulkan driver exists; Mesa's NVK needs Linux's nouveau kernel driver, which Horizon does not have
   ([deko3d](https://github.com/devkitPro/deko3d)). The renderer, with all the port's graphics fixes, would need a
   second backend: deko3d for speed or OpenGL for ease. This is most of the work: months, not weeks.
2. **No `dlopen`.** The overlays load as shared libraries today; on the Switch they would be linked into one
   executable and selected by the overlay loader.
3. **SDL3.** devkitPro packages SDL2; an SDL3 port to the Switch would need checking or the input, audio and window
   layers written on libnx directly.
4. **Speed.** Four Cortex-A57 cores at about 1 GHz, three usable. I: the game code is light (2-3 ms a frame on the
   owner's i7 at -O1), so 30 fps logic is probably fine; the renderer's cost on Maxwell is unknown.
5. **Toolchain.** devkitA64 is GCC; the corpus has only been compiled with MSVC and clang. The player would build
   the `.nro` on their PC with devkitPro installed.
6. Memory is not a problem: the guest needs 64 MB; applet mode allows about 400 MB, full-title mode more.

## Linux: first build (5 November 2026)

V: built from `main` 68464f1 (PortableKit `dissidia` ca90495) in the Steam Runtime 3 "sniper" SDK container (the
digest `packaging/sources.sh` pins; gcc 14, SDL3 3.4.16 from source, bundled LGPL FFmpeg), in WSL2 Ubuntu 24.04,
with `tools/port/linux_build.sh`. The recompiled corpus is the Windows one, copied as it is.

- **Three fixes, all small:**
  1. `port/host/d012_loading_layout.cpp` lacked `<cstdint>`.
  2. PortableKit `host/gpu/texture_pack_match.cpp`: a `{a, b}` list mixed `unsigned long long` and `std::uint64_t`,
     which is `unsigned long` on Linux. Worktree `tools/local/port/PortableKit-linux`, branch `linux`; not yet a
     patch.
  3. `port/host/d012_mod_manifest.cpp`: a track's `file` is checked as text first, so `C:/...` is outside the mod
     on every system, and a `\` is refused where it does not work (not Windows) with the BGM Maker's message. The
     shared manifest cases failed two of theirs on Linux without it.
- **Everything links:** `D012Native` (53 MB), the four overlay libraries, `BgmMaker`, `d012_tests`. No missing
  libraries under `ldd`. `d012_tests`: all passed.
- **Headless boot** (`D012_NO_RENDER=1 D012_NO_AUDIO=1`, no interpreter, a new empty data folder, 40 s): 296
  imports bound, 460,475 functions, 46 decompiled overrides, the PRX, menu and battle overlays installed, the
  autosave notice, battle overrides registered. No interpreter fallback.
- **Not checked yet:** a run with a window, sound or a controller (WSL has no usable Vulkan driver here; the Deck
  is the test machine, deploy only on the owner's request); the Windows build with fix 3 (the Windows path of the
  check is unchanged but was not rebuilt); a crash report on Linux (`d012_crash_report.cpp` is Windows-only).

## Linux: on the Deck, and two fixes (5 November 2026)

V: the portable folder (`tools/port/linux_build.sh`, which now also stages `package/Dissidia012` and its `.tar.xz`)
runs on the owner's Steam Deck as a non-Steam game with the compatibility tool on "Default" (native): RADV Vulkan
1.4, the Deck's controller, sound, the system's Noto CJK font. The owner's saves, music mod, DLC and Japanese voices
were copied in. Two faults the owner found, both in PortableKit (worktree `PortableKit-linux`, not yet patches):

1. **Music mods missing.** PortableKit's Linux FFmpeg is built with `--disable-everything` and a short list; it had
   no BRSTM/BFSTM demuxers or DSP-ADPCM (`adpcm_thp`, `adpcm_thp_le`) decoders, so every `.brstm` track was left
   out. Windows uses BtbN's full LGPL build. Added to `cmake/FFmpeg.cmake`; the PC decode check
   (`D012_TEST_MODS_DECODE=1`) takes all 31 tracks. V on the Deck.
2. **DLC music silent.** The game asks whether `ms0:/PSP/GAME/ULUS10566/2f356e6c.edat` exists, then opens
   `MS0:/PSP/GAME/ULUS10566/2F356E6C.EDAT`. A memory stick and Windows match names in any case; Linux does not.
   `host/hle/hle_io.cpp` `host_path()` now looks each part of an `ms0:` path up in any ASCII case off Windows. V on
   the Deck.

## Linux: the setup (5 November 2026)

`tools/port/linux_setup.sh --version X.Y.Z` makes `Dissidia012-Setup-<version>-x86_64.AppImage` (164 MB) in WSL:

- **The installer build** (`linux_build.sh --installer`, `D012_INSTALLER_BUILD=ON` in the sniper container): the
  program without the game's code (8.9 MB), `psp_recomp`, `sdk/`, `lib/`, `fonts/`, licences. `--compile-game`
  learned Linux: `main.so` and `overlays/*.so`, `-fPIC`, a `-shared` link with lld, its own static libstdc++ and
  libgcc kept to itself (`--exclude-libs,ALL`), memory from `sysconf`; the key includes `clang++.cfg`.
- **The toolchain** (`tools/port/linux_toolchain.sh`, 488 MB unpacked): clang 23.1.4 and lld from LLVM's release
  `.tar.xz` (the `.tar.zst` cannot be unpacked here: no zstd in WSL or the SDK image, and Windows' bsdtar refuses
  its frame size), LLVM's licence from the same tag, and a sysroot from the SDK image's packages: glibc 2.31,
  Linux and libcrypt headers, GCC 14's libgcc, libstdc++ (static) and headers, moved from `/usr/lib/gcc-14` to
  where clang looks. `bin/clang++.cfg` gives `--target` and `--sysroot=<CFGDIR>/../sysroot`. Two quirks: this GCC
  has no `libgcc_eh.a` (its unwinder is in `libgcc.a`), so an empty archive stands in for the one clang names; and
  the sysroot's absolute symlinks are made relative. clang and lld themselves need glibc 2.34, libstdc++ and zlib
  (SteamOS has glibc 2.43); the game needs 2.31.
- **The wizard** (`tools/port/linux_setup/AppRun`): zenity dialogs (SteamOS ships zenity), text prompts in a
  terminal, or none with `D012_SETUP_ISO` and `D012_SETUP_DIR`. Started from the desktop with no zenity (and so no
  terminal), it opens a terminal window and runs itself again in it: the first of `xdg-terminal-exec`,
  `x-terminal-emulator`, konsole, gnome-terminal, ptyxis, kgx, xfce4-terminal, mate-terminal, alacritty, kitty, foot,
  wezterm and xterm. The first run waits until the one in the window ends (marks in a temporary folder named by
  `D012_SETUP_IN_TERMINAL`), because the AppImage's files are there only while it runs and some terminals return at
  once; the window waits for Enter before it closes. The AppImage catalog's test (appimage.github.io) starts it that
  way, on a virtual screen with xterm and no zenity; before this the setup quit there with nothing on screen. It asks for the ISO (checked as on Windows), the
  folder (`~/Games/Dissidia012`; it refuses a folder holding the Windows version), whether to copy the ISO; copies
  the program over the previous one (`data/` and `game-code/` kept), runs `--install` and `--compile-game` with a
  progress bar from their `[compile-game] N%` lines, and adds a menu entry for "Add to Steam". The menu icon is
  the disc's own `ICON0.PNG`, which `--compile-game` writes from the player's ISO (SteamOS's bsdtar lists nothing in a PSP disc): the setup carries nothing of the game's (its
  own icon is drawn for it). No BGM Maker yet: its PSP export needs a Linux atracdenc.
- **Checks** (`tools/port/linux_check_setup.py`, the Windows packaging rules): 2,775 files, no game files, no
  paths of this machine, a program without the game's code.
- V (WSL, unattended, the USA ISO, 12 jobs): set up in 154 s (324 parts compiled in 2 min 24 s); the overlay
  libraries have the developer build's names and hashes; the installed game boots headless from
  `game-code/main.so` (460,475 functions, all overlays, no fallback); a second run takes 3 s ("already compiled
  and up to date").
- **Not checked yet:** the zenity dialogs, the menu entry, the icon and "Add to Steam" (WSL has no zenity); the
  compile time on the Deck's four cores; FUSE on SteamOS (the type-2 runtime needs `fusermount3`).

## Linux: crash reports and the version (5 November 2026)

- **A log always.** A Linux portable copy (the installer build or a packaged folder) started where its output is
  lost writes `data/logs/D012Native.log`, the previous run's kept as `D012Native-previous.log`
  (`port/host/d012_portable.cpp`): from a menu entry (`/dev/null`, a socket, closed), or from Steam. V (Deck,
  6 November): Steam gives a game a pipe that ends in its own `~/.local/share/Steam/logs/console-linux.txt` (and
  the user journal) among everything else, so with `SteamGameId` or `SteamAppId` set a pipe counts as lost too.
  A terminal, a file or another pipe keeps the output, and so does `D012_KEEP_OUTPUT=1`, which the setup sets for
  `--install` and `--compile-game` (it reads their progress through a pipe). Before switching, the program prints
  where the log is. `D012_LOG_FILE` still wins.
- **The crash report** (`port/host/d012_crash_report.cpp`, Linux part): SIGSEGV, SIGBUS, SIGILL, SIGFPE and
  `abort()` log the signal, the address, the thread and the stack as module+offset, with each module's path and
  load address, then the signal is raised again so the system keeps its core dump (SteamOS: `coredumpctl`). The
  handler runs on a stack of its own and writes straight to the log. `linux_setup.sh` keeps the release's
  unstripped `D012Native` and `psp_recomp` in `build/port/installer-linux/symbols/<version>`;
  `addr2line -f -C -e <symbols>/D012Native 0x<offset>` (or llvm-symbolizer) names a frame. V (WSL,
  `D012_TEST_CRASH=8` and `8,abort`, output to `/dev/null`): exit 139 and 134, the log written, the frame named
  `test_crash` from the kept symbols; a piped run is not redirected. On the Deck (`D012_TEST_CRASH=20` as a launch
  option) the report was complete, in Steam's console log then; `coredumpctl` listed the crash.
- **One version for both setups:** `tools/port/release-version.txt` (0.7.1), read by `package_installer.py` and
  `linux_setup.sh` unless `--version` is given.
- **Windows unchanged:** the linux branch with PortableKit-linux built for Windows with llvm-mingw
  (`tools/local/port/build-d012-linux-clang`, `build-d012-linux-installer`): it links, `d012_tests` all pass, a
  headless run takes the 31 mod tracks and the 222 DLC files, and `--compile-game` (3 min 16 s) makes `main.dll`
  and the same four overlay libraries, which boot.

## Next steps, if wanted

1. ~~Linux native build in WSL2 or the sniper container.~~ Done; see above. ~~A portable folder and a windowed
   run on the Deck.~~ Done.
2. ~~A Linux setup~~ Built, checked in WSL and run on the Deck (7 min 51 s); ~~a crash report~~ done. A Linux setup that compiles the game code on the player's machine (a shipped clang + sysroot), and a crash
   report for Linux. The download is the setup as an AppImage (`Dissidia012-Setup-x86_64.AppImage`: the setup, the
   program, clang and the sniper sysroot); what it makes is the plain folder of step 1, as `setup.exe` does on
   Windows. D: the TotK PC recomp does the same: an AppImage setup with LLVM and the sniper sysroot inside, building
   the game into a plain folder the player picks, saves and settings in it, temporary files in it too because the
   AppImage is read-only ([installer.md](https://github.com/valentderah/totk-pc-recomp/blob/HEAD/docs/installer.md)).
   An AppImage for the game itself fits badly: it is read-only, and the compiled game code and `data/` belong
   beside the program. No Flatpak: its sandbox keeps data in `~/.var/app/`.
3. macOS once a Mac is available.
4. Android: a PC-side APK build of the recompiled game, first with a gamepad, on one test phone.

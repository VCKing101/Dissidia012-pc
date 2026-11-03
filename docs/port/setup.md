# Setting up the PC port with `setup.ps1`

`setup.ps1`, at the repository root, takes a fresh clone to a playable build of the port with one command. It
checks what you need to install, downloads and verifies the build dependencies and PortableKit, checks your game
files, builds the recompiled port and makes a desktop shortcut. This page covers what each step does, the options,
and what to do when something goes wrong. The short version is in the [README](../../README.md#quick-start), and
the build steps it automates are in [port/README.md](../../port/README.md).

**To play, you do not need this page:** the Windows setup on the
[releases page](https://github.com/VCKing101/Dissidia012-pc/releases) installs the port from your ISO with nothing
else installed first (README, "Quick start"). `setup.ps1` is for building from source: following the latest
changes, developing the port, or making the Steam Deck package.

## What you need

| | Why | Get it |
| --- | --- | --- |
| Windows 10 or 11, 64-bit | | |
| Visual Studio 2022 Community (free) or Build Tools 2022, with the workload **Desktop development with C++** | CMake, Ninja and the Windows SDK's tools; its compiler builds only the optional encoder of the BGM Maker's ISO export. The port itself is compiled with clang, which setup downloads ([why](#why-clang)). The workload's default items include them all. | [visualstudio.microsoft.com/downloads](https://visualstudio.microsoft.com/downloads/) |
| Python 3.10 or newer | The setup and the port's scripts | [python.org/downloads/windows](https://www.python.org/downloads/windows/). Tick "Add python.exe to PATH" |
| Git | Fetches PortableKit and applies this port's changes to it | [git-scm.com/download/win](https://git-scm.com/download/win) |
| A graphics driver with Vulkan | The renderer. Any GPU from about 2016 on, with its vendor's current driver | AMD, NVIDIA or Intel's site |
| About 6 GB free on the drive with the repository | Build files about 1.5 GB, dependencies 1.2 GB (clang is 0.9 GB of it). Add 1.7 GB if your ISO is on another drive, because it is then copied | |
| Your own copy of the game: the USA disc **ULUS10566** or the European disc **ULES01505**, v1.00, as an `.iso` file | No game data comes with this project. The port decrypts the game's executable from it ([below](#the-decrypted-eboot)) | |

If something is missing, setup stops before changing anything and says what to install. It never installs software
by itself. These commands install the tools from PowerShell with winget, if you prefer that:

```powershell
winget install --id Microsoft.VisualStudio.2022.Community --override "--wait --passive --add Microsoft.VisualStudio.Workload.NativeDesktop --includeRecommended"
# or, without the IDE:
winget install --id Microsoft.VisualStudio.2022.BuildTools --override "--wait --passive --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"
winget install --id Python.Python.3.12
winget install --id Git.Git
```

Open a new PowerShell window after installing, so that the new programs are found.

## Running it

```powershell
git clone https://github.com/VCKing101/Dissidia012-pc.git
cd Dissidia012-pc
powershell -ExecutionPolicy Bypass -File .\setup.ps1
```

Or double-click `setup.cmd` in the folder, which does the same. (`-ExecutionPolicy Bypass` applies to this one
run only. Windows blocks scripts by default, and nothing is changed for good.)

The first time, setup asks for:

1. **Permission to continue.** It will download about 290 MB and compile for 5 to 30 minutes.
2. **Your ISO.** Type or paste its path, or drag the file into the window, then press Enter. An ISO of the right
   size at the repository root is offered as the default.

It remembers the path in `build/port/setup/state.json`, so later runs ask nothing.

### What each step does

| Step | What happens | Skipped on a re-run when |
| --- | --- | --- |
| 1. Prerequisites | Checks Visual Studio (with `vswhere`), its compiler, CMake, Ninja and Windows SDK; Python 3.10 or newer; Git; `System32\vulkan-1.dll`; free disk space. Makes `.venv` and installs [requirements-port.txt](../../requirements-port.txt) (pycdlib, Pillow) | `.venv` already has that file's packages |
| 2. Dependencies | Downloads SDL3 3.4.18, glslang 16.6.0, Vulkan-Headers vulkan-sdk-1.4.363.0 and llvm-mingw 20260922 (clang, 182 MB) into `tools/local/port-deps/dl/`, checks each SHA-256 (table below) and unpacks them where `tools/port/pk_cmake_mingw.cmd` looks: llvm-mingw into `tools/local/llvm-mingw-20260922-ucrt-x86_64`. Makes `vulkan-1.lib` from your driver's `vulkan-1.dll` with `dumpbin /exports` and `lib /def`, so no Vulkan SDK and no administrator rights are needed. FFmpeg is fetched later by PortableKit's own configure step, which checks its own pinned hash | The zips are in place and verified, and already unpacked |
| 3. PortableKit | Clones [TeamGDB/PortableKit](https://github.com/TeamGDB/PortableKit) into `tools/local/port/PortableKit`, and puts the local branch `dissidia` at commit `33800c4` plus this repository's patches (`port/portablekit-patches/*.patch`, with `git am`) | The patches have not changed since the last run |
| 4. Your game files | Checks the ISO ([below](#what-is-checked)). Takes the three overlays (`OVL_*.ELF`) and `LIBSUPPREACC.PRX` from the ISO into `build/port/ovl` and `build/port/prx`, and makes the shortcut's icon `build/port/launcher/dissidia012.ico` from the disc's `ICON0.PNG`. Makes the decrypted EBOOT: configures the build (as in step 5), compiles `D012Native` with no game code yet (PortableKit's bootstrap stage; the full build reuses these objects), and runs `D012Native --install <iso> --in-place` with `D012_DATA_DIR` and `D012_GAME_DIR` set to the temporary folder `build/port/setup/decrypt`, stopped after 60 s at most. It checks the SHA-256 of the `EBOOT.ELF` written there and removes the folder. Then it prepares `build/port/game` with `tools/port/prepare_game_dir.py`: `EBOOT.ELF` copied, `disc.iso` a hard link to your ISO (a copy if it is on another drive), and an empty `ms0/` | Files already right are kept; the ISO's SHA-1 is computed only once; nothing is decrypted while `build/port/game/EBOOT.ELF` has the right SHA-256 |
| 5. Build | Configures `tools/local/port/build-d012-clang` with clang (`tools/port/pk_cmake_mingw.cmd`), as in port/README.md ("Recompiled build"; already done in step 4 on a first run), builds `psp_recomp`, generates the C++ corpora (main with `--dispatch-call` for the main rows of `port/config/dispatch-calls.txt`, then the overlays and the PRX with `tools/port/overlay_meta.py --recompile`), and builds everything: `D012Native` and the Battle BGM Maker, `BgmMaker.exe`. Then it builds atracdenc, the encoder of the BGM Maker's "Make a PSP / PPSSPP ISO", with `tools/port/build_atracdenc.cmd`, which clones its source from GitHub the first time (`build/external/atracdenc-src`, at a fixed commit); if that fails setup warns and goes on, because only the ISO export needs it | Ninja rebuilds only what changed; the corpora are regenerated only when `psp_recomp`, the EBOOT, the overlays, `dispatch-calls.txt` or `overlay_meta.py` changed, and even then only changed files are replaced; atracdenc is built again only when its commit changes |
| 6. Shortcuts | A desktop shortcut and a Start Menu entry, "Dissidia 012 (PC port)", that run `tools/port/play_d012.ps1` in a hidden window | Re-made each time (the same shortcut) |

The output of every long command is kept in `build/port/logs/setup-*.log`. On a failure setup prints the last lines
and the log's name.

### How long it takes

On the PC it was tested on (20 logical CPUs, 32 GB of memory, a hard disk), a full first run with clang in a fresh
clone, given only the ISO and `-Deck`, took **11 minutes 21 seconds** on 2 November 2026, plus the downloads: 14 s to
configure, 42 s for the bootstrap compile, 2 s to decrypt the EBOOT, 57 s to generate the corpora, 3 min
31 s for the rest of the compile, 21 s for atracdenc, and 4 min 6 s for the Steam Deck's release build (without
`-Deck`, about 7 minutes). That test took PortableKit and the dependency zips from local copies. Compiling the
generated code needs a lot of memory, so setup lets one generated unit compile per 4 GB of memory, which is
PortableKit's rule. With 8 GB that is two at a time, so expect 30 minutes or more on such a PC. A re-run with nothing
changed takes a few seconds. After a change to the patches that rebuilds `psp_recomp`, it takes about half a minute,
because the corpora are regenerated (and come out the same).

### Options

| Option | What it does |
| --- | --- |
| `-Update` | Re-applies the PortableKit patches and rebuilds, asking nothing. Uses the ISO given before |
| `-Deck` | Also builds the release build (`tools/local/port/build-d012-release-clang`) and the Steam Deck package `build/port/deck-package/Dissidia012` with `tools/port/package_deck.py`. Sending it to the Deck is a separate step: [steam-deck.md](steam-deck.md) |
| `-NonInteractive -Iso <path>` | For scripts: never asks, fails with a message instead |
| `-Eboot <path>` | Use a decrypted EBOOT you already have instead of decrypting the ISO's. It must have the SHA-256 [below](#the-decrypted-eboot) |
| `-NoShortcuts` | No shortcuts |
| `-ShortcutDir <dir>` | The two shortcuts go under `<dir>\Desktop` and `<dir>\Start Menu` instead (for tests) |
| `-PortableKitSource <path>` | Clone PortableKit from a local clone (or another URL) instead of GitHub. It must contain `33800c4` |
| `-DepsCache <dir>` | Take the dependency zips from a folder laid out like `tools/local/port-deps/dl` before downloading. They are checked like downloads |
| `-SkipIsoHash` | Accept an ISO whose SHA-1 differs from the known dump. Its game files are still checked |

`Get-Help .\setup.ps1 -Detailed` lists them too.

## Playing

Use the shortcut, or run `.\tools\port\play_d012.ps1`. Esc on the keyboard, or L3 and R3 together on a controller,
opens the port's menu: video, controls and audio.

| What | Where |
| --- | --- |
| Saves | `build/port/data/ms0/PSP/SAVEDATA/` (the game makes them through New Game; back this folder up) |
| Settings | `build/port/data/settings.ini` (written by the port's menu) |
| Mods | `build/port/data/mods/` ([mod-folder.md](mod-folder.md)) |
| DLC you added | `build/port/data/ms0/PSP/GAME/ULUS10566/`, or `ULES01505/` with the European disc (the folder must have your disc's name; [README](../../README.md#downloadable-content-dlc)) |
| The program | `tools/local/port/build-d012-clang/bin/D012Native.exe` |
| The Battle BGM Maker | `tools/local/port/build-d012-clang/bin/BgmMaker.exe`, on `build/port/data/mods` ([mod-folder.md](mod-folder.md), section 8). Setup makes no shortcut for it |

Setup never writes to `build/port/data`, except to create the empty folder.

## Updating

```powershell
git pull
.\setup.ps1
```

Setup notices what changed and redoes only that. If the PortableKit patches changed, the branch `dissidia` is moved
to 33800c4 plus the new patches. Files that the new patches leave unchanged are not rewritten, so only the changed
parts are recompiled. `.\setup.ps1 -Update` does the same without asking anything.

**From a setup made with Visual Studio** (before 0.5.0): the first run after `git pull` downloads clang (182 MB) and
compiles the whole port once into `tools/local/port/build-d012-clang`, as long as a first setup. The game's generated
C++ is reused. Saves, settings, mods and texture packs in `build/port/data` are untouched, and the shortcuts start
the new build. The old Visual Studio folders, `tools/local/port/build-d012-recomp` and, after `-Deck`,
`build-d012-release` (about 2 GB together), are no longer used: setup says so at the end and leaves them for you to
delete. Until setup has run again, the shortcuts still start the old build.

## Why clang

Since 0.5.0, setup compiles the port with clang (llvm-mingw) rather than Visual Studio's compiler (MSVC), so that
every way of getting the port is built the same way: the Windows setup on the releases page has always used clang,
and now `setup.ps1` and the Steam Deck package it makes do too.

- **Online play needs it.** Both players' games compute every frame of a battle themselves and must get the same
  bits. The game's VFPU maths (sine, cosine, arcsine, log2, exp2) round the last bit differently in Microsoft's C
  runtime and in Wine's under Proton, so the port carries its own versions (`port/host/d012_math.cpp`;
  [communication-mode.md](communication-mode.md), section 13). Only clang lets the program use them in place of the
  C runtime's: MSVC refuses to define `sinf` (it is one of its intrinsics) and sends `exp2f` straight to
  `ucrtbase.dll`. An MSVC build would therefore part from the others in a battle, so a battle now checks the
  compiler too, and refuses a copy built with the other one with a message naming it.
- **One compiler for all.** The Windows setup, `setup.ps1` and the Deck package then make the same program from the
  same source, and a problem found in one is the same in the others.
- **Speed.** In the installer's tests (28 October 2026), the game's code compiled with clang ran about three times
  as fast as MSVC's, which PortableKit compiles without inlining (`/Ob0`) to keep its build time down.

Visual Studio is still needed for its CMake, Ninja and tools, and its compiler builds atracdenc, the optional
encoder. Building with MSVC by hand still works (`tools/port/pk_cmake.cmd`, port/README.md), for debugging with
Visual Studio for example, but such a build cannot play online against a clang build.

## The decrypted EBOOT

The game's main executable, `PSP_GAME/SYSDIR/EBOOT.BIN`, is encrypted on the disc, and the port needs it decrypted.
The port's profile (`port/host/d012_profile.cpp`) carries the public decryption key for the executable's tag, the
value pspdecrypt and the PSP emulators carry, so `D012Native --install` decrypts it from your own ISO, and setup
does that for you (step 4). The result is accepted only if its SHA-256 is
`0bc8e779aad2814a3d1bd77346f69ca5e16665845ed053cd0ad44a9cbfaefe6d` (4,640,948 bytes), the decrypted v1.00
executable. The USA and European discs hold the same one ([regions.md](regions.md)).

If you already have that file, `-Eboot <path>` uses it instead, and the bootstrap compile and the decryption are
skipped.

## What is checked

| Input | Check |
| --- | --- |
| ISO | `PARAM.SFO` says disc `ULUS10566` or `ULES01505`, version 1.00; the size is 1,754,529,792 bytes; the three overlays and `LIBSUPPREACC.PRX` have the retail SHA-256 values; the whole image's SHA-1 is `ec23f5beac89312a2a25654e202ae1c997999c9e` (Redump's USA v1.00 entry, see [usa-comparison.md](../decomp/usa-comparison.md)) or `2e6e8b71a1f66a5ba1390fa68ea36188ac4fe2b6` (Redump's European v1.00 entry, see [regions.md](regions.md)). Compressed images (`.cso`, `.chd`) are refused |
| EBOOT | The file decrypted from the ISO, or the one given with `-Eboot`: an ELF file (not the encrypted `~PSP` one) with the SHA-256 above. `D012Native --install` also checks the disc ID and the SHA-256 of the disc's `EBOOT.BIN` before it decrypts |
| SDL3-devel-3.4.18-VC.zip | SHA-256 `78d84602ae616cfe26b33a73b7d3b9b6b56e8a6650e53d92c9c6ca938f747acd` |
| glslang-16.6.0-windows-x86_64-release.zip | SHA-256 `82bf434e69b9bb4829de7e2b4bc2c5e7a7861e53d66cf75e5cc70f5f694a8d9b` |
| Vulkan-Headers-vulkan-sdk-1.4.363.0.zip | SHA-256 `f4be95220ff0ee0b1c620301ff4fe9ae48592d71f4c30f2d5c571351a153ae81`, or else its contents (see below) |
| llvm-mingw-20260922-ucrt-x86_64.zip | SHA-256 `e3ad77d117a4bea19a7a3b333341824d79a5a371004a10e25b8504e7b3047666`, from [mstorsjo/llvm-mingw](https://github.com/mstorsjo/llvm-mingw/releases/tag/20260922) |
| FFmpeg (PortableKit's pinned BtbN build) | Checked by PortableKit's `cmake/FFmpeg.cmake` against its own pinned hash |
| PortableKit | Commit `33800c4060c53f3233e08f8ab2cc4590a0872063`, with the patches applied by `git am` |

**The Vulkan headers zip.** GitHub makes this zip from the tag on request, and does not promise that the bytes stay
the same. On 24 October 2026 a new download was byte-identical to the one fetched on 18 October. In case GitHub
changes how it packs archives, setup also accepts a zip whose zip hash differs but whose **contents** match: the
SHA-256 over every file's path and SHA-256, sorted by path, must be
`a32e435c1d3b3155da7bd05c713bd82efacc8c5269fb38213a53aa4918efb163`. The tag points at commit `6802bb47`.

A file that fails its check is moved aside as `<name>.rejected` and setup stops. Nothing is unpacked from it.

## Troubleshooting

| Problem | What to do |
| --- | --- |
| "running scripts is disabled on this system" | Start it as `powershell -ExecutionPolicy Bypass -File .\setup.ps1`, or double-click `setup.cmd` |
| "Visual Studio ... not found", or C++ parts missing | Install the workload **Desktop development with C++** (Visual Studio Installer > Modify). Build Tools 2022 is enough |
| "Python 3.10 or newer" although Python is installed | Open a new PowerShell window. If `python` opens the Microsoft Store, install Python from python.org and tick "Add python.exe to PATH" |
| pip fails | Check the internet connection. Behind a proxy, set `HTTPS_PROXY` first. Delete `.venv` to start that step over |
| A download fails or a SHA-256 does not match | Run setup again. A file that still fails was changed upstream: report it, and do not use it |
| "is disc NPJH50377" | That is the Japanese release, a different build of the game. The port supports the USA release ULUS10566 and the European release ULES01505 |
| "SHA-1 ..., but a clean dump ... has ..." | The image is not the Redump dump: a different dump tool, or a modified image. If its game files passed, `-SkipIsoHash` lets you try it |
| "is the encrypted EBOOT.BIN" | `-Eboot` was given the file from the disc. Leave `-Eboot` out, and setup decrypts it |
| "an ELF file, but not the decrypted executable the port knows" | The `-Eboot` file is from another release or version, or patched. Leave `-Eboot` out |
| "could not decrypt the EBOOT from the ISO" | Read `build/port/logs/setup-decrypt.log`, which has the program's message |
| "PortableKit has uncommitted changes" | Someone edited `tools/local/port/PortableKit`. Commit or stash there, or delete that folder and run setup again |
| "the PortableKit patches do not apply" | `port/portablekit-patches` and the clone disagree. Delete `tools/local/port/PortableKit` and run setup again |
| The build fails | Read the log setup names (`build/port/logs/setup-build.log`). Out of memory: close other programs and run again (it continues). A path-length error: move the repository closer to the drive root, for example `C:\Games\Dissidia012-pc` |
| "pk_cmake.cmd failed with exit code 2", with `error C2001: newline in constant` or `warning C4819` in the log | Windows is set to a language whose code page is not UTF-8 (Chinese, Japanese, Korean and others). Fixed after v0.1.0: update and run setup again. On v0.1.0, run `$env:_CL_ = "/utf-8"` in PowerShell, then `.\setup.ps1` in the same window |
| The folder path contains `!`, `%`, `^` or `&` | Move the repository to a plain path; the Windows build scripts cannot handle those characters |
| A save copied to PPSSPP says "Unable to load data", or a save from PPSSPP will not load in the port | Fixed after v0.1.1: update and run setup again. The port then writes saves the way PPSSPP and a PSP do, and reads theirs. A save the port wrote before is fixed the next time you start the port, as the game loads it; copy it to PPSSPP after that. The save folder is `ULUS10566GameData00` (`ULES01505GameData00` with the European disc), in `build\port\data\ms0\PSP\SAVEDATA` here and in `PSP\SAVEDATA` in PPSSPP's memory stick folder. Copy, don't move, and keep a backup of both |
| The shortcut does nothing | Run `.\tools\port\play_d012.ps1` in PowerShell to see the message. If the port is not built, it says so in a window |
| Start over | Delete `tools/local/port/build-d012-clang` and `build/port/recomp` (build files), or `build/port/setup/state.json` (to be asked for the ISO again). Keep `build/port/data`: your saves are there |

## For developers

- The logic is in `tools/port/setup/`: `d012_setup.py` runs the steps, with `deps.py`, `portablekit.py`,
  `game.py` and `build.py`; `prereqs.ps1` checks the prerequisites; `vsenv.cmd` runs a command in the Visual Studio
  environment, as `pk_cmake.cmd` does; `shortcuts.ps1` makes the shortcuts.
- The patches are applied in a temporary worktree (`tools/local/port/.portablekit-apply`) with the committer
  `Dissidia 012 port setup <setup@dissidia012.invalid>` given with `git -c`, and the committer date set to the author
  date. Your own git identity is not needed, the authors are kept, and the same patches give the same commit
  everywhere. Then `dissidia` is checked out at that commit, or left as it is if it already has the same tree. A
  developer's existing clone is therefore not moved just because its commits were made with another committer.
- Everything game-derived stays in the ignored `build/` and `tools/local/`. The interpreter build
  (`tools/local/port/build-d012`) is not made by setup; port/README.md shows how.

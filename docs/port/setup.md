# Setting up the PC port with `setup.ps1`

`setup.ps1`, at the repository root, takes a fresh clone to a playable build of the port with one command. It
checks what you need to install, downloads and verifies the build dependencies and PortableKit, checks your game
files, builds the recompiled port and makes a desktop shortcut. This page covers what each step does, the options,
and what to do when something goes wrong. The short version is in the [README](../../README.md#quick-start), and
the build steps it automates are in [port/README.md](../../port/README.md).

A download that needs no Visual Studio, Python or Git is planned. Until then, the port is built on your PC.

## What you need

| | Why | Get it |
| --- | --- | --- |
| Windows 10 or 11, 64-bit | | |
| Visual Studio 2022 Community (free) or Build Tools 2022, with the workload **Desktop development with C++** | The compiler, CMake, Ninja and the Windows SDK. The workload's default items include them all. | [visualstudio.microsoft.com/downloads](https://visualstudio.microsoft.com/downloads/) |
| Python 3.10 or newer | The setup and the port's scripts | [python.org/downloads/windows](https://www.python.org/downloads/windows/). Tick "Add python.exe to PATH" |
| Git | Fetches PortableKit and applies this port's changes to it | [git-scm.com/download/win](https://git-scm.com/download/win) |
| A graphics driver with Vulkan | The renderer. Any GPU from about 2016 on, with its vendor's current driver | AMD, NVIDIA or Intel's site |
| About 5 GB free on the drive with the repository | Build files about 1.5 GB, dependencies 0.3 GB. Add 1.7 GB if your ISO is on another drive, because it is then copied | |
| Your own copy of the game: the USA disc **ULUS10566** v1.00 as an `.iso` file | No game data comes with this project. The port decrypts the game's executable from it ([below](#the-decrypted-eboot)) | |

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

1. **Permission to continue.** It will download about 100 MB and compile for 5 to 30 minutes.
2. **Your ISO.** Type or paste its path, or drag the file into the window, then press Enter. An ISO of the right
   size at the repository root is offered as the default.

It remembers the path in `build/port/setup/state.json`, so later runs ask nothing.

### What each step does

| Step | What happens | Skipped on a re-run when |
| --- | --- | --- |
| 1. Prerequisites | Checks Visual Studio (with `vswhere`), its compiler, CMake, Ninja and Windows SDK; Python 3.10 or newer; Git; `System32\vulkan-1.dll`; free disk space. Makes `.venv` and installs [requirements-port.txt](../../requirements-port.txt) (pycdlib, Pillow) | `.venv` already has that file's packages |
| 2. Dependencies | Downloads SDL3 3.4.18, glslang 16.6.0 and Vulkan-Headers vulkan-sdk-1.4.363.0 into `tools/local/port-deps/dl/`, checks each SHA-256 (table below) and unpacks them where `tools/port/pk_cmake.cmd` looks. Makes `vulkan-1.lib` from your driver's `vulkan-1.dll` with `dumpbin /exports` and `lib /def`, so no Vulkan SDK and no administrator rights are needed. FFmpeg is fetched later by PortableKit's own configure step, which checks its own pinned hash | The zips are in place and verified, and already unpacked |
| 3. PortableKit | Clones [TeamGDB/PortableKit](https://github.com/TeamGDB/PortableKit) into `tools/local/port/PortableKit`, and puts the local branch `dissidia` at commit `33800c4` plus this repository's patches (`port/portablekit-patches/*.patch`, with `git am`) | The patches have not changed since the last run |
| 4. Your game files | Checks the ISO ([below](#what-is-checked)). Takes the three overlays (`OVL_*.ELF`) and `LIBSUPPREACC.PRX` from the ISO into `build/port/ovl` and `build/port/prx`, and makes the shortcut's icon `build/port/launcher/dissidia012.ico` from the disc's `ICON0.PNG`. Makes the decrypted EBOOT: configures the build (as in step 5), compiles `D012Native` with no game code yet (PortableKit's bootstrap stage; the full build reuses these objects), and runs `D012Native --install <iso> --in-place` with `D012_DATA_DIR` and `D012_GAME_DIR` set to the temporary folder `build/port/setup/decrypt`, stopped after 60 s at most. It checks the SHA-256 of the `EBOOT.ELF` written there and removes the folder. Then it prepares `build/port/game` with `tools/port/prepare_game_dir.py`: `EBOOT.ELF` copied, `disc.iso` a hard link to your ISO (a copy if it is on another drive), and an empty `ms0/` | Files already right are kept; the ISO's SHA-1 is computed only once; nothing is decrypted while `build/port/game/EBOOT.ELF` has the right SHA-256 |
| 5. Build | Configures `tools/local/port/build-d012-recomp` as in port/README.md ("Recompiled build"; already done in step 4 on a first run), builds `psp_recomp`, generates the C++ corpora (main with `--dispatch-call` for the main rows of `port/config/dispatch-calls.txt`, then the overlays and the PRX with `tools/port/overlay_meta.py --recompile`), and builds everything | Ninja rebuilds only what changed; the corpora are regenerated only when `psp_recomp`, the EBOOT, the overlays, `dispatch-calls.txt` or `overlay_meta.py` changed, and even then only changed files are replaced |
| 6. Shortcuts | A desktop shortcut and a Start Menu entry, "Dissidia 012 (PC port)", that run `tools/port/play_d012.ps1` in a hidden window | Re-made each time (the same shortcut) |

The output of every long command is kept in `build/port/logs/setup-*.log`. On a failure setup prints the last lines
and the log's name.

### How long it takes

On the PC it was tested on (20 logical CPUs, 32 GB of memory, 24 October 2026), a full first run in a fresh clone,
given only the ISO, took **4 minutes 4 seconds**, plus the downloads: 9 s to configure, 40 s for the bootstrap
compile, 2 s to decrypt the EBOOT, 25 s to generate the corpora and 2 min 24 s for the rest of the compile, which
recompiled none of the bootstrap's host code. (That test took PortableKit and the dependency zips from local copies,
and the ISO's SHA-1 took 1 s because the file was already in the disk cache; 13 s is more usual.) Compiling
the generated code needs a lot of memory, so setup lets one generated unit compile per 4 GB of memory, which is
PortableKit's rule. With 8 GB that is two at a time, so expect 30 minutes or more on such a PC. A re-run with nothing
changed takes a few seconds. After a change to the patches that rebuilds `psp_recomp`, it takes about half a minute,
because the corpora are regenerated (and come out the same).

### Options

| Option | What it does |
| --- | --- |
| `-Update` | Re-applies the PortableKit patches and rebuilds, asking nothing. Uses the ISO given before |
| `-Deck` | Also builds the release build (`tools/local/port/build-d012-release`) and the Steam Deck package `build/port/deck-package/Dissidia012` with `tools/port/package_deck.py`. Sending it to the Deck is a separate step: [steam-deck.md](steam-deck.md) |
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
| The program | `tools/local/port/build-d012-recomp/bin/D012Native.exe` |

Setup never writes to `build/port/data`, except to create the empty folder.

## Updating

```powershell
git pull
.\setup.ps1
```

Setup notices what changed and redoes only that. If the PortableKit patches changed, the branch `dissidia` is moved
to 33800c4 plus the new patches. Files that the new patches leave unchanged are not rewritten, so only the changed
parts are recompiled. `.\setup.ps1 -Update` does the same without asking anything.

## The decrypted EBOOT

The game's main executable, `PSP_GAME/SYSDIR/EBOOT.BIN`, is encrypted on the disc, and the port needs it decrypted.
The port's profile (`port/host/d012_profile.cpp`) carries the public decryption key for the executable's tag, the
value pspdecrypt and the PSP emulators carry, so `D012Native --install` decrypts it from your own ISO, and setup
does that for you (step 4). The result is accepted only if its SHA-256 is
`0bc8e779aad2814a3d1bd77346f69ca5e16665845ed053cd0ad44a9cbfaefe6d` (4,640,948 bytes), the decrypted ULUS10566 v1.00
executable.

If you already have that file, `-Eboot <path>` uses it instead, and the bootstrap compile and the decryption are
skipped.

## What is checked

| Input | Check |
| --- | --- |
| ISO | `PARAM.SFO` says disc `ULUS10566`, version 1.00; the size is 1,754,529,792 bytes; the three overlays and `LIBSUPPREACC.PRX` have the retail SHA-256 values; the whole image's SHA-1 is `ec23f5beac89312a2a25654e202ae1c997999c9e` (Redump's USA v1.00 entry, see [usa-comparison.md](../decomp/usa-comparison.md)). Compressed images (`.cso`, `.chd`) are refused |
| EBOOT | The file decrypted from the ISO, or the one given with `-Eboot`: an ELF file (not the encrypted `~PSP` one) with the SHA-256 above. `D012Native --install` also checks the disc ID and the SHA-256 of the disc's `EBOOT.BIN` before it decrypts |
| SDL3-devel-3.4.18-VC.zip | SHA-256 `78d84602ae616cfe26b33a73b7d3b9b6b56e8a6650e53d92c9c6ca938f747acd` |
| glslang-16.6.0-windows-x86_64-release.zip | SHA-256 `82bf434e69b9bb4829de7e2b4bc2c5e7a7861e53d66cf75e5cc70f5f694a8d9b` |
| Vulkan-Headers-vulkan-sdk-1.4.363.0.zip | SHA-256 `f4be95220ff0ee0b1c620301ff4fe9ae48592d71f4c30f2d5c571351a153ae81`, or else its contents (see below) |
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
| "is disc ULES01505" | That is the European release. The port supports the USA release ULUS10566 only |
| "SHA-1 ..., but a clean dump ... has ..." | The image is not the Redump dump: a different dump tool, or a modified image. If its game files passed, `-SkipIsoHash` lets you try it |
| "is the encrypted EBOOT.BIN" | `-Eboot` was given the file from the disc. Leave `-Eboot` out, and setup decrypts it |
| "an ELF file, but not the decrypted executable the port knows" | The `-Eboot` file is from another release or version, or patched. Leave `-Eboot` out |
| "could not decrypt the EBOOT from the ISO" | Read `build/port/logs/setup-decrypt.log`, which has the program's message |
| "PortableKit has uncommitted changes" | Someone edited `tools/local/port/PortableKit`. Commit or stash there, or delete that folder and run setup again |
| "the PortableKit patches do not apply" | `port/portablekit-patches` and the clone disagree. Delete `tools/local/port/PortableKit` and run setup again |
| The build fails | Read the log setup names (`build/port/logs/setup-build.log`). Out of memory: close other programs and run again (it continues). A path-length error: move the repository closer to the drive root, for example `C:\Games\Dissidia012-pc` |
| The folder path contains `!`, `%`, `^` or `&` | Move the repository to a plain path; the Windows build scripts cannot handle those characters |
| The shortcut does nothing | Run `.\tools\port\play_d012.ps1` in PowerShell to see the message. If the port is not built, it says so in a window |
| Start over | Delete `tools/local/port/build-d012-recomp` and `build/port/recomp` (build files), or `build/port/setup/state.json` (to be asked for the ISO again). Keep `build/port/data`: your saves are there |

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

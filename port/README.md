# Dissidia 012 native port (PortableKit profile)

The native PC port of *Dissidia 012 Duodecim Final Fantasy*, USA release ULUS10566 v1.00 (and the European release
ULES01505 v1.00, which holds the same executable: [regions.md](../docs/port/regions.md)), on
[PortableKit](https://github.com/TeamGDB/PortableKit). MIT licence, like PortableKit.

This directory holds only our own code: the profile and its build file. No game data, no generated code
and no overlay corpora ever go here. Everything derived from the game lives under the ignored `build/port/`.

State (18 October 2026): under PortableKit's interpreter, with no recompiled code, the game boots to the title
screen, starts a New Game (data setup, nickname, its own save in `build/port/data/ms0`), plays the opening movie,
loads that save, runs the Moogle tutorial and its first battle. Message, save-data and keyboard dialogs are drawn
by the port and wait for the player. The recompiled build (below) runs with no interpreter from the owner's save through
the menus to Quick Battles, with the HUD's HP bars, billboarded particles and silent autosaves as on the handheld. See
[docs/port/bring-up-log.md](../docs/port/bring-up-log.md).

## Layout

| Path | What it is |
| --- | --- |
| `CMakeLists.txt` | Adds PortableKit and calls `portablekit_add_game()` |
| `host/d012_profile.cpp` | The profile: constants only (identity, hashes, memory, overlay window, saves, console settings, the extra buttons L2/R2, frame interpolation's distances for a world in metres) |
| `host/d012_hle.cpp`, `.hpp` | The profile's two hooks: a few HLE answers, an overlay-window dump for diagnosis, and the mods' and the playlist's installs |
| `host/d012_prx.cpp` | Loads the disc's `LIBSUPPREACC.PRX` for real and routes main's `scesupPreAcc` imports to it |
| `host/d012_mods.cpp`, `.hpp` | Mods: reads `<data dir>/mods`, checks each `mod.json`, keeps the catalogue of added music series and tracks, and serves a track to the game as a BGM file ([docs/port/mod-folder.md](../docs/port/mod-folder.md)) |
| `host/d012_json.cpp`, `.hpp` | A small JSON reader for the manifests |
| `host/d012_bgm_playlist.cpp`, `.hpp` | The BGM playlist: the loading screen's music choice by series and track, L2/R2 to switch the D-pad between the screen's menu and the music, mod tracks as rows of the game's BGM table, the pad-read wrapper, the series in the BATTLE BGM box and the track wheel under it ([docs/port/bgm-playlist-design.md](../docs/port/bgm-playlist-design.md), sections 10 and 11) |
| `host/d012_dlc.cpp`, `.hpp` | DLC: one startup log line saying what DLC the game will find in `ms0:/PSP/GAME/<disc id>`, how many files are still encrypted EDATA, and a warning for `.edat` files in a folder of another name; it only reports ([README](../README.md#downloadable-content-dlc)) |
| `host/d012_loading_layout.cpp`, `.hpp` | The loading screen's right-half pages moved down and made shorter for the track wheel, by checked edits of the screen's layout data in memory (section 11) |
| `host/d012_test_switches.cpp`, `.hpp` | Test switches, off by default: `D012_TEST_UNLOCK_FIGHTERS`, `D012_TEST_EX_FULL` ([docs/port/graphics-triage.md](../docs/port/graphics-triage.md)) |
| `host/d012_brave_pool_guard.cpp` | In battle, the pool of flying Brave numbers is put right before each update: a slot holding anything but one of its entries gets a free entry, or none, instead of crashing the game ([issue 16](https://github.com/VCKing101/Dissidia012-pc/issues/16)) |
| `host/d012_crash_report.cpp` | The crash report, always on: an unhandled exception or an abort logs the exception, its module and offset, and the stack, and writes a minidump to `data/logs/D012Native-crash.dmp` (`D012_CRASH_DUMP=<dir>`: one per run in that folder) ([docs/port/quit-crash.md](../docs/port/quit-crash.md)) |
| `host/d012_decomp_harness.cpp`, `.hpp`, `host/d012_decomp_bgm.cpp` | Decompiled functions as native overrides, and the differential test against the recompiled originals |
| `decomp/` | The decompiled BGM functions (C) |
| `examples/mods/example-tones/` | An example mod: a manifest and a script that writes its generated test tones (no audio files are kept) |
| `host/d012_mod_manifest.cpp`, `.hpp` | The manifest rules: reads and checks a `mod.json` into a manifest and its problems, for the game (`d012_mods.cpp` logs them) and the Battle BGM Maker alike |
| `bgm_maker/` | The Battle BGM Maker, `BgmMaker.exe`: a program of its own (SDL3, Vulkan, PortableKit's Dear ImGui, the game's FFmpeg) for making music mods, with none of the game's code ([guide](../docs/port/mod-folder.md#8-making-a-mod-with-the-battle-bgm-maker)). `psp_iso.cpp` and `psp_export.cpp` make its optional PSP / PPSSPP ISO, running atracdenc as a separate program |
| `tests/` | The port's C++ tests, `d012_tests` (`tools/port/pk_ctest.cmd <build dir>`): manifest rules with `manifest_cases.json`, which the Python tool's tests run too, the JSON writer, the BGM Maker's model, saving, suggester and ISO writer, on synthetic data |
| `tools/bgm_maker/` | The earlier Battle BGM Maker in Python (a local page in the browser), kept for developers, frozen; its `tests/` hold the end-to-end checks, which run `BgmMaker.exe`'s command line |
| `config/nids.csv` | NIDs the game imports that PortableKit's table does not name |

Outside this directory:

| Path | What it is |
| --- | --- |
| `tools/local/port/PortableKit` | The framework, a local clone on its local branch `dissidia` (not a submodule) |
| `tools/local/port/build-d012` | This port's build directory |
| `build/port/game` | The game directory: `EBOOT.ELF`, `disc.iso`, `ms0/` |
| `build/port/data` | Settings, pipeline cache and anything else the port writes (`D012_DATA_DIR`), and the memory stick `ms0/` the runner uses (`D012_MEMORY_STICK_DIR`) |
| `build/port/generated`, `build/port/overlays` | Where the AOT corpus and overlay corpora will go (`D012_WORK_DIR`); empty for now |
| `setup.ps1`, `tools/port/setup/` | The one-command setup for players ([docs/port/setup.md](../docs/port/setup.md)) |
| `tools/port/pk_cmake.cmd` | Runs cmake in the Visual Studio 2022 environment with the local dependencies |
| `tools/port/prepare_game_dir.py` | Makes `build/port/game` from the read-only inputs |
| `tools/port/run_d012.py` | Runs the port for a bounded time and keeps the log |
| `tools/port/overlay_meta.py` | Writes PortableKit's `meta.txt` for the three overlays (and can run `psp_recomp` on them) |
| `tools/port/shots_to_png.py` | Converts a run's BMP captures to PNG, optionally as a contact sheet |
| `tools/port/ge_list_summary.py` | Folds a `D012_TRACE_GE_LIST` dump into one line per draw state (targets, textures, stencil, masks); `--detail` adds depth function, blend, texture function, alpha and colour tests, cull |
| `tools/port/gfx_triage.py` | Runs one scripted scene (world map, gateway, orb, Quick Battle stages, Kuja, an EX Burst) on the recompiled build with A/B switches and converts the captures ([docs/port/graphics-triage.md](../docs/port/graphics-triage.md)) |
| `tools/port/gfx_tour.py` | The graphics tour: scripted routes through menus, the world map and a gateway, five Quick Battle stages, EX Mode and EX Burst, Kuja and the results screen, in three variants (owner's settings with the frame-replay check, 30 fps, 1x), with the GE census per scene and captures compared with a local baseline ([docs/port/graphics-census.md](../docs/port/graphics-census.md)) |
| `tools/port/check_quit_crash.py` | Runs the recompiled build to a battle or the world map and quits, N times, camera on and off, and counts the runs that crash on the way out ([docs/port/quit-crash.md](../docs/port/quit-crash.md)) |

## Build

`setup.ps1` at the repository root does every step below for the recompiled build (dependencies, the PortableKit
clone and its patches, the game directory, the corpora, the build and a shortcut), checking each input's hash and
skipping what is already done: [docs/port/setup.md](../docs/port/setup.md). The steps by hand follow.

From the repository root, in PowerShell. The configure must report `D012Native: Vulkan renderer enabled`.

```powershell
.\tools\port\pk_cmake.cmd -S port -B tools/local/port/build-d012 -G Ninja
.\tools\port\pk_cmake.cmd --build tools/local/port/build-d012 -j2
```

Cache variables: `D012_PORTABLEKIT_DIR` (default `tools/local/port/PortableKit`) and `D012_WORK_DIR` (default
`build/port`). The first configure downloads PortableKit's pinned FFmpeg build into
`tools/local/port-deps/dl/ffmpeg`. With no corpus the build takes a few minutes.

## Prepare the game directory

```powershell
.\.venv\Scripts\python.exe tools/port/prepare_game_dir.py
```

It copies `build/ULUS10566/EBOOT.ELF` (checking its SHA-256), hard-links the retail ISO at the repository root as
`disc.iso` (PortableKit opens it read-only; `--copy-iso` copies instead, and so does a hard link that fails because the
ISO is on another drive), and makes an empty `ms0/`. `--eboot FILE` and `--iso FILE` name other inputs. Files already
right are kept. No saves are copied in. The originals are never written.

`EBOOT.ELF` can also come from the disc itself: the profile carries the public decryption key for the executable's
tag (`0xD91613F0`), so PortableKit's installer, `D012Native --install image.iso --in-place` run with `D012_DATA_DIR`
set, writes the decrypted `EBOOT.ELF` (and a `settings.ini` naming the image) into that directory. `setup.ps1` does
this in a temporary folder with a bootstrap build (no corpus yet) and passes the result to `prepare_game_dir.py
--eboot`.

## Run

Always bounded. `run_d012.py` sets `D012_GAME_DIR=build/port/game`, `D012_DATA_DIR=build/port/data` and
`D012_MEMORY_STICK_DIR=build/port/data/ms0` (so the game's saves never land in `build/port/game/ms0`), stops the
process it started after at most 90 s, and writes the output to `build/port/logs/`.

```powershell
# Headless boot check
.\.venv\Scripts\python.exe tools/port/run_d012.py --headless --seconds 30 --env D012_LIST_STUBS=1

# With the window: captures every 60 frames, one perf line a second, and a clean quit at frame 1100
# (the end-of-run report needs a clean stop)
.\.venv\Scripts\python.exe tools/port/run_d012.py --seconds 55 --env D012_SCREENSHOT_DIR=build/port/shots `
    --env D012_SCREENSHOT_EVERY=60 --env D012_PERF=log --env "D012_INPUT_SCRIPT=1100:quit"
```

Useful switches (all PortableKit's, with this port's prefix `D012`): `D012_TRACE_IO`, `D012_TRACE_KERNEL`,
`D012_TRACE_GE_LIST=N[:M]`, `D012_TRACE_SAVEDATA`, `D012_TRACE_FONT`, `D012_LIST_STUBS`, `D012_STRICT_HLE`,
`PSPRECOMP_HLE_HISTOGRAM=1`, `PSPRECOMP_HLE_FIRST_CALLS=1` (each HLE function's first call, in order). Never set
`D012_AUTO_CONFIRM`: it walks through the game's prompts, past the title screen.

Driving the game: `D012_INPUT_SCRIPT` for a known prefix (frames are game frames), plus `D012_INPUT_LIVE=<file>`
to append steps while it runs, with `shot NAME` steps to look before the next input, and `speed normal` /
`speed unlimited` to change the game speed mid-run (reach a battle unthrottled, then watch it at real time). `D012_UNTHROTTLED=1` runs
menus about eight times faster than real time, which keeps a New Game inside the 90 s cap; it also brings the
title screen's idle timer, and so its attract movie, round eight times sooner.

Dialogs (framework, see the table below): the message, save-data (list, save, delete) and keyboard dialogs are
drawn over the game and wait for the player. For runs without a window, `D012_MSGDIALOG_ANSWERS=ok,no,...` and
`D012_SAVEDATA_ANSWERS=new|first|cancel|<save name>,...` answer them in order; with neither, the game waits.
`D012_MSGDIALOG_INSTANT` and `D012_SAVEDATA_INSTANT` skip the dialogs but never confirm a question, an overwrite
or a delete.

The profile's own switches:

| Variable | What it does |
| --- | --- |
| `D012_NO_SUPPREACC=1` | Do not load `LIBSUPPREACC.PRX`; its imports become logging stubs answering 0, as in plain PortableKit |
| `D012_DUMP_SLOT=N`, `D012_DUMP_SLOT_DIR=dir` | Write the overlay window to `dir/overlay_window_N.bin` at the game's Nth vblank wait, for `overlay_meta.py --check` |
| `D012_BLOOM=0` or `1` | Bloom off or on for this run, whatever the menu's Video page says (`game.bloom` in `settings.ini`; `port/host/d012_bloom.cpp`: the bloom at `0x0885E150` is left out) |
| `D012_NO_MODS=1` | Load no mods ([mod-folder.md](../docs/port/mod-folder.md)) |
| `D012_MODS_DIR=dir` | Read mods from `dir` instead of `<data dir>/mods` |
| `D012_TEST_BGM_OVERRIDE=<track id>` | Test only: the next BGM file the game opens plays that mod track instead (`example-tones:intro-loop`, or its number in hex) |
| `D012_TEST_MODS_DECODE=1` | Test only: decode every mod track at start-up and log its length, level and pitch |
| `D012_NO_BGM_PLAYLIST=1` | The game's own one-line BGM selector and screen layout: no series, no wheel, no L2/R2 focus, no mod tracks offered |
| `D012_BGM_PLAYLIST_FLAT=1` | The playlist's earlier one-line selector (the track in the box, the series on a line under it), the pages where the game has them |
| `D012_TEST_BGM_UNLOCK_ALL=1` | Test only: the selector offers every row with a weight, locked or not (the save is not touched) |
| `D012_TRACE_BGM_LAYOUT=1` | Log the loading screen's layout elements, animations, parts and anchors |
| `D012_TRACE_BGM_PLAYLIST=1` | Log every mod row and every choice of the selector with its series, id, volume and file |
| `D012_TEST_BGM_START=<id>` | Test only: the selector starts on that track (a BGM id, or a mod track's id text) instead of the game's random default |
| `D012_DECOMP_BGM=1`, `D012_DECOMP_VERIFY=1` | All twelve decompiled BGM functions in the game; the differential test of them (bgm-playlist-design.md, section 2) |
| `D012_TEST_UNLOCK_FIGHTERS=1` | Test only: every fighter can be picked on the select screens (the unlock flags are set in the running game; an autosave keeps them, so use a copy of the data directory) |
| `D012_TEST_EX_FULL=1` | Test only: in battle the player's EX gauge is kept full (R + square enters EX Mode; the next HP attack that lands is an EX Burst) |
| `D012_CRASH_DUMP=<dir>` | On a crash, write the minidump to `<dir>/crash_<pid>.dmp` instead of `<data>/logs/D012Native-crash.dmp` (the logged stack has names from `D012Native.pdb` when the build links with `/DEBUG`, else module+offset) |
| `D012_BRAVE_POOL_GUARD=0` | The Brave numbers' pool is left as it is (`port/host/d012_brave_pool_guard.cpp`): a slot holding something that is not an entry crashes the battle, as in issue 16 |
| `D012_TEST_CRASH=<s>[,abort]` | Test only: a thread reads address 0 (or calls `abort()`) that many seconds after start, to check the crash report |

And the framework changes' off switches, each restoring the old behaviour: `D012_QUICK_BY_PICTURE=1` (texture packs
with `hash = quick` are matched to the game's textures by picture instead of by their hash), `D012_NO_QUICK_MATCH=1` (such packs are refused), `D012_SHARP_RENDER_TARGETS=1` (with Texture filter on Sharp, render targets sampled
as textures are sharp too, so the bloom shows blocks), `D012_NO_COLOR_DOUBLING=1` (colour
doubling), `D012_NO_PIXEL_MASK=1` (pixel mask), `D012_NO_VBLANK_CALLBACKS=1` (callbacks in the vblank waits),
`D012_NO_STENCIL=1` (GE stencil), `D012_NO_SHARED_DEPTH=1` (depth buffers shared by address),
`D012_NO_THROUGH_DEPTH=1` (depth in through mode), `D012_NO_SCREEN_SPRITES=1` (transformed sprites upright on the
screen), `D012_NO_SAVEDATA_LIST_STAR=1` (`*` in LIST patterns), `D012_NO_HOST_AUDIO=1` (host audio files: no mod
music is decoded or served), `D012_NO_EXTRA_BUTTONS=1` (L2/R2 never reach the port; the triggers press L and R), `D012_NO_STICK_SQUARE=1`
(the left stick's circle passed on as it is, as the menu's "Full diagonals" off does), `D012_NO_FB_SNAPSHOTS=1`
(interpolated frames sample a render target's one copy), `D012_NO_DOUBLE_ALPHA=1` (blend factors 6 and 7 read the
plain alpha), `D012_NO_TEXTURE_RGB=1` (the texture's alpha is used even when TCC says colour only), `D012_CLUT_FIRST_WORD_KEY=1` (palette textures
cached by their palette's first word, as before). For tests:
`D012_CAPTURE_BLENDED=1` makes a `shot` wait for an interpolated present; `D012_GE_CENSUS=<file>` writes the GE
census, the input script's `scene NAME` naming its scenes ([graphics-census.md](../docs/port/graphics-census.md)).

## Overlays

The game loads `OVL_MENU_APP.ELF`, `OVL_BATTLE_APP.ELF` or `OVL_EXTRA_APP.ELF` into one window at `0x089DD4C0`.
Under the interpreter nothing is needed. For a recompiled build each needs a corpus and a `meta.txt`, which
PortableKit's `add_overlay.py` cannot write (it expects a Capcom `MWo3` header):

```powershell
.\.venv\Scripts\python.exe tools/port/overlay_meta.py build/ULUS10566/OVL_MENU_APP.ELF `
    build/ULUS10566/OVL_BATTLE_APP.ELF build/ULUS10566/OVL_EXTRA_APP.ELF            # meta.txt only
.\.venv\Scripts\python.exe tools/port/overlay_meta.py --recompile build/ULUS10566/OVL_MENU_APP.ELF  # also psp_recomp
```

The identity hash covers the first 4,096 bytes of the image by default. The game writes into loaded overlays
(code and data share one segment); for the menu overlay the first byte it changes is at `+0x3500`, measured
with `D012_DUMP_SLOT` and `--check`. Battle's is at `+0x455F0`; extra was not seen loaded (see
[recompiled-build.md](../docs/port/recompiled-build.md)). `overlay_meta.py` now writes into the recompiled build's
work directory, `build/port/recomp/overlays`, by default (section "Recompiled build" below).

## Framework changes

All on the clone's local branch `dissidia`, committed there and never pushed. None names the game. The same
commits are kept as the numbered patches in `portablekit-patches/`, and the table names them by patch number; on a
fresh clone of `33800c4`, apply them with `git checkout -b dissidia 33800c4` and
`git am <repo>/port/portablekit-patches/*.patch`.

| Patch | Change | Why |
| --- | --- | --- |
| 0001 | `Elf32Image::runtime_entry()` falls back to the `module_start` export when `e_entry` is 0 (`src/elf32.cpp`, `include/psprecomp/elf32.hpp`) | `EBOOT.ELF` has `e_entry` 0; its entry is the `module_start` export `0x08804040`, which is what the PSP's loader starts |
| 0002 | `VERBATIM` on the NID-table command (`cmake/PortableKit.cmake`) | With a profile `config/nids.csv`, the `|` joining the two tables was taken for a pipe by the Windows shell and the build failed |
| 0003 | GE colour doubling, texture function bit 16 (`host/gpu/ge_state.*`, `host/gpu/shaders/ge.frag`, `host/gpu/vulkan_renderer.cpp`); `<prefix>_NO_COLOR_DOUBLING` turns it off | The title screen's quads use vertex colour `0x80` with doubling on (`0xC9010100`); without it the white screen came out grey (127) |
| 0004 | The message dialog is drawn over the game (`host/ui/message_dialog.*`, `host/ui/menu.cpp`) and waits for OK, Yes, No or Back; `<prefix>_MSGDIALOG_ANSWERS` scripts answers, `<prefix>_MSGDIALOG_INSTANT` answers at once but No to questions; `sceUtilityMsgDialogAbort` (`host/hle/hle_utility.cpp`) | It answered every message at once, "Yes" to every question, without showing it |
| 0005 | `<prefix>_MEMORY_STICK_DIR` names another `ms0` (`host/main.cpp`) | So the port's fresh saves live in `build/port/data/ms0`, away from the game directory |
| 0006 | `sceFontGetFontInfo` writes the glyphs' bits per pixel, byte 260 (`host/hle/hle_font.cpp`) | Left at 0, the game computed a glyph line length of 0 and drew every string over the next: all menu text was stripes |
| 0007 | `sceDisplayWaitVblankStartCB` and `...MultiCB` run the thread's notified callbacks (`host/hle/hle_media.cpp`); `<prefix>_NO_VBLANK_CALLBACKS` turns it off | The game's only callback wait; after its first save it waited for good, on a white screen, for its `sceIoLseekAsync` callback |
| 0008 | `sceKernelCancelSema` (`host/hle/hle_threadman.cpp`, `host/kernel/kernel.hpp`) | Was a logging stub; the movie player cancels its semaphore when the opening movie ends |
| 0009 | `PSPRECOMP_HLE_FIRST_CALLS` logs each HLE function's first call with its caller (`src/runtime.cpp`, `include/psprecomp/runtime.hpp`) | The ordered list of system calls a run reached |
| 0010 | The save-data list modes, SAVE and DELETE open a drawn dialog (`host/ui/save_list_dialog.*`, `host/hle/hle_savedata.cpp`): slots with title, detail and date, then a Yes/No question (overwrite and delete open on No); `<prefix>_SAVEDATA_ANSWERS`, `<prefix>_SAVEDATA_INSTANT` | LISTSAVE silently saved to the first listed slot (overwriting it if used); LISTDELETE deleted the first save |
| 0011 | The GE pixel mask (`0xE8`, `0xE9`) becomes the colour write mask for whole channels; 16-bit formats read it as the 16-bit pixel (`host/gpu/ge_state.*`, `host/gpu/vulkan_renderer.cpp`); `<prefix>_NO_PIXEL_MASK` turns it off | The battle's stencil-only shadow-volume passes (mask `0x7FFF` in 5551) were drawn in colour: a blue screen with the scene in a diamond |
| 0012 | Import wrappers of a prefixed corpus use an uncached import slot (`tools/codegen_main.cpp`, `src/runtime.cpp`) | A run-time library with imports of its own would otherwise call the executable's imports of the same index |
| 0013 | An overlay library whose base is in no overlay slot installs on a miss inside its own range (`host/overlays.cpp`) | Lets the disc PRX, loaded at a fixed address outside the window, be a recompiled library |
| 0014 | GE stencil: the test, its operations and clear-mode stencil writes (`0x24`, `0xDC`, `0xDD`), kept in a stencil aspect of the depth attachment and in step with the colour target's alpha, each rebuilt from the other before it is read; the pixel mask limits stencil writes (`host/gpu/ge_state.*`, `host/gpu/vulkan_renderer.cpp`, `host/gpu/shaders/ge.frag`, `host/perf/frame_stats.*`); `<prefix>_NO_STENCIL` turns it off, `<prefix>_TRACE_STENCIL` logs the copies | The battle's shadow volumes showed as dark shafts under the fighters |
| 0015 | A depth buffer is shared by the colour buffers that name it: a draw first copies the newest picture of its depth buffer from the target that drew it (`host/gpu/vulkan_renderer.cpp`); `<prefix>_NO_SHARED_DEPTH` turns it off | The battle tests its shadow volumes, drawn into `0x0CC000`, against the scene's depth at `0x88000`, drawn with `0x0`/`0x44000`; each target had its own, empty, depth image |
| 0016 | Input script step `speed unlimited` / `speed normal` (`host/ui/input_script.*`) | Reach a battle unthrottled inside the 90 s cap, then watch it at real time |
| 0017 | `<prefix>_TRACE_GE_LIST=N[:M[:W]]`: a word limit per list, 20000 by default (`host/gpu/ge_state.cpp`); patch 0022 documents it | A battle frame's list is about 24,000 words; the HUD comes last |
| 0018 | Through-mode draws are depth-tested and write depth when the GE says so (`host/gpu/vulkan_renderer.cpp`); `<prefix>_NO_THROUGH_DEPTH` turns it off | The battle HUD cuts each HP bar to length with depth-only draws (every colour bit masked) and draws the bar through that depth; with no depth in through mode the bar stayed full |
| 0019 | Transformed sprites are drawn upright on the screen, as the GE draws them, not laid flat in model space; `<prefix>_TRACE_SPRITES` also lists them (`host/gpu/vulkan_renderer.cpp`); `<prefix>_NO_SCREEN_SPRITES` turns it off | Battle particles, dust and orbs are 3D sprites; laid flat they showed edge-on from the side |
| 0020 | Save-data LIST matches `*` in the save name (`host/hle/hle_savedata.cpp`); `<prefix>_NO_SAVEDATA_LIST_STAR` turns it off | The game lists `GameData*`; finding nothing, every autosave became "Unable to autosave" and the LISTSAVE screen |
| 0021 | The message and save-data dialogs drawn as a translucent band over the game, white centred text, soft highlight, hints at the lower right (`host/ui/message_dialog.*`, `save_list_dialog.*`, `widgets.*`) | They looked like the port's own menus |
| 0023 | File providers: `hle_extension::add_file_provider` serves a file at a guest path, in place of the file there or where there is none (`host/hle/hle_io.cpp`, `include/portablekit/hle_extension.hpp`, interface version 4; nine new checks in `tests/io_filter_tests.cpp`). Nothing changes until a provider is added | Mods serve their music at paths the game opens |
| 0024 | FFmpeg's libavformat and libswresample linked too: the Windows build copies `avformat-61.dll`; the source build for macOS and Linux adds the wav, mp3, ogg and flac demuxers and the MP3, Vorbis, Opus, FLAC and PCM decoders, still LGPL only (`cmake/FFmpeg.cmake`, packaging lists, notices) | Decoding mod music in common formats |
| 0025 | Host audio tracks: a host audio file (MP3, Ogg Vorbis, Opus, FLAC, WAV) decoded to 44,100 Hz stereo and played through a stand-in ATRAC3plus file whose private `PKHA` chunk names it; sceAtrac fills its frames from the file and runs everything else as for the game's own tracks (`host/audio/host_audio.*`, `host/hle/hle_atrac.cpp`); `<prefix>_NO_HOST_AUDIO` turns it off | Mod music with no ATRAC3plus encoding |
| 0026 | Extra buttons: L2, R2, L3, R3, which a PSP lacks, for a port to give a meaning of its own. `GameProfile::extra_buttons` names them; with L2 or R2 named the triggers press those instead of copies of L and R; `hle_extension::extra_buttons()` reads them (interface version 5); keyboard actions L2/R2/L3/R3 (1 and 3 for L2 and R2 by default), listed in the menu only for the named ones (`host/input/bindings.*`, `host/gpu/vulkan_renderer.*`, `host/hle/hle_media.cpp`, `host/profile.*`, `host/ui/menu.cpp`; checks in `tests/input_tests.cpp`); `<prefix>_NO_EXTRA_BUTTONS` turns it off | The BGM playlist's focus switch |
| 0027 | Game overlays: `ui::add_game_overlay` draws a port's own picture with ImGui over every game frame it asks for, `ui::game_picture()` says where the game's 480x272 picture is in the window, `ui::game_text_font()` is the game's text face as an ImGui font (`host/ui/ui.hpp`, `layer.cpp`, `menu.cpp`, `host/gpu/vulkan_renderer.*`, `host/fonts/game_font.*`). Nothing is drawn until a port adds one | Showing the BGM playlist's focus beside the game's box |
| 0028 | The left stick as the nub: dead zones on the vector's length, full from 0.95 of the radius, then circle to square, so a full push in any direction reaches the edge of the nub's square range and a full diagonal its corner; each axis rounded to the nearest step, full left/up now 0 rather than 1 (`host/input/stick.*`, `host/gpu/vulkan_renderer.cpp`, `host/settings/settings.*`, `host/ui/menu.cpp`; checks in `tests/input_tests.cpp`). On by default; the controls page's "Full diagonals" (`input.stick_square`) or `<prefix>_NO_STICK_SQUARE` turns the square off | A round stick's full diagonal reached the nub as about 0.71 per axis (bytes 217/39). The game's pad module drops each axis within 100 of 127.5 (`docs/research/pad-module.md`), so every full diagonal read as no input and 22.5 degrees off a straight push as 0.64 of one |
| 0030 | The render targets are destroyed before the descriptor pool at shutdown, and the pool's handle is cleared (`host/gpu/vulkan_renderer.cpp`) | `destroy_target` frees a sampled target's descriptor sets from that pool; after the pool was destroyed, about 1 quit in 13 faulted in the AMD driver ([docs/port/quit-crash.md](../docs/port/quit-crash.md)) |
| 0031 | Exit status 0 after closing the window, Quit in the menu or the game's `sceKernelExitGame`; 4 only after an error stop (`host/system.hpp`, `host/main.cpp`, `host/hle/hle_media.cpp`, `host/hle/hle_system.cpp`) | Every run exited with 4, an error's code, however it ended |
| 0032 | `<prefix>_CAPTURE_BLENDED`: a window capture waits for a present frame interpolation blended (`host/gpu/vulkan_renderer.cpp`, `docs/BRINGING_UP_A_GAME.md`) | Interpolation faults show only on in-between frames ([graphics-triage.md](../docs/port/graphics-triage.md)) |
| 0033 | While frames are recorded for interpolation, a draw into the shown framebuffer that samples a render target samples a snapshot of the part it reads, kept three flips (`host/gpu/vulkan_renderer.cpp`); `<prefix>_NO_FB_SNAPSHOTS` turns it off | In-between frames composited a later pass's picture of a reused scratch buffer: a scene miniature top-left in battle, the glow and blur strobing on the world map; 43 to 86% of a replayed battle frame's pixels differed, now 0 |
| 0034 | GE blend factors 6 and 7 (twice the source alpha, one minus it): the shader doubles the alpha it hands the blender, the alpha written is halved back through the blend constant (`host/gpu/vulkan_renderer.cpp`, `host/gpu/shaders/ge.frag`); `<prefix>_NO_DOUBLE_ALPHA` turns it off | Additive effects (Kuja's orbs, hit flashes) came out at half strength |
| 0035 | The texture function's TCC bit: colour-only textures keep the fragment's alpha (`host/gpu/vulkan_renderer.cpp`, `host/gpu/shaders/ge.frag`); `<prefix>_NO_TEXTURE_RGB` turns it off | The world map's swirl into a gateway blends the previous frame by the vertex alpha; the framebuffer's stencil alpha (0) hid it |
| 0036 | GE census: `<prefix>_GE_CENSUS=<file>` checks every draw against 76 features the renderer approximates or ignores and writes their counts per scene, first frame and an example draw as JSON; input script step `scene NAME` (`host/gpu/ge_census.*`, one-line hooks in `host/gpu/ge_state.cpp`, `host/gpu/vulkan_renderer.cpp`, `host/main.cpp`, `host/ui/input_script.*`; `docs/BRINGING_UP_A_GAME.md`). Nothing changes without the variable | A systematic list of what to fix before the owner meets it ([graphics-census.md](../docs/port/graphics-census.md)) |
| 0037 | The census describes a draw only at a feature's first hit and writes every 600 frames (`host/gpu/ge_census.*`) | Formatting every draw slowed a battle to real time |
| 0038 | A palette texture's cache key takes the bytes its last CLUT load read and its index shift, mask and start, as does the per-list lookup (`host/gpu/texture_decode.cpp`, `host/gpu/vulkan_renderer.cpp`); `<prefix>_CLUT_FIRST_WORD_KEY` turns it off | EX Revenge reads one framebuffer through three index shifts of one CLUT, which shared one decode; battle palettes changed after their first entry kept the old one |
| 0039 | The census counts blend factors 6/7 and TCC as handled, and as approximations only with their off switches (`host/gpu/ge_census.cpp`) | Patches 0034 and 0035 |
| 0040 | A palette texture over a render target is decoded on the GPU at the internal scale (`host/gpu/shaders/fb_clut.frag`, `host/gpu/vulkan_renderer.cpp`), respecting interpolation snapshots; `<prefix>_NO_FB_CLUT` turns it off, `<prefix>_TRACE_FB_CLUT` traces it | EX Revenge reads the frame through CLUT16 shifts to make a greyscale negative; decoded from stale memory it came out flat grey |
| 0041 | Line and line-strip primitives drawn as quads one PSP pixel wide at the internal scale through the draw's own pipeline (`host/gpu/line_quads.hpp`); `<prefix>_NO_LINES` skips them, `<prefix>_TRACE_LINES` traces them | Every hit's spark streaks (about 25,000 line draws per battle) were never drawn |
| 0042 | A block transfer out of a render target is also blitted on the GPU at the internal scale into a target for its destination, used while the guest bytes still match (`host/gpu/vulkan_renderer.cpp`); `<prefix>_NO_TRANSFER_TARGETS`, `<prefix>_TRACE_TRANSFERS` | EX Burst copies the screen into RAM and draws it as its background for 56 frames: it was 1x at any scale |
| 0043 | Environment (shade) mapping, texture map mode 2: U,V from the world-space normal and the two chosen lights (`host/gpu/shaders/ge.vert`, `host/gpu/vulkan_renderer.cpp`); `<prefix>_NO_ENV_MAP`, `<prefix>_TRACE_ENV_MAP` | Orphan's Cradle's violet wheel strips used their vertex UVs; the PSP formula is from public SDK docs, the game's edge case (no normals, unset light) unverified on hardware |
| 0044 | A texture whose first texel lies in one target's drawn area reads that target, not a newer one that only covers it with undrawn rows (`find_framebuffer_texture()`, `host/gpu/vulkan_renderer.cpp`); `<prefix>_NO_TARGET_DRAWN_AREA` restores the old choice | Lightning's RAV Water refracts a half-size copy of the last frame kept right after another target; it read the other target's undrawn rows and looked like a negative (also Razor Gale's dome) |
| 0045 | Performance instruments: a per-frame CSV (`<prefix>_PERF_CSV`) and a stall sampler (`<prefix>_TRACE_STALL_SAMPLES`) | Measuring the 45/30 fps dips ([performance.md](../docs/port/performance.md)) |
| 0046 | An overlay swapped out keeps its registrations, so swapping back is a lookup, not a re-install (about 70 ms instead of 180); `<prefix>_NO_OVERLAY_PARKING` | Menu-to-battle and world-map swaps stalled for 210-305 ms and gapped the music |
| 0047 | memcpy, memset and DMA copies done as one host copy, not byte by byte; `<prefix>_NO_FAST_HLE_COPIES` | About 0.7 ms a frame of guest time |
| 0048 | Interpolated presents between flips fall in the game's idle time, and are polled at the runtime's starvation boundaries in long stretches of guest code; `<prefix>_PRESENT_AFTER_FLIP` and `<prefix>_NO_CODE_PRESENT_POLL` restore the old pacing | One present in ten came up to 7 ms late (11/22 ms intervals, the "45" the meter showed); costs about 9 ms of display delay |
| 0049 | Performance statistics safe to note from a second thread | Patch 0050 |
| 0050 | Presents on a thread of their own: the emulation thread records and submits each game frame and hands it over; the present thread acquires, replays the interpolated frame, submits and presents at its due time (renderer state behind one lock, the swapchain the present thread's alone); `<prefix>_NO_PRESENT_THREAD` restores patch 0048's pacing exactly | Steady 60 fps without 0048's extra delay: 99th-percentile interval 17.3 ms, display delay 19.2 ms ([performance.md](../docs/port/performance.md), section 9) |
| 0051 | Tests of the present thread's hand-off and timing (`tests/present_queue_tests.cpp`) | |
| 0052 | The last stretch of a present's wait spins without yielding | Present timing to well under a millisecond (99th percentile 0.6 ms late) |
| 0053 | No present before its time when a flip moves the timing grid | Keeps presents from coming early |
| 0054 | Save data: the encrypted data file keeps the data's own length, and its hash is of the file padded with zeros to 16 bytes, as a PSP and PPSSPP write it; a save an earlier build padded is written again at the game's length when it loads (`<prefix>_NO_SAVEDATA_REPAIR` leaves it) | Saves move between the port and PPSSPP both ways: PPSSPP said "Unable to load data" for the port's saves, and the port refused PPSSPP's as broken |
| 0055 | Port markers in display lists (`GeState::MarkerSink`), and a split screen from them: the first marker copies the left view aside, the second puts the middle half of each view side by side back into the target, under what the game draws next | Local split-screen versus ([local-versus.md](../docs/port/local-versus.md)) |
| 0056 | A second player's gamepad while a port wants one (extension interface 6: `want_second_pad`, `second_pad`); the input script's `pad2`, `axis2`, `detach2`, `attach2` | Player 2's controller |
| 0057 | The input script's second pad takes player 2's place from a real one | A real pad connected during a scripted run became player 2's |
| 0058 | Whether player 1 has a gamepad (extension interface 7, `first_pad_connected`); the input script's `detach`, `attach` | Pausing a versus battle when player 1's gamepad goes |
| 0059 | `Runtime::registered_function`: the recompiled function registered at an address | An override that only sometimes replaces a function runs the original otherwise |
| 0060 | Frame interpolation on a split screen: each draw carries its view, pairs within it and blends with its own view's camera; the replay draws each view into its half; the halves are moved, not scaled | 60 fps on both halves (local-versus.md, 7.9) |
| 0061 | The disc image read through once in the background from launch, at background I/O priority on Windows, so the file cache holds it; `<prefix>_NO_DISC_READ_THROUGH`. `<prefix>_TRACE_IO` gives each read's time | A cold hard drive stalled the first battle frame (116 ms) on the music's first block ([performance.md](../docs/port/performance.md), section 10) |
| 0062 | `ui::add_menu_rows`: a port's own settings rows at the end of the menu's Video page | The split-screen field of view slider (local-versus.md, 7.14) |
| 0063 | MinGW: a port's program links with `--export-all-symbols` (MSVC's `WINDOWS_EXPORT_ALL_SYMBOLS` does nothing there, so it had no import library for its overlay libraries) and with dbghelp, and llvm-mingw's libc++ and libunwind are copied next to it | The installer build with llvm-mingw (below) |
| 0064 | psp_recomp's lowering passes try each pattern only where its fixed text starts, anchored, instead of `std::regex_replace` over the whole unit; the output is the same | libc++'s regex took the llvm-mingw recompiler 11 minutes for the main executable (now 21 s); MSVC's 10 s became 3 s |
| 0065 | Frame interpolation: the camera is the motion most meshes drawn once agree on, and copies of one mesh pair with the copy nearest where that camera carries them instead of in drawing order; `<prefix>_INTERPOLATION_ORDER_PAIRS`. `<prefix>_CHECK_REPLAY` also counts halfway pixels far from both frames, and `=N` sets its interval | World of Darkness pillars flickered at 60 and 90 fps on the Steam Deck: once a copy left the view, the next paired with its neighbour and was drawn halfway between them ([graphics-triage.md](../docs/port/graphics-triage.md)) |
| 0066 | The stall watchdog's own report says where the emulation thread is: its guest thread, the function last dispatched, pc and ra, the HLE call it is inside, and from a second of counts (HLE calls, starvation boundaries; `Runtime::activity()`) whether it is stuck in the port's handling of a call, looping through calls, or looping in the game's code | A player's freeze in the story mode menus logged only "no frame for 7 s" and the last buttons, which cannot tell a loop in the game's code from a port call that never returned ([issue 7](https://github.com/VCKing101/Dissidia012-pc/issues/7)) |
| 0067 | The menu's Dump textures (Video page, not saved) writes each texture once into `textures/dump/<disc id>`, dropping the texture cache so loaded ones are written too; a dump also writes a 512-tall texture again under the key it gets when drawn further down, and a texture dropped for a full queue is no longer marked written | Making a texture pack needed `<prefix>_TEXTURE_DUMP` at start, and the dump missed the title screen's second key, so a pack made from it lost the title once the menu appeared ([README](../README.md#hd-texture-packs)) |
| 0068 | `--adhoc-server [port] --loopback`: the ad hoc server listens on 127.0.0.1 alone and announces nothing (`ServerConfig::loopback_only`) | Two copies on one PC play through it without the firewall asking ([communication-mode.md](../docs/port/communication-mode.md), section 10) |
| 0069 | `hle_extension::first_pad_now()`: the first pad sampled at the moment of asking, not as the game last read it | A network battle sends this machine's input to the other copy before the game reads it (section 11) |
| 0070 | `sceNetAdhocMatching` (parent, child and P2P: hellos, requests, accepts, cancels, byes, keepalives, data) over the ad hoc server, its events delivered to the game's handler from the vblank; `sceNetAdhocctlGetPeerInfo`; `PtpOpen` refuses a second socket on one port to the same peer; `PtpSend`/`PtpRecv` write 0 to the length when nothing moved | The game's Communication Mode lobby is built on matching, which PortableKit lacked; the game reopens its streams every frame until one connects, and counts the length even on would-block (sections 10 and 13) |
| 0071 | Hosting a session asks the router to forward its ports (`host/adhoc/port_mapping.cpp`): NAT-PMP, then UPnP (SSDP, `AddPortMapping`); an hour's lease renewed at half time and given back on stop; `adhoc_host_forward_also()` for a game's own ports; the Network page shows the internet address with a copy button; `network.router_ports` turns it off | Players on the internet join a hosted session without port forwarding by hand; the port adds its battle link's UDP port ([communication-mode.md](../docs/port/communication-mode.md), section 14) |
| 0072 | `<PREFIX>_HOST_SESSION=1` hosts a session as the game starts | Tests of a hosting copy with nobody at it (section 14) |
| 0073 | `ui::wait_over_game()` (a band over the last frame while the game cannot go on: hold back to leave, or a notice with OK) and `Layer::escape_held()`; `ui::add_network_rows()`; `RouterStatus::shared` (the router's own outside address is private or provider-shared); `<PREFIX>_EXAMPLE_ADDRESSES`; game-neutral hosting text | Online play's waiting screen and notices, its Input delay row, and saying when opened ports cannot help ([communication-mode.md](../docs/port/communication-mode.md), section 15) |
| 0074 | The Network page: ImGui's "conflicting ID" box only with `<PREFIX>_UI_ID_CHECK=1`, and an ID each for the status rows whose labels can repeat; Hosting counts "players online" and says a player counts from when their game goes online, and a joining player gets a Joined row saying so; a server typed into Server is kept with the recent sessions (`adhoc_remember()`) | A Steam Deck player saw the ID box, and a host saw no new player after the other joined on the page |
| 0075 | A texture whose descriptor set cannot be allocated is not made (its draws sample white), and the texture cache keeps fewer so evictions give sets back; the set is allocated before the image. A frame slot holding 256 or more evicted textures waits for pending presents to free them rather than putting them off again (`<prefix>_NO_EVICTION_WAIT`, as before). `<prefix>_TEXTURE_SETS` makes the pool smaller, to test a full one | A player's game closed while scrolling a shop: the set's allocation failed unchecked on an AMD RX 9060 XT and the driver read through the null set ([issue 7](https://github.com/VCKing101/Dissidia012-pc/issues/7); [quit-crash.md](../docs/port/quit-crash.md)) |
| 0076 | Texture packs with `hash = quick` load: each texture their keys do not cover is matched to the pack's images by picture the first time it is drawn (`host/gpu/texture_pack_match.cpp`: 16x16 signatures and 64-pixel thumbnails, a clear-winner rule and a recolour check), and the matches are kept in `textures/.matched/`. `<prefix>_NO_QUICK_MATCH` refuses such packs, as before. `portablekit_texture_pack_tests --match` runs the matcher over a texture dump | PPSSPP packs for this game made with the quick hash did not load: a community HD pack and a fan-made DLC costume's pack ([texture-packs.md](../docs/port/texture-packs.md), section 4) |
| 0077 | A profile can list another region's disc of the same executable (`RegionRelease`, `region_releases`): the installer takes it and decrypts it to the same executable, the start reads the disc id from the image, and the save screens and transfers use that release's save folders; the interface names the disc it is. Textures and the ad hoc product code stay the profile's | The European disc ULES01505 holds the North American executable, overlays and data byte for byte ([regions.md](../docs/port/regions.md)) |
| 0078 | Render targets sampled as textures are always filtered smoothly; the Texture filter setting applies to the game's textures and replacement images (`host/gpu/vulkan_renderer.cpp`); `<prefix>_SHARP_RENDER_TARGETS` samples them as the textures again | With Sharp, the bloom (a 120x68 or 30x17 target stretched over the screen) showed as square blocks ([issue 13](https://github.com/VCKing101/Dissidia012-pc/issues/13)) |
| 0079 | `ui::open_panel_over_game(frame)`: a panel of the port's own over the running game, opened from one of the game's screens; the game gets no input and the menu does not open while it is up | Options' Japanese Voice Disc row opens a file browser to choose the Japanese disc ([japanese-voices.md](../docs/port/japanese-voices.md)) |
| 0080 | `ui::game_text_font()` follows the menu's Font setting: loaded again before a frame when the face changes, with the fallback face merged in for characters it lacks (`host/ui/layer.cpp`, `fonts::fallback_face`) | The BGM playlist's track list kept the face the game started with while the box changed to the new font, and a Latin font drew the key line's arrows as "?" |
| 0081 | Linux: the bundled FFmpeg built off Windows gets the BRSTM and BFSTM demuxers and the DSP-ADPCM decoders (`adpcm_thp`, `adpcm_thp_le`); `ms0:` paths are matched in any ASCII case off Windows, as a memory stick does (`host/hle/hle_io.cpp`, `host_path`); a `std::uint64_t` list that did not compile where it is `unsigned long` | The native Linux build left every `.brstm` music mod out, played DLC music silently (the game checks `2f356e6c.edat` and opens `2F356E6C.EDAT`), and did not compile ([platforms.md](../docs/port/platforms.md)) |
| 0082 | `ui::add_system_rows` adds a port's rows to the System page, after Saves; `GameProfile::project_version` shows the port's own version in About, with PortableKit's on a row of its own (`host/ui/menu.cpp`, `host/profile.hpp`) | The port's update check needed its rows on the System page, and About showed only PortableKit's version, not the port's |
| 0083 | `ui::set_dialog_skin` lets a port draw the message dialog, the save-data list and the game's keyboard in its own look while PortableKit keeps what they do; `DialogSkin::sound` reports moves, choices, backing out and refused keys; skinned save lists move with up and down and stay behind their question; `SaveSlot::icon` (ICON0.PNG) and `disc_image_path()` (`host/ui/dialog_skin.hpp`, `host/ui/message_dialog.cpp`, `host/ui/save_list_dialog.cpp`, `host/ui/text_input.cpp`, `host/hle/hle_savedata.cpp`) | The port's native dialogs draw these in the game's own menu style, with its menu sounds |
| 0084 | The interface has room for 256 textures of its own instead of 8 (`host/gpu/vulkan_renderer.cpp`) | After a native dialog, every menu page but the first was blank: the dialogs' pictures used up the room and the font atlas grew without any ([native-dialogs.md](../docs/port/native-dialogs.md)) |
| 0085 | PPSSPP's CWCheat files (`PSP/Cheats/<disc id>.ini`) run as PPSSPP runs them, every code type with its meaning, every 77 ms of the game's time; writes into recompiled code are left out and marked; `GameProfile::cheats` for a port's own, saved in `cheats.builtin`; the menu's Cheats page with a page for the file (`host/cheats/`, `host/ui/menu.cpp`, `tests/cwcheat_tests.cpp`) | Cheats with PPSSPP's results, and the public list's codes that work as menu options ([cheats.md](../docs/port/cheats.md)) |
| 0086 | `GameProfile::sharp_textures`: textures named by their dump key and size are drawn with nearest filtering whatever the Texture filter setting says; `<prefix>_NO_SHARP_PIXEL_ART` (`host/gpu/vulkan_renderer.cpp`, `host/profile.hpp`) | The pixel-art chocobo on the loading screen between battles and menus blurred with Smooth filtering at a higher resolution |
| 0087 | `ui::request_quit()` quits as the menu's Quit does, from any thread (`host/ui/ui.hpp`, `host/ui/menu.cpp`) | Circle on the title screen asks "Close the game?", and Yes closes it ([title-quit.md](../docs/port/title-quit.md)) |
| 0088 | With no pack directly in `textures/<id>`, a pack is read from its only subfolder holding `textures.ini`, or from the memory stick's `PSP/TEXTURES/<id>` (or one folder deeper), where it is (`TexturePackLocation::Source::Found`); `memory_stick_path()` (`host/gpu/texture_pack_import.cpp`, `host/hle/hle_io.cpp`) | A community pack ([D012FFRT](https://github.com/Niosa/D012FFRT)) was reported not to work: its instructions put it in PPSSPP's `PSP/TEXTURES`, and a downloaded zip nests it one folder deep ([texture-packs.md](../docs/port/texture-packs.md)) |
| 0089 | `hle_extension::hold_back()` and `Kernel::hold_back_real_time()`: the game's frames come a little later from now on, at the same rate (`host/kernel/kernel.hpp`) | Online battles keep the two copies' frames in balance, which saves a frame of input delay ([communication-mode.md](../docs/port/communication-mode.md), section 17.1) |
| 0090 | `sceNetAdhocDiscover` joins the group it names and ends once another player is in it (result 0), after its time with nobody (2), or on Stop (3); the relay forwards datagrams of up to 32 KiB (`host/hle/hle_adhoc.cpp`, `host/adhoc/protocol.hpp`) | Communications' pass-by exchange of Friend Cards and quests works online: it searches with Discover and sends 27,416-byte datagrams ([communication-mode.md](../docs/port/communication-mode.md), section 17.2) |
| 0091 | Save states (`host/snapshot.hpp`): guest RAM and VRAM, the running context and the runtime's starvation phase, the kernel (`Kernel::save_state`), and participants (`snapshot::add`): the display, controller latch, GE state, audio channels and SAS, threadman's pools, open files, ATRAC contexts (the decoder's frames since a save handed out again after a restore) | Rollback netplay restores an earlier frame and runs it again; the port's rollback test matches a plain replay in all of guest RAM ([online-next.md](../docs/port/online-next.md), section 5.7) |
| 0095 | Texture packs with `hash = quick` are hashed as PPSSPP hashes them (`texture_pack_quick_hash()`), so their keys find textures directly; `<prefix>_QUICK_BY_PICTURE` matches them by picture as 0076 did. The import screen no longer says they are matched by picture | A community HD pack redraws the title screen and much of the UI, which matching by picture cannot find ([issue 15](https://github.com/VCKing101/Dissidia012-pc/issues/15); [texture-packs.md](../docs/port/texture-packs.md), section 4) |
| 0096 | The ad hoc server counts each player's relay connections and datagrams (sent, received, sent to nobody) and logs a player in a group 10 s with no relay connection; the client records why each connection failed and notices a relay that never answers while the server does (`adhoc::relay_unreachable()`), says so over the game (`<prefix>_NO_ADHOC_NOTICE`) and in Status; the host's list shows each player's room by the game's name (`GameProfile::adhoc_group_name`), who is not seen, and players alone in different rooms; Save network log starts with the build, the hosted server's players, the router and discovery, and a port's own lines (`ui::add_network_log_lines`). `<prefix>_ADHOC_LOOPBACK=1` hosts on 127.0.0.1 alone with no discovery, for copies on one PC that no firewall asks about | Two players saw each other on the Network page but never in the game's Online Lobby, and nothing said why ([play-online.md](../docs/port/play-online.md), If something goes wrong) |

Patches 0012 and 0013 were made on a separate worktree branch during the recompiled-build work and merged onto
`dissidia` on 18 October 2026; patches 0014 and 0015 followed the same day (bring-up log, section "Stencil"), and
patches 0016 to 0022 that evening (section "HP bar, orbs and saves"), patches 0023 to 0025 on 19 October for mods
([mod-folder.md](../docs/port/mod-folder.md)), and patches 0026 and 0027 the same day for the BGM playlist
([bgm-playlist-design.md](../docs/port/bgm-playlist-design.md), section 10); patch 0028 that evening, for analog
diagonals that felt dead on a real controller. Patches 0036 to 0039 came on 20 October with the graphics census
([graphics-census.md](../docs/port/graphics-census.md)), and 0040 to 0043 on 21 October from its findings (depth clipping, findings 3 and 6, stays on the
unmerged branch `depth-clip`: no visible change here); 0044 the same day for a report from the Steam Deck, and 0045 to 0048 for the frame-rate dips; 0049 to 0053 on 22 October for the present thread. Patch 0054 came on 26 October, for a player whose save would not move between the port
and PPSSPP (`portablekit_savedata_tests` and both directions checked against PPSSPP 1.20.4). Patches 0055 to 0061
came from 25 to 26 October for local split-screen versus, made on the worktree branch `local-versus` and moved onto
`dissidia` when that branch was merged. Patch 0062 came on 28 October for the split halves' field of view, on
`local-versus` again (the branch `versus-fov` here), and moved onto `dissidia` when that was merged the same day. Patches 0063 and 0064 came on 28 October for the installer build, and 0065 on 31 October for a report from the Steam Deck (made on the worktree branch `interp-pairing`). Patches 0066 and 0067 came on 1 November for issue 7 and texture packs (made in a clone of the branch in the worktree `github-issues-review-06bf08`). Patches 0068 to 0070 came on 1 November for online play, made on the branch `adhoc-matching-012` and moved onto `dissidia` plus 0066 as the branch `online-012`, and on 2 November onto 0067 too. Patches 0071 and 0072 came the same evening for online play without port forwarding, on `online-012`. Patch 0073 came the same night for the player-facing pieces (roadmap step 5), and 0074 on 2 November from the release test against the Steam Deck. Patch 0075 came on 3 November for issue 7, from the crash report of v0.5.0 (made in the worktree `github-issues-review-06bf08`), and 0076 the same day for quick-hash texture packs (made on the branch `quick-rekey`, off `dissidia` at 0075). Patch 0077 came the same day for the European disc, made on the branch `region` (in the worktree `dissidia-multi-region-ab99b4`) and moved onto 0076. Patch 0078 came on 4 November for issue 13's bloom, on the branch `bloom`, and 0079 the same day for Japanese voices (issue 8), made in the worktree `github-issues-review-06bf08` on 0077; the two touch different files. Patch 0080 came on 5 November for the BGM track list's font, on the branch `overlay-font-follows`, and 0081 on 6 November for the native Linux build, on the branch `linux` (worktree `PortableKit-linux`) on 0080. Patch 0082 came the same day for the update check, on the branch `update-check` (worktree `PortableKit-linux`) on 0081. Patch 0083 came the same day for the native dialogs, on the branch `native-dialogs` (worktree `PortableKit-linux`) on 0082. Patch 0084 came the same day for the blank menu the native dialogs left, on the branch `cheats` (worktree `PortableKit-linux`) on 0083. Patch 0085 came the same day for cheats, on the same branch. Patch 0086 came the same day for the loading screen's pixel art, on the branch `pixel-art` (worktree `PortableKit-linux`) on 0085. Patch 0087 came the same day for closing the game from the title screen, on the branch `title-quit` (worktree `PortableKit-linux`) on 0086. Patch 0088 came on 7 November for texture packs put where the port did not look, on the branch `pack-locations` (worktree `PortableKit-linux`) on 0087. Patches 0089 and 0090 came the same day for online play (the frames' balance and Communications), on the branch `comm-online` (worktree `PortableKit-versus`) on 0088. Patch 0091 came the same night for rollback's save states, on the branch `rollback` (worktree `PortableKit-rollback`) on 0090. Patch 0095 came on 8 November for issue 15, on the branch `quick-hash` (in the worktree `github-issues-review-06bf08`) on 0088.
PortableKit's 21 framework tests pass on the branch and `portablekit_host_check` builds.

## Recompiled build

The EBOOT, the three overlays and `LIBSUPPREACC.PRX` recompiled with `psp_recomp` and compiled in. It reaches the
title screen with `PSPRECOMP_NO_INTERPRETER=1` and no interpreter fallbacks. Full notes, timings and numbers:
[docs/port/recompiled-build.md](../docs/port/recompiled-build.md).

It has its own build directory and work directory, so the interpreter build above stays corpus-free (`portablekit_add_game()` re-globs `<work dir>/generated` and `<work dir>/overlays` on every build):

| Path | What it is |
| --- | --- |
| `tools/local/port/build-d012-recomp` | Build directory |
| `build/port/recomp/generated`, `build/port/recomp/overlays` | Main corpus; one corpus and `meta.txt` per library (`D012_WORK_DIR=build/port/recomp`) |
| `build/port/ovl`, `build/port/prx` | Copies of the overlay ELFs and of the disc PRX that the corpora are generated from |
| `build/port/data-recomp` | Data directory for its runs |

`setup.ps1` makes this build with clang instead, in `tools/local/port/build-d012-clang` (`tools/port/pk_cmake_mingw.cmd`
with `-DPSPRECOMP_GENERATED_OPT_LEVEL=1`, the options otherwise the same). The MSVC commands below still work, for
Visual Studio's debugger for example, but an MSVC build cannot play online against a clang build: the game's maths
are the port's own only under clang (docs/port/setup.md, "Why clang").

```powershell
.\tools\port\pk_cmake.cmd -S port -B tools/local/port/build-d012-recomp -G Ninja `
    "-DD012_PORTABLEKIT_DIR=$PWD/tools/local/port/PortableKit" `
    "-DD012_WORK_DIR=$PWD/build/port/recomp" -DPSPRECOMP_GENERATED_JOBS=4
.\tools\port\pk_cmake.cmd --build tools/local/port/build-d012-recomp --target psp_recomp -j4
tools\local\port\build-d012-recomp\portablekit\psp_recomp.exe build/port/game/EBOOT.ELF `
    --auto build/port/recomp/generated 0x08804000
New-Item -ItemType Directory -Force build/port/ovl, build/port/prx
Copy-Item build/ULUS10566/OVL_MENU_APP.ELF, build/ULUS10566/OVL_BATTLE_APP.ELF, build/ULUS10566/OVL_EXTRA_APP.ELF build/port/ovl
Copy-Item orig/ULUS10566/PSP_GAME/USRDIR/DATA/MODULE/LIBSUPPREACC.PRX build/port/prx
.\.venv\Scripts\python.exe tools/port/overlay_meta.py --recompile build/port/ovl/OVL_MENU_APP.ELF `
    build/port/ovl/OVL_BATTLE_APP.ELF build/port/ovl/OVL_EXTRA_APP.ELF build/port/prx/LIBSUPPREACC.PRX
.\tools\port\pk_cmake.cmd --build tools/local/port/build-d012-recomp -j4
.\.venv\Scripts\python.exe tools/port/run_d012.py --seconds 55 --build-dir tools/local/port/build-d012-recomp `
    --data-dir build/port/data-recomp --env PSPRECOMP_NO_INTERPRETER=1 --env D012_PERF=log `
    --env "D012_INPUT_SCRIPT=1100:quit"
```

Generating takes seconds and compiling minutes, not hours (no ccache). A good run logs
`[overlay] installed suppreacc` and `[overlay] installed menu` and never `No recompiled function registered`.
The PRX corpus only matches while the profile loads the PRX at `0x09B81000`.

## Installer build

For an installer that needs nothing else installed: the program
without the game's code, built with llvm-mingw, the compiler the installer ships. The installer compiles the game's
code on the player's PC into `game-code/` beside the program; `host/d012_game_code.cpp` loads `game-code/main.dll`
(a corpus library, PortableKit's `host/corpus_library.hpp`) and the portable layout points `D012_OVERLAY_DIR` at
`game-code/overlays`. `D012_GAME_CODE_DIR` names another folder.

In this build the portable layout always applies: `data/` beside the program holds what `--install <iso>` makes
(`EBOOT.ELF`, and `settings.ini` naming the player's disc image where it is) with the saves, settings and mods, as
PortableKit lays out a data directory; `D012_GAME_DIR` is set only when a `game/` folder is there. It is a windowed
program, so no console opens behind the game; started without one, it writes its output to
`data/logs/D012Native.log`, unbuffered and through one open file for both streams, so a crash or a kill loses no line;
a crash adds its report there and a minidump, `data/logs/D012Native-crash.dmp`.

**The Windows setup**: `tools/port/package_installer.py --version X.Y.Z` builds the installer build
(`tools/port/pk_cmake_mingw.cmd`), stages the folder (program, `BgmMaker.exe`, DLLs, `psp_recomp.exe`, `sdk/`,
llvm-mingw trimmed as PortableKit's own packaging trims it, `atracdenc.exe` with its LGPL licence and source zip,
`README.txt`, licences and `tools/port/installer/THIRD_PARTY_NOTICES.md`),
refuses it if it finds a file of the game by name or content, a program with the game's code linked in, or a path of
this machine, and compiles `tools/port/installer/dissidia012.iss` with Inno Setup 7 (`tools/local/innosetup`) into
`build/port/installer/dist/Dissidia012-PC-Setup-X.Y.Z.exe` (76 MB). The wizard asks for the folder (no
administrator rights) and the ISO, installs, runs `--install` and `--compile-game` with a progress bar from the
latter's output, and makes the shortcuts; running it again over an installation is the update, and uninstalling keeps
`data/` unless the player says otherwise. Checked on 28 October 2026 with a silent install (`/VERYSILENT /ISO=...`):
3 min 25 s from the setup to a playable game, an update over it in 18 s (no compile), an uninstall that leaves only
`data/`. The checks refuse a planted file of each kind they look for. Checked again for 0.4.0 on 30 October 2026: a
fresh install in 196 s with the optional "Battle BGM Maker" ticked: the shortcut on `BgmMaker.exe`, which found `data/mods`, the player's
ISO (from `settings.ini`) and `atracdenc.exe`, and made a PSP / PPSSPP ISO from the command line in 13 s; 0.3.0 with
its Python tool, then 0.4.0 over it in 19 s, removed `bgm-maker/` and FFmpeg's programs and kept the mods.

```powershell
cmake -S port -B tools/local/port/build-d012-installer -G Ninja -DCMAKE_BUILD_TYPE=Release `
      -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DD012_INSTALLER_BUILD=ON -DPORTABLEKIT_RELEASE=ON
```

with llvm-mingw's `bin` on `PATH` and the dependency paths `tools/port/pk_cmake.cmd` passes. It reads no
`generated/` or `overlays/`, so it builds without the game: a 7.5 MB program, against 120 MB with the corpus linked
in. Its `bin/` is laid out as an installation: the program, `psp_recomp.exe` and `sdk/` (the three headers the
game's code includes, the two entry-point templates and `dispatch-calls.txt`).

`D012Native --compile-game [--iso PATH] [--opt N] [--jobs N] [--force]` (`host/d012_compile_game.cpp`) makes the
game's code from the player's disc, after `--install <iso>` has decrypted the executable into `game/`: it reads the
three overlays and `LIBSUPPREACC.PRX` from the ISO (checked by SHA-256), names each overlay's corpus as
`tools/port/overlay_meta.py` does, runs `psp_recomp` five times with the dispatch calls, compiles every unit with
`toolchain/bin/clang++` (`D012_TOOLCHAIN_DIR` names another) and links `game-code/main.dll` and one library per
overlay, each on its own. It writes `game-code/key.txt`, a hash of everything the code is made from, and does
nothing when that matches (2 s), so an installer can run it after every update. Progress goes to stdout and to
`game-code-status.txt`; the tools' output to `game-code-build.log`.

Measured on 28 October 2026 (i7-12700K, 8 jobs, -O1): 4 minutes from the ISO, of which about 1 for the five
recompiles and 3 for the compile. The C++ it generates is byte for byte the developer pipeline's (main and all
four overlays, with the same overlay names and entry files). The game's code runs about three times faster than
the MSVC build's: 2.3 to 2.8 against 6.8 ms of game code a frame in battle, at 60 fps.

## Steam Deck

The Windows release build, packaged as one self-contained folder and run on the Deck through Proton; updates go
over the home network with one command. Step by step, for the owner: [docs/port/steam-deck.md](../docs/port/steam-deck.md).

| Path | What it is |
| --- | --- |
| `tools/local/port/build-d012-release-clang` | Release build directory (`PORTABLEKIT_RELEASE=ON`, the recompiled build's corpus in `build/port/recomp`), with clang as `setup.ps1 -Deck` makes it; `build-d012-release` is the MSVC one |
| `host/d012_portable.cpp` | The portable layout: with `game/EBOOT.ELF` next to the executable, `D012_GAME_DIR`, `D012_DATA_DIR` and `D012_MEMORY_STICK_DIR` default to `<folder>/game`, `<folder>/data` and `<data>/ms0`, and the interpreter is off (`PSPRECOMP_NO_INTERPRETER=1`; `D012_INTERPRETER=1` keeps it). A variable already set always wins. A release build turns the interpreter off wherever it runs |
| `tools/port/package_deck.py` | Assembles `build/port/deck-package/Dissidia012`: the program, its DLLs, `overlays/`, licences, `game/` (EBOOT.ELF copied, disc.iso hard-linked) and `data/settings.ini` with Deck defaults if absent; never saves or mods |
| `tools/port/deploy_deck.py` | Builds, packages and sends only what changed (SHA-256 manifest, `.deploy-manifest.json` in the destination) over Windows' OpenSSH with a key; never touches the destination's `data/`. `--local-target DIR` deploys to a local folder |

```powershell
.\.venv\Scripts\python.exe tools/port/deploy_deck.py            # build, package, send to deck@steamdeck.local
.\.venv\Scripts\python.exe tools/port/deploy_deck.py --dry-run  # what it would send
```

## Rules for this directory

- Our own MIT code only. No game bytes, no generated code, no code from PPSSPP (GPL) or from any other port's
  generated code. Short addresses and facts are fine.
- Game-specific facts stay here, never in PortableKit's `host/`, `src/` or `include/`.
- Saves: the only save in use is the one the game created itself, through New Game, in `build/port/data/ms0`
  (`ULUS10566GameData00`). No other saves are copied, read or referenced. Saves from earlier test runs of this
  session were moved to `build/port/data/set_aside/`, not deleted.

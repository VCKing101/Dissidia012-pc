# Dissidia 012 native port (PortableKit profile)

The native PC port of *Dissidia 012 Duodecim Final Fantasy*, USA release ULUS10566 v1.00, on
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
| `host/d012_loading_layout.cpp`, `.hpp` | The loading screen's right-half pages moved down and made shorter for the track wheel, by checked edits of the screen's layout data in memory (section 11) |
| `host/d012_test_switches.cpp`, `.hpp` | Test switches, off by default: `D012_TEST_UNLOCK_FIGHTERS`, `D012_TEST_EX_FULL` ([docs/port/graphics-triage.md](../docs/port/graphics-triage.md)) |
| `host/d012_crash_report.cpp` | `D012_CRASH_DUMP=<dir>`: a minidump and a symbolised stack on an unhandled exception, for diagnosis ([docs/port/quit-crash.md](../docs/port/quit-crash.md)) |
| `host/d012_decomp_harness.cpp`, `.hpp`, `host/d012_decomp_bgm.cpp` | Decompiled functions as native overrides, and the differential test against the recompiled originals |
| `decomp/` | The decompiled BGM functions (C) |
| `examples/mods/example-tones/` | An example mod: a manifest and a script that writes its generated test tones (no audio files are kept) |
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
| `D012_CRASH_DUMP=<dir>` | On an unhandled exception, write `<dir>/crash_<pid>.dmp` and log the faulting stack (names from `D012Native.pdb` when the build links with `/DEBUG`) |

And the framework changes' off switches, each restoring the old behaviour: `D012_NO_COLOR_DOUBLING=1` (colour
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

Patches 0012 and 0013 were made on a separate worktree branch during the recompiled-build work and merged onto
`dissidia` on 18 October 2026; patches 0014 and 0015 followed the same day (bring-up log, section "Stencil"), and
patches 0016 to 0022 that evening (section "HP bar, orbs and saves"), patches 0023 to 0025 on 19 October for mods
([mod-folder.md](../docs/port/mod-folder.md)), and patches 0026 and 0027 the same day for the BGM playlist
([bgm-playlist-design.md](../docs/port/bgm-playlist-design.md), section 10); patch 0028 that evening, for analog
diagonals that felt dead on a real controller. Patches 0036 to 0039 came on 20 October with the graphics census
([graphics-census.md](../docs/port/graphics-census.md)), and 0040 to 0043 on 21 October from its findings (depth clipping, findings 3 and 6, stays on the
unmerged branch `depth-clip`: no visible change here); 0044 the same day for a report from the Steam Deck, and 0045 to 0048 for the frame-rate dips; 0049 to 0053 on 22 October for the present thread.
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

## Steam Deck

The Windows release build, packaged as one self-contained folder and run on the Deck through Proton; updates go
over the home network with one command. Step by step, for the owner: [docs/port/steam-deck.md](../docs/port/steam-deck.md).

| Path | What it is |
| --- | --- |
| `tools/local/port/build-d012-release` | Release build directory (`PORTABLEKIT_RELEASE=ON`, the recompiled build's corpus in `build/port/recomp`) |
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

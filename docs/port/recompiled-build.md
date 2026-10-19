# Phase 2: the recompiled build

> Log of 18 October 2026. Phase 2 of the [plan](README.md): the EBOOT, the three overlays and the disc PRX turned
> into C++ with PortableKit's `psp_recomp`, compiled, and run to the title screen. PortableKit `33800c4` plus the
> three phase 1 commits, plus two commits on the clone's local branch `dissidia-recomp` (worktree
> `tools/local/port/PortableKit-recomp`). Windows 11, MSVC 19.44, i7-12700K (20 threads), 32 GB, Radeon RX 9070 XT.
>
> Later the same day both commits were merged onto `dissidia` (now patches 0012 and 0013) and the worktree was
> removed; the recompiled build now uses the main clone. Paths below that name the worktree are historical.

Conventions as in the other port notes: "V:" verified (by a run, a trace or a capture), "I:" inference.

---

## 0. Summary

- **The recompiled build reaches the title screen with no interpreter.** V: with `PSPRECOMP_NO_INTERPRETER=1`
  the game boots, shows the logo and credits and stops on NEW GAME / LOAD GAME / EXTRAS. The captures at frames
  300 and 900 are identical to those of the same build with the interpreter enabled. 460,475 functions are
  registered from the main corpus, and the menu overlay's corpus and the disc PRX's corpus install on the first
  call into them.
- **Zero interpreter fallbacks on the boot path.** The only fallbacks were the six entry points of
  `LIBSUPPREACC.PRX` (3,722 instructions). That PRX is now recompiled too, which needed two small framework
  changes (section 7).
- **Compiling took minutes, not hours.** `psp_recomp` takes 8 s for main and 14 s for all three overlays. MSVC
  compiles the 113 main units in 80 s and the 195 overlay units in about 2 minutes, at 4 jobs. A clean rebuild of
  everything (host, corpus, four libraries) took 272 s (section 3).
- **Speed.** On the title screen, guest time per frame drops from 2.5 ms (interpreter) to 0.9 ms. With no frame cap
  (headless), the game runs at about 4,250% of PSP speed instead of 1,330%. Both builds hold 30 fps on screen.
- **Overlay hashes.** The default hash window (4,096 bytes) is inside the part that never changes for menu
  (first change `+0x3500`) and battle (`+0x455F0`). Extra was not seen loaded; its first 4,096 bytes are its
  descriptor and code. The three hashes are all different. V: battle's corpus installed and replaced menu's when the
  game loaded battle by itself (the title screen's attract demo, in a headless run).

---

## 1. Layout

Nothing here is tracked. Everything derived from the game is under the ignored `build/port/`:

| Path | What it is |
| --- | --- |
| `tools/local/port/PortableKit-recomp` | Worktree of the clone on branch `dissidia-recomp` (from `dissidia` after patch 0003) |
| `tools/local/port/build-d012-recomp` | The recompiled build's build directory |
| `tools/local/port/build-d012-recomp-interp` | The same sources with no corpus, built only to compare speed |
| `tools/local/port/build-framework-recomp` | PortableKit's own build of the worktree, for its tests |
| `build/port/recomp` | The recompiled build's `D012_WORK_DIR` (the profile's `PROFILE_DIR`) |
| `build/port/recomp/generated` | Main corpus: 113 units, registry, report |
| `build/port/recomp/overlays/<prefix>/` | One corpus and `meta.txt` per overlay library (menu, battle, extra, suppreacc) |
| `build/port/ovl`, `build/port/prx` | Copies of the three `OVL_*.ELF` from `build/ULUS10566/` and of `LIBSUPPREACC.PRX` from `orig/` |
| `build/port/data-recomp` | The recompiled build's data directory |
| `build/port/recomp/logs`, `shots*`, `dumps` | Build and run logs, captures, overlay window dumps; `title_screen_recomp.png` is frame 900 of the last run with no interpreter |

**Why `build/port/recomp` and not `build/port/generated`.** `portablekit_add_game()` globs
`<PROFILE_DIR>/generated/*.cpp` and `<PROFILE_DIR>/overlays/*/meta.txt` with `CONFIGURE_DEPENDS`, so the glob is
checked again on every `cmake --build`. A corpus in `build/port/generated` would be picked up by the interpreter
build (`build-d012`, `D012_WORK_DIR=build/port`) the next time it is built. The recompiled build therefore has a
work directory of its own. `port/CMakeLists.txt` needed no change for this: `D012_WORK_DIR` was already a cache
variable.

---

## 2. Steps

From the repository root, in PowerShell. `pk_cmake.cmd` is the Visual Studio wrapper from phase 1.

```powershell
# 0. Framework worktree (once)
git -C tools/local/port/PortableKit worktree add ../PortableKit-recomp -b dissidia-recomp dissidia

# 1. Configure, then build psp_recomp
.\tools\port\pk_cmake.cmd -S port -B tools/local/port/build-d012-recomp -G Ninja `
    "-DD012_PORTABLEKIT_DIR=$PWD/tools/local/port/PortableKit-recomp" `
    "-DD012_WORK_DIR=$PWD/build/port/recomp" -DPSPRECOMP_GENERATED_JOBS=4
.\tools\port\pk_cmake.cmd --build tools/local/port/build-d012-recomp --target psp_recomp -j4

# 2. Main corpus (EBOOT.ELF from prepare_game_dir.py), with --dispatch-call for the main rows of
#    port/config/dispatch-calls.txt (overlay_meta.py passes the overlay rows itself)
tools\local\port\build-d012-recomp\portablekit\psp_recomp.exe build/port/game/EBOOT.ELF `
    --auto build/port/recomp/generated 0x08804000 --dispatch-call 0x08872138

# 3. Overlays and the disc PRX: meta.txt plus corpus, into build/port/recomp/overlays
New-Item -ItemType Directory -Force build/port/ovl, build/port/prx
Copy-Item build/ULUS10566/OVL_MENU_APP.ELF, build/ULUS10566/OVL_BATTLE_APP.ELF, build/ULUS10566/OVL_EXTRA_APP.ELF build/port/ovl
Copy-Item orig/ULUS10566/PSP_GAME/USRDIR/DATA/MODULE/LIBSUPPREACC.PRX build/port/prx
.\.venv\Scripts\python.exe tools/port/overlay_meta.py --recompile build/port/ovl/OVL_MENU_APP.ELF `
    build/port/ovl/OVL_BATTLE_APP.ELF build/port/ovl/OVL_EXTRA_APP.ELF build/port/prx/LIBSUPPREACC.PRX

# 4. Compile everything (the glob picks up the corpus and the four meta.txt files)
.\tools\port\pk_cmake.cmd --build tools/local/port/build-d012-recomp -j4

# 5. Run to the title screen, interpreter off, clean quit at frame 1100
.\.venv\Scripts\python.exe tools/port/run_d012.py --seconds 55 --build-dir tools/local/port/build-d012-recomp `
    --data-dir build/port/data-recomp --log build/port/recomp/logs/run.log --env PSPRECOMP_NO_INTERPRETER=1 `
    --env D012_PERF=log --env D012_SCREENSHOT_DIR=build/port/recomp/shots --env D012_SCREENSHOT_EVERY=300 `
    --env "D012_INPUT_SCRIPT=1100:quit"
```

The run must log `[overlay] installed suppreacc` and `[overlay] installed menu`, and must not stop with
`No recompiled function registered at ...`. `PSPRECOMP_INTERPRETER_VERBOSE=1` (with the interpreter on) names
every fallback address instead of only the first.

`run_d012.py` gained `--build-dir` and `--data-dir`; their defaults are the interpreter build's, as before.
`overlay_meta.py` now writes to `build/port/recomp/overlays` by default, uses the recompiled build's `psp_recomp`
when it exists (`--recompiler` names another), and reads a PRX (section 5).

---

## 3. Corpus sizes and compile time

Generation (`psp_recomp --auto`, default 16 KiB units):

| Corpus | Units | Registered entries | Emitted instructions | Function seeds | Import wrappers | Source | `psp_recomp` |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| main (`EBOOT.ELF`) | 113 | 460,471 | 340,605 | 8,777 | 296 | 117.7 MB | 8 s |
| menu | 95 | 388,966 | 255,079 | 6,830 | 0 | 117.4 MB | |
| battle | 98 | 399,020 | 221,421 | 6,001 | 0 | 122.9 MB | 14 s for the three |
| extra | 2 | 6,078 | 2,724 | 86 | 0 | 1.8 MB | |
| suppreacc (PRX) | 1 | 3,539 | 2,827 | 70 | 26 | 1.1 MB | under 1 s |

Every word of each executable section is an entry, as the gap analysis said (1.1). Main generated no
`unsupported` sites except 113 "invalid internal function entry" guards and 100 `break` traps. The overlays carry
about 16,000 `rt.unsupported` sites because code and data share their one section; checked against the
inventories' instruction ranges, the only four inside code are data words the inventory counts as code (a float
table at menu `0x08B4BFEC`) and the slots of `j` words in battle's jump tables (`0x08A20678`, `0x08A83C40`,
`0x08ADDCF0`), where the trap sits on the label of the preceding data word, never on the function's own entry.

Compile (MSVC `/O2 /Ob0 /bigobj`, `PSPRECOMP_GENERATED_JOBS=4`, no ccache), from the Ninja log:

| Part | Units | Wall time | Compiler time summed | Longest unit |
| --- | ---: | ---: | ---: | ---: |
| main corpus, first build | 113 | 80 s | 294 s | 9.7 s |
| three overlays (menu, battle, extra), first build, with linking | 195 | 122 s | | |
| clean rebuild of everything (`--target clean`, then a build): framework tools, host, main corpus, four libraries | 439 steps | **272 s** | 1,072 s | 13.4 s |
| of which main corpus | 113 | 93 s | 335 s | 13.4 s |
| of which menu / battle | 95 / 98 | 65 s / 68 s | 244 s / 250 s | 3.0 s / 7.7 s |

I: the gap analysis expected one to three hours. Each unit is one function of about 4,096 labels with
inlining off (`/Ob0`), which MSVC compiles in seconds. During the clean build another agent was compiling and
running the interpreter build on the same machine, so the wall times are upper bounds.

Outputs: `D012Native.exe` 82 MB (the main corpus is linked in), overlay libraries in
`build-d012-recomp/bin/overlays/` (menu 54 MB, battle 56 MB, extra 0.7 MB, suppreacc 0.5 MB).

---

## 4. Overlay dispatch and slot hashes

**Main's calls into the overlay window.** V: in the main corpus, 852 fixed `jal`/`j` sites with 542 targets in
the window (`0x089DD4C0..0x08B62F70`) are emitted as `ctx.pc = target; rt.invoke_chained_call(...)`; the counts
are the survey's (3.4). The window has no unit in main, so the first such call misses, returns to the outer
dispatcher, and the missing-function hook (`host/overlays.cpp`) hashes the window, picks the library whose hash
matches and registers it. Later calls find the overlay's functions directly. Main's 72 code pointers into the
window (vtables) take the same path through `jalr`. The overlays call main's import stubs and functions through
the same lookup. Nothing per site was needed.

**Swapping.** V: in a headless run the game itself replaced menu with battle (the attract demo):
`[overlay] menu was replaced in 0x089DD4C0`, then `[overlay] installed battle`. The first 64 bytes of the three
images differ (descriptor tags 2, 1, 3), which is what `revalidate_overlays` checks every frame before rehashing.

**Hash window.** `meta.txt` hashes the first `64 + code_size` bytes; the game writes into a loaded overlay, so the
window has to end before the first byte it changes.

| Overlay | Size | First byte the game changes | Measured | `code_size` | Hash (FNV-1a 64) |
| --- | ---: | --- | --- | ---: | --- |
| menu | 1,555,864 | `+0x3500` (167 bytes in 40 places) | V: dump on the title screen (phase 1) | 4,032 | `0x29F9A274007A5CA2` |
| battle | 1,596,080 | `+0x455F0` (311 bytes in 92 places) | V: dump at vblank wait 20,000 of a headless run, in the attract demo | 4,032 | `0xA3D8F60BD768B4A8` |
| extra | 24,312 | not seen loaded | I: the first 4,096 bytes are the 16-byte descriptor (tag 3, no initialisers) and function bodies `0x089DD4D0..0x089DE744` (inventory) | 4,032 | `0xBDF6E45D953CC467` |
| suppreacc | 18,032 | n/a: the first 4,096 bytes are `.text` | V: installs on the first call | 4,032 | `0x91D1F6D6529636EA` |

The default window distinguishes all three overlays and is inside the unchanged prefix of the two that were
measured. Library prefixes are `ovl089DD4C0_menu_29F9A274007A5CA2` and so on.

Battle dump, reproducible:

```powershell
.\.venv\Scripts\python.exe tools/port/run_d012.py --headless --seconds 40 --build-dir tools/local/port/build-d012-recomp `
    --data-dir build/port/data-recomp --env D012_DUMP_SLOT=20000 --env D012_DUMP_SLOT_DIR=build/port/recomp/dumps
.\.venv\Scripts\python.exe tools/port/overlay_meta.py --out build/scratch/port/overlay_meta_check_battle `
    --check build/port/recomp/dumps/overlay_window_20000.bin build/port/ovl/OVL_BATTLE_APP.ELF
```

Headless runs go past the title screen on their own: with no frame pacing the game reaches the title in under a
second and starts its attract demo, which loads battle. No input is sent and nothing is saved.

---

## 5. Fallbacks found and fixed

Run with the interpreter on and `PSPRECOMP_INTERPRETER_VERBOSE=1`, before the PRX had a corpus
(`recomp-title-1.log`): six entry points, all in `LIBSUPPREACC.PRX`, 3,722 instructions over the whole run.

| Address | Instructions | What |
| --- | ---: | --- |
| `0x09B81160` | 540 | first `scesupPreAcc` export called by main |
| `0x09B81804` | 1,024 | PRX internal |
| `0x09B8124C` | 1,024 | PRX internal |
| `0x09B8414C`, `0x09B84148` | 555, 572 | PRX internal |
| `0x09B81298` | 7 | PRX internal |

With `PSPRECOMP_NO_INTERPRETER=1` the run stopped at once: `No recompiled function registered at 0x09B81160`.

Fix: the PRX is recompiled as an overlay library at the address the profile loads it to (`0x09B81000`, the first
block of the user partition, the same on every run). That took:

- `overlay_meta.py` reads a PRX (`e_type 0xFFA0`), relocates it to `--prx-base` the way PortableKit's
  `Elf32Image::apply_relocations` does, and hashes the relocated image; `psp_recomp --auto` relocates to the same
  base. V: the hash matched at run time (`[overlay] installed suppreacc (17 KiB) at 0x09B81000`).
- Framework commit 2 (section 7): a library whose base is in no overlay slot installs on a miss inside its own
  range. The profile's slot list did not need to change.
- Framework commit 1: the PRX's 26 import wrappers would otherwise have called main's imports 0 to 25 through the
  shared binding cache.

After the fix, with `PSPRECOMP_NO_INTERPRETER=1` (`recomp-nointerp-2.log`, `-3.log`): no interpreter entries, the
title screen at frame 900, and captures at frames 300 and 900 identical to the run with the PRX interpreted.
The PRX's `sceIoGetstat("ms0:/PSP/SAVEDATA/ULUS10566Data")` answers `0x80010002` as before, and the save-data
list and autosave notice that follow are the same.

---

## 6. Performance against the interpreter

Same sources, same frame script (clean quit at frame 1100), windowed at 960x544 with FIFO present; the interpreter
build is `build-d012-recomp-interp` (no corpus). Averages of the `D012_PERF=log` lines.

| | Interpreter | Recompiled (no interpreter) |
| --- | ---: | ---: |
| Functions registered | 300 | 460,475 + 4 libraries |
| Interpreted instructions in the run | 147.3 million | 0 |
| First second (load) | guest 20.4 ms, frame max 382 ms | guest 7.5 ms, frame max 261 ms |
| Boot, seconds 2-14 (logo, credits) | 30.0 fps, guest 1.7 ms (max 3.3) | 30.0 fps, guest 0.8 ms (max 2.0) |
| Title screen, second 17 on | 30.0 fps, guest 2.5 ms (max 2.8) | 30.0 fps, guest 0.9 ms (max 0.9) |
| Headless, 12 s, no frame cap | 1,330% speed, about 400 fps | 4,250% speed, about 1,280 fps |

Guest time includes the HLE work done on the guest thread (audio mixing, sceSas, display lists), not only the
CPU, which is why the windowed gain is 2.7x rather than the "twenty times" PortableKit quotes for game code. The
headless figure (3.2x) also covers the attract demo for most of the 12 s. Both builds are paced to 30 fps on
screen; the title screen leaves about 32 ms of each 33 ms frame waiting in either.

One windowed run (`recomp-nointerp-2.log`) had three seconds of very slow frames on the title screen (guest up to
1.2 s). It did not happen again in the next run, and another game process was running on the machine at the
time. I: contention, not the build.

---

## 7. Framework changes

On the clone's local branch `dissidia-recomp`, never pushed. Patches in `port/portablekit-patches/recomp/`
(`git format-patch dissidia..dissidia-recomp`). None names the game.

| Patch | Change | Why |
| --- | --- | --- |
| `1e2055d` | Import wrappers of a prefixed corpus pass slot `0xFFFFFFFF`, which `Runtime::invoke_import_cached` looks up by name on every call (`tools/codegen_main.cpp`, `src/runtime.cpp`) | Slots index one cache filled by the executable's corpus; a run-time library with imports of its own would have called the executable's imports of the same index |
| `cc72924` | `install_overlay_for` and the unsupported-instruction hook also match a library whose base lies in no overlay slot and whose range holds the missed address (`host/overlays.cpp`) | Lets a module loaded at a fixed address outside the window (here the disc PRX) be a recompiled library |

No header changed, so neither change rebuilds generated code. The main corpus is generated as before (its import
slots are unchanged). PortableKit's 21 framework tests pass on the branch (`tools/local/port/build-framework-recomp`), and the
new `psp_recomp` regenerates the main corpus byte for byte.

---

## 8. What is left

1. **Menus and battle under the recompiled build.** Done later the same day: menus and Quick Battles with
   `PSPRECOMP_NO_INTERPRETER=1` and no fallbacks (bring-up log, section 27). Originally: only the boot path was run. Battle's corpus installed in the
   attract demo, but no menu screen past the title and no battle was played, by rule. Run them with
   `PSPRECOMP_NO_INTERPRETER=1` and record fallbacks the same way.
2. **Extra's hash window** is inferred, not measured: load the extra overlay (it holds the data-install code,
   I: reached from the menus) with `D012_DUMP_SLOT` and `overlay_meta.py --check`.
3. **The PRX corpus depends on the load address.** It matches only if the profile's allocation puts the PRX at
   `0x09B81000`. If the profile ever allocates something before it, the hash fails and the PRX runs interpreted
   again (with the interpreter on) or stops the run (with it off). `overlay_meta.py --prx-base` regenerates it
   for another address.
4. **Merging.** `dissidia-recomp` branches after patch 0003; `dissidia` has moved on (font and message-dialog
   commits). The two framework commits touch files those do not.
5. **Interpreter build and corpus share a profile.** The recompiled build uses `D012_WORK_DIR=build/port/recomp`
   so that the interpreter build stays corpus-free. When only one build is wanted, the corpus can move to
   `build/port/generated` and `build/port/overlays`.
6. **Optimisation.** `/Ob0` and "every word is an entry" leave speed on the table (no register caching across
   instructions). Not needed for the title screen; worth measuring in battle.
7. **ccache** is not installed. A clean rebuild costs the times in section 3; with ccache it would be seconds.

## Reproduce

Section 2, then for the comparison:

```powershell
.\tools\port\pk_cmake.cmd -S port -B tools/local/port/build-d012-recomp-interp -G Ninja `
    "-DD012_PORTABLEKIT_DIR=$PWD/tools/local/port/PortableKit-recomp" "-DD012_WORK_DIR=$PWD/build/port/recomp-interp"
.\tools\port\pk_cmake.cmd --build tools/local/port/build-d012-recomp-interp --target D012Native -j4
.\.venv\Scripts\python.exe tools/port/run_d012.py --seconds 55 --build-dir tools/local/port/build-d012-recomp-interp `
    --data-dir build/port/data-recomp --env D012_PERF=log --env "D012_INPUT_SCRIPT=1100:quit"
```

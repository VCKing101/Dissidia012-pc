# Native PC port: research

Direction set by the project owner on 18 October 2026: research turning Dissidia 012 (USA ULUS10566) into a native
PC port for Windows and Linux/Steam Deck. An earlier PPSSPP-plugin mod is shelved and kept out of the public
repository. The byte-identical builds stay as the reference for what the original code does.

- [Binary survey](binary-survey.md): what the game's code needs from a port, measured from the four ELFs and the
  disc. Scripts in `tools/port/` regenerate every number into `build/scratch/port/`.
- [Approaches](approaches.md): prior art, the options compared, and a phased plan. Web research, linked throughout.

## Summary (18 October 2026)

**The route.** Static recompilation of the whole game to C++, running on a native PSP runtime, with decompiled C
swapped in function by function where enhancements need it. At 0.44% matching C (278 of about 21,000 functions),
waiting for a decompilation is not an option; the N64 and Xbox 360 recompiled ports also started from machine code.

**PSP recompilation exists now.** It appeared between July and October 2026:
- [PortableKit](https://github.com/TeamGDB/PortableKit) (MIT) is a recompiler, an interpreter fallback, a
  re-implemented PSP kernel and system modules, a Vulkan renderer of GE command lists, and ATRAC3 and movie audio.
  Its Monster Hunter Portable 3rd port, [Yakumo](https://github.com/TeamGDB/Yakumo), is playable on Windows, Linux,
  Steam Deck and macOS, with 355 overlays, adjustable resolution, a right-stick camera and display interpolation over
  30 fps logic.
- Another, independent Dissidia 012 port effort (EU ULES01505) exists on a different framework, PSPRecomp; see
  [approaches](approaches.md).

**What the survey says about difficulty.**
- The OS surface is small and well known: 297 imported functions, all but the disc PRX's 8 named in PPSSPP's tables.
  Overlays import nothing. The 48 network functions are only needed for ad hoc play.
- The code is regular for a recompiler: one SNC switch-table idiom (207 tables), 90% of indirect calls are C++
  virtual calls, no self-modifying code.
- The hard parts, in order:
  1. Graphics. libGu is statically linked, and the game renders to off-screen targets, copies the screen with block
     transfers, and runs display lists stored in model data. Translating it needs a PPSSPP-grade GE renderer.
  2. Finding all the code. About 1,229 functions are missing from the inventory. Main also calls into the overlay
     window 852 times, so recompiled main must know which overlay is loaded.
  3. Kernel semantics: callbacks, semaphores, interrupt-disable locks, VFPU state per thread.
  4. Codecs: ATRAC3, ATRAC3plus, H.264 movies through low-level sceMpeg, sceSas ADPCM.
  5. VFPU precision: 113 distinct VFPU opcodes; gameplay square roots and reciprocals run on it.
- Battles keep 30 Hz logic paced by a vblank counter (`main:0x08883F9C`); high frame rate means display
  interpolation, as Yakumo does, not faster logic.

**REA does not help.** 6.5.0 and 6.6.0 still accept PSP files only on Linux, only `ET_EXEC` with a GNU
`.MIPS.abiflags` record (these SNC-built ELFs have none), and not overlays. The project's Ghidra scripts,
disassembler and the new `tools/port/` scripts cover the analysis.

## Studies (18 October 2026)

- [PortableKit gap analysis](portablekit-gap-analysis.md) (`tools/port/portablekit_coverage.py`):
  - **System functions:** of the game's 297, PortableKit implements 226, returns a fixed value for 30, marks 8 as
    unverified and lacks 33. Of the 33, 8 are the disc PRX (`LIBSUPPREACC.PRX`, never loaded) and 12 are ad hoc
    networking.
  - **CPU:** all 113 VFPU opcodes run in both the recompiler and the interpreter.
  - **Function discovery:** not a blocker. Its `--auto` mode treats every executable word as a possible entry.
  - **Boot:** first blocker is `EBOOT.ELF`'s `e_entry` of 0; the real entry is `module_start` `0x08804040`.
  - **Overlays:** need a small tool of our own to describe each overlay.
  - **Graphics gaps for battles:** depth sharing an address with a colour buffer, morph weights, mipmaps, and the
    screen copy read back from the GPU.
  - **Missing on this PC:** SDL3, the Vulkan SDK and ccache.
- **EU and USA code are identical.** The EU image's decrypted `EBOOT.OLD` and its overlays match the USA ones byte
  for byte, so EU and USA addresses are the same.
- **The menu overlay's runtime object** at `0x09AE7070` is built by `menu:0x089DD4D0`. That constructor is matched C
  here (`src/menu/runtime_lifecycle.c`). It runs from main's menu start-up callback `0x0887801C` after the game's own
  overlay loader loads `ovl_menu_app.elf`. Main also has a 47-entry static-initialiser table at `0x089C60B0`.

## Phase 1 bring-up (18 October 2026)

- [Bring-up log](bring-up-log.md): the port's own tree is `port/` (profile, build file, [README](../../port/README.md)).
  Under PortableKit's interpreter, with no recompiled code, the game reaches the **title screen** at 30 fps:
  Square Enix logo, credits, logo animation, then NEW GAME / LOAD GAME / EXTRAS. It took three framework fixes on
  the clone's local `dissidia` branch (`module_start` entry when `e_entry` is 0, a Windows quoting bug in the NID
  build step, GE colour doubling), `LIBSUPPREACC.PRX` loaded for real by the profile, and four small HLE answers.
  No stubbed call blocked it. Next: the corpus, drawn message dialogs before any save work, stencil.
- [Step 2](bring-up-log.md#phase-1-step-2-past-the-title-screen), still interpreted: **New Game to the tutorial
  battle.** The message and save-data dialogs are now drawn by the port and wait for the player. The game created
  its own save in the port's fresh `build/port/data/ms0`, played its opening movie, loaded the save, ran the
  Moogle tutorial and its first battle at full speed. Eight more framework commits (dialogs, glyph buffers,
  callbacks in the vblank wait, `sceKernelCancelSema`, the GE pixel mask). Next: stencil (battle shadows and
  blur), then the hub and Quick Battle.

## Phase 2 recompiled build (18 October 2026)

- [Recompiled build](recompiled-build.md): main, the three overlays and `LIBSUPPREACC.PRX` recompiled with
  `psp_recomp` and compiled in minutes, not hours. The game reaches the title screen with
  `PSPRECOMP_NO_INTERPRETER=1` and zero interpreter fallbacks; guest time per frame on the title screen goes from
  2.5 ms to 0.9 ms. Two more framework commits, since merged onto `dissidia`. Menus and battle are not run yet.

## Mods (19 October 2026)

- [Mod folder](mod-folder.md): mods that add content and never replace any, in `<data dir>/mods/<mod>/mod.json`.
  The first kind is battle music: series and tracks in MP3, Ogg Vorbis, Opus, FLAC or WAV, with loop points,
  played through a stand-in ATRAC3plus file that PortableKit's sceAtrac fills from the host file (three framework
  commits). A test switch played a generated mod track in place of a Quick Battle's music, looping where its
  manifest says, in the recompiled build with no interpreter. The game-side 2D selector is separate work.

## Next steps

1. ~~Read PortableKit's sources.~~ Done; see above.
2. ~~Install the missing build dependencies, write a constants-only PortableKit profile for ULUS10566 and bring
   the game up under its interpreter to the title screen.~~ Done; see the [bring-up log](bring-up-log.md). Menus
   and a battle under the interpreter remain.
3. Recompile the EBOOT and three overlays; complete function discovery first.
4. Native overrides for the camera and the extra buttons; then interpolation and wide screens.

## Decisions

- **Licence: MIT** (owner, 18 October 2026), matching PortableKit. No PPSSPP source may be copied into the port;
  PPSSPP stays a behaviour reference only, as the survey already uses it.
- PortableKit (`33800c4`, 7 October 2026) is cloned into ignored `tools/local/port/`, with this port's framework
  changes on a local branch `dissidia` (kept as patches in [`port/portablekit-patches/`](../../port/portablekit-patches/)).

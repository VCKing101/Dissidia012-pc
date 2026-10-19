# Native PC port: approaches and prior art

Research date: 2026-10-18. Sources are web pages only. No repositories were cloned and no tools were run. Every factual claim links to the page it came from. Where a page did not say something, this document says so. Effort figures in the recommendation are estimates, not sourced facts, and are labelled that way.

Target: Dissidia 012 Duodecim Final Fantasy, USA `ULUS10566`. One main EBOOT and three overlays with overlapping load addresses. SNC 1.2.7503.0 build. 278 functions (0.44% of about 4.7 MB of code) matched in C so far.

Caution on sources: most of the PSP projects below are between one and four months old, and several say they were built with heavy AI help. Their status comes from their own READMEs. None of it was checked by building or running them.

---

## 1. Static recompilation prior art

### N64Recomp, N64ModernRuntime, RT64, Zelda64Recomp

- **What N64Recomp does.** It splits an N64 binary "into functions that are each individually recompiled into a C function". A TOML config gives input and output paths and can stub or skip functions and patch single instructions. MIT licence. ([N64Recomp](https://github.com/N64Recomp/N64Recomp))
- **What it needs from the binary.** At present the only way to give it metadata is an ELF with symbols. A custom metadata format is planned. ([README](https://raw.githubusercontent.com/N64Recomp/N64Recomp/main/README.md))
  - Function boundaries come from those symbols.
  - Jump tables: it tries to turn a `jr` into a `switch` when it can identify the table.
  - Delay slots: handled by duplicating instructions.
  - Compilers: tested mainly with gcc 2.7.2, IDO and clang. "Modern mips gcc may trip up the recompiler". ([N64Recomp](https://github.com/N64Recomp/N64Recomp))
- **Overlays.** "Statically linked and relocatable overlays can both be handled by this tool." ([README](https://raw.githubusercontent.com/N64Recomp/N64Recomp/main/README.md))
  - Indirect calls become lookups, for example `jalr $25` becomes `LOOKUP_FUNC(ctx->r25)(rdram, ctx);`. The runtime keeps a list of which sections are loaded and where, and resolves the call from that list.
  - For relocatable overlays, instructions that carry relocation data (`lui`, `addiu`, loads and stores) are emitted with `RELOC_HI16`/`RELOC_LO16` macros. These take a section index, and the runtime fills in the immediate from that section's load address.
  - The README does not say in so many words how two overlays that share an address are told apart. In practice it is the loaded-section table plus the section index in each relocation.
- **Patches and mods.** "Single file output mode" emits every function into one file, so patched functions placed earlier on the linker line override the originals. Recompiling to Lua for runtime mods and a hook system are listed as planned. ([README](https://raw.githubusercontent.com/N64Recomp/N64Recomp/main/README.md))
- **OS layer.** N64ModernRuntime has two parts. `ultramodern` reimplements libultra: threads, message queues, timers, VI timing, audio and controllers. `librecomp` connects the generated code to it and adds overlay handling, PI DMA and saves. GPL-3.0. ([N64ModernRuntime](https://github.com/N64Recomp/N64ModernRuntime))
- **Graphics.** RT64 is an N64 renderer for D3D12, Vulkan and Metal, under MIT. ([RT64](https://github.com/rt64/rt64))
  - It matches draw calls across frames and generates in-between frames. Where it can, it splits each transform into components and interpolates those separately.
  - For ports it has an extended command set. The project says using it "is vital to providing accurate frame interpolation and widescreen enhancements for 2D elements".
  - The extended commands include matrix groups (`gEXMatrixGroupDecomposed`, `gEXMatrixGroupNoInterpolate`) and edge alignment for viewports, rectangles and scissors (`gEXSetViewportAlign`, `gEXSetRectAlign`, `gEXSetScissorAlign`). ([rt64_extended_gbi.h](https://raw.githubusercontent.com/rt64/rt64/main/include/rt64_extended_gbi.h))
- **Zelda64Recomp (Majora's Mask).** GPL-3.0. ([Zelda64Recomp](https://github.com/Zelda64Recomp/Zelda64Recomp))
  - Runs at the monitor's refresh rate, and "Changing framerate has no effect on gameplay".
  - Any aspect ratio, "with most effects modded to work correctly in widescreen".
  - Right-stick camera, gyro aiming, and mods that can change any part of the game.
  - It uses headers from the Majora's Mask decompilation.
  - The patch set holds dozens of files named `*_transform_tagging.c` (actor, camera, effect, particle, sky, UI and others) plus `ui_patches.c` and `input_latency.c`. ([patches/](https://github.com/Zelda64Recomp/Zelda64Recomp/tree/dev/patches))
  - Coverage dates the public release to May 2024. ([DSOGaming, 2024-05-11](https://www.dsogaming.com/news/the-legend-of-zelda-majoras-mask-just-got-an-unofficial-pc-port-with-support-for-high-framerates-widescreen-ultrawide-gyro-aim-autosaves-and-low-input-lag/))

### XenonRecomp, XenosRecomp, Unleashed Recompiled (hedge-dev)

- **XenonRecomp** turns Xbox 360 XEX files into C++ and is "heavily inspired" by N64Recomp. MIT licence; it borrows from Xenia. ([XenonRecomp](https://github.com/hedge-dev/XenonRecomp))
  - Function boundaries come from `.pdata` where present, plus estimates and manual entries.
  - XenonAnalyse detects jump tables and writes them out as TOML. The detection was written for Sonic Unleashed and may need changes for other games.
  - You must list the `setjmp`/`longjmp` addresses and the register save/restore helpers.
  - Two patching mechanisms: function overrides (`PPC_FUNC_IMPL`, weak linking, so a replacement can still call the original) and mid-asm hooks at any instruction address.
  - It "does not provide a runtime implementation". Exceptions and MMIO are unsupported.
- **XenosRecomp** converts Xenos shader binaries to HLSL for DXIL or SPIR-V. "Do not expect the recompiler to work out of the box". Several parts are hard-coded for Unleashed. MIT. ([XenosRecomp](https://github.com/hedge-dev/XenosRecomp))
- **Unleashed Recompiled.** GPL-3.0. ([UnleashedRecomp](https://github.com/hedge-dev/UnleashedRecomp))
  - The renderer was written from scratch for D3D12 or Vulkan, and has its own Xbox 360 kernel translation layer.
  - Ultrawide works out of the box. UI can align to the screen edges or to a 16:9 safe area, and cutscenes keep their original aspect ratio.
  - Mods use the Hedge Mod Manager format. "Code modding is not yet possible".
  - The release came out in early March 2025. ([Wccftech, 2025-03-03](https://wccftech.com/sonic-unleashed-unofficial-pc-port-download/amp/); [GamingOnLinux, 2025-03](https://www.gamingonlinux.com/2025/03/sonic-unleashed-gets-a-full-fan-made-pc-port-with-linux-steam-deck-support))

### PS1, PS2 and PS3 recompilers

- **PS2Recomp (ran-j).** GPL-3.0. ([PS2Recomp](https://github.com/ran-j/PS2Recomp))
  - Takes a PS2 ELF, a TOML config and a function-map CSV exported from Ghidra (recommended for stripped retail games).
  - Extra entry points get `entry_...` wrappers. Calls bind through relocation symbols, and anything unresolved falls back to `runtime->lookupFunction`.
  - Has a syscall dispatcher, IRX modules with HLE fallbacks, raw instruction patches by address, and per-game override modules.
  - "Performance is very bad for VU and GS". Still experimental.
- **ps3recomp (sp00nznet).** Translates the PPU code ahead of time and links it with HLE replacements for the PS3 system libraries. RSX output goes to D3D12 or Vulkan. SPU programs are lifted per image. Some games reach initialisation or open a graphics window; none are fully playable. ([Generation Amiga, 2026-06-15](https://www.generationamiga.com/2026/06/15/ps3recomp-the-new-tool-bringing-native-playstation-3-ports-closer-to-pc/))
- **ReXGlue SDK** (Xbox 360) builds on Xenia and on XenonRecomp's analysis, and says it is in early development. The licence type could not be read from its page. ([rexglue-sdk](https://github.com/rexglue/rexglue-sdk))
- **PS1.** PSXRecomp translates R3000A code to C. RecompOne targets C#. Crash Bandicoot and Ape Escape builds exist. ([psxrecomp](https://github.com/mstan/psxrecomp); [readonlymemo](https://readonlymemo.com/ps1-recompiled-progress-recompone-pixygarden-interview/); [Generation Amiga, 2026-08-09](https://www.generationamiga.com/2026/08/09/crash-bandicoot-ps1-recomp-is-now-fully-playable-on-linux-and-windows/))

### How they ship legally

The pattern is the same everywhere:

- No game files are distributed. Zelda64Recomp: "The original game is required to build or run this project", and it accepts only the US ROM. ([Zelda64Recomp](https://github.com/Zelda64Recomp/Zelda64Recomp))
- An installer checks the user's own copy and accepts only unmodified originals ([UnleashedRecomp](https://github.com/hedge-dev/UnleashedRecomp)). PortableKit checks the disc ID and the SHA-256 of `EBOOT.BIN` before it decrypts anything ([BRINGING_UP_A_GAME.md](https://raw.githubusercontent.com/TeamGDB/PortableKit/HEAD/docs/BRINGING_UP_A_GAME.md)).
- Generated code is never committed ([PortableKit SOURCE_PROVENANCE.md](https://raw.githubusercontent.com/TeamGDB/PortableKit/HEAD/docs/SOURCE_PROVENANCE.md)).
- **Counter-example.** The Super Mario 64 PC port was distributed as a prebuilt executable. Nintendo's representatives then targeted the sites hosting it, and one complaint called it "an unauthorized derivative work". ([VGC, 2020-05-08](https://www.videogameschronicle.com//news/nintendo-takes-action-against-mario-64-pc-port))
- I found no case of Square Enix acting against a fan port. That is not evidence that it would not.

---

## 2. PSP-specific prior art

PSP static recompilation went from nothing to several working projects between July and October 2026.

| Project | Licence | Status (own README) | Notes |
|---|---|---|---|
| [TeamGDB/PortableKit](https://github.com/TeamGDB/PortableKit) | MIT | "Early, and honest about it". CI passes on Linux, macOS and Windows. Last commit seen 2026-10-07 ([commits](https://github.com/TeamGDB/PortableKit/commits/main)). | Recompiler plus interpreter fallback, HLE kernel and modules, Vulkan GE renderer on SDL3, audio including ATRAC3, movies, saves, ad hoc networking, settings UI and installer. It was pulled out of Yakumo "with their history". ([README](https://raw.githubusercontent.com/TeamGDB/PortableKit/HEAD/README.md)) |
| [TeamGDB/Yakumo](https://github.com/TeamGDB/Yakumo) (Monster Hunter Portable 3rd HD) | MIT | "Playable". v0.7.5 released 13 Oct, earliest visible release v0.4.0 on 19 Sep ([releases](https://github.com/TeamGDB/Yakumo/releases)). Commits are co-authored with "codex" ([commits](https://github.com/TeamGDB/Yakumo/commits/main)). | Packages for Windows, Linux/Steam Deck (Flatpak), Android and macOS. Details below. |
| [arnaldolima1/gow-ghost-of-sparta-pc](https://github.com/arnaldolima1/gow-ghost-of-sparta-pc) | MIT | "Alpha. Only the beginning of the game has been played so far." | Built on a PortableKit fork. Vulkan, up to ×5 internal resolution, "60 fps and full speed" in the first level, PPSSPP-format texture packs. |
| [jessicanataliagta/PSPRecomp](https://github.com/jessicanataliagta/PSPRecomp) | MIT | First working profile: GTA: Vice City Stories (Windows/DX12). Commits 12–14 Aug 2026 ([commits](https://github.com/jessicanataliagta/PSPRecomp/commits/main)). | Analyser, `psp_recomp` C++ emitter, and runtime split into profiles. Coverage of VCS: [GTAboom, 2026-08-13](https://www.gtaboom.com/vice-city-stories-runs-natively-on-pc-now-no-emulator-needed-0148). |
| [sp00nznet/psprecomp](https://github.com/sp00nznet/psprecomp) | MIT | "It does not yet render." The main game stalls in start-up. Commits around 20–21 Jul 2026 ([commits](https://github.com/sp00nznet/psprecomp/commits/main)). | Emits C. Functions found by a "fixpoint walk over exports, relocations and jump tables" (92% of `.text`), and every label can be dispatched. VFPU partly done. HLE layer referenced against uOFW. Uses PPSSPP only as a separate-process "oracle". |
| [zmbkilla/PSP2I_Recomp](https://raw.githubusercontent.com/zmbkilla/PSP2I_Recomp/HEAD/README.md) (Phantasy Star Portable 2 Infinity) | Not stated | Native Windows/Android/macOS. D3D11. Right-stick camera. "60 FPS mode is not properly implemented". | Built on sp00nznet's `allegrexrecomp`. "Heavy use of AI-assisted development, primarily Claude". |
| [sal063/Ace-Combat-X-Recompilation](https://github.com/sal063/Ace-Combat-X-Recompilation) | GPL-2.0 | Tests run up to the first HLE call. 10 commits. | Own recompiler, `pspreco`. Vulkan GE with SDL3, headless software build, PPSSPP-format texture packs. ([Generation Amiga, 2026-08-16](https://www.generationamiga.com/2026/08/16/ace-combat-x-is-making-an-unexpected-jump-from-psp-to-windows/)) |
| PortableKit's other ports: Purun (LocoRoco 2), Tenkawa (DBZ Tenkaichi Tag Team) | — | Purun: first level plays at 60 fps, macOS only. Tenkawa: title screen under the interpreter only. | ([Purun](https://raw.githubusercontent.com/TeamGDB/Purun/HEAD/README.md), [Tenkawa](https://raw.githubusercontent.com/TeamGDB/Tenkawa/HEAD/README.md)) |

### Yakumo in detail

Yakumo is the most complete PSP port and the closest model for this project. ([Yakumo README](https://raw.githubusercontent.com/TeamGDB/Yakumo/HEAD/README.md))

- **Overlays.** The game loads "355 code overlays at run time into a handful of shared memory slots". Each overlay is recompiled into its own library, and the matching library is installed between frames. Code the libraries miss falls back to the interpreter. Generating the overlays takes about 40 minutes the first time.
- **How the swap works.** The framework provides a missing-function hook, an unsupported-instruction hook and `Runtime::unregister_functions(start, end)`. A profile "typically builds each overlay as its own shared library, identifies the loaded one by hashing guest memory", and swaps it in. ([Yakumo ARCHITECTURE.md](https://raw.githubusercontent.com/TeamGDB/Yakumo/HEAD/docs/ARCHITECTURE.md)) This fits Dissidia's three overlays that share load addresses.
- **Frame rate.** The simulation stays at 30 fps, and optional interpolation presents 45/60/90/120 fps or the display rate "without changing game speed". The method and its latency are not documented.
- **Other features.** Adjustable internal resolution, an aspect-ratio setting (behaviour not described), PPSSPP-compatible texture packs, file-replacement mods (no code mods yet, issue #81), right-stick camera, rebindable keyboard and mouse, controller presets.

### PortableKit bring-up process

([BRINGING_UP_A_GAME.md](https://raw.githubusercontent.com/TeamGDB/PortableKit/HEAD/docs/BRINGING_UP_A_GAME.md); [ARCHITECTURE.md](https://raw.githubusercontent.com/TeamGDB/PortableKit/HEAD/docs/ARCHITECTURE.md))

1. Write a constants-only profile.
2. Install and decrypt the executable.
3. Run the whole game under the interpreter, which is "roughly twenty times slower". Each run reports the HLE import gap and the first unimplemented call.
4. Only after the game boots, run `psp_recomp --auto` (minutes), then compile (hours).

Hooks a profile can use:

- `Runtime::register_native_fast_path(address, callback)` replaces a guest function with native code and falls back to the generated one if no callback is registered.
- Profiles can add HLE calls and patch the loaded image.
- `PSPRECOMP_NO_INTERPRETER=1` checks that nothing falls back to the interpreter.

**Instruction coverage.** No VFPU instruction is listed as unimplemented. Many are "implemented, unverified". Trap instructions and `mfc2`/`mtc2`/`cfc2`/`ctc2` are not implemented. ([INSTRUCTION_COVERAGE.md](https://raw.githubusercontent.com/TeamGDB/PortableKit/HEAD/docs/INSTRUCTION_COVERAGE.md))

**PPSSPP code.** PortableKit says PPSSPP was "referenced, not ported": format facts were read from PPSSPP's source, but no code was copied. FFmpeg is dynamically linked under the LGPL. ([SOURCE_PROVENANCE.md](https://raw.githubusercontent.com/TeamGDB/PortableKit/HEAD/docs/SOURCE_PROVENANCE.md))

**Not documented:** how PortableKit finds functions, handles jump tables or deals with delay slots.

### Finished PSP decompilations

- **decomp.dev cannot be used to find them.** Its main index and new submissions are disabled. The site cites AI-assisted "hostile forks" and a "race to port". ([decomp.dev](https://decomp.dev/))
- Matching PSP decompilations I found, none complete and none ported to PC:
  - [sotn-decomp](https://github.com/xeeynamo/sotn-decomp) includes the PSP build (`pspeu`) and uses MW MIPS C Compiler 3.0.1.
  - [ts2psp-decomp](https://github.com/DaniKH1/ts2psp-decomp): 168 of 7,497 functions readable, mwccpsp.
  - [ygo-tf-5-decomp](https://github.com/elijahtuyay/ygo-tf-5-decomp): early, mwccpsp.
- Not found: any finished PSP decompilation, any PSP decompilation turned into a PC port, or any other SNC-compiled PSP decomp.

### Dissidia specifically

- **Dissidia 012.** One other, independent native-port effort was found, on PSPRecomp, targeting the EU release (ULES01505). It was in early bring-up at the time.
- **Dissidia (2008).** Nothing found.
- **PPSSPP.** It has a game-specific replacement named `dissidia_recordframe_avi` ([ReplaceTables.cpp](https://raw.githubusercontent.com/hrydgard/ppsspp/master/Core/HLE/ReplaceTables.cpp)). No Dissidia game ID appears in PPSSPP's `compat.ini` ([compat.ini](https://raw.githubusercontent.com/hrydgard/ppsspp/master/assets/compat.ini)).
- **Cheats.** Region-specific CWCheat files exist ([Almar's Guides](https://almarsguides.com/retro/walkthroughs/PSP/Games/Dissidia012FinalFantasy/CWCheat/USA/)). I found no Dissidia 012 60 fps or widescreen code. There is a general method for hunting frame-divider codes ([PPSSPP forum](https://forums.ppsspp.org/showthread.php?pid=105135)).

---

## 3. The PSP runtime layer

What a native runtime has to replace: the kernel (threads, semaphores, event flags, memory, modules), file I/O, display and vblank, the GE (display lists to a GPU), audio (sceAudio, sceSas, ATRAC3), video (sceMpeg/PSMF), sceUtility dialogs and savedata, input, and the VFPU. Both PortableKit's component list and PPSSPP's HLE directory cover all of these ([PortableKit](https://github.com/TeamGDB/PortableKit); [PPSSPP Core/HLE](https://github.com/hrydgard/ppsspp/tree/master/Core/HLE)).

### Reusable code

| Source | Licence | What it offers |
|---|---|---|
| PPSSPP | Each source file says "version 2.0 or later versions" ([sceAtrac.cpp header](https://raw.githubusercontent.com/hrydgard/ppsspp/master/Core/HLE/sceAtrac.cpp)). LICENSE.TXT is plain GPLv2 ([LICENSE.TXT](https://raw.githubusercontent.com/hrydgard/ppsspp/master/LICENSE.TXT)). | HLE modules (`sceAtrac`, `sceAudio`, `sceGe`, `sceDisplay`, `sceIo`, `sceKernel*`, `sceFont` and more). Vulkan, D3D11 and OpenGL backends; D3D9 removed in 1.20. Latest release 1.20.4. ([PPSSPP](https://github.com/hrydgard/ppsspp); [APKMirror listing dates](https://www.apkmirror.com/?p=13844286)) |
| PPSSPP libretro core | GPLv2 ([libretro docs](https://docs.libretro.com/library/ppsspp/)) | Kept in-tree under `libretro/`, with Vulkan, D3D11 and GL contexts ([libretro/](https://github.com/hrydgard/ppsspp/tree/master/libretro)). Rendering resolution goes up to 4800×2720. |
| PPSSPP headless | GPL | A `headless/` target exists in the repo ([PPSSPP](https://github.com/hrydgard/ppsspp)). Distributions build it with CMake `HEADLESS=ON` ([openSUSE spec](https://src.opensuse.org/rpm/ppsspp/src/branch/factory/ppsspp.spec)). |
| PortableKit | MIT | A complete runtime already used for shipped ports (see §2). |
| uOFW | MIT, plus BSD parts from PSPSDK | Reverse-engineered PSP firmware, "far from complete" ([uofw](https://github.com/uofw/uofw)). |
| Jpcsp | GPL-3.0-or-later, Java | HLE emulator. Last dated release v0.7 (2013) ([jpcsp](https://github.com/jpcsp/jpcsp)). Not practical to embed. |
| ghidra-allegrex | Apache-2.0 | Allegrex and VFPU decoding for Ghidra, PSP relocation types A and B, import/export of PPSSPP `.sym` files. VFPU prefixes are not modelled ([ghidra-allegrex](https://github.com/kotcrab/ghidra-allegrex)). |

### What the GPL means for a port

- If a port links PPSSPP code, the whole combined program must be GPL. The GPL requires derivative works to be under the GPL. The FSF treats even dynamic linking as creating a derivative work. Separate programs talking over pipes or sockets are normally an "aggregate". ([Wikipedia: GPL, linking and aggregates](https://en.wikipedia.org/wiki/GNU_General_Public_License))
- Because PPSSPP is "or later", its code can go into a GPL-3.0 program. MIT code can be combined with GPL code. ([same](https://en.wikipedia.org/wiki/GNU_General_Public_License))
- PortableKit and sp00nznet/psprecomp both avoid copying PPSSPP code so they can stay MIT ([SOURCE_PROVENANCE.md](https://raw.githubusercontent.com/TeamGDB/PortableKit/HEAD/docs/SOURCE_PROVENANCE.md); [psprecomp](https://github.com/sp00nznet/psprecomp)). Ace Combat X is GPL-2.0 ([repo](https://github.com/sal063/Ace-Combat-X-Recompilation)).
- Practical result: GPL is acceptable if the port's own code is GPL. It only causes trouble if you want an MIT or other permissive port.

### How hard GE at high resolution is (from PPSSPP's experience)

- **Upscaling artifacts.** Higher render resolutions show "thin lines between elements in menus and HUD". Blur and bloom drawn at the higher resolution look like "a strange outline, after image or ghosting", so a "Lower resolution for effects" option exists. ([PPSSPP graphics settings](https://www.ppsspp.org/docs/settings/graphics/))
- **Readbacks.** When a game copies render output into RAM, PPSSPP by default assumes the CPU will read it, which forces a "very costly readback". A per-game option creates framebuffers over the destination instead. ([Progress report, spring 2023](https://www.ppsspp.org/blog/progress-report-spring-2023))
- **Depth buffer quirks.** These needed separate fixes:
  - Binding the depth buffer as a colour target ([#15772](https://github.com/hrydgard/ppsspp/pull/15772)).
  - Texturing from depth ([#13262](https://github.com/hrydgard/ppsspp/pull/13262)).
  - Block transfers into depth ([commit 88f2657](https://github.com/hrydgard/ppsspp/commit/88f2657bb173c6491052f4361501081cf05baeff)).
  - Depth swizzle when depth shares VRAM with textures ([#22435](https://github.com/hrydgard/ppsspp/pull/22435)).
  - Render offsets that reuse a depth address ([#22466](https://github.com/hrydgard/ppsspp/pull/22466)).
  - A software depth buffer run alongside hardware rendering ([#19748](https://github.com/hrydgard/ppsspp/pull/19748)).
- **Per-game function hooks.** PPSSPP fingerprints functions by hashing their opcodes with immediates masked out, then replaces or hooks them (`REPFLAG_HOOKENTER`, `REPFLAG_HOOKEXIT`). Many of these entries handle framebuffer downloads (`*_download_frame`) and frame-rate hacks (`gow_fps_hack`). ([ReplaceTables.cpp](https://raw.githubusercontent.com/hrydgard/ppsspp/master/Core/HLE/ReplaceTables.cpp))
- **For a single game** (my analysis, not sourced): a one-game renderer only needs the GE features and framebuffer tricks that Dissidia actually uses. That is less work than PPSSPP's general case, but which features those are has to be measured. PPSSPP's `compat.ini` lists no Dissidia flags, which suggests (but does not prove) that Dissidia uses no unusual framebuffer tricks.

---

## 4. Alternatives compared

### (a) PPSSPP-based "wrapper" port

A trimmed PPSSPP (or its libretro core) that boots only this game, with game-specific HLE hooks and replaced functions. PPSSPP's own `ReplaceTables` mechanism is the model for those hooks ([ReplaceTables.cpp](https://raw.githubusercontent.com/hrydgard/ppsspp/master/Core/HLE/ReplaceTables.cpp)).

- **Gains.** High resolution is already solved, up to 10×, with known artifacts ([libretro docs](https://docs.libretro.com/library/ppsspp/); [PPSSPP settings](https://www.ppsspp.org/docs/settings/graphics/)). The existing PPSSPP plugin mod carries over almost directly.
- **Limits.** PPSSPP has no frame interpolation, only "Render duplicate frames" ([PPSSPP settings](https://www.ppsspp.org/docs/settings/graphics/)). Interpolation would have to be added to its GPU layer. The game still runs on PPSSPP's JIT. The result must be GPL.
- **Risks.** Low technical risk. The result is still an emulator underneath, which matters if the aim is a "port".

### (b) Static recompilation with a native runtime

Use PortableKit or PSPRecomp with a Dissidia profile.

- **Gains.** Yakumo shows on PSP hardware what this route delivers: overlays, adjustable internal resolution, 30→N fps interpolation, Windows and Steam Deck builds, right-stick camera, texture packs ([Yakumo](https://raw.githubusercontent.com/TeamGDB/Yakumo/HEAD/README.md)). Mods can override functions natively through `register_native_fast_path` ([ARCHITECTURE.md](https://raw.githubusercontent.com/TeamGDB/PortableKit/HEAD/docs/ARCHITECTURE.md)). MIT licence.
- **Risks.**
  - The framework is very young.
  - Its HLE, GE and VFPU coverage is only as good as the games it has been tested on. VFPU instructions are "implemented, unverified" ([INSTRUCTION_COVERAGE.md](https://raw.githubusercontent.com/TeamGDB/PortableKit/HEAD/docs/INSTRUCTION_COVERAGE.md)).
  - Compiling the generated code "takes hours" ([BRINGING_UP_A_GAME.md](https://raw.githubusercontent.com/TeamGDB/PortableKit/HEAD/docs/BRINGING_UP_A_GAME.md)).

### (c) Full non-matching decompilation to portable C

- **Reference timelines.**
  - Super Mario 64 decomp: described as "a two-year-long project". The PC port followed in May 2020. ([Waxy, 2020-05-07](https://waxy.org/2020/05/ars-technica-on-the-massive-effort-to-reverse-engineer-n64-source-code/); [My Nintendo News, 2020-05-04](https://mynintendonews.com/2020/05/04/pc-port-of-super-mario-64-has-appeared-with-4k-and-widescreen-support/))
  - Ocarina of Time decomp: done after "nearly two years" ([VGC, 2021-11-27](https://www.videogameschronicle.com/news/zelda-64-has-been-fully-decompiled-potentially-opening-the-door-for-mods-and-ports/)). Ship of Harkinian followed in 2022 ([DSOGaming](https://www.dsogaming.com/news/the-second-unofficial-pc-port-for-the-legend-of-zelda-majoras-mask-has-been-released-for-free/)).
  - Melee (3.88 MB of code, a team): just over six years, with AI help at the end ([Held Games, 2026-09-09](https://heldgames.com/guides/melee-decompiled-ai-astra)).
  - Snowboard Kids (2,119 functions, AI-assisted): 84 days to 100% matched ([Capital and Compute, 2026-10-12](https://capitalandcompute.net/blog/which-games-has-ai-decompiled/)).
- **Non-matching vs matching speed.** I found no source that measures this. One source notes that matching is the part AI handles well, while making the code readable is still mostly human work ([Capital and Compute](https://capitalandcompute.net/blog/which-games-has-ai-decompiled/)).
- **Gains.** The most freedom: real source code, any renderer, logic changes. The decomp-based Majora's Mask port was expected to make custom features easier to add than the recompiled one ([DSOGaming](https://www.dsogaming.com/news/the-second-unofficial-pc-port-for-the-legend-of-zelda-majoras-mask-has-been-released-for-free/)).
- **Risks.** This game's code is larger than Melee's, and the project is at 0.44%. Without a matching check, decompiled C can drift from the original's behaviour. The project's current SNC matching setup would be given up for most code.

### (d) Hybrid: recompile everything, replace functions with C over time

- **Mechanisms that already exist:**
  - N64Recomp's single-file mode, where patches come first on the linker line ([README](https://raw.githubusercontent.com/N64Recomp/N64Recomp/main/README.md)).
  - XenonRecomp's weak `PPC_FUNC_IMPL` overrides ([XenonRecomp](https://github.com/hedge-dev/XenonRecomp)).
  - PortableKit's `register_native_fast_path` ([ARCHITECTURE.md](https://raw.githubusercontent.com/TeamGDB/PortableKit/HEAD/docs/ARCHITECTURE.md)).
- **How Zelda64Recomp used it.** Its patches compile against the Majora's Mask decomp headers ([Zelda64Recomp](https://github.com/Zelda64Recomp/Zelda64Recomp)). For Dissidia, the 278 matched functions and recovered structs play that role. Each newly decompiled function can be swapped in and checked against the recompiled original.
- **Gains.** The game is playable early. Decompilation effort can be spent only where enhancements need it: camera, HUD layout, render submission, input.
- **Risks.** The guest memory layout and calling convention stay in place, so replaced functions must keep the guest ABI.

### Comparison table

Effort figures are my estimates for one experienced person with AI help, assuming the frameworks behave as their READMEs say. They are not sourced.

| | (a) PPSSPP wrapper | (b) Recomp + runtime | (c) Non-matching decomp | (d) Recomp then C replacement |
|---|---|---|---|---|
| Time to first playable | Weeks | Weeks to a few months (unknown HLE/GE gaps) | Years | Same as (b) |
| High resolution | Yes, exists | Yes, if the renderer has it (PortableKit does) | You write the renderer | Same as (b) |
| High frame rate | Must add interpolation to PPSSPP's GPU layer | Interpolation exists in Yakumo | Full control | Same as (b), plus logic changes where decompiled |
| Widescreen beyond 16:9 | Game patches (projection, HUD) | Game patches | Source changes | Game patches, then C |
| Right stick, L2/R2/L3/R3 | Carries over from the existing plugin | Profile hooks / native overrides | Source changes | Native overrides |
| Code mods | Hook table | Native fast paths | Full | Full for replaced functions |
| Licence | GPL (forced) | MIT available | Your choice | MIT available |
| Main risk | Remains an emulator. Cannot become MIT. | Young framework. Dissidia-specific gaps unknown. | Scale | Keeping the guest ABI correct |

---

## 5. REA (Reverse Engineer Anything)

Repo: [morluto/rea](https://github.com/morluto/rea). REA's own code is MIT. It is an MCP server and CLI that sits in front of Hopper, Ghidra or IDA. The npm page returned HTTP 403, so the npm publish date and version history could not be checked there. The changelog lists **6.5.0 (2026-10-13)** and **6.6.0 (2026-10-18)** ([CHANGELOG.md](https://raw.githubusercontent.com/morluto/rea/main/CHANGELOG.md)).

### What 6.5.0 changed for this project

- **The PSP profile is now released, unchanged from main.** 6.5.0 adds the "PSP static ELF profile", limited to "bounded little-endian ELF32 `ET_EXEC` with the documented PSP/ABI declarations" and requiring "a compatible caller-installed Allegrex extension". "PRX, shared/relocatable images, PBP/ISO/CSO and runtime emulation are outside scope." ([CHANGELOG.md](https://raw.githubusercontent.com/morluto/rea/main/CHANGELOG.md))
- **The admission rules are strict.** ([docs/ghidra-psp.md](https://raw.githubusercontent.com/morluto/rea/main/docs/ghidra-psp.md))
  - Platform: "limited to Linux x64".
  - `e_flags` must be exactly `0x10a23001`.
  - A version-0 `.MIPS.abiflags` record is required, with set values (ISA level 2, single-float, `flags1=1` and so on). "Missing, uninspected, contradictory and other producer declarations are refused."
  - "live code overlays are outside this first profile."
- **A new rule makes MIPS stricter.** "MIPS targets with unresolved GNU ABI attributes are refused" ([CHANGELOG.md](https://raw.githubusercontent.com/morluto/rea/main/CHANGELOG.md)).
- **Windows still cannot take MIPS.** Windows Ghidra P0 admits only "native, non-managed x86, x86-64, ARMNT (Thumb-2) or ARM64 PE" images and raw NMOS 6502 images ([docs/windows-ghidra-p0.md](https://raw.githubusercontent.com/morluto/rea/main/docs/windows-ghidra-p0.md)). The 6.6.0 changelog adds ARM PE and notes that "Windows P0 does not admit m68k". The parts I read show no PSP changes in 6.6.0.
- **Result for this game.** The SNC-built EBOOT has no `.MIPS.abiflags`, so it is refused by rule. The `0xffa8` overlays are not `ET_EXEC`, and overlays are excluded anyway. Windows is excluded. Nothing in 6.5.0 or 6.6.0 lifts any of these limits.

### What REA offers beyond a Ghidra front end

- **Operations.** Sessions, `binary_overview`, `analyze_function`, call graph and call-path tracing, function and artifact comparison, `read_bytes`, segments, and an "evidence"/"unknowns" ledger ([docs/mcp-contracts.md](https://raw.githubusercontent.com/morluto/rea/main/docs/mcp-contracts.md)). Windows P0 lists "26 read-only Ghidra inventory, memory, and function-analysis operations" ([windows-ghidra-p0.md](https://raw.githubusercontent.com/morluto/rea/main/docs/windows-ghidra-p0.md)).
- **Missing features.** I found no batch decompilation, type or struct recovery, or bulk renaming written back to a Ghidra project. "Each new provider session still imports the target into a fresh temporary project" ([mcp-contracts.md](https://raw.githubusercontent.com/morluto/rea/main/docs/mcp-contracts.md)). PSP metadata edits are "session-local" ([ghidra-psp.md](https://raw.githubusercontent.com/morluto/rea/main/docs/ghidra-psp.md)). The roadmap only mentions "type ... verification" and has no PSP plans ([docs/roadmap.md](https://raw.githubusercontent.com/morluto/rea/main/docs/roadmap.md)).

**Verdict: REA 6.5.0 does not help this project.** It refuses the binaries on Windows and on Linux. Even if it accepted them, it would add read-only analysis that the project's own Ghidra scripts and ghidra-allegrex already provide, with nothing saved between sessions. Its documented strengths (JavaScript/Electron, Android, managed code, captures) do not apply here.

---

## 6. Frame rate and widescreen in recompiled ports

### How others did it

- **Zelda64Recomp.** Game logic is unchanged ("Changing framerate has no effect on gameplay"). RT64 generates in-between frames by matching draw calls across frames and interpolating their transforms ([Zelda64Recomp](https://github.com/Zelda64Recomp/Zelda64Recomp); [RT64](https://github.com/rt64/rt64)).
  - Good results needed the game to *tag* its transforms with matrix-group IDs. That is what the many `*_transform_tagging.c` patches are for ([patches/](https://github.com/Zelda64Recomp/Zelda64Recomp/tree/dev/patches); [rt64_extended_gbi.h](https://raw.githubusercontent.com/rt64/rt64/main/include/rt64_extended_gbi.h)).
  - Widescreen needed effects "modded to work correctly in widescreen" and HUD alignment commands.
- **Unleashed Recompiled.** It took the other route and raised the simulation rate itself. The original game's "time step itself is subject to large deviations". The port fixed "a vast amount of glitches" that showed up at high frame rates, though some gameplay bugs remain, and it may adopt "transform interpolation" later ([UnleashedRecomp](https://github.com/hedge-dev/UnleashedRecomp)).
- **Yakumo (PSP).** It keeps the 30 fps simulation and interpolates presentation up to the display rate. The method is not documented ([Yakumo](https://raw.githubusercontent.com/TeamGDB/Yakumo/HEAD/README.md)).
- **Other PSP ports.** God of War: Ghost of Sparta reaches 60 fps by removing the one-flip-per-vblank cap ([gow-ghost-of-sparta-pc](https://github.com/arnaldolima1/gow-ghost-of-sparta-pc)). PSP2i's 60 fps mode "is not properly implemented" ([PSP2I_Recomp](https://raw.githubusercontent.com/zmbkilla/PSP2I_Recomp/HEAD/README.md)).
- **Fixed timestep with interpolation.** The standard technique renders an interpolation between the previous and the current simulation state ([Fiedler, "Fix Your Timestep", 2004](https://gafferongames.com/post/fix_your_timestep/)).

### What this means for Dissidia's 30 Hz battles

This part is my analysis, not sourced.

- **Keep battle logic at 30 Hz.** In a fighting game, frame data (startup, active frames, recovery, hit-stun, input buffers) is counted in 30 Hz ticks. Running the simulation at 60 Hz would mean finding and rescaling every timer, velocity and animation step. Missing one changes how the game plays, and Unleashed shows how long that list gets.
- **Interpolate presentation.** This needs a stable identity for each draw or transform between ticks, in the RT64 matrix-group style. For a PSP game, that means hooks where the game submits model, bone and camera matrices to the GE. Those submission functions are good early candidates for decompilation or native overrides.
- **Latency cost.** Interpolating between two finished ticks shows a state up to one 30 Hz tick (about 33 ms) old, unless the renderer extrapolates. That latency should be measured and offered as an option, not imposed.
- **Widescreen.** The PSP screen is 480×272, the libretro core's default resolution ([libretro docs](https://docs.libretro.com/library/ppsspp/)), so it is already close to 16:9. Real work starts only at 21:9 and wider: widen the projection, anchor the HUD to screen edges, extend 2D backgrounds, and fix culling. Each of these is a game-side patch in every approach.

---

## Recommended path

Recommendation: route (b) as the base, growing into (d). Keep route (a) as a fallback if Phase 1 fails.

| Phase | Work | Exit criterion | Rough effort (estimate) |
|---|---|---|---|
| 0. Survey | Read PortableKit's `PROFILE_GUIDE.md`, `HLE_EXTENSIONS.md` and source. Decide the licence (MIT with PortableKit, or GPL if any PPSSPP code is used). | Written go/no-go | 2–4 days |
| 1. Interpreter bring-up | Write a constants-only PortableKit profile for `ULUS10566`. Run under the interpreter. Fill HLE gaps until the title screen, then the menus, then a battle. | Battle playable (slowly) under the interpreter | 2–8 weeks; the most uncertain phase |
| 2. Recompile | Run `psp_recomp` on the EBOOT and the three overlays. Swap overlay libraries per slot by hashing guest memory, as Yakumo does. Reach zero interpreter fallbacks (`PSPRECOMP_NO_INTERPRETER=1`). | Full speed, no fallbacks, on Windows and Steam Deck | 2–6 weeks |
| 3. Native features | Right-stick camera and L2/R2/L3/R3 as native overrides and profile hooks. Turn on internal resolution scaling. | Camera and extra buttons working natively | 2–4 weeks |
| 4. Presentation | Add transform tagging and interpolation for battles (30 Hz logic, N Hz display). Add HUD anchoring and projection changes for wider screens. | 60+ fps display, 21:9 option, latency measured | 1–3 months |
| 5. Grow the C | Swap matched or non-matching C functions in for recompiled ones, starting with the camera, HUD and render-submit paths the enhancements touch. The matching build stays the reference. | Ongoing | Ongoing |

**If Phase 1 stalls** because of missing HLE or GE features that PortableKit's author will not take upstream, build route (a): a single-game PPSSPP build under the GPL. It already handles high resolution, and the existing plugin work carries over.

---

## Open questions

1. What licence does this repo want for a port? It decides whether PPSSPP code can ever be used.
2. Does PortableKit's maintainer accept outside profiles and framework patches? (The Ghost of Sparta port carries its changes on a fork branch: [gow-ghost-of-sparta-pc](https://github.com/arnaldolima1/gow-ghost-of-sparta-pc).)
3. How do the three overlays get loaded? `sceKernelLoadModule` PRXs (`e_type 0xffa8`) or raw copies? How many slots are there? This decides whether swapping libraries by memory hash is enough.
4. Which VFPU instructions does Dissidia use, and are they among PortableKit's "implemented, unverified" set?
5. Does Dissidia read framebuffers back with the CPU, or render to texture for effects (for example the EX Burst and summon screens)? This decides how much of PPSSPP's framebuffer experience must be repeated.
6. Where does the battle loop submit matrices to the GE? Can they be tagged for interpolation without decompiling the renderer?
7. How long does one 30 Hz battle tick take in recompiled code, and what latency does interpolation add in practice?
8. Not found in this research: how PortableKit or Yakumo find functions, handle jump tables and delay slots, or implement interpolation; the timing of non-matching vs matching decompilation; REA's npm publish date (npm returned 403).

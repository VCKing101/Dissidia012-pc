# PortableKit gap analysis for Dissidia 012 (ULUS10566)

> Source-reading report, 18 October 2026. PortableKit commit `33800c4` (7 October 2026), cloned read-only at
> `tools/local/port/PortableKit`. Nothing in the clone was built, configured or run. Inputs from this repository: the
> survey outputs in `build/scratch/port/` (see [binary-survey.md](binary-survey.md)), the four decrypted ELFs in
> `build/ULUS10566/` and the disc PRX.

Conventions as in the survey: "V:" = verified (from PortableKit's source, or from the game's binaries by a script);
"I:" = inference. Source paths below are relative to the PortableKit clone; `file:line` points at the line read.
PortableKit was written against other games, so "implemented" means "code exists", not "works for Dissidia".

Script (new, in `tools/port/`, run with `.venv\Scripts\python.exe`):

| Script | Output | What it does |
| --- | --- | --- |
| `portablekit_coverage.py <clone>` | `portablekit_coverage.json` | Reads PortableKit's `configs/nids.csv` and every `hle.add`/`hle.try_add` in `host/` (loops included), classifies each of the game's imports; maps every opcode in `instruction_census.json` to PortableKit's decoder kind and checks for a case in the interpreter and the recompiler; checks the ELFs against the loader's assumptions (entry point, overlay identification) |

The HLE classes are a source-text heuristic. "Constant answer" means the handler only returns a literal or named
constant; "UNVERIFIED" means the handler logs PortableKit's own UNVERIFIED marker. The authoritative list for a run is
the port's `<PREFIX>_LIST_STUBS=1` output (`host/system.cpp:120`).

---

## 0. Summary

| Area | Result |
| --- | --- |
| HLE, 297 distinct imports | **226 implemented, 30 constant answers, 8 UNVERIFIED, 33 missing**. Of the 33: 8 are `scesupPreAcc` (the disc PRX, not firmware), 12 are ad hoc networking (10 of them `sceNetAdhocMatching`), 13 are the rest |
| HLE outside networking and the PRX | 12 missing functions with call sites: `sceUtilityScreenshot*` (4), `sceFontFindFont`, `sceFontGetFontList`, `sceFontSetAltCharacterCode`, `sceMpegGetPcmAu`, `sceAudioOutput2GetRestSample`, `sceKernelGetGPI`, `sceKernelCancelSema`, `sceIoChstat` |
| CPU, 219 opcodes counted by the census | All decoded. 216 run in both the interpreter and the recompiler. `break` (47, fatal paths only) stops both; `cfc1`/`ctc1` (2 each) are supported for FCR31 only, which is the only control register the game uses |
| VFPU, 113 opcodes / 10,885 instructions | **113 of 113 in both the interpreter and the recompiler**. None is in PortableKit's "implemented, not yet run by any game" list; one (`vcrs.t`) is in its "verified by a game" list; the rest are "there before the page" (verified only in the sense that the existing ports run them) |
| Recompiler | Emits every word of the executable sections as a dispatch entry, so the ~1,229 uninventoried functions are covered without a function list |
| Overlays | Mechanism fits (slot hash, dispatch-miss install, I-cache-flush revalidation), but the tooling assumes Capcom's `MWo3` overlay header; Dissidia's `0xFFA8` ELFs need their own `meta.txt` writer |
| Boot | **Blocks before the first instruction**: `EBOOT.ELF` has `e_entry = 0`; PortableKit starts at `e_entry`. The real entry is the `module_start` export, `0x08804040` |
| GE | Render targets, framebuffer-as-texture, block transfers, CLUT loads, CALL/RET, depth test, skinning and splines exist. Missing or simplified: stencil, logic op, colour test, texture env colour, TFX add, mipmaps, morph weights, bounding-box jump, depth-buffer addressing, targets larger than 480x272 |
| Media | ATRAC3/ATRAC3plus and H.264 through FFmpeg (LGPL, dynamic); sceMpeg low-level path implemented; sceSas native (ADPCM, ADSR, noise, PCM) without reverb; fonts drawn from host TrueType, PGF not read |
| Build here | Visual Studio 2022 (MSVC 14.44), its bundled CMake 3.31.6 and Ninja 1.12.1, Python 3.12, Git present. **Missing: SDL3, Vulkan SDK (headers, `glslangValidator`), ccache.** FFmpeg for Windows is downloaded at configure time |

Top blockers per stage are in [section 8](#8-gaps-by-bring-up-stage).

---

## 1. Architecture

### 1.1 How the recompiler finds code, jump tables and delay slots

V: `psp_recomp <ELF> --auto <dir> [load_base] [unit_span] [--prefix P] [--code START-END]` (`tools/codegen_main.cpp:1822`)
runs a function analysis and then ignores it for emission:

- **Executable ranges** are the `SHF_EXECINSTR` PROGBITS sections backed by a loaded segment, or executable segments
  when there are no such sections (`src/program_analysis.cpp:66`). `--code` adds ranges the section table does not mark
  (`:528`). For Dissidia: main `.text` `0x08804040..0x089C51DC` plus the stub section `0x089C5770..0x089C60B0`; each
  overlay's single section (flags 7, so executable).
- **Seeds** (`:402`): the ELF entry, every `jal` target, constants built by `lui`/`addiu`/`ori` chains
  (`:297`), words in non-executable sections that point at a stack-frame prologue or follow a `jr ra` (`:373`, ET_EXEC
  only), and `R_MIPS_32` relocations. A CFG walk per seed (`:430`) follows branches and `j`, stops at `jr`/`jalr`, and
  does not read jump tables.
- **Emission ignores all of that.** "Emit every instruction word of the executable sections, and make every one of them
  a dispatcher entry" (`tools/codegen_main.cpp:1628`). Words are bucketed into fixed 16 KiB units (`:1639`, default
  span `0x4000`, `:1851`); each unit becomes one C++ function with a label per word and a dense PC-to-label table
  (`:1045`). The analysis only feeds the report's `discovered_function_seeds` count (`:1798`). Data words decode to a
  trap that is never reached unless executed.
- **Delay slots**: a branch emits its delay-slot instruction inline before the transfer, once for ordinary branches and
  inside the taken arm for likely branches (`:1131`-`:1165`). The slot word is also emitted again as its own label.
  Control flow in a delay slot is refused (`:1138`); the coverage script finds none in the game's function bodies.
- **Jump tables**: `jr reg` sets `local_pc` and re-enters the unit's dispatch table (`:1242`-`:1247`), at most 2,048
  local transfers before handing back to the outer dispatcher. Because every word is a label, all 207 SNC tables and
  the 20 library `jr` sites work without table recovery. A target in another unit returns to the outer dispatcher.
- **Unsupported words** become `rt.unsupported(pc, word, "... not lowered yet")` (`:1254`, `:884`).

I: The cost of "every word is an entry" is code quality and compile time, not coverage: no register caching across
instructions (the register-cache passes were removed because MSVC could not compile them, `:1720`), and each unit is a
single function of roughly 4,096 labels.

The manual mode `psp_recomp <ELF> <functions.csv> <out.cpp>` (`:1297`) takes `name,address,size` rows
(`configs/functions.example.csv`, loader at `:42`) but still labels every instruction inside each range, only supports
the default load base and has no `--prefix`. It is not useful for this game.

### 1.2 Code outside known functions

V: In a corpus build there is no "outside": every word of every executable section has a registered entry. What is
outside is code in memory the corpus was not generated from (an overlay with no library loaded, an unlisted module).
The outer loop (`src/runtime.cpp:584`, miss path at `:654`) asks the missing-function hook, then falls back to the
interpreter (`:211`). With `PSPRECOMP_NO_INTERPRETER=1` it stops and names the address.

### 1.3 Calls, returns, virtual calls, imports

V: A `jal` to a label in the same unit is a `goto` (`tools/codegen_main.cpp:1177`). A `jal` elsewhere runs the callee
as a nested C++ call (`invoke_chained_direct` / `invoke_chained_call`) and resumes locally only if `ctx.pc` comes back
equal to the return address (`:1183`-`:1219`). `jalr` (all C++ virtual calls) does the same through
`Runtime::invoke_chained_call` (`:1228`; `src/runtime.cpp:240`), which looks the target up in a dense unit table or a
per-word table; no vtable analysis is involved. Nesting is bounded at 48 (`src/runtime.cpp:167`, `PSPRECOMP_CHAIN_DEPTH`);
past it, control returns to the outer dispatcher. A `jr ra` is just another register jump, so the 14 functions that
use `$ra` as a general register need nothing special.

V: A `jal` to an import stub always leaves the unit (`tools/codegen_main.cpp:989`); the import wrapper calls the bound
HLE function by slot and returns to `$ra` (`:1283`). Thread switches happen only there and at the starvation boundary
(every 20,000 dispatches, `host/kernel/kernel.cpp:105`-`:107`).

I: Main's 852 direct calls into the overlay window are fixed `jal`s to addresses with no generated unit in main, so
they take the runtime lookup path (`tools/codegen_main.cpp:1005`-`:1017`), which finds whichever overlay corpus is
installed. The 10 targets defined by more than one overlay need nothing extra.

### 1.4 Interpreter

V: `src/interpreter.cpp` executes from the missing address in slices of 1,024 instructions (`:26`,
`PSPRECOMP_INTERPRETER_BUDGET`) and leaves as soon as it reaches any registered address (`:914`), so imports, overrides
and recompiled code take over. It shares the decoder with the recompiler and the same VFPU helpers in
`AllegrexContext`. With no corpus at all the runtime binds the import stubs itself (`host/system.cpp:136`) and
interprets the whole game, "roughly twenty times slower" (`docs/ARCHITECTURE.md`). `PSPRECOMP_INTERPRETER_WATCH`
prints registers at chosen addresses.

### 1.5 Run-time overlays

V: `host/overlays.cpp`:

- The profile lists slots (`GameProfile::overlay_slots`, `host/profile.hpp:189`); `slot_of` finds the slot of a missed
  address (`:111`).
- Each overlay corpus is a shared library exporting `portablekit_overlay_info` (base, size, code size, FNV-1a hash) and
  `portablekit_register_overlay` (`host/overlay_module.hpp`). The host loads all libraries from `overlays/` at start-up.
- On a dispatch miss inside a slot, `install_overlay_for` (`:275`) hashes the first `64 + code_size` bytes of the slot,
  picks the library whose hash matches, calls `Runtime::unregister_functions` over the slot and registers the new
  corpus. Unknown images run interpreted; `<PREFIX>_DUMP_OVERLAYS` dumps them.
- `revalidate_overlays` (`:329`) drops a corpus whose image changed. It runs on every flip
  (`host/hle/hle_media.cpp:267`) and on `sceKernelIcacheInvalidateRange`/`All` (`host/hle/hle_system.cpp:52`).
  Dissidia's loader calls `sceKernelIcacheInvalidateRange` right after reading an overlay (survey 3.5), which is the
  cue this expects.
- Overlay corpora are generated with `--prefix` and are not in the main fast unit table (`tools/codegen_main.cpp:1741`).

Fit for Dissidia (V: `portablekit_coverage.json`, `boot.overlays`):

| Item | PortableKit assumes | Dissidia |
| --- | --- | --- |
| Slots | Fixed addresses | One window, `0x089DD4C0..0x08B62F70` |
| Image header | `MWo3` + 64 bytes; `tools/add_overlay.py:37` refuses anything else; `read_overlay_header` (`host/overlays.cpp:76`) is only used for names and dumps | 16-byte descriptor `{tag, counts, tables}` then code. No `MWo3` |
| Identity | FNV-1a of `64 + code_size` bytes from the slot base | First 64 bytes differ between menu, battle and extra (tags 2, 1, 3); so do the first 4,096 |
| Recompiling | `wrap_overlay.py` wraps a raw dump in an ELF | The disc files are already plain ELFs with one executable section at `0x089DD4C0`; `psp_recomp` can read them directly (no module info, so no imports, `src/elf32.cpp:340`) |
| Data in the hashed range | "the game writes into an overlay's data section" | Code and data share one section; the hashed range must stop before the first written data word |

So a Dissidia profile needs a small tool of its own (in this repository) that runs `psp_recomp` on each OVL ELF with
`--prefix` and writes `meta.txt` (`base`, `name`, `size`, `code_size`, `hash`) for `portablekit_add_game()`
(`cmake/PortableKit.cmake:584`). Small.

### 1.6 Native overrides and profile hooks

V:

- `Runtime::register_native_fast_path(address, fn)` (`src/runtime.cpp:1231`) is only reached from profile-generated code
  that calls `invoke_native_fast_path` (`:1240`); it is not a general hook.
- The general override is `Runtime::register_function(address, fn, name)` (`src/runtime.cpp:430`): a host function
  registered at a guest entry replaces the unit's entry there; a name not starting with `recomp_unit_` makes it
  non-chainable, so callers reach it through dispatch. The host function sets `ctx.pc` to `$ra` to return.
  I: Inline `goto`s inside the same 16 KiB unit bypass it; a replaced function called by a neighbour in the same unit
  is still run as recompiled code. Whether that matters per function has to be checked per override.
- Profile hooks (`host/profile.hpp:237`-`:287`): `register_extra_hle(HleRegistrar&)` to add or replace HLE,
  `patch_loaded_image(Runtime&, const Elf32Image&)` to patch memory before start, `camera` (`CameraDriver`, called at
  every flip), `view_aspect_frame` (widescreen), `interpolation_thresholds`, `frame_vblanks`, `fast_loading_allowed`,
  `glyph_cache`, `trigger_profiles`, `keyboard_defaults`.
- HLE extension modules (`docs/HLE_EXTENSIONS.md`, `include/portablekit/hle_extension.hpp`) can fill in, override or
  chain HLE from a separately built library, and can call back into guest code (`call_guest`).
- Run-time hooks for code that changes: `set_runtime_missing_function_hook`, `set_runtime_unsupported_hook`
  (`include/psprecomp/runtime.hpp:241`, `:248`); the overlay support already uses both.

### 1.7 What a Dissidia profile contains

From `host/profile.hpp:137` and `docs/PROFILE_GUIDE.md`. Values marked "?" are not filled in yet.

| Field | Dissidia value or source |
| --- | --- |
| `app_name`, `project_name`, `env_prefix`, `data_*` | Our choice, e.g. `env_prefix = "D012"` |
| `disc_id`, `game_title`, `release_name` | `ULUS10566` from `PARAM.SFO` |
| `encrypted_executable_sha256` | SHA-256 of `PSP_GAME/SYSDIR/EBOOT.BIN` (?) |
| `executable_sha256` | SHA-256 of the decrypted `EBOOT.ELF` (our inventory's `reference_sha256`), or of an entry-fixed copy (1.8) |
| `decryption_tag`, `decryption_key` | Word at `0xD0` of `EBOOT.BIN` and the published key for it (?). Not needed when the decrypted ELF is supplied |
| `load_base`, `guest_ram_bytes` | `0x08804000`; 32 MiB first (I: may need more, see 8) |
| `boot_path` | `disc0:/PSP_GAME/SYSDIR/EBOOT.BIN` |
| `overlay_slots` | `{0x089DD4C0, 0x08B62F70}` |
| `save_game_name`, `save_folders` | From the savedata calls (?) |
| `system_language`, `confirm_button` | 1 (English), 1 (cross) for a USA console |
| `frame_vblanks` | 2 for battles (survey 5.2). I: menus may run at 1; the field is one value per game |
| `register_extra_hle` | The `scesupPreAcc` bindings and any Dissidia-only HLE |
| `patch_loaded_image` | Version checks for anything a native override depends on |
| `camera`, `view_aspect_frame`, `interpolation_thresholds` | Later: right-stick camera, wide screens, world-scale cut thresholds |

Plus a `CMakeLists.txt` calling `portablekit_add_game()`, optional `config/nids.csv` for NIDs the framework lacks,
`generated/` and `overlays/` (never committed).

### 1.8 Supplying decrypted inputs

V: Two routes (`host/main.cpp:102`-`:220`, `docs/BRINGING_UP_A_GAME.md`):

- `--install image.iso`: the installer checks the disc id and the SHA-256 of `EBOOT.BIN`, decrypts with the profile's
  tag and key, checks the result, and keeps the ISO as `disc0:`.
- `<PREFIX>_GAME_DIR` pointing at a directory with `EBOOT.ELF`, `disc.iso` and `ms0/`. `disc0:` must be an ISO9660
  image (`host/kernel/iso_image.hpp`); an extracted `PSP_GAME` tree is not read. The retail ISO in the repository root
  serves. An executable whose hash the profile does not know runs interpreted only, with a warning (`host/main.cpp:372`).

The overlays and `LIBSUPPREACC.PRX` are plain ELFs on the disc and are read by the game itself through `sceIoRead`;
nothing needs decrypting except `EBOOT.BIN`.

**Entry point.** V: `EBOOT.ELF` has `e_entry = 0x00000000`; its `module_start` export (`syslib` NID `0xD632ACDB`) is
`0x08804040` (`boot` in the JSON). PortableKit starts the loader thread at `Elf32Image::runtime_entry()`, which is
`e_entry` for an ET_EXEC (`src/elf32.cpp:162`, `host/system.cpp:156`, `host/main.cpp:422`), and never reads the export.
Fix: read `module_start` when `e_entry` is 0 (a framework seam,
a few lines), or supply an `EBOOT.ELF` with `e_entry` set and declare that file's hash.

---

## 2. HLE coverage

V: `portablekit_coverage.json` (`hle`). PortableKit's table names 567 NIDs; 539 `(library, name)` pairs are registered.

### 2.1 By group

| Group | Fns | Sites | Implemented | Constant | UNVERIFIED | Missing |
| --- | ---: | ---: | ---: | ---: | ---: | --- |
| Kernel: threads | 13 | 45 | 13 | | | |
| Kernel: semaphores | 5 | 52 | 4 | | | CancelSema (2 sites) |
| Kernel: event flags | 5 | 10 | 5 | | | |
| Kernel: callbacks | 3 | 23 | 3 | | | |
| Kernel: time | 5 | 93 | 5 | | | |
| Kernel: interrupts | 2 | 9 | 2 | | | |
| Kernel: memory | 12 | 15 | 10 | 2 | | |
| Kernel: cache | 6 | 28 | 1 | 5 | | |
| Kernel: modules | 7 | 10 | 5 | 2 | | |
| Kernel: exit | 2 | 2 | 1 | 1 | | |
| Kernel: stdio/debug | 5 | 7 | 1 | 3 | | GetGPI (NID unknown) |
| Kernel: libc helpers | 2 | 20 | 2 | | | |
| I/O | 23 | 86 | 22 | | | Chstat (2) |
| Display | 6 | 12 | 6 | | | |
| GE | 11 | 26 | 7 | 4 | | |
| Controller | 3 | 5 | 3 | | | |
| sceAudio | 13 | 17 | 11 | 1 | | Output2GetRestSample (NID unknown) |
| sceSas | 27 | 21 | 23 | 4 | | |
| sceAtrac | 10 | 13 | 10 | | | |
| sceMpeg | 25 | 31 | 24 | | | GetPcmAu (NID unknown) |
| Utility: savedata | 4 | 31 | 4 | | | |
| Utility: message dialog | 5 | 5 | 4 | | | Abort (0 sites) |
| Utility: OSK | 4 | 8 | 4 | | | |
| Utility: screenshot | 4 | 4 | | | | all 4 |
| Utility: modules | 2 | 8 | | 2 | | |
| Utility: system params | 4 | 4 | 3 | | 1 | |
| Font (sceLibFont) | 10 | 14 | 7 | | | FindFont, SetAltCharacterCode (NIDs unknown), GetFontList |
| UMD | 6 | 15 | 5 | 1 | | |
| Power | 8 | 28 | 3 | 5 | | |
| DRM | 2 | 6 | | | 2 | |
| Net / ad hoc | 48 | 67 | 35 | | 1 | Matching x10, ctlGetPeerInfo, GetPdpStat |
| Other: RTC | 7 | 66 | 3 | | 4 | |
| Other: scesupPreAcc | 8 | 9 | | | | all 8 (disc PRX) |
| **Total** | **297** | | **226** | **30** | **8** | **33** |

"Sites" counts direct call sites across all modules (survey 1.2); a function imported by both main and the PRX is
counted once.

### 2.2 Notes on the groups that block boot or are called most

- **Kernel and threads.** All thread, event-flag, callback, time and interrupt calls are implemented, including
  `sceKernelCpuSuspendIntr`, `DelayThreadCB`, `WaitThreadEndCB`, `VolatileMemLock` (returns `0x08400000`, 4 MiB,
  `host/hle/hle_sysmem.cpp:104`), `AllocMemoryBlock`/`GetMemoryBlockAddr` and the SDK-version setters. The scheduler is
  cooperative with a starvation preemption every 20,000 dispatches (`host/kernel/kernel.cpp:105`), which matches the
  survey's "one guest thread at a time" design. Missing: `sceKernelCancelSema` (2 sites), `sceKernelGetGPI`.
- **I/O.** Async open/read/lseek/ioctl with callbacks, devctl for `ms0:`/`fatms0:` callbacks, insertion and free space
  (`0x02415821`, `0x02415822`, `0x02425823`, `0x02425818` handled at `host/hle/hle_io.cpp:945`-`:1014`).
  `0x02425824` falls to the default. Missing: `sceIoChstat` (2 sites, main and PRX). Disc access is through the ISO.
- **Display, GE, controller.** All implemented. `sceGeEdramGetAddr` is a constant (VRAM base). Lists run
  synchronously when enqueued or when the stall address moves.
- **Audio.** sceAudio channels and Output2 implemented except `sceAudioOutput2GetRestSample` (its NID is not in the
  table). sceSas implemented natively; reverb calls accepted and dropped (`host/hle/hle_media.cpp:1084`).
- **ATRAC3plus library (sceAtrac3plus)**: all 10 imported functions implemented, streaming verified on Patapon
  (`docs/HLE_COVERAGE.md`).
- **sceMpeg**: 24 of 25 including the ring buffer with guest callback, `GetAvcAu`/`GetAtracAu`, `AvcDecode` with
  pixel modes, `AvcDecodeFlush`, `AtracDecode`. Missing `sceMpegGetPcmAu` (1 site).
- **Utility.** Savedata (all four, with PortableKit's own save screen), message dialog, OSK implemented.
  `sceUtilityLoadModule`/`UnloadModule` return 0 (modules are HLE). Screenshot is missing entirely (4 sites; I: the
  game's screenshot feature).
- **libfont.** `sceKernelLoadModule("libfont.prx")` is acknowledged without loading code
  (`host/hle/hle_system.cpp:103`). 7 of 10 font calls implemented, glyphs from host TrueType (5 below). `FindFont` and
  `SetAltCharacterCode` have no NID in the table; `GetFontList` has a NID but no handler.
- **UMD, power.** Implemented or constant; `scePower_469989ad` (the 333/222 MHz switch) answers 0.
- **scesupPreAcc.** `sceKernelLoadModule("libsuppreacc.prx")` is also acknowledged without loading
  (`host/hle/hle_system.cpp:123`): there is no PRX loader. The 8 imports main and extra call (NIDs `0x13AE25B3
  0x28C5F696 0x348BA3E2 0x86DEBD66 0xB03FF882 0x110E318B 0x2EC3F4D9 0xA0EAF444`) become logging stubs returning 0.
  Options: (a) recompile the 3,539-instruction PRX (`Elf32Image` relocates `0xFFA0` PRX images,
  `src/elf32.cpp:223`) at a fixed address, load it from `patch_loaded_image`, bind its own imports, and register HLE
  handlers for the 8 NIDs that jump to its exports; (b) reimplement the 8 functions in `register_extra_hle`. I: (a) is
  medium work and needs no knowledge of what the functions do; (b) needs them understood first.
- **Ad hoc.** PDP, PTP, adhocctl and discovery implemented (Yakumo's); `sceNetAdhocMatching` not at all. Stubbing it to
  failure is enough for single-player.

---

## 3. CPU coverage

V: `portablekit_coverage.json` (`cpu`). Census names are rabbitizer's; pseudo-ops are mapped to their base instruction
(`move` to `or`, `b`/`beqz` to `beq`, `negu` to `subu`, `vnop` to PortableKit's `Vflush` kind, `vuc2ifs` to `vuc2i`).

| Class | Opcodes | Instructions | Decoded | Interpreter | Recompiler |
| --- | ---: | ---: | --- | --- | --- |
| VFPU | 113 | 10,885 | 113 | 113 | 113 |
| FPU (incl. `c.cond.s`, `bc1*`) | 26 | 92,123 | 26 | 24 + `cfc1`/`ctc1` FCR31 only | same |
| Allegrex-specific (`ext ins seh seb wsbw bitrev clz min max madd msub rotr rotrv movz movn`) | 15 | 843 | 15 | 15 | 15 |
| Unaligned (`lwl lwr swl swr`) | 4 | 150 | 4 | 4 | 4 |
| `cache` | 1 | 5 | yes | no-op | no-op |
| `break` | 1 | 47 | yes | stops (no case) | stops ("guest break trap", `tools/codegen_main.cpp:661`) |
| Everything else (ALU, loads/stores, branches incl. 4,957 likely, jumps) | 59 | | all | all | all |

V: The game's `cfc1`/`ctc1` all name register 31 (`fpu_control_registers_used`), which both paths support
(`src/interpreter.cpp:430`, `tools/codegen_main.cpp:259`). The `break` words follow calls to fatal/assert paths
(survey 2.1), so a stop there is the right outcome.

**VFPU detail.** Every census opcode maps to a decoder kind (`src/decoder.cpp:207`-`:448`) and to a case in both
`execute_simple` (`src/interpreter.cpp:472`-`:831`) and `emit_regular` (`tools/codegen_main.cpp:330`-`:882`):
prefixes, `lv/sv.s/.q`, `mtv/mfv`, `bvt/bvf/bvtl/bvfl`, all arithmetic, `vdot/vscl/vhdp`, `vcmp/vcmov/vmin/vmax/vsgn`,
`vcrs.t/vcrsp.t`, `vmmul/vtfm/vhtfm/vmscl/vmidt/vmzero/vmmov`, `vrot`, `vcst`, `vfim/viim`, conversions
(`vf2in/vf2iz/vi2f/vh2f/vuc2i/vi2uc`), `vocp/vbfy1/vfad/vsat0`, `vrndf1/vrndf2` and `vnop`. Size limits that exist
(`vbfy1` refuses .s/.t, `src/interpreter.cpp:752`) do not affect the forms the game uses (`.p`, `.q`).

- **Unverified.** PortableKit's unverified list is the `Extra` set in `src/extra_instructions.cpp` (`lvl/lvr/svl/svr.q`,
  `vsbn vdet vwbn vrnds vsbz vlgb vi2c vi2us vsrt1-4 vbfy2 vmfvc vmtvc vt4444 vt5551`, `ll`, `sc`). **The game uses
  none of them.** Only `vcrs.t` is listed as "verified by a game" (MHP2G); for the rest, `docs/INSTRUCTION_COVERAGE.md`
  says they predate the page and which game ran which "is not recorded".
- **Precision.** I: `vsin/vcos/vasin/vexp2/vlog2/vrcp/vrsq/vsqrt` are computed with host `std::` functions in float
  (`src/interpreter.cpp:159`, `tools/codegen_main.cpp:796`), not the PSP's approximations. `vsqrt.s`
  and `vrcp.s` run in battle gameplay code (survey 2.2), so results can differ from a PSP in the last bits; this only
  matters for ad hoc play against real hardware and for replay determinism against PSP-made data.
- **PRNG.** `vrndf1/vrndf2` use PortableKit's own generator (`docs/INSTRUCTION_COVERAGE.md`: "not the console's");
  three main functions use them.

---

## 4. GE renderer coverage

V: `host/gpu/ge_state.cpp` (command decoder), `host/gpu/vulkan_renderer.cpp` (backend), `host/hle/hle_media.cpp`
(lists, transfers).

| Feature (survey 4) | PortableKit | Where |
| --- | --- | --- |
| Display lists, BASE/OFFSET/ORIGIN, JUMP, CALL/RET, END/FINISH/SIGNAL (incl. SIGNAL 0x10-0x12 list flow) | Yes. CALL pushes only the return pc, not BASE/offset | `ge_state.cpp:1351`-`:1470` |
| CALL into asset data (model renderer, GMO lists) | Yes: CALL takes any address, lists run from guest memory | `:1402` |
| Bounding box + conditional jump (`BJUMP`) | Box ignored, jump never taken (geometry always drawn) | `:1398` |
| Off-screen targets 64/128/256/512 wide | One Vulkan target per colour-buffer address, each the full 480x272 x scale extent; draws clipped to 480x272 (`scissor` clamp) | `vulkan_renderer.cpp:1062`, `:3729`, `:6557` |
| Framebuffer as texture | Yes, when the texture's buffer width equals the target's stride, a direct-colour format of the same pixel size, unswizzled, and guest memory under it unchanged | `:4538`-`:4574` |
| Block transfers / `sceGuCopyImage` of the 480x272 screen | Yes: a source inside a drawn target is read back from the GPU first (waits for the GPU), then copied row by row in guest memory | `ge_state.cpp:826`, `hle_media.cpp:141`, `vulkan_renderer.cpp:6703` |
| CLUT load and formats, CLUT4/8/16/32, DXT1/3/5, swizzle | Yes | `ge_state.cpp:853`, `texture_decode.cpp` |
| Depth test, depth write, depth range, clear mode | Yes; one depth image per colour target | `ge_state.cpp:632`-`:636`, `vulkan_renderer.cpp:3736` |
| Depth buffer address (`ZBUFPTR`), texture-from-depth, depth/colour aliasing | Parsed, not used: depth follows the colour target, not its address | `ge_state.cpp:659` |
| Skinning (8 bones), weights | Yes, on the CPU at decode | `ge_state.cpp:254`-`:330` |
| Morph targets / morph weights (`0x2C-0x33`) | No: only the first morph target is decoded, weights ignored | `:399`, `:431` |
| Bezier, spline patches | Yes | `:1204` |
| Lighting (4 lights), material, fog, texgen matrix | Yes | `:728`-`:810` |
| Alpha test, blending, fixed blend colours | Yes | `ge.frag:62`-`:80` |
| Texture functions | Modulate, decal; "blend" mixes by the texel instead of the env colour (`0xCA` not parsed); "add" falls to replace | `ge.frag:46`-`:55` |
| Mipmaps (`0xA1`-`0xA7`, `0xA9`-`0xAF`, `0xB9`-`0xBF`) | Not parsed; level 0 only. The game builds `0xA1`/`0xA9`/`0xB9` in 2 places each (`memory_scan.json`) | |
| Stencil test and op (`0x24`, `0xDC`, `0xDD`), logic op, colour test, per-bit masks (`0xE8`/`0xE9`), dither | Not parsed (fall to `trace_unhandled`, `ge_state.cpp:892`); stencil attachment is "don't care" | `vulkan_renderer.cpp:2343` |
| Readback of the displayed frame for CPU use | Every frame at 480x272 into guest VRAM (`write_back_frame`) | `vulkan_renderer.hpp:131` |
| Movie frames written by the CPU | `upload_frame` / `show_cpu_picture_in_vram` | `vulkan_renderer.hpp:178` |

Unknown commands are counted and reported at exit (`GeState::report_ignored_commands`, `host/main.cpp:433`), and
`<PREFIX>_TRACE_GE_LIST`, `_TRACE_FB_TEXTURES` show lists and render-target use. Since libGu sends most state through
generic setters, the survey could not list every command the game sends; the first runs will.

**Internal resolution.** V: every target is `480 x scale` by `272 x scale` (`vulkan_renderer.cpp:2108`); the menu offers
up to 6x (`:70`); scale 0 follows the window. Through-mode 2D is scaled with it. Under the "Fill" aspect the target takes
the window's shape and the game must widen its own view through `view_aspect_frame`. I: A 64-wide target is a 64x36
corner of a full-size target, so downsample chains keep their ratios; a target or copy wider or taller than 480x272 is
cut off.

**Frame interpolation.** V: `host/gpu/frame_interpolation.hpp`, `frame_pacing.hpp`. The game keeps its own frame rate
(`frame_vblanks`, default 2). Each frame's draws are recorded; an in-between present replays the older frame's draws
with world/view/projection blended towards the matching draw of the newer frame. Draws match on vertex address, index
address, vertex type, texture address, count, primitive and target. Only perspective draws into the displayed
framebuffer are blended; 2D, orthographic, clears and render-to-texture passes are shown as drawn. Skinned draws blend
their CPU-skinned vertices. Cuts are detected from the fraction of matched draws and the camera's turn and travel,
with distances in MHP3rd's world units (`CutThresholds`, overridable per profile). Presents run on a fixed grid a short
delay behind the frames, so the shown picture lags the newest frame by up to one game frame plus the frame's run time.
I: Dissidia's effects drawn into off-screen targets will update at 30 Hz while geometry moves at 60+.

---

## 5. Media

| Format | PortableKit | Backend |
| --- | --- | --- |
| ATRAC3 (6,110 voice/SE streams, 2,577 mono) | `sceAtrac3plus` HLE, WAVE parsing, streaming | FFmpeg `atrac3` (`host/audio/atrac_decoder.cpp:54`) |
| ATRAC3plus (BGM, 197 tracks) | Same | FFmpeg `atrac3p` |
| PSMF/H.264 movies through low-level sceMpeg | Ring buffer, PSMF demux (`host/movie/psmf_demuxer.cpp`), AU extraction, `AvcDecode` to pixels | FFmpeg `h264` (`host/movie/avc_decoder.cpp:64`) |
| Movie audio (ATRAC3plus) | `sceMpegAtracDecode` | FFmpeg `atrac3p` |
| sceSas PS-ADPCM (SSCF banks) | Native: VAG ADPCM, loops, pitch, ADSR (simple and full), noise, PCM voices, 32 voices | `host/audio/sas_core.cpp`. **No reverb** |
| PGF fonts | Not read. Glyphs are rasterised from a host TrueType/OpenType font with stb_truetype, metrics tuned to how MHP3rd draws them | `host/fonts/game_font.hpp` |
| GIM, GMO, PACKAGE.BIN formats | Game code; nothing needed from the runtime | |

V: FFmpeg is LGPL, linked dynamically, built with only `atrac3,atrac3p,h264` (`cmake/FFmpeg.cmake`, macOS/Linux from
source with `make`; Windows from a prebuilt BtbN LGPL zip downloaded at configure time). `-DPORTABLEKIT_FFMPEG=OFF`
builds without music and movies. Native code covers sceSas, PSMF headers and demuxing, and the sceMpeg/sceAtrac
bookkeeping.

---

## 6. Function discovery

V: PortableKit does not need the inventory's function list, and does not use one in `--auto` mode: every word of every
executable section is emitted and registered (1.1). The ~1,229 likely-missing functions (vtable targets in unknown
bytes) are therefore covered, as are jump-table targets and the 80 main-to-overlay targets that are not inventoried
entries. The manual CSV mode exists but is worse for this game. `tools/ghidra/ExportPSPFunctions.java` exports a
Ghidra function list for that mode only.

I: What the inventory still buys: function boundaries for native overrides, for reading profiles' hooks, and for the
decomp. Not for the port's coverage.

Unit counts at the default 16 KiB span (V, from section sizes): main 113 units (460,471 words), menu 95 (388,966),
battle 98 (399,020), extra 2 (6,078): **308 units, 1.25 million emitted words**. Main fits the 512-unit fast table
(`include/psprecomp/corpus_abi.hpp:228`).

---

## 7. Build requirements and this machine

V: `README.md`, `docs/BUILDING.md`, `CMakeLists.txt`, `cmake/PortableKit.cmake:303`-`:310`, `cmake/FFmpeg.cmake`. Probed
on this machine on 18 October 2026, nothing installed.

| Need | For | On this machine |
| --- | --- | --- |
| C++20 compiler: MSVC 2022 (CI: 19.44), GCC 13, Apple clang; the desktop app's Windows build used llvm-mingw | everything | VS 2022 Community with MSVC 14.29, 14.44 (= CI's 19.44) and 14.16; Windows SDK 10.0.22621/26100. MSYS2 GCC 14.2 (ucrt64). WSL Ubuntu 24.04 with GCC 13.3. No clang |
| CMake >= 3.20 | everything | CMake 3.31.6 bundled with VS (not on PATH). None in WSL |
| Ninja (needed for the generated-code job pool) | ports | Ninja 1.12.1 bundled with VS. None in WSL |
| Python 3 | build scripts | 3.12 on Windows, `python3` in WSL |
| SDL3 | window, input, audio output | **Not found** (no vcpkg, no MSYS2 package, not in WSL) |
| Vulkan loader + headers | renderer | Loader DLL present (driver); **no Vulkan SDK / headers** (`VULKAN_SDK` unset) |
| `glslangValidator` | shaders | **Not found** |
| FFmpeg | music, movies | Windows: downloaded at configure (BtbN LGPL 7.1.5). WSL has `libavcodec` dev files for `-DPORTABLEKIT_FFMPEG=system` |
| `make` + C compiler | building bundled FFmpeg on Linux/macOS | WSL `make`; MSYS2 `mingw32-make` |
| ccache | saves hours on rebuilds | **Not found** |
| Git | | Present |

Without SDL3, Vulkan or glslang the build produces a headless port (`docs/BUILDING.md`), which is enough for the first
interpreter runs (`<PREFIX>_NO_RENDER=1 <PREFIX>_NO_AUDIO=1`). The framework alone needs only compiler, CMake, Ninja
and Python.

Hardware: i7-12700K (20 threads), 31.8 GiB RAM (WSL sees 15 GiB). The generated-code pool allows one compile per
4 GiB (`CMakeLists.txt:36`-`:50`), so 7 at once here.

**Compile time.** V: PortableKit's only measurement is an Apple M1 with 8 GB: generating a 2.6 MB executable (153
units) took about 2.5 minutes and compiling it "hours" with 2 jobs; each overlay "seconds to a minute"; ccache makes
an unchanged rebuild take seconds (`docs/BUILDING.md`). Yakumo reports about 40 minutes for its 355 overlays the first
time. `PSPRECOMP_GENERATED_OPT_LEVEL=0` compiles much faster and runs much slower. I: Dissidia is about 2x MHP3rd's
corpus (308 units against 153) with 3.5x the parallel jobs, so the first full compile should take on the order of one
to three hours here, MSVC at `/O2 /Ob0 /bigobj` (`cmake/PortableKit.cmake:280`). Each unit costs over 1 GB of compiler
memory (`CMakeLists.txt:76`). Not measured.

---

## 8. Gaps by bring-up stage

Ranked by how early they stop the game. Size: S = hours to a day, M = days, L = a week or more (estimates).

### Stage 1: boot to title (interpreter, no corpus)

| # | Gap | Size | Notes |
| --- | --- | --- | --- |
| 1 | `e_entry = 0`: PortableKit starts at address 0 | S | Read `module_start` (`0x08804040`) when `e_entry` is 0, upstream; or ship an entry-fixed `EBOOT.ELF` |
| 2 | Constants-only profile, `GAME_DIR` with `EBOOT.ELF`, the retail ISO as `disc.iso`, `ms0/` | S | Section 1.7 |
| 3 | `scesupPreAcc` (8 imports, disc PRX not loaded) answering 0 | S to M | First see whether 0 is accepted; otherwise recompile and link the PRX (option a, 2.2) |
| 4 | First-run HLE gaps on the boot path: `sceKernelGetGPI`, `sceIoChstat`, `0x02425824` devctl, `sceKernelCancelSema`; and the semantics of what is there but untraced for this game (callbacks in CB waits, async I/O, UMD/MS callbacks, power callbacks) | S each | The interpreter run names them in order |
| 5 | Memory budget: 256 KiB loader stack (`host/kernel/kernel.cpp:23`) + 3.86 MiB `EFFECT_ALLOCATOR` + `UserSbrk` heap in what is left above `0x09B70D00` | S | Raise `guest_ram_bytes` if an allocation fails |
| 6 | GE features of the title and loading screens: unknown until traced (stencil, env colour, mipmaps are candidates) | S to M | `report_ignored_commands` after the run |
| 7 | libfont: `sceFontFindFont`, `sceFontSetAltCharacterCode` (NIDs missing), `sceFontGetFontList`; glyph metrics tuned to MHP3rd | S to M | Only if the title path uses libfont (not determined) |

### Stage 2: menus (menu overlay)

| # | Gap | Size | Notes |
| --- | --- | --- | --- |
| 1 | Overlay loads and runs interpreted (no corpus) | none | Works as is; slow |
| 2 | Savedata through PortableKit's own save screen; mapping of Dissidia's save folders | S | `save_folders` |
| 3 | OSK, message dialog present; screenshot utility missing (4 imports) | S | Stub to "cancelled" or implement |
| 4 | Render targets for menu effects (64-512 wide), framebuffer-as-texture with matching strides, CLUT8 GIM textures | M if any assumption breaks | Wider-than-480 targets are cut off; stride must match exactly |
| 5 | Texture function "blend"/"add" approximations, stencil, colour mask | M | Visible as wrong UI blending if used |
| 6 | `sceRtc` (4 UNVERIFIED), system params, `scePower_469989ad` | S | |

### Stage 3: battle (battle overlay)

| # | Gap | Size | Notes |
| --- | --- | --- | --- |
| 1 | VFPU semantics in gameplay (inline normalize 148 times: `vdot.t`, `vsqrt.s`, `vrcp.s`, `vcmovt.s`, prefixes) | S to M | All implemented; correctness for this game unverified; precision differs from hardware |
| 2 | `sceGuCopyImage` of the 480x272 screen (`battle:0x08ACD138`): GPU readback then CPU copy, every time | S to M | Works but stalls the GPU and drops the copy to 480x272; may need a GPU-side copy for speed and resolution |
| 3 | Model renderer display lists: skinning OK; morph weights, mipmaps, BJUMP culling missing | M | Morph only if GMO data uses it (not determined) |
| 4 | Depth by colour target, not by address; `0x08866AE0` switches depth between `0x110000` and `0x88000`, and `0x88000` is a colour buffer in the other mode | M to L | Matters only if the game textures from depth or relies on depth contents across targets |
| 5 | Stencil (shadows, masks) if used | M | |
| 6 | Speed under the interpreter: battle at 1/20 speed may break timing-sensitive code before a corpus exists | none (method) | Move to the recompiled build |

### Stage 4: movies and audio

| # | Gap | Size | Notes |
| --- | --- | --- | --- |
| 1 | FFmpeg present at build (Windows download, or `system` in WSL) | S | |
| 2 | `sceMpegGetPcmAu` missing (1 site) | S | |
| 3 | `sceAudioOutput2GetRestSample` missing NID (1 site, audio mixer path) | S | Add NID and handler |
| 4 | sceSas reverb dropped | M | Audible, not blocking |
| 5 | Mono ATRAC3 voice streams (2,577, block 192) through `GetSoundSample`/streaming | S to M | Streaming verified on Patapon, mono not traced |

### Stage 5: recompiled build (after stage 3 under the interpreter)

| # | Gap | Size | Notes |
| --- | --- | --- | --- |
| 1 | Toolchain: SDL3, Vulkan SDK, glslang, ccache missing here | S | Installs; owner's decision |
| 2 | Overlay `meta.txt` writer for `0xFFA8` ELFs (no `MWo3`), choice of `code_size` that excludes written data | S | 1.5 |
| 3 | First compile of 308 units, hours | M (machine time) | ccache afterwards |
| 4 | `PSPRECOMP_NO_INTERPRETER=1` clean run: anything still interpreted is the PRX or an unlisted overlay image | S | |
| 5 | Native overrides only through `register_function` at entries; same-unit `goto`s bypass them | M per feature | For camera, input, new character |

Small overall: the entry point, the overlay metadata tool, the missing HLE NIDs and handlers, the profile itself.
Large: whatever the GE turns out to need beyond PortableKit's MHP3rd-shaped renderer (render targets larger than the
screen, depth aliasing, stencil, morph), and the presentation work (interpolation tuned to Dissidia, effects in
off-screen targets, wide screens).

---

## 9. Not determined

- **Whether the game boots past `scesupPreAcc` returning 0**, and what the 8 functions do (names unknown; I: data
  install / pre-access support).
- **Which GE commands the game sends** beyond those built as constants: stencil, logic op, colour test, env colour,
  mipmap levels, morph weights, BJUMP. Needs a traced run.
- **Whether render targets or copies exceed 480x272**, and whether any texture reads a target with a different stride
  or format than it was drawn with (both fall back to stale guest memory in PortableKit).
- **Whether the game textures from depth or aliases depth and colour buffers.**
- **Whether GMO models use morph targets.**
- **Whether the CPU reads VRAM** beyond the displayed frame PortableKit writes back.
- **Whether the title and menus use libfont** (and so hit the missing font calls and MHP3rd-tuned glyph metrics).
- **The `UserSbrk` heap size** and so whether 32 MiB of guest RAM is enough with PortableKit's 256 KiB loader stack.
- **Frame interval per mode** (battle 2 vblanks; menus and movies not measured), against `frame_vblanks` being a single
  value.
- **The `code_size` to hash for each overlay**: the first offset the game writes into each overlay's section is a
  run-time question; the first 4,096 bytes are distinct between overlays, but whether they are never written is not
  proven.
- **Whether `register_function` overrides are reached** for each function a feature wants to replace, given same-unit
  `goto`s.
- **Actual compile time and memory** on this machine (nothing was built), and interpreter speed for this game.
- **Behaviour of every "implemented" HLE call for this game's arguments.** The table records that code exists; only
  runs show whether it answers what Dissidia expects.
- **PortableKit's own test status at `33800c4`**: CI is described as passing; not checked.
- **Submodules**: none are used (`third_party/` is in-tree); nothing was missing from the clone for reading.

## Reproduce

```powershell
.\.venv\Scripts\python.exe tools/port/portablekit_coverage.py tools/local/port/PortableKit
```

Needs `imports.json` and `instruction_census.json` from the survey scripts, the four ELFs in `build/ULUS10566/`, and
`tools/inspect_imports.py`.

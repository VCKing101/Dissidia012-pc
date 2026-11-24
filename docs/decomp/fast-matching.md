# Fast matching workflow

Added 12 November 2026. Three changes make function matching fast enough to work at scale.

## 1. Compiles run from WSL's own filesystem

SNC runs through wibo in WSL. Started from the Windows drive mount, each compile took 1.5 to 4.5 seconds, almost all of it file I/O; the compiler itself needs about 16 ms. `tools/compile_psp.py` now sends the sources and headers to WSL as one tar, and `tools/snc_batch_guest.py` compiles every job in parallel from a copy of the compiler and wibo under `/tmp/d012-compilers` (refreshed when the Windows copy changes). The build scripts compile all their units in one batch.

| Step | Before | After |
| --- | ---: | ---: |
| Battle overlay full build | 8 min 27 s | 19 s |
| Menu / extra full build | minutes | 17 s / 9 s |
| Checking 13 sources | about 20 s | 1.9 s |

Set `D012_SLOW_COMPILE=1` to use the original one-process-per-file path.

## 2. `tools/dc.py`: one loop for any module

```powershell
.\.venv\Scripts\python.exe tools/dc.py candidates battle --max 64      # unmatched functions, smallest first
.\.venv\Scripts\python.exe tools/dc.py context battle 0x089DF148       # retail assembly, Ghidra pseudocode, callers
.\.venv\Scripts\python.exe tools/dc.py check battle src/battle/x.c ... # compile and compare, side-by-side diff
.\.venv\Scripts\python.exe tools/dc.py integrate battle src/battle/x.c # add exact units to the build config
```

`check` compares every function named `func_XXXXXXXX` with retail bytes at that address, resolving relocations to real addresses; nothing is masked. Names `func_`, `D_` and `jtbl_` followed by eight hex digits resolve to that address, so a source needs no configuration until it is integrated. `integrate` refuses anything that is not an exact, contiguous, in-order match, adds the referenced symbols and a feature assignment, and leaves the full module build as the final proof.

Only `.text` and switch jump tables (section 4) are linked, so matched sources cannot use string literals or their own static data; globals are `extern D_XXXXXXXX` declarations.

## 3. `tools/dc_auto.py`: pseudocode to exact C in bulk

For every unmatched function within a size range, the tool rewrites the provisional Ghidra pseudocode into C (size types become C types, `FUN_`/`DAT_` become address names, callees get prototypes from their own pseudocode), compiles all of them in one batch, and keeps the exact matches. Failed functions are retried with unsigned byte and word types, unprototyped callees, and with Ghidra's `v = a; else v = b; return v;` rewritten as two returns: SNC's `-g` line information makes that shape matter.

```powershell
.\.venv\Scripts\python.exe tools/dc_auto.py battle --max 256 --write --report build/dc-results/auto-battle.json
```

Matches are written to `src/<module>/auto/<module>_<address>.c` with a header saying where they came from. They are exact but keep Ghidra's placeholder names (`param_1`, `uVar1`) and raw offsets; renaming and typing them is a separate readability pass that `dc.py check` keeps honest. Nothing is integrated automatically.

## Working with agents

Hand each agent a contiguous address range listed in `build/dc-claims/<module>-<agent>.txt`; neighbouring functions share structures. `dc.py candidates` and `dc_auto.py` skip claimed addresses. Agents write sources and a results file in `build/dc-results/`, and never edit `config/` or run module builds; integration and the full builds happen in one place afterwards.

## 4. Switch jump tables

Added 12 November 2026. A `switch` that SNC compiles to a jump table now matches, integrates and links like any other function.

**What SNC emits.** Each table goes in the object's `.rodata` (8-byte aligned, zero padding between tables and at the end) under a local symbol `$LSW_n`. The function reaches it with `lui`/`lw` HI16/LO16 relocations against `$LSW_n`; every entry is an `R_MIPS_32` against the object's `.text` section holding the case's offset.

**Where retail keeps them.** Survey of every inventoried function, 12 November 2026:

| Module | Functions with tables | Table loads | Where the tables are |
| --- | ---: | ---: | --- |
| battle | 77 | 80 | `defined_data_ranges` right after their object's code (median 0x908 bytes after the function) |
| menu | 46 | 57 | `defined_data_ranges` after their object's code (median 0x26AC bytes) |
| main | 62 | 69 | the read-only data section `0x089C6970`-`0x089D2800`, after all code |
| extra | 1 | 1 | a defined data range after the code |

Main also has 16 functions whose only register jumps are tail calls through function pointers; those have no table. Tables of neighbouring functions sit together in the original object's order, so a unit with several switch functions keeps its tables in one block.

**How the tools handle them** (`tools/c_rodata.py`):
- `dc.py check` reads the retail `lui`/`lw` pair at the same instructions as the compiled HI16/LO16 relocations, derives where the unit's whole `.rodata` starts, resolves the function with that address and compares the table bytes too, entries resolved against the unit's real `.text` address. It prints `rodata ($LSW_1, ...): N bytes at 0x...; MATCH`. If the compiled pair does not line up with a retail instruction it says `NOT PLACED` and the `lui`/`lw` words show in the diff with address 0.
- `dc.py integrate` records the placement in the unit: `"rodata": {"address": "0x08A65660", "size": 120}`. It refuses a table that overlaps inventory instructions or another unit's table.
- `prepare_battle`, `prepare_menu` and `prepare_main` place `{object}(.rodata)` at that address between `ASSERT`s, splitting the original-byte range around it, and recheck the table bytes against retail. The verifiers check the raw object's table again, the parts (`matching-c-rodata`) and the byte accounting.
- **No code credit.** Table bytes are data. They never enter `recovered_c_bytes` or the matched-C percentages; the build reports list them separately as `c_rodata_bytes`, and `progress.py` mentions them as data.

**Limits.** The unit's whole `.rodata` is placed as one block, so it must sit contiguously in retail exactly as SNC lays it out. String literals (`$LNst.n`, also in `.rodata`) go through the same check but are untested on real functions; float constants are built inline and need nothing. Everything else fails loudly: `.data`/`.bss`, global symbols in `.rodata`, code reaching `.rodata` through its section symbol, other relocation types or entries pointing outside `.text`, and the extra overlay, whose splat-based build cannot place `.rodata` yet. SNC pads the section to 8 bytes; retail has zeros there too in every case tried. A switch inside a C++ in-class inline helper would also put a table for the discarded copy in `.rodata`; its entries point into the discarded section, so that fails loudly too. `dc_auto.py` still rejects every object with a data section, so jump-table functions are matched by hand for now.

**Proved on:** battle `func_08A64FB0`, `func_08A65000`, `func_08A65054` (one unit, three tables, 252 code bytes, 120 table bytes) and `func_08A9C40C` (56 / 184), main `func_089237E8` (68 / 48) and menu `func_08AC76A4` (92 / 32). All three module builds stay byte-identical.

## 5. Build profiles: Sony GCC for main's SDK libraries

Added 14 November 2026. Main `0x089985B0`–`0x089C2730` (805 functions) is Sony SDK library code, built with **psp-gcc 1.3.1 (GCC 3.3.3, Allegrex 2.2.1) `-O2 -G0`** rather than the game's SNC. The evidence: every register move in the range is GCC's `addu rd, rs, $zero`, never SNC's `or`; the first function at `0x089985B0` starts the `addu` run and the first `or` comes at `0x089C2730`. GCC also reproduces functions there that SNC cannot (an `addu` move scheduled before an asm block, a `nop` the assembler puts in the `jr ra` slot, float arguments spilled to `4($sp)`).

**Configuration.** `config/main.matches.json` has two profiles with the same compiler, `tools/compilers/psp-gcc-1.3.1/bin/psp-gcc`:
- `sdk_gcc`: `-O2 -G0`, the default for the whole range;
- `sdk_gcc_o3`: `-O3 -G0`, for the SHA-1 and graphics (sceGu-like) libraries at `0x0899F030`–`0x089A2C24` and the string library at `0x089AB038`–`0x089AB388`.

A unit built either way carries `"profile": "<name>"`; units without one use the module's pinned SNC compiler. Progress counts these units under the feature `sdk_libraries` (SDK support libraries).

The `-O3` span: 37 graphics and SHA-1 sources that differ under `-O2` match byte for byte under `-O3`, and none of the graphics functions matches under `-O2`. Register renaming at `-O3` gives their `t`-register-heavy leaf code. The two small units already integrated in the span match under both settings. The span ends where the double-precision maths functions start (`0x089A2C24`); the last `-O3` evidence is at `0x089A20CC`. On 22 November 2026 the span was widened to start at `0x0899F030`, where SHA-1 really starts (its update function matches only under `-O3`), and a second span was added for the string library `0x089AB038`–`0x089AB388`: six of its functions differ under `-O2` (register renaming) and match under `-O3`, and the seventh, a 32-byte memory fill, matches under both.

Other flag findings (not configured yet): libgcc's 64-bit divide and modulo functions (`__divdi3`, `__udivdi3`, `__umoddi3`, 4.3 KB) come within a register swap with `-fexceptions -fnon-call-exceptions`, as GCC's libgcc makefile builds them. GCC's Allegrex builtins (`__builtin_allegrex_clz`, `_ctz`, `_bitrev`, `_min`, …) are compiler intrinsics, not inline asm, and three matched image-library sources use them. Two SNC functions sit inside the GCC range: `0x0899B654` and `0x0899B698`.

**How the tools handle it.**
- `tools/compile_psp.py` compiles GCC units in the same WSL batches as SNC (`tools/snc_batch_guest.py`, job kind `gcc`). The toolchain is copied once to `/tmp/d012-compilers/psp-gcc-1.3.1`; the driver writes assembly with `-S -nostdinc -Iinclude` and that toolchain's own `psp-as -EL -G0` assembles it. The first batch after a change to the toolchain spends about 30 s on the copy. A GCC call without `-G0` is a compiler probe (`tools/probe_compilers.py`) and keeps the original probe script.
- `dc.py check`, `integrate` and `dc_auto.py` choose the profile by address: the address at the end of the file name (`gu_matrix_mode_089a2678.c`), or else the `func_` names in the file when they all fall in one profile's range. Result lines end in `(sdk_gcc)`. `--profile NAME` or `--profile default` overrides the choice for `check` and `integrate`. `integrate` refuses a unit whose addresses belong to another profile than the one it was compiled with, and records the profile on the unit.
- `prepare_main.py` compiles each unit with its profile (one batch per profile) and links GCC and SNC objects together. Both Sony assemblers write machine `0xA2`, so the existing machine-identifier patch applies to both. The prepare report lists `build_profiles` (units per profile), and `tools/main_build_provenance.py` fingerprints the GCC driver, `cc1`, `specs` and `psp-as`.
- **Old GNU as symbol tables.** psp-gcc 1.3.1's `psp-as` counts its last local symbol (the `FILE` symbol) as global in `.symtab`'s `sh_info`, and modern `ld` refuses the object ("local symbol at index N (>= sh_info of N)"). `compile_psp.repair_local_symbol_count` corrects that one header field for profile units, and only when every local symbol already precedes every global one.
- **Jumps to labels in the same unit.** GCC writes unconditional jumps as `j label`, which the assembler records as `R_MIPS_26` against the unit's own `.text` section (SNC does the same for `j 1f` in inline asm). The linker always placed these correctly; the per-unit byte checks in `dc.py`, `dc_auto.py`, `prepare_*` and `verify_*` now resolve `<section .text>` to the unit's start address too.

**Not all of the range is psp-gcc 1.3.1.** The whole range uses GCC's `addu` moves, but 197 of its 805 functions (73,612 bytes) contain instructions 1.3.1 never produces:
- sibling calls (`j func_…` leaving the function), which the MIPS port gained in GCC 3.4;
- Allegrex `ins`/`ext`, for which 1.3.1 uses `and`/`or` masks even with bitfields.

The agent that matched 0x089AC568–0x089C26E4 also found GCC 3.4-style block reordering in many functions without either marker: duplicated epilogues, loop bodies after the epilogue, and rotated loop tests. It also found constant multiplies done as shift and add. psp-gcc 1.7.1 does none of this either, and homebrew pspdev (GCC 15.2) is far too new. These libraries were evidently built with a later Sony psp-gcc (3.4 or 4.x based). decomp.me's compiler archive has only 1.3.1 and 1.7.1. Until a matching compiler turns up, those functions can't be matched; markers by 16 KB block are listed in `build/scratch/gcc/markers.json`.

**Limits.** Float constants that GCC places in `.rodata.cst4`, string literals, and calls to libgcc helpers by name (`__extendsfdf2`, …) are not linkable yet; declare retail constants as `extern const float D_XXXXXXXX;` instead. The 92 VFPU functions in the range are inline asm; 14 of them (the vector library at `0x089A38A8`–`0x089A47CC`, `src/main/sdk/libvfpu_*.c`) are integrated and counted separately (section 6). Main `0x0889D934`–`0x0889E2FC` (11 functions) is hand-written assembly and stays assembly.

**SNC units in the range.** 20 tiny functions there were matched under SNC before the profile existed. 15 give the same bytes under GCC and moved to `sdk_gcc` unchanged. Three more (`func_0899C348`, `func_089B4908`, `func_089B493C`) had their stores in an odd order that SNC needed; with the stores in natural field order GCC gives retail, so they were rewritten that way and moved too. Two 12-byte getters (`func_0899B648`, `func_089A04E8`: `lui $a0; jr ra; lw $v0`) still match only under SNC: every C shape tried makes GCC use `$v1` for the high half, and they are the only two such getters in the range. They stay SNC units until a GCC form is found.

## Results, 12 November 2026

One evening with these tools took verified matching C from **278 functions / 20,944 bytes** to **5,892 functions / 337,504 bytes** (0.44% to 7.15% of inventoried function bytes). All four modules still rebuild byte-identical and all 191 tests pass.

| Source | Functions | Bytes |
| --- | ---: | ---: |
| `dc_auto.py`, functions up to 256 bytes | 5,058 | 263,308 |
| `dc_auto.py`, 260 to 1,024 bytes | 20 | 7,836 |
| Agents, first round (contiguous ranges in battle and main) | 239 | 20,388 |
| Agents, second round (`dc_auto` near misses in menu and main) | 305 | 20,076 |

Each agent matched about 120 to 150 functions in 30 to 50 minutes, at roughly 400,000 tokens. Near misses (2 to 52 differing words) stay as sources outside the build, listed in `build/dc-results/agent*.md`.

`dc_auto` stops paying off above about 256 bytes. What remains is about 4.4 MB: 10,582 functions up to 256 bytes that the automatic pass rejected or missed, 4,184 larger plain functions, 364 VFPU functions, 175 with jump tables or register tail calls (tables are now supported; see section 4) and 90 split across address ranges.

## Overnight run, 12–13 November 2026

Agents ran in waves until the morning (the 5-hour usage cap paused them twice). Verified totals went from 5,892 functions / 337,504 bytes to **9,867 functions / 693,728 bytes** (14.70% of inventoried function bytes); all four modules rebuild byte-identically and all tests pass.

| Module | Functions | Bytes |
| --- | ---: | ---: |
| Main executable | 3,700 | 220,964 |
| Menu overlay | 2,967 | 224,332 |
| Battle overlay | 3,176 | 246,316 |
| Extra overlay | 24 | 2,116 |

- Matching waves worked through `dc_auto` near misses, 160–300 functions per agent; Opus agents matched 70–100% of their lists (most 90%+), often by spotting a repeated shape and generating the sources with a small script (kept under `build/scratch/agent*/`). A Sonnet agent on a list of equal difficulty matched 145/160 against Opus's 160/160, using more tokens, so Opus was the better use of the allowance.
- Readability: every generated battle file, 1,400 of the 1,907 menu files (regions 1–3; region 4 was cut off) and 96 main files were rewritten with real structs and names, byte-identically, at 150–500 files per agent-hour. Region headers are `include/<module>/auto_region*.h`.
- Jump tables (section 4), stack spills and several other patterns were solved; see below.
- Near misses and their reasons stay listed in `build/dc-results/agent*.md` with their sources kept out of the build.

## Session of 14 November 2026

One more 5-hour window of agents, then integration. Verified totals went from 9,867 functions / 693,728 bytes to **10,716 functions / 796,520 bytes (16.88%)**; all four modules rebuild byte-identically and all 204 tests pass.

| Module | Functions | Bytes |
| --- | ---: | ---: |
| Main executable | 3,989 | 252,232 |
| Menu overlay | 3,177 | 249,456 |
| Battle overlay | 3,526 | 292,716 |
| Extra overlay | 24 | 2,116 |

- Matching: five agents added 828 functions from near-miss lists (two were cut off by the usage cap part way; their remaining addresses are still in `build/dc-claims/`).
- Readability: the menu overlay is now readable throughout (all 1,907 generated files), main has 856 of about 2,020. Main region 3 (400 rewrites in `build/readability/main/`) is not installed: its agent was cut off while writing `include/main/auto_region3.h`, which fails one of its own layout assertions.
- **Pointer-to-member dispatch** (`build/dc-results/pmf-dispatch.md`): SNC's PMF is 8 bytes `{short delta; short index; fn or vtable offset}`. Real C++ PMF syntax gives a different load order from retail, so 21 menu dispatchers are matched with the call lowered by hand in C (load delta, index and target in retail's order, adjust `this`, follow the vtable when the index is non-zero, call through a function-pointer variable). The in-place `this` adjust variant and one null-test delay slot are still open.
- **VFPU** (`build/dc-results/vfpu.md`): SNC has no vector arithmetic or intrinsics but compiles GCC-style inline asm with VFPU instructions and schedules C around it; game code used small helpers with fixed-register asm (one normalise block appears 193 times). 16 functions match this way. The 12 in game code are integrated and counted separately (section 6); the 4 SDK ones stay out because that range is GCC-built: the GCC versions cover three, and `0x089A37DC` still needs a GCC match.
- **Sony GCC region**: main `0x089985B0`–`0x089C2730` (805 functions, SDK libraries) was built with Sony psp-gcc, not SNC; it reproduced 14 of 14 SDK functions tried, 11 of them only under GCC. The per-unit build profiles that followed are in section 5. Main `0x0889D934`–`0x0889E2FC` (11 functions) is hand-written assembly and should stay assembly.

## Overnight run, 14–15 November 2026

Seven agents over two 5-hour windows, then integration. Verified totals went from 10,887 functions / 810,672 bytes (after the GCC profile work of the evening) to **11,665 functions / 972,932 bytes (20.62%)**; all four modules rebuild byte-identically and all 219 tests pass.

| Module | Functions | Bytes |
| --- | ---: | ---: |
| Main executable | 4,335 | 302,956 |
| Menu overlay | 3,442 | 299,372 |
| Battle overlay | 3,864 | 368,488 |
| Extra overlay | 24 | 2,116 |

- Lists: `build/scratch/overnight-chunks.json` splits every unmatched function of 24–400 bytes (no VFPU) into address-ordered lists of 250 (main 10, battle 7, menu 8). Done: battle 0 (agent28, 229 of 250), menu 0 (agent29, 190), main 0 (agent30, 175); part done: battle 1 (agent31, 109, stopped early to save usage, 117 untouched) and menu 1 (agent32, a Sonnet agent, 75, about 150 untouched). Their near misses and skips are listed in `build/dc-results/agent28.md`–`agent32.md`.
- An Opus agent matched 70–92% of a fresh list of 250 within the night. Three Opus agents at once used about 45% of a 5-hour window per hour, one about 15–19%; the Sonnet agent used about 7% per hour but stopped well before its deadline twice and attempted mostly small functions.
- The integrator refuses a file whose functions are not contiguous and in address order; nine such agent files (phase-class clones with a padding gap between them) were split one function per file and then integrated.
- GCC profiles: two agents on main's SDK region added 76 GCC matches (`build/dc-results/gcc1.md`, `gcc2.md`); main now has 189 functions / 14,448 bytes built by psp-gcc 1.3.1 (151 under `sdk_gcc`, 38 under `sdk_gcc_o3`). Much of the rest of the region is later Sony GCC output (sibling calls through `j`, Allegrex `ins`/`ext`, block reordering) that neither 1.3.1, 1.7.1 nor a modern GCC reproduces; a later Sony psp-gcc build would be needed.
- The reviewed menu controller gained handler 12 (`0x089DDF24`): 15/20 handlers, 2,916 of 8,188 bytes.
- Tooling gaps the agents hit: `__ashldi3` calls (64-bit shifts) are not resolved to main's libgcc copy at `0x089985B0`, and SNC's per-object 64-bit literal pools (`$Lcnst`) cannot be placed, so a few otherwise exact functions stay near misses.

## Overnight run, 21–22 November 2026

The night started with a map of the texture path (`docs/port/texture-pipeline.md`, `docs/port/texture_types.h`); the matching agents then took the texture-path functions first and the queued lists after. Five Opus agents over two 5-hour windows and a Sonnet agent at the end, then integration. Verified C/C++ totals went from 11,665 functions / 972,932 bytes to **12,065 functions / 1,059,036 bytes (22.45% of inventoried function bytes)**; the separate VFPU count went from 26 / 1,888 to 35 / 2,460. All four modules rebuild byte-identically and all 225 tests pass (7 skipped).

| Module | Functions | Bytes |
| --- | ---: | ---: |
| Main executable | 4,492 | 329,120 |
| Menu overlay | 3,548 | 326,392 |
| Battle overlay | 4,001 | 401,408 |
| Extra overlay | 24 | 2,116 |

- agent33, the 110 game-side functions on the texture path (main, SNC): 42 matches / 9,232 bytes, 40 near misses, 28 skips. agent37 took the 40 near misses again (9 more matches) and then 225 untried main functions (88 matches): 97 / 14,484 bytes in all.
- agent34, Sony's image, graphics and GMO libraries (main, psp-gcc 1.3.1): 27 units, 9 of them VFPU inline assembly (572 bytes, separate count). Seven are exact only under `-O3`, which widened the `sdk_gcc_o3` profile (section 5). Most of the GMO library is later Sony GCC output.
- agent35, the rest of battle list 1 (116): 81 matches / 23,168 bytes; 80 integrated. agent36, the rest of menu list 1 (154): 106 / 27,020 bytes.
- Five of agent31's battle near misses matched once agent35's motion-call prototype and colour constructor (below) were applied to them.
- Not integrated: `0x08A3788C` matches alone but shares its 8-byte `$Lcnst` pool entry (`0x08A38490`) with `0x08A37418`, which is already integrated; two units cannot place the same bytes, and reading the entry through an `extern` moves one store. Functions that came from one retail object and share a pool entry need to go into one unit together.
- agent38 (Sonnet), battle list 2 (250, `0x08A7EE74`–`0x08ACA77C`) from 06:55: 52 matches / 9,036 bytes, 25 near misses, 8 skips; it reached about 85 functions and stopped at 07:35. Its patterns: 0/1 functions as a C++ `bool` with `return true`, small range tests compiled to `slti` chains are a `switch`, `self->h = h = call();` keeps the result in `v0`, and a local name sweep settles float compare order.
- Usage: one Opus agent used about 18% of a 5-hour window per hour, two about 35%. The first window ran out at about 02:40 and both agents running then were resumed after the reset.

## Overnight run, 24–25 November 2026

The queued lists from the previous night, worked by Sonnet agents two or three at a time, with one Opus agent at the start. Each agent ran until its own context filled (20–80 minutes) and was replaced by a fresh one on what it had not reached. Verified C/C++ totals went from 12,065 functions / 1,059,036 bytes to **13,569 functions / 1,249,576 bytes (26.49% of inventoried function bytes)**; the separate VFPU count is unchanged at 35 / 2,460. All four modules rebuild byte-identically and all 225 tests pass (7 skipped).

| Module | Functions | Bytes | Added |
| --- | ---: | ---: | ---: |
| Main executable | 5,026 | 392,160 | 534 |
| Menu overlay | 4,101 | 396,840 | 553 |
| Battle overlay | 4,418 | 458,460 | 417 |
| Extra overlay | 24 | 2,116 | 0 |

- 27 agents (agent39–agent65) matched 1,445 integrated functions; the lead added 59 by hand. Results and each agent's notes are in `build/dc-results/agent39.md`–`agent65.md`.
- Sonnet against Opus: one Sonnet agent used about 10% of a 5-hour window per hour and matched 20–140 functions before its context filled; the Opus agent used about three times as much per hour and matched 67 in 75 minutes. Two Sonnet agents fit a 5-hour window; three need a pause before the reset.
- Lists matter more than models: agents given the leftovers of earlier agents matched about a quarter as many as agents given untouched ranges, so fresh ranges went first and leftovers last.
- Families gave the most matches per hour (see the patterns below). The lead's own fixes: the battle input-check family at `0x08B31640` (29 functions, 4,896 bytes, from one generated source shape that an agent had skipped after eight attempts), and 30 main functions (stat pairs, stat adds, two hook setters, two destructors; 3,528 bytes) that agents had left 2–7 words off, fixed by the delay-slot helper pattern.
- Not integrated: a battle source for `0x08ACD0BC`, already in a unit; two menu matches (`0x08B34478`, `0x08B51518`) whose files the agent deleted after listing them. Three main units were renamed to lower-case file names, which `dc.py integrate` requires.
- WSL stopped answering from about 03:01 to 03:50 (`Wsl/Service/CreateInstance/HCS_E_CONNECTION_TIMEOUT`), during a batch build of main while two agents were compiling; every compile goes through WSL, so the agents were stopped. It recovered without a restart. After that, builds ran with at most one agent compiling.

## SNC patterns learned while matching

Collected from both agent rounds (544 hand matches). Each was confirmed by a byte match.

**Control flow and blocks**
- `-g` line information makes source shape matter. Ghidra's `if (c) { v = a; } else { v = b; } return v;` loses; `if (c) { return a; } else { return b; }` matches, and so does writing the braces out rather than an early `return` on one line.
- Early-return blocks are laid out out of line, in reverse order, after the next unconditional branch; the last one before the main body stays inline. Try `if (x) return;` when retail has `b end` stubs.
- A then-block that declares its own local is placed after pending early returns. A local such as `short kind = p->f;` also stops reloads.
- Loop variables declared in an inner block (`{ int i; for (...) }`, the shape C++ `for (int i...)` gives) change prologue order, block layout and register order. The same applies to `int r = call();` inside a loop body.
- `if (!c) continue; ...; goto done;` places the found block after the loop exit with an explicit `b`.
- `c ? a : b` produces branch-likely with the false arm in the delay slot.

**Values and registers**
- `ori $r, $zero, 0` comes from a named local holding zero; a literal `return 0` gives `move $v0, $zero`.
- If retail loads a field once and reuses it, copy it into a local first.
- `(unsigned char)(x ? 1 : 0) == 1` keeps an `andi 0xFF`. A C++ `bool` with `return true` loads its own constant 1.
- `*out = local;` for a float struct produces `mfc1` + `sw`.
- Address arithmetic order follows the source: `*((u8 *)p + OFF + i)` adds base first, `p->arr[i]` may add the index first. `T *a = p->arr; &a[i]` adds the field offset before the multiply; a per-iteration `T *e = &p->arr[i]` reproduces pointer induction.
- Ghidra's `char *` struct bases compile differently from integer bases: `*(int *)(p + 4)` with `int p` matches where `char *p` adds the offset separately.

**Helpers, inlining and C++**
- An explicit `b` to the next block, or `ori` 0/1 results returned through a temporary, usually means an inlined helper. Write the helper as a static member function defined inside a C++ struct body: SNC puts that copy in `.gnu.linkonce.t`, which the build discards (`unused_inline_symbols`). A C `static __inline` or out-of-class `inline` helper also gets a standalone copy in `.text`, which `dc.py check` reports as NOT LINKABLE.
- Branch-likely (`beql`/`bnel`) in if/else code appears when the logic sits in such an in-class member.
- `lh delta / lw fn / addu / jalr` is a real C++ virtual call. Declare a class with placeholder virtuals: virtual number N is at vtable offset 8 × (N + 1). Emulating it in C gets the argument registers in the wrong order.

**More from the second round**
- `if (a == b) { return; } body;` as the first statement gives `bnel` with the body's first store in the delay slot; the same early return as the last statement gives `bnez body; b end`.
- In loops, writing the whole `self->p->arr[i]` chain (no cached `p`) gave retail's register order every time; SNC hoists the load itself. The same goes for passing `self->p` and `self->p->arr` straight into a call.
- A value used in both arms of an if/else but a few registers off: write the expression in each arm and let SNC hoist it.
- A value sitting in `t1` (or a later argument register) at a `jal` is a trailing argument Ghidra dropped. A lone `ori` constant among argument registers is a named local (`int one = 1;`).
- `sltiu` + `beq` against `$at` is a small `switch`, not an if/else chain.
- Struct returned by value: `addiu a0, sp, 8; jal f; ...; move a0, sp; jal g` is `T v = f(id); g(&v);`.
- A vptr after data (`lw a1, 0x544(a0)`): declare `char data[0x544];` before the virtual functions.
- Constructors and destructors: put the whole body in an in-class static helper and pass it the vtable address; 21 identical deleting destructors matched through one helper. Member subobject destructors are called with flags 0.
- Pass-through arguments: if retail never touches `a1`, `f12` or `t0`..`t3`, the function takes extra parameters it forwards untouched; declare and pass them.
- Flag test followed by a reload of the same word: `union { unsigned flags; struct { unsigned active:1; } bits; }`, test the bitfield and write the word.
- An unsigned array index keeps `addiu -3` before the shift; a signed one folds it into the offset.
- `mult` operand order follows operand types (`int * short` versus `int * int`).
- The C++ `for (int i...)` scoping is not always right: with a narrowed parameter as the start value, plain C `int i; for (i = 1; ...)` matched. Try both.

**Solved overnight (12–13 November)**
- Values kept at `0($sp)` across calls ("stack spills"): SNC -O2 -g never keeps a C++ local whose class has a constructor, or a one-element array `T x[1]`, in a register. Construct it where retail stores the value, copy it back where retail reloads (`r = call(self); self = saved.value;`), read fields into locals before constructing it, and declare several spilled values as arrays in slot order. A sweep of every `-X` option found no retail setting that explains it. Details and examples: `build/dc-results/stack-spill.md` (66 of 97 skipped functions matched).
- Destructors that load the vtable in the null check's delay slot, five-argument calls with the callback `%lo` in the `jal` delay slot, and one `lui` shared by two `%lo` loads of an 8-byte global: in-class static helpers (pass the vtable in), and `unsigned long long` globals passed as 64-bit arguments.
- Binary searches: `bsearch(begin, end, &key, Order())` with the comparator temporary built by an in-class constructor; STLport sort and heap helpers take a one-byte comparator by value.
- `delete p` with a declared `virtual ~T()` reproduces SNC's virtual-destructor call; SNC puts a class's vptr after all of its own data, so a vptr at offset 0 needs the virtuals in a data-free base class.

**Learned overnight (14–15 November)**
- Retail loops that reload everything each pass (unfolded guard, no strength reduction, count reloaded): write continue-style tests and re-derive the array inside the body (`ListEntry *entries = list->entries; e = &entries[i]; if (!e->effect) continue;`), and declare call results and tested fields as locals inside the body. This turned nine battle skips into matches.
- Loop entry: C with `int i;` at the top jumps into the loop and puts early-return stubs before it; C++ `for (int i...)` falls straight in. Choose by retail's layout.
- Local names decide float compare operand order (`rate > one` gives `c.lt.s one, rate`, `rate > unity` gives `c.le.s` with `bc1t`); named float constant locals (`float unity = 1.0f; float zero = 0.0f;`) are materialised once, their registers in declaration order.
- Argument moves follow the callee prototype's parameter order, so a prototype's int/float interleaving must follow retail. Store a call's result in a local before the next call (`float r = f(); g(self->x, r);`) to stop a field being kept in an s-register across it.
- Wrappers decide evaluation order: an in-class helper that reads a global inside (`Loader::Load(...)` over `D_089D7830`) evaluates its arguments first, a direct call loads the global first.
- An inline helper taking a computed index (`Frame(work, i * 10)` returning `work->handles[0x25 + row]`) stops strength reduction; a large-offset inline accessor (`(T *)((char *)work + 0x652C)`) keeps retail's two `addiu`.
- Placement new on a block just stored reproduces retail's store, reload, null test and constructor call; a by-value struct return in C++ gives retail's copy, an out-pointer first parameter gives none.
- Struct copies: flat float structs copy as `lwc1/mfc1/sw`, a struct with a nested Vec3 member as `lw/sw`; inline by-value helpers (`copy(p) { return *p; }`) rebuild retail's 3–4 stack-slot copy chains; `__attribute__((aligned(16)))` on matrix and Vec4 locals fixes stack offsets.
- s-register numbering follows first use: compute the first-used value at the top of a loop body, copy parameters into locals at the top, or give a variable a dead `= 0` early. Not a general rule, but it fixed a dozen functions.
- An LCG update must be chained (`x = (D = D * a + b) >> 16 & m`) to schedule like retail; `int r = cond ? 1 : 0;` in an `unsigned char` function gives `beql` plus the final `andi`.
- Each switch-dispatch function with a jump table must sit alone in its file for the table data to be placed.

**Learned overnight (21–22 November)**
- The motion calls `func_0882FA10` / `func_0882FAC4` / `func_0882FB90` take `(void *motion, int id, float x, float y, int arg)`. Declared that way, SNC sets up the arguments in retail's order; the older `(float, float, motion, id, arg)` prototypes in many battle files only matched with extra locals. Values both calls use (the motion pointer, `0.0f`) are set before the branch when retail hoists them.
- Odd argument shuffles at a call are often 64-bit (`long long`) parameters; an `unsigned long long` declared parameter fixed five main functions.
- Colour words unpacked through a constructor (`struct Rgba { Rgba(unsigned v) { r = v & 0xFF; g = v >> 8 & 0xFF; ... } }; Rgba value(*word); color = value;`) give retail's shift-before-mask order.
- Local arrays initialised from a table: write the copy as an explicit call (`func_08935524(local, D_<table>, sizeof local)`) from an `extern const` table at the retail address; the initialiser form puts the table in `.data`, which cannot be placed.
- Operand order in additions follows the names (alphabetical) and the types (`unsigned` operands with `i + base`); renaming locals fixed two functions.
- A constant 1 reused across calls is kept apart from a later `return 1` by an `unsigned char` callee parameter or a block-scope local; a literal `-1` keeps compare operand order.
- `&&` against nested ifs, `if (!x) { call; return; }` before the rest, and `int i;` at the top against `for (int i...)` each decided several layouts (one function went from 88 differing words to a match); index forms (`slots[i * 3]`) beat pointer walks.
- Loops that keep their arguments live reverse the parameter registers at entry (`move a3, a2; move a2, a0; ...`); this is SNC's allocation, not a different signature.
- Framebuffer arguments typed `void *` stop SNC merging an int constant with an equal pointer constant; a value OR-ed from four shifts needs separate `c |= ...` statements.
- Declaring `T **link;` before a list-unlink `if` fixes its delay slot.

**Learned overnight (24–25 November)**
- Delay slots: when retail fills a branch (or `bnel`) delay slot with a `lui`/`lw` from the next block and the build leaves a `nop`, move the whole body into an in-class static helper and make the `extern "C"` function call it; writing the body on one line does not help. A value reloaded from a stack struct after a call fills the slot when the reload comes before the test (`int ok = f(...); w = l.w; if (ok) ...`). This turned two near-miss families (12 and 14 functions) and four smaller ones into matches.
- A field tested from the incoming `a0` and then reloaded from `s0` for a call argument: test `self->handle != 0` directly and pass an in-class accessor `self->H()` as the argument (SNC does not merge the two loads).
- `andi s1, 0xFF` at the join after `s1 = 0` / `s1 = 1`: build the flag in an `int t`, then `unsigned char r = t;`, in a function returning `unsigned char`. An extra `b` to the epilogue before the last return comes from putting that return in its own block with a local (`{ int hit = ...; return hit != 0 || r; }`).
- A compare against a named `int one = 1;` declared after the call keeps a fresh `ori`; declared before the call it is kept in a saved register. A named `int zero = 0;` gives `ori`, a literal 0 gives `move $zero`.
- Retail's `bnez x, body; nop; b end` shape is `if (!x) return;` written first with the body at top level; retail often tests the non-zero side first, so swapping then and else is worth a try. `do { r = f(); if (r == 0) break; } while (1);` gives the `beqz/nop/b` loop exit; `if (r == -1) { return -1; } return r;` gives `bne` plus an explicit `b`.
- Small `bgtz/bltz/slti` ladders are plain `switch` statements with the cases in the order of retail's bodies (read it from the `jal` order, not Ghidra's); empty `case K: break;` plus `default: break;` gives the `beq/b/nop` stub ladder.
- Pointer-to-member calls through an object (`lw aN, OFF(obj); addiu ..., 8*(k+1); lh; lw; addu; jalr`) are C++ virtual calls on a class with `char data[OFF];` before its virtuals; a vptr at offset 0 needs a data-free class. About 40 wrapper functions matched this way, including float and 64-bit arguments (a `u64` travels in `a2/a3`).
- Families: same-size neighbours with the same shape usually differ only in constants, globals and callees. Matching one and generating the rest gave 29 battle input checks, 33 binary searches, 17 release loops, 14 stat adds, 12 stat pairs, 11 flag checks, 9 block copies and several smaller groups (templates: `src/battle/input_check_08b31640.cpp`, `src/main/bsearch_088b715c.cpp`, `src/main/stat_add_0895b104.cpp`, `src/main/stat_pair_08974d20.cpp`).
- Ghidra drops arguments a callee takes but the function only forwards: if retail never touches `a0`/`a1` before a call, declare and pass them. Copy parameters into locals in the order retail moves them into saved registers.
- `T *volatile D_xxx` reproduces retail reloading a global pointer; release loops are C++ `for (int i = 0, *p = self; i < N; i++, p++)`, constructor-style clear loops plain C `int i; int *p; for (i = 0, p = self; ...)`.
- Stack temporaries with an outgoing-argument area: one struct of locals with a leading `int pad[4]` fixes the offsets and frame size.

**Not solved yet**
- s-register permutations in otherwise identical code, constants that SNC keeps in a saved register where retail rematerialises them (often a `lui`), and single load or `$ra` scheduling slots remain the commonest causes of the 181 near misses left overnight.
- A spilled parameter that is also copied into another argument register for the call: SNC swaps the parameter registers and retail does not.
- Pointer-to-member-function dispatch through state tables ending in `jalr; nop` (about 40 menu functions); an investigation was cut off by the usage limit before reporting.
- Loop rotation in a few search loops, `sw $ra` scheduled before dependent loads, and bit sets kept as `and ~m; or m`.
- Readable rewrites of the generated files are installed in battle and most of menu; the per-region headers (`include/<module>/auto_region*.h`) overlap and still need merging into real class headers.
- Still open after 24–25 November: parameter-spill loaders (retail stores `a1` then `a0` at `4($sp)`/`0($sp)` before the first call and keeps neither in a saved register), four-halfword copy loops (`lhu; sll/sra; sh`), flags built from `mode == 1` by branches (`bne; ori 0; b; ori 1`), bool helpers on a float test (`bc1fl`/`bc1tl`), pointer-to-member dispatch through a stack table, a result variable's `ori 0` that retail puts in a loop test's delay slot, and loops where only the counter and pointer registers swap.

## 6. VFPU inline assembly is counted separately (15 November 2026)

The project owner decided that functions built from C with VFPU inline assembly count separately from C/C++. Other matching projects do the same: assembly is kept where the compiler cannot emit an instruction at all (PS2 VU0 macro mode, a DS `clz`), and it is reported apart from decompiled C; assembly that only forces a match is not accepted.

- A unit declares `"inline_asm": "vfpu"` in its match configuration. `tools/match_config.py` keeps it out of `configured_totals` and reports it through `configured_inline_asm_totals`.
- The verifiers build and byte-check these units like any other, but report them as `inline_asm_functions` / `inline_asm_bytes`, not in `recovered_c_*`. They refuse a unit whose source contains inline assembly (or includes `vfpu.h`) without the declaration, and a declaration whose source has none.
- `tools/progress.py` prints the separate count under the C/C++ headline. Tests: `tools/test_inline_asm_units.py`.
- Integrated: the 12 SNC game-code matches from `build/dc-results/vfpu.md` (main 9, battle 3; they use `include/vfpu.h`) and the 14 psp-gcc vector-library functions, split one per file into `src/main/sdk/libvfpu_*.c`.
- Result: 26 functions / 1,888 bytes (main 23, battle 3) in the separate count; the C/C++ headline is unchanged at 11,665 / 972,932. Battle's raw instruction words fell from 24 to 16 bytes. All four modules rebuild byte-identically and all 224 tests pass (7 skipped).
- Not integrated: the 4 SNC matches of SDK library functions (`sdk_vec3_length_*`, `sdk_vec4_dot_*`), because that range is GCC-built. The GCC versions cover `0x089A3AF0`, `0x089A3B10` and `0x089A3BF8`; `0x089A37DC` still needs a GCC match.

### Later Sony GCC: none public

A web search on 15 November found no Sony psp-gcc later than 1.7.1. The decomp community's compiler host (decompme/compilers, `platforms/psp`) carries psp-gcc 1.3.1 and 1.7.1 (both already here), SNC 1.2.7503.0 and eleven Metrowerks `mwccpsp` 3.0.1 builds (121 to 219). No public page says which GCC each Sony SDK release shipped, and no other PSP matching project documents one. Notes and links: `build/scratch/psp-gcc-search.md`. Nothing was downloaded.

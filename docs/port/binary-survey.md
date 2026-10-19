# Dissidia 012 (ULUS10566 v1.00) - binary survey for a native PC port

> Static-analysis report, 18 October 2026. Inputs: the four decrypted ELFs in `build/ULUS10566/` (hashes match
> `config/inventory/*.json`), the unencrypted disc PRX `LIBSUPPREACC.PRX`, the retail ISO (headers only), and the
> maintained function inventories (revision 18). Nothing was run in an emulator.

Conventions: "V:" = verified from the binaries (address and method given). "I:" = inference. Counts over
"functions" use the inventory's function-body ranges, which are still provisional for main, menu and battle.

Scripts (all in `tools/port/`, run with `.venv\Scripts\python.exe`, output to ignored `build/scratch/port/`):

| Script | Output | What it does |
| --- | --- | --- |
| `ppsspp_nids.py` | `ppsspp_nids.json` | Reads PPSSPP 1.20.4's compiled HLE tables out of `tools/local/ppsspp/PPSSPPWindows64.exe` (1,750 NIDs in 98 modules). Run first. |
| `survey_imports.py` | `imports.json` | Import tables of EBOOT and the PRX, NID names, groups, direct call sites per module |
| `instruction_census.py` | `instruction_census.json` | Opcode counts over function bodies (rabbitizer, Allegrex + VFPU) |
| `control_flow.py` | `control_flow.json` | Jump tables, `jalr` shapes, code pointers in data, cross-module calls, hidden functions |
| `api_arguments.py` | `api_arguments.json` | Constant arguments at OS call sites (threads, memory, display, audio, modules) |
| `memory_scan.py` | `memory_scan.json` | Hard-coded address regions, libGu location, GE command words built in code |
| `media_survey.py` | `media.json` | Media formats from ISO file headers and every PACKAGE.BIN entry header |
| `show_function.py` | terminal | Disassembles one inventoried function with import stubs named (analysis aid) |

---

## 0. Headline findings

1. V: The whole OS surface is **297 distinct imported functions**: 296 in EBOOT.ELF (31 libraries) plus one more in the
   disc PRX. The three overlays import nothing; they call main's import stubs directly. 288 of 296 NIDs resolve to
   PPSSPP names; the 8 that do not are `scesupPreAcc`, which is the game's own disc PRX, not firmware.
2. V: Code is **1,179,485 instructions** in 21,282 inventoried functions. VFPU: **10,885 instructions, 113 distinct
   opcodes, 376 functions**, 84% of it in main. FPU (single precision only): 92,123. Branch-likely: 4,957.
   No `syscall`, `ll`/`sc`, `sync`, `mfic`/`mtic` or `halt` inside function bodies.
3. V: Indirect control flow is regular. 207 switch jump tables (3,774 entries) all follow one SNC idiom; 20 other
   `jr reg` sites, all in GCC-built library code. Of 3,484 `jalr`, **3,129 (90%) are C++ virtual calls** with SNC's
   8-byte vtable slot `{s16 this-delta, pad, fn}`.
4. V: **Main calls directly into the overlay address window 852 times (542 targets)**, mostly into battle, and main's
   data holds 72 overlay code addresses (vtables). Ten targets are function entries in more than one overlay.
   A recompiled main therefore needs overlay-aware dispatch for those sites.
5. V: About **1,229 functions are probably missing from the inventory**: data words (vtable slots) point into
   "unknown" bytes at addresses that start with a stack-frame prologue or a bare `jr ra` (main 415, menu 169,
   battle 645). 70 of main's direct call targets in battle also land in unknown bytes. Complete function discovery is
   a prerequisite for static recompilation; the 235,172 unknown bytes are not only data.
6. V: **libGu is statically linked** (around `0x0899F468..0x089A2310` in main, context block in BSS at `0x09AF67E8`).
   The game submits through it (3 `sceGeListEnQueue`, 1 `EnQueueHead`, 1 `UpdateStallAddr` site). It also uses
   off-screen render targets (draw buffers 64/128/256/512 wide), `sceGuCopyImage` of the 480x272 screen, CLUT loads,
   and a GE-heavy model renderer with display-list `CALL`s in the library code at the end of main.
7. V: Frame pacing is a vblank counter: `main:0x08883F9C` waits with `sceDisplayWaitVblankStart(CB)` until a target
   number of vblanks has passed since the last flip, then swaps; the frame's dt is stored as elapsed vblanks x a
   constant. Seven game threads plus callbacks, priorities 0x10 to 0x6F.
8. V: Memory use fits a PSP-1000: fixed-address main image, a 1.6 MB overlay window, a 16.8 MB BSS, one 3.86 MB
   memory block (`EFFECT_ALLOCATOR`), and the 4 MB volatile partition via `sceKernelVolatileMemLock`. No code uses
   the extended RAM of later models, the scratchpad, kernel addresses or hardware registers as memory.
9. V: Media to decode: ATRAC3 (6,110 voice/SE streams in PACKAGE.BIN), ATRAC3plus (134 BGM files plus 63 in
   PACKAGE.BIN), PSMF movies with H.264 video and ATRAC3plus audio (34 files, 511 MB), PS-ADPCM through sceSas
   (SSCF banks), GIM textures, GMO models and PGF fonts through sceLibFont.

---

## 1. OS and library surface

### 1.1 Binaries that import

| Binary | Module name | Import libraries | Imported functions | Exports |
| --- | --- | ---: | ---: | --- |
| `EBOOT.ELF` | `FFSTJP` | 31 | 296 (stubs `0x089C5770..0x089C60B0`) | `syslib` (module_start/info) |
| `LIBSUPPREACC.PRX` (disc, unencrypted, type 0xFFA0) | `scesupPreAcc_library` | 4 | 26 (25 shared with main) | `scesupPreAcc`, 13 functions |
| `LIBFONT.PRX` (disc, SCE-wrapped) | not analysed | ? | ? | Sony libfont; main imports `sceLibFont` from it |
| `OVL_MENU/BATTLE/EXTRA_APP.ELF` | none (type 0xFFA8, one PT_LOAD at `0x089DD4C0`) | 0 | 0 | none |

V: Main loads the two PRXs with `sceKernelLoadModule` through a wrapper at `main:0x0886EEBC`, with names
`libsuppreacc.prx` (`main:0x0886F284`) and `libfont.prx` (`main:0x08881A94`) formatted into
`disc0:/PSP_GAME/USRDIR/data/module/%s`. It loads firmware modules with `sceUtilityLoadModule`: 0x300 avcodec,
0x301 sascore, 0x302 atrac3plus (loop at `main:0x088A1A80`), 0x303 mpegbase (`0x088ECF48`), 0x100/0x101 net
common/adhoc (`0x0890E61C`), 0x500 NP DRM (`0x08862650`).

V: `LIBSUPPREACC.PRX` is Sony code built with GCC 3.3.6 + allegrex-2.2.2 (its `.comment` section), 3,539
instructions, no VFPU or FPU. It imports only I/O (18), callbacks (3), power lock (2) and power callbacks (2).
Its export names are not in PPSSPP's tables, because PPSSPP loads and runs this PRX. A port can recompile it with the
rest of the game or reimplement the 8 entry points main imports (5 called from main, 3 from extra).

### 1.2 Functions by group

NIDs were named from PPSSPP 1.20.4's HLE tables. "Sites" counts direct `jal`/`j` to the stub from all four modules.
Totals: main 661 sites, menu 103, extra 17, battle 9. Battle makes almost no OS calls itself.

| Group | Fns | Sites | Functions (sites) |
| --- | ---: | ---: | --- |
| Kernel: threads | 13 | 45 | CreateThread 6, StartThread 6, ExitDeleteThread 6, WaitThreadEnd 6, DelayThreadCB 6, ExitThread 5, DeleteThread 3, DelayThread 2, TerminateThread 1, ChangeThreadPriority 1, ChangeCurrentThreadAttr 1, WaitThreadEndCB 1, GetThreadId 1 |
| Kernel: semaphores | 5 | 52 | CreateSema 6, DeleteSema 6, SignalSema 20, WaitSema 18, CancelSema 2 |
| Kernel: event flags | 5 | 10 | Create 2, Delete 4, Set 2, Wait 2, Clear 0 |
| Kernel: callbacks | 2 (+1 PRX) | 23 | CreateCallback 13, DeleteCallback 10; PRX also uses CheckCallback |
| Kernel: time | 5 | 93 | GetSystemTimeLow 79, GetSystemTimeWide 14; LibcGettimeofday/LibcClock/LibcTime 0 |
| Kernel: interrupts | 2 | 9 | CpuSuspendIntr 4, CpuResumeIntr 5 |
| Kernel: memory | 12 | 15 | AllocPartitionMemory, FreePartitionMemory, GetBlockHeadAddr, AllocMemoryBlock, GetMemoryBlockAddr, FreeMemoryBlock, TotalFreeMemSize 2, VolatileMemLock, VolatileMemUnlock, DmacMemcpy 3, SetCompiledSdkVersion603_605, SetCompilerVersion |
| Kernel: cache | 6 | 28 | DcacheWritebackAll 17, DcacheWritebackRange 5, DcacheInvalidateRange 3, DcacheWritebackInvalidateAll 1, DcacheWritebackInvalidateRange 1, IcacheInvalidateRange 1 |
| Kernel: modules | 7 | 10 | Load/Start/Stop/UnloadModule, StopUnloadSelfModuleWithStatus, GetModuleId, GetModuleIdByAddress |
| Kernel: exit | 2 | 2 | ExitGame, RegisterExitCallback |
| Kernel: stdio/debug | 5 | 7 | Stdin/Stdout/Stderr, Printf 2, GetGPI 2 |
| Kernel: libc helpers | 2 | 20 | sceKernelMemcpy 15, sceKernelMemset 5 |
| I/O | 23 | 86 | Open 6, OpenAsync, Read 8, ReadAsync, Write 2, Lseek 12, LseekAsync, Close 9, IoctlAsync, Devctl 16, Getstat 6, Chstat 2, Dopen/Dread/Dclose, Mkdir, Rmdir, Remove 2, Rename, SetAsyncCallback 4, WaitAsync 4, PollAsync 0, Chdir 0 |
| Controller | 3 | 5 | SetSamplingCycle, SetSamplingMode, ReadBufferPositive 3 |
| Display | 6 | 12 | SetMode, SetFrameBuf 3, GetFrameBuf 2, WaitVblankStart 2, WaitVblankStartCB 2, GetVcount 2 |
| GE | 11 | 26 | EdramGetAddr 13, ListEnQueue 3, ListEnQueueHead 1, ListUpdateStallAddr 1, ListSync 2, DrawSync 2, SetCallback 1, UnsetCallback 2, EdramGetSize 1, Continue 0, Break 0 |
| Audio: sceAudio | 13 | 17 | Output2Reserve/OutputBlocking 2/GetRestSample/Release; ChReserve 2, ChRelease 6, SetChannelDataLen, OutputBlocking 2, OutputPannedBlocking; 4 unused |
| Audio: sceSas | 27 | 21 | Init, CoreWithMix, SetVoice, SetPitch 2, SetVolume 2, SetSimpleADSR 2, KeyOn, KeyOff, SetPause, GetPauseFlag, GetEndFlag, GetEnvelopeHeight, GetOutputmode 3, RevType, RevEVOL, RevVON; 11 unused |
| Audio: sceAtrac | 10 | 13 | SetDataAndGetID, DecodeData, AddStreamData, GetStreamDataInfo, GetRemainFrame, GetSoundSample, SetLoopNum 2, ResetPlayPosition, ReleaseAtracID 2, GetInternalErrorInfo 2 |
| Video: sceMpeg | 25 | 31 | Init/Finish, QueryMemSize, Create/Delete, Ringbuffer x5, RegistStream 3, UnRegistStream 3, GetAvcAu, GetAtracAu, AvcDecode, AvcDecodeMode, AvcDecodeFlush 2, AtracDecode, GetPcmAu, InitAu 2, FlushAllStream, Malloc/FreeAvcEsBuf, QueryStreamOffset, QueryAtracEsSize |
| Utility: savedata | 4 | 31 | InitStart 8, Update 7, GetStatus 7, ShutdownStart 9 (main, menu, extra) |
| Utility: message dialog | 5 | 5 | InitStart, Update, GetStatus 2, ShutdownStart, Abort 0 |
| Utility: OSK | 4 | 8 | InitStart, Update, GetStatus, ShutdownStart (menu only, 2 each) |
| Utility: screenshot | 4 | 4 | InitStart, Update, GetStatus, ShutdownStart |
| Utility: modules | 2 | 8 | sceUtilityLoadModule 5, UnloadModule 3 |
| Utility: system params | 4 | 4 | GetSystemParamInt, GetSystemParamString, sceImposeGet/SetLanguageMode |
| Power | 8 | 28 | scePowerRegisterCallback 5, UnregisterCallback 3, scePower_469989ad 2 (clock set), CheckWlanCoexistenceClock 4, GetCpuClockFrequencyInt, sceKernelPowerLock 5, PowerUnlock 5, PowerTick 3 |
| Net / ad hoc | 48 | 67 | sceNet 4, sceNetAdhoc 16 (PDP + PTP), sceNetAdhocctl 12, sceNetAdhocMatching 10, sceNetAdhocDiscover 4, sceWlanDrv 2 |
| UMD | 6 | 15 | Activate, CheckMedium, GetDriveStat 9, WaitDriveStat 2, Register/UnRegisterUMDCallBack |
| Font | 10 | 14 | NewLib, GetNumFontList, GetFontList, FindFont, Open, GetFontInfo, GetCharInfo 5, GetCharGlyphImage 2, SetAltCharacterCode, Close 0 |
| DRM | 2 | 6 | sceNpDrmSetLicenseeKey, sceNpDrmEdataSetupKey 5 (DLC `.edat`) |
| Other: RTC | 7 | 66 | GetCurrentClockLocalTime 23, GetTick 19, GetDayOfWeek 9, SetTick 7, TickAddDays 6, TickAddSeconds, GetTickResolution |
| Other: scesupPreAcc | 8 | 9 | NIDs `0x13AE25B3 0x28C5F696 0x348BA3E2 0x86DEBD66 0xB03FF882` (main), `0x110E318B 0x2EC3F4D9 0xA0EAF444` (extra); names unknown |
| **Total** | **296** | **790** | 26 imports have no direct call site and are probably dead |

Notes:

- V: `scePower_469989ad` is named only by its NID in PPSSPP. The game calls it with `(333, 333, 166)` at
  `main:0x08860E3C` and `(222, 222, 111)` at `main:0x08860E78`: it switches the CPU between 333 and 222 MHz.
- V: `sceKernelAllocMemoryBlock` / `GetMemoryBlockAddr` / `SetCompiledSdkVersion603_605` are firmware 6.xx APIs.
- V: `sceIoDevctl` is used on `fatms0:` (commands `0x02415821`, `0x02415822`, `0x02425823`) and `ms0:`
  (`0x02425818` free space, `0x02425824`): Memory Stick callbacks, insertion state and capacity for data install.
- V: The 26 imports with no direct call site are not referenced as code addresses anywhere either (the only data
  words naming stubs are the import table itself), so they can be stubbed.

---

## 2. Instruction census

Counted over all inventoried function bodies (1,179,485 words = the 4,717,940-byte function-body union).

| Module | Instructions | VFPU | VFPU ops | VFPU fns | FPU | Branch-likely | `jal` | `j` | `jr` | `jalr` | `nop` |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| main | 445,877 | 9,097 | 113 | 285 | 44,837 | 2,647 | 24,390 | 1,013 | 9,015 | 968 | 22,065 |
| menu | 351,616 | 101 | 12 | 7 | 3,893 | 784 | 28,431 | 0 | 6,351 | 1,205 | 17,052 |
| battle | 376,293 | 1,687 | 19 | 84 | 43,365 | 1,512 | 26,410 | 0 | 6,347 | 1,293 | 25,783 |
| extra | 5,699 | 0 | 0 | 0 | 28 | 14 | 447 | 0 | 101 | 18 | 283 |
| **All** | **1,179,485** | **10,885** | **113** | **376** | **92,123** | **4,957** | **79,678** | **1,013** | **21,814** | **3,484** | **65,183** |

No function body contains an undecodable word. The PRX adds 3,539 instructions (no VFPU/FPU, 28 branch-likely).

### 2.1 Allegrex-specific and system instructions

| Opcode | Count | Where |
| --- | ---: | --- |
| `ext` / `ins` | 189 / 272 | main only |
| `seh` / `seb` | 51 / 2 | main only |
| `wsbw` / `bitrev` | 13 / 1 | main only |
| `clz` / `clo` | 30 / 0 | main only |
| `min` / `max` | 25 / 12 | main only |
| `madd` / `msub` (`maddu`/`msubu`) | 25 / 3 (0 / 0) | main only |
| `rotr` / `rotrv` | 2 / 6 | main only |
| `movz` / `movn` | 97 / 115 | main only |
| `lwl` / `lwr` / `swl` / `swr` | 29 / 105 / 8 / 8 | main 140, menu 10 |
| `break` | 47 | main; follows calls to fatal/assert paths (e.g. `0x08934300`) |
| `cache` | 5 | main, hand-written VFPU code: ops 0x1B (writeback-invalidate D line) at `0x0899F7B4`, `0x089A709C`; 0x1E (create dirty line) at `0x089B2988`, `0x089B2A6C`, `0x089B2B50` |
| `syscall`, `ll`, `sc`, `sync`, `mfic`, `mtic`, `halt`, trap ops | 0 | none in function bodies |

V: Every Allegrex-specific op, every `j`, every `cache` and all 20 computed `jr` sites are in main, and almost all in
`0x08990000..0x089C51DC` (address histogram in the census). This is GCC-built SDK/library code (the PRX's
compiler string shows Sony shipped GCC-built libraries). The game code proper, including all three overlays, is SNC
C++ output and uses none of these, `movz/movn` included; it does use unaligned `lwl/lwr` (menu 10) and VFPU math.

### 2.2 VFPU

By class: load/store 4,666, arithmetic 1,840, constants/init 971, compare/select 838, transcendental/reciprocal
765, prefixes (`vpfxs/t/d`) 510, matrix 413, move/rotate 357, GPR/control moves 265, conversion/pack 119,
VFPU branches 114, `vnop` 23, random 4.

All 113 opcodes with counts:

`lv.q` 2262, `sv.q` 1424, `sv.s` 523, `vzero.s` 521, `lv.s` 457, `vscl.t` 452, `vdot.t` 444, `vcmp.s` 407,
`vpfxd` 331, `vsqrt.s` 326, `vcmovt.s` 319, `vrcp.s` 294, `mtv` 249, `vone.s` 209, `vrot.q` 194, `vidt.q` 162,
`vadd.q` 161, `vscl.q` 131, `vmmul.q` 125, `vsub.q` 119, `vtfm4.q` 109, `vpfxt` 100, `vmul.s` 93, `vsub.t` 86,
`vadd.t` 83, `vpfxs` 79, `vmov.s` 74, `bvt` 71, `vmul.q` 71, `vmmul.t` 65, `vrsq.s` 61, `vcst.s` 48, `vmul.t` 43,
`vmidt.q` 40, `bvf` 39, `vcrsp.t` 36, `vadd.s` 32, `vi2f.q` 31, `vmov.q` 27, `vocp.s` 27, `vsin.s` 25, `vdot.p` 25,
`vnop` 23, `vsub.s` 22, `vdot.q` 22, `vmax.q` 22, `vmin.q` 22, `vuc2i.s` 21, `vmov.t` 20, `vh2f.p` 18, `vdiv.s` 17,
`mfv` 16, `vscl.p` 16, `vasin.s` 16, `vf2in.q` 16, `vmov.p` 14, `vmax.s` 13, `vsat0.q` 12, `viim.s` 11, `vdiv.t` 9,
`vfim.s` 9, `vmax.t` 8, `vmscl.t` 8, `vmin.p` 8, `vmax.p` 8, `vfad.t` 8, `vmul.p` 8, `vcrs.t` 8, `vzero.t` 7,
`vcmp.q` 7, `vcos.s` 7, `vneg.q` 6, `vf2iz.q` 6, `vabs.q` 6, `vrot.t` 6, `vcmovt.q` 6, `vmin.s` 5, `vmmov.q` 5,
`vi2uc.q` 5, `vmscl.q` 5, `vsgn.s` 5, `vneg.s` 4, `vhtfm4.q` 4, `vcmp.p` 4, `vone.q` 3, `bvtl` 3, `vmmov.t` 3,
`vtfm3.t` 3, `vabs.s` 2, `vmin.t` 2, `vrndf2.q` 2, `vrot.p` 2, `vneg.t` 2, `vrsq.t` 2, `vsgn.p` 2, `vlog2.s` 2,
`vcos.t` 2, `vsin.t` 2, `vmidt.t` 1, `vrndf1.s` 1, `vrndf2.t` 1, `vmzero.q` 1, `vbfy1.q` 1, `vbfy1.p` 1, `vrsq.p` 1,
`vfad.q` 1, `bvfl` 1, `vi2f.s` 1, `vexp2.s` 1, `vf2iz.t` 1, `vone.t` 1, `vadd.p` 1, `vsub.p` 1.
(`vuc2i.s` is printed by rabbitizer as `vuc2ifs.s`.)

V: Battle's 1,687 VFPU instructions are almost all one inlined 3-vector normalize, repeated 148 times
(`lv.q; vdot.t; vsqrt.s; vcmp.s; vpfxd; vrcp.s; vcmovt.s; vscl.t; sv`), in gameplay functions such as
`battle:0x08A2AD00` and `0x08A5065C`; menu has the same idiom 7 times. I: an inline vector-normalize helper. `vsin/vcos/vasin/vexp2/vlog2`
appear only in main's library zone (`0x089A4520..0x089BA3A8`). `vrndf1/vrndf2` (hardware PRNG) appear in three
main functions (`0x089775C0`, `0x08978178`, `0x08988DEC`). No `vrnds`/`vrndi` (no seeding).

---

## 3. Control-flow hazards for static recompilation

### 3.1 Indirect jumps

| Module | Jump tables | Entries | Bound found (`sltiu`) | Targets all inside the function | Other `jr reg` |
| --- | ---: | ---: | ---: | ---: | ---: |
| main | 69 | 1,713 | 56 | 69 | 20 |
| menu | 57 | 672 | 42 | 57 | 0 |
| battle | 80 | 1,382 | 71 | 78 | 0 |
| extra | 1 | 7 | 1 | 1 | 0 |
| **All** | **207** | **3,774** | **170** | **205** | **20** |

V: Every table uses one SNC idiom: `addiu idx; sltiu t, idx, N; beqz t, default; sll idx, 2; lui at, hi;
addu at, at, idx; lw at, lo(at); jr at`. Tables live in read-only data (e.g. main `0x089C6E30`). For the 37 tables
without a visible bound the entry count above is a lower estimate (entries were read until a target left the
function). The two battle tables whose targets leave the function (`0x08ADD950` -> table `0x08ADDCB0`, and
`0x08B4A6E4` -> `0x08B4A810`) mean those function boundaries are wrong, not that the table jumps elsewhere.

V: The 20 other `jr reg` sites are in main's library zone (`0x0899D44C..0x089BFA98`): tail calls through loaded
pointers and two `t9` dispatches in `0x089ABF9C`/`0x089ABFD0`. They need a runtime address-to-function lookup.

### 3.2 Indirect calls (`jalr`)

| Shape | main | menu | battle | extra |
| --- | ---: | ---: | ---: | ---: |
| Virtual call, SNC slot (`lw vptr; addiu slot; lh delta; lw fn; jalr; addu this,delta`) | 761 | 1,098 | 1,253 | 17 |
| Virtual call, other two-load shape | 9 | 14 | 2 | 0 |
| Function pointer passed in a register | 56 | 63 | 35 | 1 |
| Function-pointer field of an object/argument | 60 | 8 | 0 | 0 |
| Indexed function-pointer table | 11 | 15 | 1 | 0 |
| Pointer on stack / global / constant / computed | 51 | 3 | 0 | 0 |
| Definition not found within 16 instructions | 20 | 4 | 2 | 0 |
| **Total** | **968** | **1,205** | **1,293** | **18** |

So 3,129 of 3,484 (90%) are virtual calls. Vtable-like runs of `{delta word, code pointer}` pairs in data: main 209
(806 slots), menu 210 (1,026), battle 298 (1,372, longest 85 slots), extra 1 (21).

### 3.3 Code reached only indirectly

| Module | Functions | No direct `jal`/`j` caller | ...address in data | ...address built in code (`lui`+`addiu`) | ...no reference found | Code-pointer words in data |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| main | 8,621 | 1,233 | 850 | 360 | 23 | 1,259 (989 targets) |
| menu | 6,294 | 1,202 | 899 | 250 | 53 | 1,156 (1,000) |
| battle | 6,267 | 1,434 | 1,301 | 94 | 39 | 2,019 (1,657) |
| extra | 100 | 31 | 20 | 9 | 2 | 21 |

Import stubs are excluded from the main counts. Thread entries, callbacks and the overlay initializer tables are part
of the "address built in code" and "address in data" columns.

V: **Likely missing functions.** Words in each module's own data that point into the inventory's unknown bytes at a
stack-frame prologue (`addiu sp, sp, -n`) or a bare `jr ra`: main 415, menu 169, battle 645 distinct targets.
These are most likely virtual methods the Ghidra pass never reached. A recompiler must seed function discovery from
vtables, jump tables, main-to-overlay calls and `lui`/`addiu` pairs, not only from direct calls.

### 3.4 How main and the overlays call each other

| Direction | Mechanism | Count |
| --- | --- | --- |
| menu -> main | direct `jal` | 15,754 |
| battle -> main | direct `jal` | 12,390 |
| extra -> main | direct `jal` | 330 |
| overlay data -> main code | code pointers in overlay data | menu 76, battle 265 |
| **main -> overlay window** | direct `jal` | **852 sites, 542 targets** |
| main data -> overlay code | code pointers in main data (e.g. vtable at `0x089C9C7C` with battle slots) | 72 (battle 39, menu 30, extra 2, menu+battle 1) |
| loader -> overlay | descriptor initializer/cleanup tables (see `docs/decomp/overlay-hook-audit.md`) | menu 8 inits, battle 21 inits + 5 cleanups |

V: The 542 main-to-overlay targets: 364 are function entries only in battle, 85 only in menu, 3 only in extra,
10 in more than one overlay (`0x089DD4D0`, the overlay prefix entry, is in all three). The other 80 are not inventoried
entries: 70 lie in battle's unknown bytes, 15 in menu's unknown bytes, 62/9 fall inside existing menu/battle function
bodies at addresses that are not entries there (consistent with them being battle-only targets). The 241 main
functions making these calls span `0x08804498..0x08934114`, i.e. ordinary game code: main was linked knowing battle's
and menu's addresses.

I: For recompilation, each overlay is compiled as its own unit for the shared window, and main's 852 sites plus
the 72 data pointers resolve through "which overlay is loaded now" (or by static knowledge where only one overlay
defines the target). Calling a target whose overlay is not loaded is a bug on the PSP too, so a checked dispatch is
enough.

### 3.5 Runtime code modification

V: `sceKernelIcacheInvalidateRange` and `sceKernelDcacheWritebackInvalidateAll` are each called once, both in the
overlay loader `main:0x089331FC` after reading an overlay into the window. The other cache calls (`Dcache*` 26 sites)
are in GE/DMA paths. No other evidence of code being written: no `cache` I-cache ops, no stores into code addresses
were found by the scans used here (a full proof would need data-flow analysis). Import stubs are the standard
`jr ra; nop` pairs that the PSP loader patches; a recompiler replaces them with calls into the runtime.

### 3.6 Other hazards

- V: **`$ra` used as a general register** in 14 functions, all hand-written or library code: main `0x08872138`,
  `0x0889E258`, `0x0893A590`, `0x0893BBC4`, `0x08987848`, `0x089A64F8` (VFPU skinning, `addu ra, ra, t6`),
  `0x089AFD9C`, `0x089BD8CC`; menu `0x08A01E50`, `0x08AD1DA8`; battle `0x089E5CAC`, `0x089ED3A0`, `0x08A5D2D0`,
  `0x08AB81E0`. A recompiler must not treat every `jr ra` in these as a return without checking.
- V: The CRT start code at `0x08804040..0x08804160` (outside the inventory) reads `$k0` (`sw s5, 4(k0)` at
  `0x08804230`) and contains `jal 0` calls guarded by weak-symbol null checks (2 sites).
- V: No `$gp`-relative accesses anywhere, so no small-data base register to emulate.
- V: No conditional branch leaves its own function; `j` occurs only in main (939 within the same function,
  72 tail calls to function starts, 1 into the middle of another function, 1 to undecoded bytes).
- Delay slots (every branch and jump) and 4,957 branch-likely instructions need the usual translation.

### 3.7 Unknown bytes (from the inventory)

| Module | Unknown bytes | Ranges | <=16 B | <=64 B | <=256 B | <=4 KiB | Words that look like RAM pointers |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| main | 49,040 | 546 | 236 | 153 | 114 | 43 | 0 |
| menu | 116,144 | 1,108 | 626 | 212 | 162 | 108 | 1,649 |
| battle | 69,988 | 1,188 | 626 | 236 | 291 | 35 | 3,689 |
| extra | 0 | 0 | | | | | |

Largest ranges: main `0x0894DD74` (2,616 B), `0x08998C18` (1,588 B); menu `0x08AA7ED0` (2,360 B); battle
`0x08B00018` (2,176 B). Main's unknown bytes sit inside its code section (data is in separate sections), so they are
mostly undiscovered code or literal pools. The overlays mix code and data in one section, so their unknown bytes are
both: the pointer-like words are data, and section 3.3 shows several hundred function entries among them.

---

## 4. Graphics

### 4.1 How display lists are built and submitted

V: libGu is statically linked in main. Evidence: the only caller of `sceGeListUpdateStallAddr` is an 88-byte function
at `0x089A22B8` (PPSSPP's known-function list has `_sceGuUpdateStallAddr` at 88 bytes); `0x0899F468` (316 B) creates
the `SceGuSignal` event flag, sets the GE callback, enqueues the first list at `0x089D1510` and syncs (sceGuInit
shape); `0x0899FA64` calls both `ListEnQueue` and `ListEnQueueHead` (sceGuSendList shape); `0x089A0190` calls
`ListSync`/`DrawSync` (sceGuSync shape); `0x0899FB08` and `0x0899FBE4` call `sceDisplaySetMode`/`SetFrameBuf`
(sceGuDisplay / sceGuSwapBuffers shape). Many 28-90 byte setters between `0x0899FF9C` and `0x089A2310` write one or
two GE words through the current list, whose write cursor is at `(*0x09AF6830) + 8` (e.g. `0x089A0BE4` writes
commands 0x4C/0x4D, texture offset). `0x089A12FC` is 524 bytes, the size PPSSPP lists for `sceGupSetStatus`.

V: The GE command words built in code (`lui reg, CMD<<8` then `or`) are mostly in libGu and in a GE-heavy library at
the end of main (`0x0899C764..0x089BD8CC`): that code sets texture and CLUT addresses, loads CLUTs, flushes the
texture cache and `CALL`s display lists that are not built by libGu (`0x089BD8CC`, 2,980 B; `0x089B2DE8`;
`0x0899C764`). I: this is a model renderer that calls command lists stored with model data. Only about six functions
in the game code proper build GE words themselves (e.g. `main:0x08974630` does its own CLUT load and transfer).

Call counts: `sceGeListEnQueue` 3, `sceGeListEnQueueHead` 1, `sceGeListSync` 2, `sceGeDrawSync` 2,
`sceGeListUpdateStallAddr` 1, `sceDisplaySetFrameBuf` 3 (all in libGu), `sceDisplayWaitVblankStart` 2 and
`...StartCB` 2 (both in the frame pacer), `sceGeEdramGetAddr` 13.

### 4.2 Render targets, CLUTs and transfers

V: Callers of the libGu draw-buffer setter at `0x089A02E0` (writes FRAMEBUFPTR/FRAMEBUFWIDTH, 16 call sites, all in
main game code): `0x08861208` sets 512-, 256-, 128- and 64-wide targets in formats 0 (5650) and 3 (8888), and
`0x0885E150` a 64-wide 8888 target. Both also call `sceGeEdramGetAddr`. `0x08866AE0` switches between an 8888 frame
buffer at VRAM offset 0x88000 and a 5551 one at 0x44000, with depth at 0x110000 or 0x88000; `0x0886603C` sets the
main 512-wide buffers at offset 0. I: these are off-screen render targets later used as textures (downsampled
copies, glow or shadow passes), which is render-to-texture.

V: `sceGuCopyImage` (`0x089A09A8`, writes the TRANSFER* commands) is called twice; the battle call
`battle:0x08ACD138` copies a 480x272 region after calling `sceDisplayGetFrameBuf`, and the next function
`battle:0x08ACD1E8` calls `sceKernelDcacheWritebackRange`. `sceGuClutLoad` (`0x089A1024`) has 6 call sites, with palettes in main data
(`0x089D6050`, `0x089D6090`, `0x089D60D0`, `0x089D68D0`).

V: VRAM is addressed as `sceGeEdramGetAddr()` plus offsets. No `lui 0x0400`/`0x4400` value is used directly as a
load/store base anywhere. Whether the CPU writes or reads VRAM through computed pointers (for example reading back
a render target) cannot be settled statically.

### 4.3 What this means for the renderer

- A GE-command translator (PPSSPP's approach) handles everything above, including model display lists `CALL`ed from
  data, at the cost of reimplementing the GE: vertex decoding (including skinning and morph weights), the texture
  formats and swizzle, CLUT modes, the framebuffer-as-texture tracking, block transfers, depth/stencil semantics.
  Upscaling and higher frame rates are then a matter of that backend, as in PPSSPP.
- A higher-level renderer (hooking the game's draw calls or GMO rendering) would avoid most of the GE, but would have
  to replace libGu use across ~100 call sites, the model renderer, every off-screen pass and the 2D UI. Given that
  model data carries GE lists, a high-level path still needs a GE decoder for those lists.
- I: The practical route is GE-level translation first (reusing PPSSPP's GPU code is the obvious source, licensing
  permitting), with selective high-level replacements later.

---

## 5. Threads and timing

### 5.1 Threads created

| Name | Entry | Priority | Stack | Attr | Created at |
| --- | --- | ---: | ---: | --- | --- |
| `ffst_thread` (main) | `0x08804160` | 0x30 | 64 KiB (0x40 KiB from `0x089D85A4`) | user (+0x80000000 flag) | CRT `0x08804110` |
| `MS Write` | `0x0886FF1C` | 0x21 | 4 KiB | 0x4000 (VFPU) | `0x08870154` |
| `SQEXTEC Movie Streaming` | `0x088EF908` | 0x6F | 4 KiB | 0 | `0x088EED5C` |
| `SQEXTEC Movie Decode` | `0x088EDC38` | 0x6F | 2 KiB | 0 | `0x088EED5C` |
| `SQEXTEC Movie AudioOutput` | `0x088EDD44` | 0x10 | 1 KiB | 0 | `0x088EED5C` |
| `AUDIO MIXER THREAD` | `0x08991D60` | 0x10 | 64 KiB | 0 | `0x089913EC` |

Lower number = higher priority. Priority and stack of `ffst_thread` come from CRT globals `0x089D85A8` (0x30) and
`0x089D85A4` (0x40 KiB). `sceKernelChangeCurrentThreadAttr(0, 0x4000)` (VFPU on) is called by the exit-callback setup
`0x08933398`. The strings `SceWaveMain`/`SceWaveMainEvf` (Sony libwave) are present but no code references them;
libwave's thread function `0x089C1C8C` has no caller.

Callbacks (13 `sceKernelCreateCallback` sites): UMD (`FILE_MANAGER_DVD_DvdUmdCallback`), async I/O (four
`FMI_*AsyncCallback`), power (`FMI_PowerCallback`, `SAP_PowerCallback`), `VOLATILE_MEMORY_LOADER`, `memory stick`,
`UTILITY`, `exit`, and two in extra (power and Memory Stick for data install). Callbacks only run while a thread is in
a `...CB` wait: `sceDisplayWaitVblankStartCB`, `sceKernelDelayThreadCB` (13,000 us loops in the movie threads, 500 us
in movie audio), `sceKernelWaitThreadEndCB`.

Synchronisation objects: six semaphores (`SQEXTEC MoviePlayer Display/Audio Async`, `AudioReplayLock`,
`AudioReplay` with max 0xF7F, `VOICE LOCK`, `STREAM LOCK`), two event flags (`AudioReplayWait`, `SceGuSignal`),
and `sceKernelCpuSuspendIntr`/`ResumeIntr` pairs (9 sites) used as global critical sections.

### 5.2 Frame pacing

V: `main:0x08883F9C(timer, useNonCB, flag)` is the per-frame end, called from `0x08864B68` (I: the main loop) and
ten other loops (loading, menus, movies), always with the timer object at `*0x089D69E0`. It: reads `sceKernelGetSystemTimeLow`; calls libGu sync
`0x089A0190(4, 0)`; waits one vblank (`sceDisplayWaitVblankStart` or `...CB`), reads `sceDisplayGetVcount`, and keeps
waiting while `vcount - last_flip_vcount < timer+0x10`; calls `sceGuSwapBuffers` (`0x0899FBE4`); records the vcount
and stores the frame's dt as a float, `elapsed_vblanks x constant(0x089C9F50)`, in `timer+0x18`.

I: `timer+0x10` is the frame interval in vblanks (1 = 60 fps, 2 = 30 fps); battles were measured at 30 updates per
second (measured during the earlier PPSSPP-plugin work), and `docs/research/battle-camera.md` finds dt in 1/60 s units at `0x089D5EAC`.
A port can drive this from a host clock by replacing the vblank wait; going above the stock rate also needs the
game's frame-step logic to accept dt below one vblank, which is a game-code change, not a runtime one.

### 5.3 Scheduler dependencies

V: Movie and audio threads run at priority 0x10 (above the main thread's 0x30) with blocking audio output, and the
movie threads at 0x6F poll with 13 ms `DelayThreadCB`. The audio mixer delays 1,000 us between iterations.

I: The code uses semaphores and interrupt-disable sections rather than spin-waits, so a host scheduler that is
correct for blocking primitives should work. What cannot be ruled out statically is code that relies on strict
priority preemption (a higher-priority thread finishing its step before the main thread resumes, without a lock).
The safe design is PPSSPP's: run guest threads one at a time under a PSP-style priority scheduler (fibers or host
threads with a global run token), and treat `CpuSuspendIntr` as "no guest thread switch".

### 5.4 Input

V: `sceCtrlSetSamplingMode(1)` (analog) and `sceCtrlSetSamplingCycle(16667)` at `0x08872F90`;
`sceCtrlReadBufferPositive` at `0x08871E24`, `0x0890D664` and `battle:0x08B134E8`. Details of the pad module are in
`docs/research/pad-module.md`.

---

## 6. Memory

| Range | Size | Use |
| --- | ---: | --- |
| `0x08400000..0x08800000` | 4 MiB | Volatile partition, taken with `sceKernelVolatileMemLock(0, &ptr, &size)` (`0x08882E80`) for the `VOLATILE_MEMORY_LOADER` path |
| `0x08804000..0x089DD3C0` | 1.85 MiB | Main code, rodata, data (fixed-address `ET_EXEC`, not relocatable) |
| `0x089DD4C0..0x08B62F70` | up to 1,596,080 B | Overlay window: battle 1,596,080, menu 1,555,864, extra 24,312 bytes (code and data together, no overlay BSS) |
| `0x08B63170..0x08B63280` | 256 B | Main's second segment (initialized) |
| `0x08B63280..0x09B70D00` | 16,833,152 B | Main BSS: static pools and objects (e.g. pad manager at `0x08C15B90`, libGu context `0x09AF67E8`) |
| user partition | 0x3DD000 (3.86 MiB) | `sceKernelAllocMemoryBlock("EFFECT_ALLOCATOR", 0, 0x3DD000)` at `0x088A5054` |
| user partition | from a weak CRT symbol | `UserSbrk` libc heap via `sceKernelAllocPartitionMemory(2, "UserSbrk", 3, size, 0x1000)` at `0x0893431C` |

I: Image + overlay window + BSS reach `0x09B70D00`; adding the 3.86 MiB block leaves about 0.7 MiB of the 24 MiB
user partition (`0x08800000..0x0A000000`) for thread stacks, the two PRXs and the libc heap. The game targets the
PSP-1000 memory map.

V: Hard-coded address checks over every `lui` and its first use: no scratchpad (`0x00010000`) base is used for a load
or store (the 1,689 `lui 0x0001` uses are the constant 65536; the 11 that look like a base, all in
`main:0x0887F294`, are in annulled branch-likely delay slots and never reach the load); no VRAM, kernel (`0x88xxxxxx`) or hardware-register
(`0xBCxxxxxx`) value is used as a memory base (the `0xBC`/`0xBD` values are float constants); `0x0A000000` and
`0x0B000000` appear only as constants (GE `CALL`/`RET` words and upper-bound compares), never as a base. The 21,000+
`lui` values in user RAM are ordinary absolute addresses of globals, which is why the port must keep a flat 32-bit
guest address space (`rdram`-style) rather than native pointers.

---

## 7. Media and data formats

From file headers in the ISO and in every PACKAGE.BIN entry (11,043 entries):

| Format | Where | Count / size | Decoder needed |
| --- | --- | --- | --- |
| ATRAC3, 44.1 kHz, 66 kbps, stereo | PACKAGE.BIN RIFF/WAVE | 3,533 | sceAtrac (ATRAC3) |
| ATRAC3, 44.1 kHz, 66 kbps, mono (block 192) | PACKAGE.BIN RIFF/WAVE (streamed voice) | 2,577 | sceAtrac (ATRAC3) |
| ATRAC3plus, 44.1 kHz stereo, 96/65/48 kbps | `USRDIR/DATA/BGM/*.AT3` | 134 files, 149.8 MB (111 / 21 / 2) | sceAtrac (ATRAC3plus) |
| ATRAC3plus, 44.1 kHz stereo | PACKAGE.BIN RIFF/WAVE | 63 (41 / 20 / 2) | sceAtrac (ATRAC3plus) |
| PSMF 0014 / 0015, AVC video + ATRAC3plus audio | `USRDIR/DATA/MOVIE/*.PMF` | 34 files, 511.0 MB (27 / 7) | sceMpeg: H.264 decode, ATRAC3plus audio, ring buffer and AU parsing |
| SSCF sound banks, PS-ADPCM clips | PACKAGE.BIN | 131 banks, 22.4 MB | sceSas voices (VAG ADPCM, ADSR, pitch, reverb); format in `tools/sscf_bank.py` |
| GIM (`MIG.00.1PSP`), 8-bit indexed + 32-bit palette | PACKAGE.BIN, menu paks | 993 entries + 142 paks | game code uploads to the GE (CLUT8 textures) |
| GMO (`OMG.00.1PSP`) models with motions | PACKAGE.BIN | 779 entries, 536.1 MB | game/library code; carries GE command data |
| ARC, MPK, menu pak, mess/mlng, DES4, SEQ, VOLD, PNG | PACKAGE.BIN | see `docs/research/character-assets.md` | game code (no OS decoder) |
| PGF fonts | firmware `flash0:/font` via sceLibFont | 10 font calls | sceLibFont plus font files (PPSSPP ships substitutes in `assets/flash0/font`) |
| `ICON1.PMF`, `SND0.AT3` | `PSP_GAME/` | 1 each | XMB only, not needed by the game |

V: Movie playback is the game's own player (`SQEXTEC Movie*` threads, `DrawMovieTask`, `data/movie/%s`) on top of
sceMpeg directly; `scePsmf`/`scePsmfPlayer` are not imported. Movie audio goes out through `sceAudioCh*` with
2,048-sample buffers (`0x088ECFD4`) and `sceAudioOutputPannedBlocking`; game audio is mixed by the
`AUDIO MIXER THREAD` (`0x08991D60`) into `sceAudioOutput2` (SRC channel), using `__sceSasCoreWithMix` (32 voices,
44,100 Hz, `__sceSasInit` at `0x089A4ED4`) and `sceAtracDecodeData`.

---

## 8. What this means for a port

Ranked by expected effort, hardest first.

1. **GE emulation or translation.** libGu plus a model renderer that `CALL`s GE lists from asset data, off-screen
   render targets used as textures, screen copies with block transfers, CLUT textures. This is the bulk of PPSSPP's
   GPU work and the part the modernisation goals (resolution, frame rate) depend on.
2. **Complete code discovery across a shared overlay window.** Roughly 1,229 likely uninventoried functions (mostly
   virtual methods), 235 KB of unknown bytes, 3,484 indirect calls, 852 main-to-overlay direct calls and 72 main data
   pointers into overlays, 10 targets defined by more than one overlay. The recompiler needs a guest-address to
   host-function table per overlay and a fallback (interpreter or abort-with-address) for anything missed. The 0.44%
   matching decompilation does not help here; the inventory and these scans do.
3. **Kernel and threading semantics.** Six threads plus the main thread, 13 callbacks delivered in `CB` waits,
   semaphores, event flags, interrupt-disable sections, async I/O with callbacks, and per-thread VFPU state. Correct
   but simple if guest threads run one at a time under a priority scheduler.
4. **Media codecs.** ATRAC3 and ATRAC3plus (all music, voice, movie audio), H.264 PSMF through sceMpeg's low-level
   API (ring buffer, AU extraction, `AvcDecode` into a frame buffer), and a sceSas mixer with ADSR and reverb. All
   have open implementations (FFmpeg, PPSSPP), so this is integration work rather than research.
5. **VFPU.** 10,885 instructions, 113 opcodes, 510 prefix instructions, matrix ops, and approximations (`vsin/vcos`
   in quarter turns, `vrcp/vrsq/vsqrt`). SNC uses `vsqrt.s`/`vrcp.s` in battle gameplay code, so results that differ
   from the PSP in the last bits can desynchronise replays and ad hoc play with real hardware. The three PRNG uses
   are in main code whose purpose was not traced.
6. **System utilities.** Savedata (31 call sites, also from menu and extra), message dialog, OSK (menu), screenshot
   and the system-parameter queries need native replacements with their own UI; savedata needs a host format.
7. **Ad hoc networking** (48 functions). Can be stubbed to "no wireless" at first; matching PPSSPP's ad hoc
   emulation is a later option.
8. **The small stuff.** RTC, UMD and Memory Stick device state, power callbacks and clock switching, DRM/edata for
   DLC (`sceNpDrmEdataSetupKey`), the disc PRX `LIBSUPPREACC.PRX` (recompile it; it is small and unencrypted), and
   libfont (HLE as PPSSPP does, needing PGF font files).

Low-risk findings: no self-modifying code beyond overlay loading; no `syscall`, `ll/sc`, `$gp`, scratchpad or hardware
register use; branches never leave their function; the SNC game code uses a single switch idiom and a single
virtual-call idiom.

---

## 9. What could not be determined

- **Names of the 8 `scesupPreAcc` NIDs** and the meaning of its 13 exports (PPSSPP does not name them).
- **`LIBFONT.PRX`**: SCE-wrapped and not extracted locally; not decrypted or analysed.
- **Exact libGu and model-library boundaries and identities.** Identification is by call pattern and function size,
  not by signature hashing. PPSSPP's `knownfuncs.ini` hashes could confirm them, but its hash needs XXH3, which is not
  in the venv (it would need a package install).
- **Whether the CPU reads or writes VRAM or render targets directly** through computed pointers.
- **Real thread interleaving and priority dependence**, actual vblank intervals per mode, and which display lists
  run in a frame: runtime questions.
- **Which overlay is loaded when each main-to-overlay call site runs**, beyond the address-ownership evidence above.
- **Argument values passed through wrappers**: display mode width/height, the libc heap size (`UserSbrk`), sceSas
  grain, the `sceKernelChangeThreadPriority` values, and several I/O paths were not constant at the call site.
- **Entry counts of the 37 jump tables without a visible bound** (reported values are lower estimates).
- **Function boundaries.** All per-function figures inherit the provisional main/menu/battle inventories; the
  "likely missing functions" count is a heuristic and may include false positives (data that happens to point at a
  prologue) and misses (entries that start differently).
- **PSMF video dimensions and audio channel layout** (header fields beyond the stream ids were not validated).
- Whether the 26 imports with no call site are reached some other way (no evidence found) and whether libwave is
  fully dead (its thread has no caller; some of its channel helpers are used by the movie player).

## Reproduce

```powershell
.\.venv\Scripts\python.exe tools/port/ppsspp_nids.py
.\.venv\Scripts\python.exe tools/port/survey_imports.py
.\.venv\Scripts\python.exe tools/port/instruction_census.py
.\.venv\Scripts\python.exe tools/port/control_flow.py
.\.venv\Scripts\python.exe tools/port/api_arguments.py
.\.venv\Scripts\python.exe tools/port/memory_scan.py
.\.venv\Scripts\python.exe tools/port/media_survey.py      # reads the retail ISO at the repository root, ~1 min
.\.venv\Scripts\python.exe tools/port/show_function.py main 0x08883F9C
```

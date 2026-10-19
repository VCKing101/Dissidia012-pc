# First C matches and compiler investigation

Updated 11 October 2026. Target: verified USA ULUS10566 v1.00 extra overlay.

## Integrated result

Twenty-four functions are now rebuilt from C across six units under `src/extra/`:

| Retail address | Bytes | Reconstructed behavior |
| --- | ---: | --- |
| `0x089DD4D0` | 180 | Attach and clear state, create callbacks, register power slot 2 and Memory Stick FAT notifications |
| `0x089DD584` | 116 | Unregister and delete both callbacks, then detach state |
| `0x089DD5F8` | 52 | Set/clear the byte at `0x45A1` for suspending/resume-complete flags |
| `0x089DD62C` | 28 | If the event value is 2, set the byte at `0x459F`; return zero |
| `0x089DD648` | 20 | Follow the owner's state pointer and update fields at `0x600` and `0x604` |
| `0x089DD65C` | 20 | Follow the owner's state pointer and update fields at `0x4578` and `0x457C` |
| `0x089DD670` | 100 | Query total/available capacity in KiB, record status/flag, and return total capacity |
| `0x089DD6D4` | 128 | Test a path through a main-program helper; tolerate file-not-found, return true only on success |
| `0x089DDB88` | 136 | Copy a 64-bit global into state, call a support import, then invoke the transfer helper on success |
| `0x089DDC10` | 136 | Copy a 64-bit global into state and call a four-argument support import; record status/error |
| `0x089DDC98` | 76 | Call a support-library routine, record its status, and set the error flag on nonzero status |
| `0x089DDCE4` | 132 | Call a path helper and a follow-up on success; tolerate file-not-found |
| `0x089DDD68` | 168 | Fetch a byte count, then clear a buffer or copy bytes into it and submit its address |
| `0x089DDE10` | 16 | Write a value to `0x4594`, then publish the flag at `0x459D` |
| `0x089DDE20` | 16 | Write a value to `0x4590`, then publish the flag at `0x459C` |
| `0x089DDF24` | 72 | Poll a notification, flag a zero result as status `0x80000014` |
| `0x089DDF6C` | 180 | Transfer a buffer through the support library and advance the 64-bit cursor by a rounded result |
| `0x089DE020` | 152 | Start asynchronous work for the current selected entry, passing a callback and state |
| `0x089DE0B8` | 172 | Poll completion, record errors and advance the transfer cursor/index |
| `0x089DE164` | 48 | Write one of four state slots, rejecting indices outside 0 through 3 |
| `0x089DE69C` | 44 | Load the global context and forward three arguments to the power callback |
| `0x089DE6C8` | 44 | Load the global context and forward three arguments to the event callback |
| `0x089DE6F4` | 40 | Prepend global context and forward two completion arguments |
| `0x089DE71C` | 40 | Prepend global context and forward two notification arguments |

All twenty-four compile and link byte-identically with **SNC 1.2.7503.0 at `-O2 -g`**. They replace their assembly bodies at the original addresses. The state structure in `include/extra/data_install.h` reproduces the cleared region, naming capacity fields and callback identifiers where analysis establishes their meaning. Unknown fields retain offset names. No inline assembly or copied instruction words are used in these C functions.

The compiled functions total **2,116 bytes**. All other functions and embedded data retain the generated assembly representation, and 232 bytes of original ELF header/metadata remain preserved. The complete 24,544-byte overlay still has retail SHA-256:

`f6b7be5e15c28774985417f1ec2c64f6655e79cc3c9dce7600fa7f6bacabca01`

This establishes a working C + assembly build. It does not establish the original compiler for every module, completion of the extra overlay, a full-game rebuild, or a native port.

## Comparison results

The six-function matrix covers the two pointer-update functions, the power and event callbacks, and two field/flag updates. All six match with source-debug generation enabled. All relocated dispatch, capacity, setup, status and buffer routines are verified through the integrated build.

| Compiler candidate | Tested optimization settings | Exact functions out of six |
| --- | --- | --- |
| CodeWarrior PSP 3.0.1 build 147 | O0, O1, O2, O3, O4 size, O4 speed | 0 at every tested setting |
| CodeWarrior PSP 3.0.1 build 219 | Same six settings | 0 at every tested setting |
| SNC 1.2.7503.0 | O0, O1 | 0 |
| SNC 1.2.7503.0 | O2, O3, O4, O5 | 5 |
| SNC 1.2.7503.0 | O2, O3, O4, O5 with `-g` | 6 |
| Sony PSP GCC 1.3.1, GCC 3.3.3/Allegrex 2.2.1 | O2, G0 | 0 |
| Sony PSP GCC 1.7.1, GCC 3.3.3/Allegrex 2.2.2 | O2, G0 | 0 |

CodeWarrior probes use `-lang c -sdatathreshold 0 -char unsigned`; other untested flags could change the result. Sony GCC probes generate assembly through their historical frontend and use the already verified Allegrex assembler for object generation. These are six specific source candidates, not an exhaustive compiler/flag/source search. None of the zero-match results rules out an entire compiler family.

SNC is the best tested lead for this overlay. The matrix alone cannot uniquely discriminate optimization levels O2 through O5. The build uses O2 with source-debug generation, verified against all twenty-four functions. Exact whole-game compiler/build settings remain unconfirmed until more representative code and modules are checked.

## Optimized source-debug generation

Three reconstructions were one instruction shorter than retail without debug generation: the power callback, 64-bit query, and support-library status routine. Examining SNC's emitted assembly showed that the differences originated in its code generator, before assembly. Tests of scheduling/flow controls and alternate source forms did not resolve the differences.

Adding `-g` produced exact retail sequences with the ordinary C bodies. The initial ten-function control gave 7/10, 10/10, 7/10. The expanded control experiment across every integrated C unit now gives:

| Compiler profile | Exact functions |
| --- | ---: |
| `-O2` | 12 / 24 |
| `-O2 -g` | 24 / 24 |
| `-O2 -g -Xnosourcedebug=1` | 13 / 24 |

This demonstrates the influence of source-debug generation. In the power callback it preserves the ordinary branch/NOP/separate zero store; in the query and status routines it preserves the retail NOP before the conditional flag write. The expanded experiment also distinguishes debug generation from source-debug alone: `func_089DE0B8` matches with `-g -Xnosourcedebug=1` but differs without `-g`. No instruction patching or explicit NOPs are used in the C reconstructions.

The emitted assembly identifies the installed compiler components as `SN ANSI_C Front End 4.112 SN Systems 1.1.07 Apr 29 2010` and the corresponding backend. That identifies the tool we tested; it is not a compiler string recovered from the game itself. Retail symbol stripping would not rule out source-debug generation during compilation, but the game's actual build configuration still cannot be proven from these matches alone.

## Callback signature and qualifier evidence

The callback-source experiment separates pointer shape, volatility, and debug generation:

| Source model for four callback candidates | `-O2` | `-O2 -g` |
| --- | --- | --- |
| Direct typed state pointer | 0 | Both field/flag updates |
| Generic user-data pointer, cast inside the function | Event callback | All four |
| Generic user-data pointer, cast to volatile state | Event callback and both field/flag updates | All four |

The explicit cast reproduces argument-register reuse in the event and power callbacks. Under the initial non-debug profile, volatile accesses were needed to reproduce ordering in the two update callbacks; under the matching debug profile, both qualified and unqualified cast models match. Therefore volatility is not uniquely recoverable from these bytes. The integrated callbacks retain explicit volatile accesses pending further analysis of asynchronous users.

The dispatch wrappers accept three arguments, shift them into the next three argument registers, and prepend the context word from `0x089DE79C`. This is consistent with a C++ member-function adapter, but an explicit-context C design could have the same ABI. The recovered C retains neutral address-based names and an opaque context word.

The power callback's masks `0x10000` and `0x40000` correspond to suspending and resume-complete events in [PSPSDK's power header](https://github.com/pspdev/pspsdk/blob/master/src/power/psppower.h). That supports interpreting byte `0x45A1` as a suspend/resume state flag; its exact application-level meaning still needs caller analysis.

The power callback is now integrated at its full retail 52-byte size. Its suspending/resume-complete constants are named in the partial state header.

`func_089DD670` is integrated at its full retail 100-byte size. It establishes two eight-byte fields at offsets `0x608` and `0x610`, a status word at `0x4598`, and a byte flag at `0x459E`.

## Callback lifecycle and state region

The initialization routine attaches caller-supplied state and clears `0x45A8` bytes. It creates the callbacks named `data_install_power` and `data_install_fat`, storing their identifiers at offsets `0x624` and `0x620`. It registers the power callback in slot 2, and passes the FAT callback identifier through a local word to `sceIoDevctl("fatms0:", 0x02415821, ...)`. Teardown uses command `0x02415822`, deletes the FAT callback, unregisters power slot 2, deletes the power callback and clears the owner's state pointer. Neither recovered routine allocates or frees the state region; the caller's ownership contract is still unknown. The routines ignore the API status returns, as the retail instructions do.

The import NIDs agree with [PSPSDK's power stubs](https://github.com/pspdev/pspsdk/blob/master/src/power/scePower.S), [ThreadMan stubs](https://github.com/pspdev/pspsdk/blob/master/src/user/ThreadManForUser.S), and [I/O stubs](https://github.com/pspdev/pspsdk/blob/master/src/user/IoFileMgrForUser.S):

| Main stub | NID | API |
| --- | --- | --- |
| `0x089C57F8` | `0x04B7766E` | `scePowerRegisterCallback` |
| `0x089C5810` | `0xDFA8BAF8` | `scePowerUnregisterCallback` |
| `0x089C5F80` | `0xE81CAF8F` | `sceKernelCreateCallback` |
| `0x089C5F90` | `0xEDBA5844` | `sceKernelDeleteCallback` |
| `0x089C5E20` | `0x54F5FB11` | `sceIoDevctl` |

The state declaration now exposes the two callback words, a 4,050-word table at `0x628`, its count at `0x4570`, and the following word at `0x4574`. The table stores PSP addresses explicitly as 32-bit values. Its layout comes from the table-building instructions and does not establish a desktop-port pointer type or the original class declaration.

## Table-building candidate: not integrated

`func_089DD99C` is 492 bytes. It clears the table and two counters, rounds a helper's initial byte count up to an 8 KiB boundary using 32-bit arithmetic, and tracks subsequent offsets in 64 bits. It scans entries until the source count is exhausted or an entry pointer is null. Eligible entries have a nonzero high flag nibble no greater than the signed input argument. A size helper contributes to a 32-bit running total before a second size helper is compared with state offset `0x604`. Oversized entries are skipped. Accepted entries are stored in the table and sent to another helper with their source index and 64-bit offset.

On source exhaustion or a null entry, the routine calls a finalization helper and publishes the selected count. Reaching 4,050 selected entries takes a separate return path that bypasses both operations. These details come from the retail instructions; the helper names and the meaning of the two different size values are not yet established.

The candidate in `tools/probes/data_install_table.c` compiles to the correct 492-byte size with SNC `-O2 -g`. Before the buffer-size branch, its instructions match except for two earlier branch destinations. Overall, 27 instruction words differ because the loop increment and buffer-test paths are arranged differently. Tested structured loop/exit forms, C++ frontend compilation, and O3/O4/O5 with debug generation did not yield an exact match. Flow, size and scheduling controls did not resolve it either; invalid control values were rejected and excluded from comparisons. This candidate is not in `config/extra.matches.json`, does not replace assembly, and is not counted in recovered C progress.

Tracing the main helpers establishes that the two size values are raw length and 8 KiB-aligned length. The destination table contains a 128-byte header and 4,050 records of 28 bytes. See [data-install structures](data-install-format.md) for the field evidence, header write and completion behavior. The adjacent transfer routines match independently even though the table-building C candidate remains unfinished.

## Transfer and completion recovery

The new `data_install_transfer.c` unit covers five contiguous functions from `0x089DDF24` through `0x089DE194` (end exclusive), totaling 624 bytes. It handles initial notification polling, a buffer transfer through `func_886FF7C`, asynchronous dispatch through `func_8870000`, completion polling and a four-slot setter. The two new callback adapters extend the existing wrapper unit by 80 bytes. All seven functions pass raw relocation resolution and linked comparison.

The bounded setter's compound positive range check compiled eight bytes shorter than retail. Changing the ordinary C source to an early return for `index < 0 || index >= 4` reproduced the full 48-byte retail sequence. This is source control-flow recovery, without padding or injected instructions.

The two 64-bit cursor updates round unsigned 32-bit results up to 8 KiB and add them to the field at offset `0x618`, including the original error paths. Completion polling returns 1 when a result is available, even when that result sets the separate error flag. It advances the current index and does not clear the completion flag. Caller sequencing and runtime data installation remain untested; these behaviors are preserved because the bytes match.

## Capacity and buffer semantics

The main helper at `0x089A5490` checks FAT readiness through `sceIoDevctl` command `0x02425823`, then reads capacity information with command `0x02425818`. [PPSSPP's I/O implementation](https://github.com/hrydgard/ppsspp/blob/master/Core/HLE/sceIo.cpp) corroborates those command meanings and the five-word capacity record: maximum clusters, free clusters, maximum sectors, sector size and sectors per cluster. The game's own instructions establish the calculations:

```text
cluster_bytes = sector_size * sectors_per_cluster
capacity_kib = max_clusters * cluster_bytes / 1024
available_kib = max(free_clusters - 2, 0) * cluster_bytes / 1024
```

These formulas describe ordinary nonnegative device values. The actual code uses signed 64-bit division with truncation toward zero and has intermediate-width behavior; this is not an exact replacement C implementation for the main helper. The two output fields are now named `capacity_kib` and `available_kib`. Their original declared signedness remains uncertain, and the existing matching unsigned storage preserves the returned bits. The two-cluster reserve comes from the game binary, not the emulator's simulated free-space value. On query failure, the wrapper records status and leaves capacity storage untouched.

The main helpers at `0x08935524` and `0x08935828` are byte-copy and byte-fill loops, each returning the destination address. That supports interpreting `func_089DDD68` as conditional buffer clearing/copying. It does not establish all ownership or allocation rules for the buffer at state offset `0x600`.

The two path wrappers explicitly suppress their error flag for `0x80010002`, identified as file-not-found by [PPSSPP's error-code declarations](https://github.com/hrydgard/ppsspp/blob/master/Core/HLE/ErrorCodes.h). They retain numeric names for the main helpers because their broader filesystem behavior is not yet recovered.

The capacity helper's register moves, immediate loads and stack sequences differ from the matching SNC overlay profile and resemble GCC output. This is a lead for separately compiled library code or mixed toolchains; it does not identify the compiler for the entire main executable. The support PRX also contains GCC comments. Treat each module's compiler identification independently.

## Import and support-library evidence

The new ELF import inspector decoded the decrypted main executable's module information: module `FFSTJP`, **31 import libraries and 296 function imports**. It records addresses, numeric NIDs and library names in ignored `analysis/ULUS10566/main/imports.json`.

The imported routine at `0x089C5D40` belongs to `scesupPreAcc`, NID `0xA0EAF444`. Its export in the supplied support PRX is at relative address `0x6B0`; inspection supported a no-argument call. The newly recovered status routine records its return value and sets a byte flag when it is nonzero. The public function name remains unknown.

Both void-return and status-return source forms reproduce `func_089DDC98`; the integrated source returns the status left in the return register. That is a conservative reconstruction, not proof of the original C/C++ return declaration. Caller tracing remains necessary. Import names were obtained from the ELF records rather than guessed from stub addresses.

The additional calls at `0x089C5D10` and `0x089C5D28` also belong to `scesupPreAcc`, with NIDs `0x110E318B` and `0x2EC3F4D9`. Their matching callers pass four arguments and one argument respectively; public names and argument meanings remain unknown. As with the status routine, the return declaration of `func_089DDC10` remains provisional. The 64-bit field at state offset `0x618` receives the global at `0x089DE788` in both setup routines.

The inspector now reads function exports as well as imports. The support PRX has four import libraries, 26 imported functions and 13 `scesupPreAcc` function exports. Main import `0x089C5D48`, NID `0xB03FF882`, resolves to the PRX export at relative address `0xA28`. Inspection of that implementation reaches `sceIoWrite`; the public export name is still unknown. Transfer calls confirm that state offset `0x618` serves as a byte cursor, now named `transfer_offset`. The fields at `0x600`, `0x604` and `0x4574` are named `buffer_address`, `buffer_capacity_bytes` and `current_entry_index`. Unknown fields retain numeric names.

## Historical-tool compatibility

The compilers are installed locally from the public [decomp.me compiler catalog](https://github.com/decompme/compilers/tree/main/platforms/psp) and [sotn-decomp compiler release](https://github.com/Xeeynamo/sotn-decomp/releases/tag/cc1-psx-26). Artifact URLs and measured/published SHA-256 values are recorded in `config/compiler-artifacts.json`. They are ignored local build inputs.

Execution uses [wibo 1.2.0](https://github.com/decompals/wibo), inside Ubuntu-24.04 WSL. A native CodeWarrior invocation opened a FLEXlm License Finder and timed out; the maintained commands use the community's wibo compiler environment and do not depend on native dialogs.

Old Sony GCC frontends can fail to stat files on the Windows-mounted filesystem. The probe adapter copies the small source set and compiler package to a temporary Linux directory for each run. Missing vendor link-time `pspspecs` warnings remain in their logs; these probes stop at assembly/object generation and do not require a vendor SDK link.

SNC objects report `e_flags = 0x10A23001`. Modern GNU binutils interpret the old PSP machine code `0x00A20000` as a Loongson identifier and reject mixing it with Allegrex assembler objects. `tools/prepare_extra.py` keeps `data_install.raw.o` intact and changes only the machine field in a separate link copy to Allegrex `0x00840000`. The resulting flags are `0x10843001`. Exactly one byte of the object header changes (file offset 38); all instruction and data bytes are unchanged. Unexpected input machine flags cause failure.

The preparation script validates each contiguous C unit, rejects overlapping units, verifies compiler function order/text size, and generates assembly/link sections around every recovered range. The linker combines six C units with intervening assembly. Generated files use LF line endings explicitly for WSL. Configured C references also generate `PROVIDE` definitions for symbols that splat did not name; existing assembly labels and splat definitions remain authoritative.

For the wrappers, the raw compiler output contains `R_MIPS_26`, `R_MIPS_HI16` and `R_MIPS_LO16` records. The verifier resolves those fields using known symbol addresses and signed HI/LO addends, preserving opcode/register bits. It compares the entire resolved function with retail rather than masking address fields. Unsupported types, missing symbols, HI16 references lacking a later LO16, and jumps outside the caller's 256 MiB region fail verification. A standalone LO16 is valid: `func_089DDCE4` reuses a saved high-half register for a second context access without another HI16 record. This real sequence is checked against GNU's independently linked output and retail, and a separate check rejects an incomplete HI16.

The final check also compares each linked C function's address and body, then every byte of the packaged overlay. This prevents preserved assembly or incorrectly resolved references from being counted as recovered C.

The comparator previously sorted relocation records by instruction offset. A nonmatching loop candidate exposed a second HI16 whose instruction was later than the LO16 it was grouped with in the ELF table. That sorting was incorrect: the resolver now preserves table order. [LLVM's MIPS ELF writer](https://github.com/llvm/llvm-project/blob/main/llvm/lib/Target/Mips/MCTargetDesc/MipsELFObjectWriter.cpp) documents grouped high/low ordering. A synthetic assembly fixture reproduces the case and agrees with GNU's independent linker, including signed-low carry at address `0x12348000`. Sorting the fixture's table reproduces the old failure. The fixture is separate from the game payload and contains no game instructions.

## Repeatable commands

Required compiler and execution wrapper:

```powershell
.\.venv\Scripts\python.exe tools/setup_compilers.py
.\.venv\Scripts\python.exe tools/build_extra.py
```

Install optional candidates for the full comparison matrix:

```powershell
.\.venv\Scripts\python.exe tools/setup_compilers.py mwccpsp_147 mwccpsp_219 psp-gcc-1.3.1 psp-gcc-1.7.1
.\.venv\Scripts\python.exe tools/probe_compilers.py
```

Reproduce the callback-signature experiment and validate relocation checks:

```powershell
.\.venv\Scripts\python.exe tools/probe_callbacks.py
.\.venv\Scripts\python.exe tools/probe_debug_profile.py
.\.venv\Scripts\python.exe tools/inspect_imports.py
.\.venv\Scripts\python.exe tools/test_matching.py
```

Compare the saved nonmatching table candidate without changing build configuration:

```powershell
.\.venv\Scripts\python.exe tools/probe_candidate.py tools/probes/data_install_table.c --function func_089DD99C:0x089DD99C:492 --symbol D_089D5FF0=0x089D5FF0 --symbol func_8871674=0x08871674 --symbol func_8871684=0x08871684 --symbol func_88716C8=0x088716C8 --symbol func_886FCDC=0x0886FCDC --symbol func_886FAC0=0x0886FAC0 --symbol func_886FAA0=0x0886FAA0 --show-differences
```

The candidate probe pins the retail hash, resolves addresses exactly and writes a mismatch report under `analysis/candidate-probes/`. A successful compiler invocation is not a match result; inspect the printed result and JSON.

The compiler matrix writes `analysis/compiler-probes/results.json`; the callback experiment writes `analysis/callback-probes/results.json`; debug profiles write `analysis/debug-profile-probes/results.json`. The final build writes `build/ULUS10566/extra.verify.json`, including all twenty-four C functions and their byte count. Generated assembly and compiler output remain local.

## Validation completed

- All 26 compiler/optimization/debug combinations completed against the current six candidates.
- The expanded debug control experiment reproduced the 12/24, 24/24, 13/24 outcomes across all integrated C units.
- All twenty-four raw compiled C functions match retail after exact relocation resolution where needed.
- All twenty-four linked functions have their exact retail addresses and bytes.
- The complete rebuilt overlay is byte-identical to the original, including ELF packaging.
- A negative C candidate deliberately writing to the wrong field was rejected by the same function gate used by the build.
- The compiler setup command was rerun using its pinned archives successfully.
- Nine regression checks passed, including wrong call/global targets, a jump with identical low address bits in the wrong region, high-half reuse, an incomplete HI16 reference and grouped high relocations in nonmonotonic instruction order.
- The 492-byte table-building candidate remains nonmatching and excluded from all recovery totals.

Runtime gameplay is not newly tested here. Original ISO and extracted reference bytes remain unchanged.

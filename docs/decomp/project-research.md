# Starting a Dissidia 012 decompilation

Research and local inspection: 10 October 2026. Target: European ULES-01505, version 1.00.

**Historical research:** the current target, milestones and reporting policy are maintained in [scope and roadmap](scope-and-roadmap.md), with live results in [progress.md](progress.md). The proposed setup statements below describe the initial inspection and are not the current implementation status.

Follow-up: a subsequently supplied USA ULUS10566 v1.00 ISO matches the published retail reference and is the recommended starting input. See [the USA comparison](usa-comparison.md). BOOT.BIN is also all zeros in that verified image; this alone is not a modification indicator.

## Recommendation

Start with a matching PSP decompilation: reconstruct readable C/C++ that produces the original machine code, while retaining assembly for unfinished functions. The first useful deliverable is a reproducible, verified rebuild of one small game module. A native PC port is a later, separate undertaking involving platform services, graphics, audio, input and asset loading.

The supplied ISO is useful for preliminary analysis, but **it is modified and truncated**. Obtain a complete, unmodified EU reference dump before declaring retail byte matches or a whole-game baseline. Preliminary work on the fully present overlays can continue while that is resolved. Do not attempt to recover missing movie data by padding the image.

The full folder inventory is in [iso-inventory.json](iso-inventory.json). Only the ISO was present before this research; no existing build system or Git repository was found.

## What the local image actually contains

These findings come from direct reads of your file, rather than assumptions based on the filename.

| Property | Observed value |
| --- | --- |
| Image | `ULES-01505.ISO` |
| Title in PARAM.SFO | `DISSIDIA duodecim FINAL FANTASY` |
| Disc ID | `ULES01505` |
| APP_VER / DISC_VERSION | `01.00` / `1.00` |
| Declared minimum system version | `6.35` |
| Actual image size | 1,614,608,384 bytes |
| ISO9660 declared volume size | 1,754,529,792 bytes |
| Files in primary ISO9660 directory tree | 190 |
| SHA-1 | `2f11e41888d3fab3081360bce25dcc00cea52199` |
| SHA-256 | `0b312fad0744d99e8bc2f45ebe934f50b86444a1c5778ddd7821f75753ec27ca` |

These hashes identify this particular local image; they are **not certified retail reference hashes**. SHA-256 was independently verified using PowerShell's Get-FileHash.

`PSP_GAME/USRDIR/DATA/MOVIE/END.PMF` is declared as 161,398,784 bytes at sector 777,888. Its declared end is byte 1,754,513,408, beyond the image's actual end by **139,905,024 bytes**. Only 21,493,760 bytes of its declared extent are available. This is missing file content, not merely omitted disc padding. It is the only file whose declared extent extends beyond the image in this directory inventory. Being within bounds does not establish that the remaining files are unmodified or internally correct.

There is also direct evidence of a replaced boot path: the current `EBOOT.BIN` identifies itself as `Prometheus Loader` and contains a reference to `disc0:/PSP_GAME/SYSDIR/EBOOT.OLD`. `prometheus.prx` identifies itself as `Prometheus`. `BOOT.BIN` is entirely zero bytes. A small `PACK.bin` also contains an added distributor message. Do not treat the replacement loader as Square Enix game code.

| File inside ISO | Bytes | Initial interpretation |
| --- | ---: | --- |
| `PSP_GAME/SYSDIR/EBOOT.BIN` | 11,394 | Prometheus loader, ELF type `0xFFA0` |
| `PSP_GAME/SYSDIR/EBOOT.OLD` | 4,640,948 | Probable game executable, ELF32 little endian, MIPS, type `0x0002`; provenance needs comparison |
| `PSP_GAME/SYSDIR/BOOT.BIN` | 4,622,516 | All-zero placeholder |
| `PSP_GAME/USRDIR/DATA/OVL_EXTRA_APP.ELF` | 24,544 | Smallest game overlay candidate |
| `PSP_GAME/USRDIR/DATA/OVL_MENU_APP.ELF` | 1,556,096 | Menu overlay candidate |
| `PSP_GAME/USRDIR/DATA/OVL_BATTLE_APP.ELF` | 1,596,312 | Battle overlay candidate |
| `PSP_GAME/USRDIR/DATA/PACKAGE.BIN` | 1,034,358,784 | Main asset archive |
| `PSP_GAME/USRDIR/DATA/PACKAGE_INFO.BIN` | 132,532 | Archive index |
| `PSP_GAME/USRDIR/DATA/MODULE/LIBSUPPREACC.PRX` | 24,868 | Support library, module name `scesupPreAcc_library` |
| `PSP_GAME/USRDIR/DATA/MODULE/LIBFONT.PRX` | 20,752 | `~SCE` wrapped module |

The three overlays are ELF type `0xFFA8`. Each declares a loadable segment at **`0x089DD4C0`**, with its payload at file offset `0x54`. This supports the interpretation that different overlays occupy a shared address range; confirm the switching behavior in PPSSPP. Give each overlay its own Ghidra program and module-qualified symbols. The same address in two overlays is not necessarily the same function.

`EBOOT.OLD` has unusual metadata: entry point zero, blank section names, and two loadable segments. Their declared virtual addresses are `0x08804040` and `0x08B63170`; the second segment's memory size is much larger than its file size. These are observations, not a validated runtime map. Check how the PSP loader interprets its program headers and locate the actual module entry. Do not blindly rebase every file to the commonly suggested `0x08804000`.

No compiler comment was recovered from `EBOOT.OLD` or the three overlays. `LIBSUPPREACC.PRX` has two GCC comments: `3.3.6+allegrex-2.2.2-psp-1.14.0` and `3.3.6+allegrex-2.2.2-psp-1.15.2.internal`. They describe this support library only. **The compiler, version, flags, and C/C++ mix for Dissidia's own code remain unknown.**

## What a matching project requires

Matching decompilation reconstructs new source whose compiled output equals the target binary. It generally needs the original compiler or a compatible reconstruction. Ghidra pseudocode helps explain behavior, but does not recover the exact original source or establish a byte match. Names, types, structures and original source-file boundaries often need to be inferred. [Decompedia introduction](https://github.com/decompals/decompedia/blob/main/home.md).

For this project, the required pieces are:

1. **A fixed reference version.** Record the clean ISO hash plus hashes for the executable and every overlay. Keep any decrypted or normalized analysis copy associated with its original input and transformation procedure.
2. **A repeatable extractor.** Extract just the relevant files initially. Keep original bytes locally and never overwrite the reference. Supporting assets can remain untouched while code recovery proceeds.
3. **A correct module map.** Separate code from data, identify function boundaries, account for BSS, jump tables, imports, relocations, overlay loads and alignment. PSP uses Allegrex, so generic MIPS tools can miss platform-specific instructions.
4. **Compiler identification.** Compare candidate toolchains on several functions with meaningful control flow, calls, integer and floating-point operations. A getter matching multiple compilers is weak evidence. Record versions and flags per module or unit; vendor libraries may use another compiler.
5. **Assembly and linking.** Initially preserve unfinished functions and data, then replace functions with C/C++ incrementally. Control section order, addresses and padding. The final linked result must agree with the target, beyond any relocation-aware per-function comparison.
6. **Verification and progress.** Keep behavior checks, function matches, module byte equality and recovered-C progress distinct. An assembly-backed rebuild can match perfectly while C decompilation progress remains zero.

This is a substantial reverse-engineering project. It requires C/C++, MIPS calling conventions and delay slots, binary formats, compiler behavior, and build-system work. Begin with a small milestone before estimating the whole game's duration. No defensible completion percentage or function count exists yet.

## Tools and reference projects

| Tool | Role and specific caveat |
| --- | --- |
| Python 3 and Git | Extraction, checksums, configuration and version control. Python 3.12 and Git are available locally. |
| ISO reader: pycdlib or 7-Zip | Repeatable local extraction. The included inspector uses only Python's standard library and extracts no files. |
| Ghidra + ghidra-allegrex | Static analysis, PSP calling conventions, instructions, relocations and PPSSPP symbol exchange. Use an extension release compatible with the installed Ghidra version and its required JDK. |
| PPSSPP desktop | Breakpoints, memory inspection, runtime overlay behavior, symbol export and boot testing. |
| splat + spimdisasm/rabbitizer | Candidate PSP-aware split/disassembly pipeline and linker-script generation. Our unusual main ELF and overlay type still need an integration trial. |
| PSP-aware assembler/linker/binutils | Reassemble original instruction bytes and link units. Check Allegrex/VFPU support and instruction scheduling; an ordinary modern MIPS compiler is not sufficient. |
| Identified original compiler | The matching compiler is a research result, not an installation choice already settled. CodeWarrior PSP and historical GCC are candidates to investigate. |
| m2c | Draft C from supported assembly; manually correct types and unsupported instruction handling. |
| decomp.me / asm-differ / objdiff | Interactive function matching and local comparisons. objdiff supports PSP MIPS objects and progress reports. |
| Make or Ninja | Reproducible build, verification, and later CI. WSL/Linux is a practical option for community scripts; its installation/configuration was not verified here. |

The Allegrex extension documents PSP relocation support, symbol interchange, and VFPU decompiler limitations, including unmodeled prefix semantics. Treat vector pseudocode as analysis assistance and verify actual instructions. The `0xFFA8` overlay import path has not been tested; if ELF import fails, map its segment manually using the recorded offset and address. [ghidra-allegrex](https://github.com/kotcrab/ghidra-allegrex).

PPSSPP documents a decrypted-EBOOT dump option under Settings → Tools → Developer Tools. That is useful for a clean encrypted retail executable. For this modified ISO, confirm that a dump is the real game module rather than the replacement loader. The already visible ELF overlays provide another immediate analysis entry point. [PPSSPP developer tools](https://www.ppsspp.org/docs/development/developer-tools/).

splat explicitly supports PSP; its documented Python package is `splat64[mips]`. PSPSDK supplies open-source PSP development interfaces, but modern homebrew builds do not establish retail compiler compatibility. [splat](https://github.com/ethteck/splat), [PSPSDK](https://github.com/pspdev/pspsdk). Drafting and comparison references: [m2c](https://github.com/matt-kempster/m2c), [asm-differ](https://github.com/simonlindholm/asm-differ), [objdiff](https://github.com/encounter/objdiff).

Useful projects to study:

- **Symphony of the Night:** a concrete matching PSP EU target with extraction/build/check commands, compiler integration and PSP split configurations. Borrow its workflow and PSP build techniques after reviewing their applicability. Its related PSX source is an advantage Dissidia does not currently have. [Repository](https://github.com/Xeeynamo/sotn-decomp), [PSP workflow](https://github.com/Xeeynamo/sotn-decomp/wiki/Decompilation-(PSP-edition)).
- **Yu-Gi-Oh! Tag Force 5:** useful PSP startup documentation covering compiler clues, splitting, local matching and relocation-aware comparison. Its documented CodeWarrior configuration belongs to that game. Its notes also show why matching trivial functions cannot uniquely identify compiler flags. [Repository](https://github.com/angelof-exe/ygo-tf-5-decomp), [matching notes](https://github.com/angelof-exe/ygo-tf-5-decomp/blob/main/docs/06-splitting-and-matching.md).
- **Monster Hunter Portable 2nd G:** another PSP matching repository with configuration and build tooling to inspect. This research does not certify its completion or suitability as a drop-in template. [Repository](https://github.com/tclamb/mhp2g-decomp).

Searches did not identify an established Dissidia 012 matching decompilation. This is a bounded search finding, not proof that none exists. Asset mods, texture replacements and static recompilers answer different parts of the problem. `decomp-toolkit` and its template target GameCube/Wii, so their project organization may be informative but they are not a PSP extraction/build solution. [decomp-toolkit](https://github.com/encounter/decomp-toolkit).

## Dissidia's asset archive

There is an existing Dissidia-specific archive tool: [dissidia_filelist](https://github.com/hearhellacopters/dissidia_filelist). Its author tested it on **US ULUS10566**, so EU index compatibility and filename mappings require validation. It explains how `PACKAGE_INFO.BIN` associates hashed paths with locations in `PACKAGE.BIN`, and can extract files and reconstruct the index. This is asset unpacking, not matching executable decompilation.

For EU, inspect the index, check entry bounds, classify payload signatures and verify sample extraction before trusting the US filename list. Locate the EU file-loading function in PPSSPP to recover names dynamically. The author's sample breakpoint address is from the US game and must not be assumed valid for our EU build. Keep this work alongside executable recovery; unpacking the entire archive is not a prerequisite for the first function match.

## Proposed first milestones

### 1. Establish the reference

Obtain a complete unmodified ULES-01505 dump, inventory it, verify every file extent and preserve its hashes. PPSSPP has an official [UMD dumping guide](https://www.ppsspp.org/docs/getting-started/dumping-games/). Compare the clean executable and overlay hashes against this image's provisional inputs. Changes to the main executable are particularly plausible because the boot chain is patched.

Pass condition: a complete baseline with a documented original-to-analysis transformation. A decrypted ELF may be the executable matching target; recreating encryption and the whole ISO container is a separate packaging concern.

### 2. Map the smallest overlay

Start with `OVL_EXTRA_APP.ELF` as a size-based candidate, then check whether its functions are actually simple and reachable. Extract locally, import in a separate Allegrex program, validate its `0x089DD4C0` mapping, locate callers, and distinguish code from embedded data. Inspect the main program enough to understand how this overlay is loaded.

Pass condition: a credible map of functions, data, external references and runtime addresses. If the smallest overlay is mostly opaque runtime glue, choose a simple helper elsewhere instead.

### 3. Prove the build pipeline

Create a PSP split configuration, preserve raw headers/unknown metadata, assemble recovered instructions and link at the recorded addresses. Preserve untouched bytes while gradually converting identified regions into assembly. Exact-copy reconstruction checks extraction coverage; an actual assemble/link check proves the toolchain can reproduce instruction encodings and layout.

Pass condition: a reproducible overlay rebuild and exact comparison against the verified reference, with clear documentation of which regions remain raw blobs.

### 4. Identify compiler behavior and match C

Compile candidate source with plausible historical PSP toolchains. Use several discriminating functions to narrow compiler and flags. Then replace one well-understood helper with C and verify its generated instructions and final linked bytes. Do not declare a compiler identified from a single trivial match or from the support library's GCC comments.

Pass condition: at least one integrated C match, with a recorded toolchain hypothesis and evidence. Continue narrowing any unresolved version or flags.

### 5. Scale and publish progress

Grow into main-program utilities, menu behavior and battle code. Use neutral address-based names until behavior is understood, and maintain module identity in symbols. Keep assembly fallbacks out of reported C recovery. Test representative startup, menus, combat and saves as the rebuild develops.

For an eventual public project, keep the ISO, extracted binaries, assets and private compiler packages as local inputs. Track your source, scripts, configuration and research. Require contributors to provide their own inputs; do not upload the game image to CI.

A proposed later layout is `src/`, `include/`, `config/`, `tools/`, `docs/`, with ignored local `orig/`, `extracted/`, `asm/`, `assets/`, `analysis/` and `build/`. The current files are research groundwork, not a working game build.

## Joining decomp.dev

decomp.dev publishes measured progress. Once the project has target and source-built objects, generate `objdiff.json` for all units and emit a report with `objdiff-cli report generate -o build/report.json`. Upload that report from default-branch CI as an artifact named `VERSION_report`, for example `ULES01505_report`. A repository administrator can then add the project at `https://decomp.dev/manage/new`. The GitHub app supports faster refresh and PR progress comments. [Official linked integration guide](https://github.com/decompals/decompedia/blob/main/tools/decomp-dev.md).

Build progress objects without unfinished assembly fallbacks so preserved instructions are not counted as recovered C. Plan for separate main/overlay and game/library categories. ISO asset sizes should not dominate the executable-code denominator.

## Reproduce this inspection

From the project root in PowerShell:

```powershell
python tools/inspect_iso.py ULES-01505.ISO --output docs/decomp/iso-inventory.json
Get-FileHash -LiteralPath ULES-01505.ISO -Algorithm SHA256
```

The script reads ISO9660 directory records, PARAM.SFO, ELF headers/sections, executable hashes and extent bounds. It writes metadata only, and never extracts or executes game content. It supports ordinary single-extent ISO9660, not CSO/UDF or multi-extent files. Directory and ELF metadata parsing is an initial inspection, not a complete disc-integrity or code-validity audit.

No emulator boot, Ghidra import, compiler trial, decryption, source recovery or matching rebuild was performed in this research. Those are the next implementation milestones.

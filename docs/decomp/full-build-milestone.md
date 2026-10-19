# Complete scoped binary build foundation

**Later source milestone:** [the three-module C expansion](source-recovery-milestone.md)
brings current recovery to 102 functions / 5,228 bytes while retaining all four
byte-identical builds. The figures below preserve the first complete-build snapshot.

Major milestone, 11 October 2026. **All four primary target files now rebuild byte-for-byte**: the decrypted main executable and the three overlays. Main integrates its first **15 matching C functions / 836 bytes**, bringing project recovery to **60 functions / 3,244 bytes**.

This completes the fixed-address binary build foundation. Most C/C++, full code/data audits, encrypted executable/ISO packaging and gameplay scenario validation remain unfinished. File equality is build fidelity, not 100% source recovery.

## Verified files

| Target | Matching C functions | Matching C bytes | Full file |
| --- | ---: | ---: | --- |
| Main `EBOOT.ELF` | 15 | 836 | Byte-identical |
| Extra overlay | 24 | 2,116 | Byte-identical |
| Menu overlay | 11 | 180 | Byte-identical |
| Battle overlay | 10 | 112 | Byte-identical |
| Total | **60** | **3,244** | **4 / 4 verified** |

Main output is `build/ULUS10566/EBOOT.ELF`: **4,640,948 bytes**, SHA-256 `0bc8e779aad2814a3d1bd77346f69ca5e16665845ed053cd0ad44a9cbfaefe6d`. It matches the pinned decrypted USA reference, not a newly encrypted `EBOOT.BIN`. The [overlay milestones](menu-build-milestone.md) record their file identities and explicit preservation policies.

## Main is a two-segment executable

The [layout audit](main-layout-notes.md) establishes two initialized `PT_LOAD` spans:

| Segment | Virtual address | Original file offset | Initialized bytes | Total memory bytes |
| ---: | --- | ---: | ---: | ---: |
| 0 | `0x08804040` | 4,120 | 1,938,304 | 1,938,304 |
| 1 | `0x08B63170` | 1,945,600 | 256 | 16,833,424 |

The build emits **1,938,560 initialized load bytes** in two independent fixed-address outputs. It never flattens the VMA gap or materializes BSS. The **16,833,168 memory-only bytes** retain their original program/section-header representation and do not become file or source-recovery bytes.

Packaging starts from the pinned original file and replaces only the two rebuilt initialized spans. All **2,702,388 bytes outside those spans** remain original, including headers, file gaps, register information and tail/section metadata. The verifier independently parses the actual load geometry and rejects BSS reads, crossing ranges, overlap and changes to the complete file.

The original declared alignment has noncongruent file/address residues. The pipeline preserves this observed PSP ELF layout rather than imposing a generic ELF rewrite. No meaning beyond the inspected layout is assigned to the unused file regions or unusual physical-address field.

## Main source and assembly

Eight C units use SNC 1.2.7503.0 **`-O2 -g`**:

| Recovered behavior | Functions | Bytes |
| --- | ---: | ---: |
| Callback-word setter | 1 | 16 |
| Test a state word for nonzero | 1 | 12 |
| Two state-word writes and a two-field initializer | 3 | 40 |
| Release wrapper around an existing main helper | 1 | 28 |
| Float-record initializer, setter and four getters | 6 | 64 |
| Menu/battle/extra shutdown callbacks clearing global words | 3 | 676 |

The three shutdown callbacks clear the 83 word addresses established in the [overlay dispatch audit](overlay-dispatch-audit.md). They are main lifecycle callbacks, separate from the overlays' descriptor hooks. Source comments record their table/caller evidence and keep unknown globals neutral.

`include/main/state_prefixes.h` models observed fields only. It does not establish complete object sizes or original C++ classes. The float initializer writes an integer sentinel and two zero floats; the third float remains untouched, matching the original instructions. The release wrapper retains its existing dependency instead of inventing an implementation for the callee.

Each raw C function passes fully resolved, unmasked relocation comparison. Linked symbols retain the exact original addresses and bytes. Raw compiler and assembler objects are preserved; only a separate link copy's historical machine identifier is normalized. The **296 SDK import stubs earn no C credit**, with an exclusion derived independently from the ELF import declarations and checked by preparation as well.

Main initialized-load accounting is:

| Contribution | Bytes |
| --- | ---: |
| Matching C | 836 |
| Reassembled inventory instructions | 1,790,656 |
| Explicit raw fallback instruction words | 1,352 |
| Preserved original data/unclassified loaded bytes | 145,716 |
| Total initialized load spans | **1,938,560** |

The 338 raw words primarily cover source-prefix encodings and `vuc2ifs` forms rejected by the installed historical assembler. Their addresses, words and reasons remain explicit in local `main.prepare.json`. Compatible numeric control-register syntax handles `cfc1`/`ctc1` without a raw fallback. Raw words and copied bytes earn no C or data-source credit, and the broader instruction classifications remain provisional.

## Validation and remaining work

**94 checks pass** across all four builds, inventories, callback paths, relocation handling and progress integrity. Main-specific checks cover whole-file equality, memory-only BSS, the VMA gap, range crossings, unusual original alignment, changed tail bytes, incorrect compiled C behind unchanged output, SDK import relabeling, prepared-part kinds, stale source/header/layout proof and the four-module report.

Source/tool fingerprints include C units, headers, compiler, assembler, wibo, local objcopy, maintained build inputs and canonical instruction/load geometry. Inventory source labels can update without a proof cycle; a changed layout invalidates the build. A separate read-only review found no blocking verification or source-attribution issue.

Inventory revision 8 retains **21,279 candidates / 4,717,900 function-body bytes**, explicit unknown regions and two untraced battle additions. Main/menu/battle full-layout audits are still incomplete, so the audited whole-game code percentage remains unknown. Extra remains **9.28% matching by reviewed code bytes**. Feature ranks reflect evidence, not equal percentages of completion.

Original input binaries, source ISOs and annotated analysis projects are preserved. The support PRXs remain auxiliary dependencies outside the four-file build scope. At this build milestone no game ISO had been repackaged or gameplay scenarios run. The subsequent [runtime milestone](runtime-baseline.md) adds local decrypted packages and first-screen boot evidence. No newly encrypted retail executable, repository publication or decomp.dev upload has been produced.

## Reproduce

```powershell
.\.venv\Scripts\python.exe tools/build_extra.py
.\.venv\Scripts\python.exe tools/build_battle.py
.\.venv\Scripts\python.exe tools/build_menu.py
.\.venv\Scripts\python.exe tools/build_main.py
.\.venv\Scripts\python.exe tools/build_inventory.py
.\.venv\Scripts\python.exe tools/progress.py
.\.venv\Scripts\python.exe -m unittest discover -s tools -p 'test_*.py'
```

`tools/verify_main.py` rechecks the existing main result independently. The compiler-only main pilot is `tools/probes/main_pilot.py`; it is supporting evidence, not a substitute for the complete build.

Next: establish a reversible packaging/emulator workflow for the original-versus-rebuilt boot/menu/battle baseline, alongside broader source recovery and code/data audits. Keep the data-install loop mismatch bounded and preserve the working builds as the reference for subsequent changes.

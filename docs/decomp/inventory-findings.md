# First whole-scope function inventory

Inventory revision 1, 11 October 2026. All four references are hash-pinned USA ULUS10566 v1.00 inputs. The maintained metadata is under `config/inventory/`; raw Ghidra output and projects remain under ignored `analysis/`.

**Revision 2 update:** the [extra-overlay layout audit](extra-layout-audit.md) resolves its 1,105 unknown bytes, reviews all 100 boundaries and establishes a 22,796-byte code denominator. The extra module is now 9.28% matching C by reviewed code bytes. Other module classifications below remain provisional; the revision 1 tables are retained as the original snapshot.

## Results

**Revision 8 update:** the [complete scoped build milestone](full-build-milestone.md) integrates fifteen main C functions and verifies all four files. Function totals and unknown-byte partitions remain unchanged; current matching recovery is **60 functions / 3,244 bytes**. Main/menu/battle full-layout audits remain provisional, and SDK import stubs still receive no source credit.

**Revision 7 update:** the [menu build milestone](menu-build-milestone.md) integrates eleven C functions and verifies the third overlay. Function totals and unknown-byte partitions are unchanged; current matching source totals are **45 functions / 2,408 bytes**. Main/menu/battle full-layout classifications remain provisional.

**Revision 6 update:** [deferred battle setup](deferred-battle-audit.md) adds 24 candidates: 22 traced and two explicitly untraced. Current totals are **21,279 candidates / 4,717,900 function-body bytes**. The [battle build milestone](battle-build-milestone.md) integrates ten C functions separately from that mapping audit; project matching recovery is now 34 functions / 2,228 bytes. Historical tables below remain snapshots.

**Revision 5 update:** the [overlay dispatch audit](overlay-dispatch-audit.md) establishes the menu prefix's caller and adds 63 candidates across main/menu/battle. Current totals are **21,255 candidates / 4,716,320 function-body bytes**, with 236,992 unknown executable-section bytes. Matching-C credit is unchanged; the historical tables below remain revision 1 snapshots.

**Revision 4 update:** the [menu data review](menu-data-audit.md) validates 60 sentinel tables and configuration data, resolving 8,033 previously unknown bytes. Function counts and matching-C credit are unchanged. The menu module remains provisional; current unknown-byte totals appear in [progress](progress.md).

**Revision 3 update:** the [menu/battle hook audit](overlay-hook-audit.md) adds 26 loader-declared functions and validates the initializer tables and cleanup records. Current totals are **21,192 candidates / 4,711,248 function-body bytes**. The tables below remain the revision 1 snapshot; use [current progress](progress.md) for current totals.

| Module | Ghidra entries | Added declared/reviewed entries | Current candidates | Function-body bytes, union |
| --- | ---: | ---: | ---: | ---: |
| Main executable | 8,583 | 34 ELF-declared imports | 8,617 | 1,782,824 |
| Extra overlay | 97 | 3 reviewed leaf routines | 100 | 22,796 |
| Menu overlay | 6,270 | 0 | 6,270 | 1,405,524 |
| Battle overlay | 6,179 | 0 | 6,179 | 1,499,460 |
| Total | 21,129 | 37 | **21,166** | **4,710,604** |

These are module-qualified candidate entries, not 21,166 verified original source functions or distinct gameplay algorithms. Library code may be duplicated across binaries. Each function retains its exact address ranges, including noncontiguous bodies, confidence and available direct call targets. The three overlays have overlapping runtime addresses, so identities include the module name.

No overlapping function-body bytes were found in this revision. Accounting uses the union of ranges, so later shared tails or overlapping annotations cannot inflate the denominator.

## Checks and corrections

The original extra-overlay program was processed read-only with analysis disabled. Main/menu/battle were imported into separate Ghidra projects using the installed Allegrex processor. Their automatic analysis finished without the configured 600-second timeout. The exporter records metadata only; it does not bulk-export decompiler source or change annotations. Normal Ghidra analyzers may use the decompiler internally, and their results remain provisional.

The validator checks the Ghidra input SHA-256 against the reference, function identities, entries and body lengths, and file-backed executable-section bounds. It reconciles all 24 integrated C functions against exact contiguous ranges and the existing retail comparison. The eight-byte PSP import stubs are validated against ELF-declared import locations and their actual encoding.

The main executable declares 296 PSP imports. Ghidra had made functions for 262 of them, leaving 34 unused stubs out of its function list. Those 34 were added using the import records and checked stub bytes, with primary ownership `platform_imports`. They contribute no matched C bytes.

For the extra overlay, all 97 Ghidra boundaries agreed in size with spimdisasm. Spimdisasm proposed four additional entries. Manual review accepted three:

- `extra:0x089DE668`, eight bytes: returns the first argument through a `jr ra` delay slot.
- `extra:0x089E0DFC`, 28 bytes: a pointer-chain flag update with its own return and store delay slot.
- `extra:0x089E0E18`, 28 bytes: an adjacent, independent flag update with another return.

The proposed four-byte function at `0x089DD4C0` was rejected as a function boundary. The leading word is 3 followed by zero fill; its broader prefix/padding role has not been established. The 16-byte prefix remains unclassified, rather than being credited as a function or silently discarded. Reviewed additions and exclusions are recorded in `config/inventory-overrides.json`, pinned to the reference hash and checked for conflicts. None of the three additions is counted as recovered C.

## Remaining classification work

| Module | Identified instruction bytes | Identified data within executable sections | Unclassified executable-section bytes |
| --- | ---: | ---: | ---: |
| Main | 1,792,508 | 0 | 49,376 |
| Extra | 22,796 | 411 | 1,105 |
| Menu | 1,410,324 | 20,667 | 124,873 |
| Battle | 1,503,192 | 17,116 | 75,772 |
| Total | 4,728,820 | 38,194 | **251,126** |

Together these partitions cover all 5,018,140 file-backed executable-flagged section bytes. Identified instruction counts combine Ghidra decoding with checked import stubs and reviewed leaf ranges. They are not a certification that every decoded instruction is reachable game code. Identified data is also provisional. Unknown bytes may include tables, constants, padding, missing code or overlay descriptors; they remain part of the scope.

The main analysis reported a decompiler flow warning at `0x08926DFC`, following a target outside mapped memory. That function is an audit priority. The menu/battle leading words and pointers also indicate that their executable sections contain descriptor data; do not infer pure code from section flags or from the first detected function alone.

Feature ownership remains mostly unclassified. Only the 24 already matched data-install functions and the ELF-declared import functions have supported primary ownership tags in this revision. Module names do not automatically establish story, combat, menu or cutscene ownership.

## Progress interpretation

The matching numerator remains **24 functions / 2,116 C bytes**. Across the provisional function-body union, coverage is **0.0449%**. The audited whole-game headline remains unknown pending classification and boundary review.

The extra-overlay provisional denominator increased from 97 functions / 22,732 bytes to 100 functions / 22,796 bytes. Its count coverage is now **24.00%**, and function-byte coverage **9.28%**. The numerator has not regressed. Its payload coverage is still **8.70%**, and the full overlay still rebuilds byte-identically. See [current progress](progress.md).

## Reproduce and inspect

First export the inventories:

```powershell
.\tools\run_inventory.ps1 -Module main -ProjectName dissidia-main-dispatch-v1
.\tools\run_inventory.ps1 -Module menu -ProjectName dissidia-menu-dispatch-v1
.\tools\run_inventory.ps1 -Module battle -ProjectName dissidia-battle-deferred-v1
.\tools\run_inventory.ps1 -Module extra
.\.venv\Scripts\python.exe tools/build_inventory.py
.\.venv\Scripts\python.exe tools/progress.py
```

When a project exists, the command exports it read-only without reanalysis. Close that project in the GUI before running headless export. To perform a fresh import while preserving annotations, supply a new project name, for example `-ProjectName dissidia-main-review`; the reference must still pass its pinned hash check.

Open a saved project with `.\tools\open_ghidra.ps1 -Module main` (or `menu`, `battle`, `extra`). Reviewed metadata additions do not silently create functions in the existing Ghidra database; the three extra leaves are recorded in the maintained inventory and review notes.

Run `tools/test_inventory.py`, `tools/test_progress.py` and `tools/test_matching.py` for range accounting, progress integrity and the existing matching checks.

## Next audit queue

1. Extra code/data partition and boundary review completed in revision 2; continue semantic ownership and source recovery separately.
2. Review the main flow-warning site and reconcile executable-section gaps with symbols, imports, jump tables and padding.
3. Deferred battle callbacks and guarded setup are checked in revision 6. Continue data gaps and representative vector-code regions using the recovered initialization graph; keep the two untraced candidates provisional.
4. Connect verified subsystem entry points to feature owners using call references and runtime evidence. Keep shared services and unresolved ownership visible.
5. Establish assembly-backed builds for the larger modules after their split maps are sufficiently credible.

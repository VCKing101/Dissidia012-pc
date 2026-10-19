# Third byte-identical overlay build

**Current source update:** [the source expansion](source-recovery-milestone.md)
brings menu to 17 matching functions / 580 bytes, including all eight descriptor
initializers. This page retains the initial menu build snapshot.

**Later milestone:** the [main executable now rebuilds exactly](full-build-milestone.md), completing all four scoped file builds and bringing current recovery to 60 functions / 3,244 bytes. The figures below retain this third-build snapshot.

Major build-foundation milestone, 11 October 2026. **The full USA menu overlay now rebuilds byte-for-byte**, with **11 matching C functions / 180 bytes** integrated. Extra, battle and menu are all verified: **three of the four scoped modules** now have complete file builds. The project total is **45 exact C functions / 2,408 bytes**.

This is a build-fidelity milestone. Most source, full menu/battle code-data audits, gameplay scenarios and the main executable build remain unfinished. Three verified binaries do not mean 75% of the game is decompiled.

## Verified output and accounting

Output: `build/ULUS10566/OVL_MENU_APP.ELF`.

| Measure | Result |
| --- | --- |
| Complete file | 1,556,096 bytes, identical to retail |
| SHA-256 | `f021f96fa05da5907563a28c232966aa8a344526d6b8bdee728053ec75224d0a` |
| Loaded payload | 1,555,864 bytes at `0x089DD4C0` |
| Matching C | 11 functions / 180 bytes |
| Reassembled inventory instruction bytes | 1,410,752 |
| Explicit raw fallback words | 16 words / 64 bytes |
| Preserved original payload bytes | 144,868 |
| Preserved ELF header/metadata | 232 bytes |

The four payload categories sum exactly to 1,555,864 bytes. Linker assertions preserve every assembly chunk and C unit's start/end address, and the complete payload extent. Full-file equality includes the original ELF header and metadata.

## Menu source recovery

Eight units compile using the already verified SNC 1.2.7503.0 **`-O2 -g`** profile:

| Behavior | Functions | Bytes |
| --- | ---: | ---: |
| Descriptor initializers set observed global words to one | 5 | 80 |
| Getters return reviewed configuration-data addresses | 5 | 60 |
| Float initializer doubles one value and stores two fields | 1 | 40 |

The float initializer at `menu:0x08A93DF4` reads `0x08A93E20`, multiplies by `2.0f`, and stores to `0x08A94670+0x20` and `+0x60`. The layout and callers are documented in the [menu data review](menu-data-audit.md). Its precise object purpose and other fields remain unknown.

`include/menu/configuration.h` supplies one compatible partial-object declaration to both the initializer and its getter. Review found the first draft declared that object with incompatible external types in separate units; the shared declaration fixes that issue while preserving all 11 byte matches. The struct describes observed fields only; its size is not evidence of the complete original object size.

Sources under `src/menu/` keep neutral names for unknown flags and data. Actual descriptor hooks or callers support these entry points. Raw compiled functions are compared after resolving every relocation, then linked bytes and addresses are checked independently. Nothing is masked, and original raw objects are preserved separately from normalized linker copies. These pilot matches do not establish the toolchain for every menu function.

## Explicit fallbacks and preserved data

The installed historical PSP assembler handles the scalar and VFPU code. It rejects ten `lwl`/`lwr` words in the current instruction map, including bodies at `0x08A2907C` and `0x08A7CBE4`; those words remain explicit raw fallbacks. Six `0xFFFFFFFF` words identified as instructions by Ghidra are also preserved raw and may be data. Addresses and reasons appear in local `build/ULUS10566/menu.prepare.json`.

The 144,868 preserved payload bytes include reviewed data, padding and still-unclassified regions. None of these bytes, or the 64 raw fallback bytes, earns matching-C credit. Exact output does not prove that all provisional instruction ranges are code, so menu's audited code percentage remains pending.

## Verification and progress

**83 checks pass**, including all existing extra/battle checks and new menu rejection cases. The tests reject a changed output byte, another overlay's binary, incorrect raw C behind unchanged file bytes, stale shared-header provenance, duplicate unit names before compilation, and prepared C regions that misidentify their actual units.

The verifier requires complete retail equality, raw relocation-resolved C matches, original linked addresses, nonoverlapping C ranges, canonical layout identity, exact payload partition/accounting and C-part identities matching the unit manifest. Source/header, compiler, assembler, wibo, build scripts and layout fingerprints must remain current. Inventory confidence/owner updates do not cause a build-report cycle; instruction-range changes invalidate the proof.

The function inventory remains **21,279 candidates / 4,717,900 function-body bytes**. This step changes source attribution without inventing new entries or removing unknown bytes. Menu initialization/configuration is now ranked **mapped in part**, with its recovered leaf helpers counted separately from broader flow ownership. No menu mode, stage or whole-game completion percentage is claimed.

Input binaries and source ISOs are preserved. Existing annotated Ghidra projects remain intact. No emulator gameplay, ISO repackaging, repository publication or decomp.dev upload was performed.

## Reproduce

```powershell
.\.venv\Scripts\python.exe tools/build_menu.py
.\.venv\Scripts\python.exe tools/build_inventory.py
.\.venv\Scripts\python.exe tools/progress.py
.\.venv\Scripts\python.exe -m unittest discover -s tools -p 'test_*.py'
```

`tools/verify_menu.py` independently rechecks existing output. `tools/probes/menu_pilot.py` repeats the compiler-only experiment; it supports the source result and does not replace whole-file integration.

Next major milestone: reconstruct the decrypted main executable with the same fixed-address verification and explicit preservation policy. Completing all four scoped builds will establish the full build foundation, while source recovery and runtime scenario validation continue separately.

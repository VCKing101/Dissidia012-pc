# Menu and battle loader-hook audit

Inventory revision 3, 11 October 2026. The loader's tables expose **26 functions missing from the first automatic inventories**, adding 644 function-body bytes. These entry points now appear in the maintained maps. No additional C has been matched; menu/battle boundaries and their full code/data layouts remain provisional.

## Descriptor and consumer evidence

Each overlay's 16-byte descriptor starts at `0x089DD4C0`. The format is `{u32 tag, u16 initializer_count, u16 cleanup_count, u32 initializer_table, u32 cleanup_table}`. Main `0x089331FC` loads the ELF segment and calls `0x0893F810` with its base. That walker reads count at +4, table at +8 and calls each u32 entry. Main `0x0893F888`, reached through `0x0893335C`, reads count at +6 and table at +12. Each cleanup table entry points to a 12-byte record; the walker loads a1 from +0, a0 from +4 and the callback from +8.

These are observed initialization/cleanup roles. The original source's type names and tag meanings remain unknown. See the [extra audit](extra-layout-audit.md) for the loader trace.

| Module | Tag | Initializers | Initializer table | Cleanup records | Cleanup table |
| --- | ---: | ---: | --- | ---: | --- |
| Extra | 3 | 0 | null | 0 | null |
| Menu | 2 | 8 | `0x08B59238` | 0 | `0x08B59258` |
| Battle | 1 | 21 | `0x08B62F08` | 5 | `0x08B62F5C` |

Menu's empty cleanup table points exactly one past its loaded payload. A zero count causes no read, so this is valid. Both nonempty battle tables are completely file backed.

## Function-map corrections

The original menu map contained one of eight initializer entries; the battle map contained six of 21. New, separate Ghidra projects were imported from the hash-pinned references, then seeded at those loader-declared entries and the four local cleanup callbacks. Existing projects and annotations were preserved. Control-flow disassembly supplies provisional bodies; table references establish entry points, not complete source recovery or an independent boundary audit.

| Module | Added initializers | Added cleanup functions | Added function bytes | Current candidates |
| --- | ---: | ---: | ---: | ---: |
| Menu | 7 | 0 | 152 | 6,277 |
| Battle | 15 | 4 | 492 | 6,198 |

Together with main and extra, the inventory now contains **21,192 module-qualified candidates / 4,711,248 function-body bytes**. There are no overlapping function bodies. Most of the added bodies were already decoded instructions outside named functions; this correction does not imply 644 previously unknown bytes were classified.

## Cleanup records and namespaces

| Battle record | a1 | a0 | Callback |
| --- | --- | --- | --- |
| `0x08A4AAD0` | 0 | 0 | `battle:0x08A48E78` |
| `0x08A4AADC` | 0 | 0 | `battle:0x08A48E80` |
| `0x08A4DA80` | 0 | 0 | `battle:0x08A4CCD8` |
| `0x08A4DA8C` | 0 | 0 | `battle:0x08A4CCE0` |
| `0x08B125CC` | 0 | `0x09B6C2B0` | `main:0x0887A644` |

The first four callbacks are separate eight-byte `jr ra; nop` functions. Bounded disassembly independently confirms their encodings. The fifth callback resolves to the existing main-executable function. Its object argument lies outside the file-backed overlay; it is an argument value, not an instruction entry or another overlay function. The five records remain data, including their embedded code pointers.

`tools/audit_layout.py` validates descriptor/table bounds and alignment, all initializer entry points, cleanup-record bounds, local/main callback resolution and nonoverlapping data objects. `tools/build_inventory.py` checks these data regions against all decoded instructions and function bodies before combining them with existing defined data. The maintained `hook_audit` contains every resolved entry, argument, data region and consumer address; qualifying overlay function entries receive `entry_review`. This partial audit does not set the module's `audited` flag.

## Progress and validation

Matching recovery remains **24 C functions / 2,116 bytes**, all in extra, which is **9.28%** of its reviewed code. The existing extra build still passes a full-file retail comparison. Whole-game audited completion and game-feature percentages remain unavailable. Unknown executable-section bytes total **249,897**: main 49,376, menu 124,857 and battle 75,664.

All **36 inventory, progress and matching checks pass**, including rejection of interior initializer entries, out-of-bounds records, callbacks into data, missing main callbacks, overlapping hook data, unaligned tables and truncated descriptors. They also verify that these corrections add no matched C credit.

## Reproduce

Create the hook-seeded projects only once, with fresh names:

```powershell
.\tools\run_inventory.ps1 -Module menu -ProjectName dissidia-menu-hooks-v1 -SeedOverlayHooks
.\tools\run_inventory.ps1 -Module battle -ProjectName dissidia-battle-hooks-v1 -SeedOverlayHooks
```

Seeding an existing project is refused. To refresh inventories from the projects already created here, export read-only:

```powershell
.\tools\run_inventory.ps1 -Module main
.\tools\run_inventory.ps1 -Module extra
.\tools\run_inventory.ps1 -Module menu -ProjectName dissidia-menu-hooks-v1
.\tools\run_inventory.ps1 -Module battle -ProjectName dissidia-battle-hooks-v1
.\.venv\Scripts\python.exe tools/build_inventory.py
.\.venv\Scripts\python.exe tools/progress.py
.\.venv\Scripts\python.exe -m unittest discover -s tools -p 'test_*.py'
```

Close the selected Ghidra project in the GUI before headless exports. Open a seeded project with `.\tools\open_ghidra.ps1 -Module menu -ProjectName dissidia-menu-hooks-v1` (or the equivalent battle name). Local binaries, disassembly and Ghidra databases remain ignored by Git.

Revision 4 completes the first [menu data-region review](menu-data-audit.md), validating 60 tables and configuration data. Its unknown-byte and test totals supersede this revision 3 snapshot. Next: investigate the undecoded menu prefix and remaining pointer/dispatch tables, then continue battle and main gap audits. Assembly builds for the larger modules depend on these split maps; feature ownership and runtime scenarios remain separate work.

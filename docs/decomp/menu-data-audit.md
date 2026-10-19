# First menu data-region review

**Later source/build milestone:** the [menu overlay now rebuilds byte-for-byte](menu-build-milestone.md), with eleven matching C functions using these reviewed data references. The data-review figures below remain the revision 4 snapshot.

Inventory revision 4, 11 October 2026. Two selected regions within menu data islands now have repeatable data checks: **8,265 data bytes and five zero padding bytes**, of which **8,033 bytes were previously unclassified**. Menu unknown executable-section bytes fall from 124,857 to **116,824**. This is a partial layout review; the whole menu module remains provisional.

## Regions and evidence

All endpoints below are exclusive. Addresses belong to the menu overlay loaded at `0x089DD4C0`.

| Region | Bytes | Classification | Evidence |
| --- | ---: | --- | --- |
| `0x08A08EF8..0x08A08FD9` | 225 | Six ASCII resource paths | Terminated `menu/… .bin` paths, including Colosseum UI resource names |
| `0x08A08FD9..0x08A08FDA` | 1 | Zero padding | Byte value checked |
| `0x08A08FDA..0x08A0A672` | 5,784 | 60 halfword record tables | Table arguments traced to shared readers in main; record steps and sentinel checks verified |
| `0x08A93E1C..0x08A93E20` | 4 | Zero padding | Gap after the initializer's return/delay slot; byte values checked |
| `0x08A93E20..0x08A946F0` | 2,256 | Numeric configuration data | Pointer getters, indexed halfword accesses, float initializer reads/writes and an embedded initializer pointer |

The resource names are a clue to Colosseum UI ownership. They do not establish a complete game-mode function set or its completion percentage. The first selected region ends where another halfword lookup starts; further adjacent data remains outside this review.

## Sentinel table formats

The tables have **930 records including their 60 terminal records**. They are not one uniform format:

| Record bytes | Tables | Consumer-visible stop field |
| ---: | ---: | --- |
| 4 | 6 | First halfword equals `0xFFFF` |
| 6 | 2 | First halfword equals `0xFFFF` |
| 8 | 52 | Either first halfword or halfword at +4 equals `0xFFFF`, depending on the reader |

Eight main-executable readers corroborate the formats. `0x088D3910`, `0x088D3B88` and `0x088D3C24` step four bytes; `0x088D3F7C` steps six. `0x088D3CF0` and `0x088D3F00` step eight and test the first halfword; `0x088D428C` and `0x088D44EC` step eight and test +4. The same eight-byte tables may contain `0xFFFF` in other fields without ending the walk. Some terminal records end with a zero halfword rather than all-ones bytes.

For each table, the maintained review records its start/end, format, a representative caller's pointer construction and call site, and the shared reader. Validation checks the reader's halfword load, sentinel constant/comparison and cursor increment against the pinned main executable. It then checks that the table ends at its first consumer-visible sentinel. All 60 table spans abut, with no internal gaps or overlaps.

One table at `0x08A0A192` uses a saved register rather than constructing a2 immediately before the call. Menu `0x089FF848` materializes the pointer in s1 at `0x089FF874/0x089FF880`; `0x089FF9D8` moves it to a2 before calling main `0x088D428C` at `0x089FF9E0`. The function preserves s1 and retains that value across its intervening calls. This case was traced separately rather than inferred from adjacent pointer patterns.

## Configuration data and initializer

Menu initializer `0x08A93DF4` reads the single-precision constant at `0x08A93E20`, multiplies it by two, and stores the result at `0x08A94690` and `0x08A946D0`. The source constant is approximately pi. Getter `0x08A93DDC` returns the surrounding object base, `0x08A94670`; the two stores are at offsets +0x20 and +0x60. Two adjacent 64-byte blocks are visible, but their original type and field names remain unknown.

Other getters return `0x08A93E24`, `0x08A9443C`, `0x08A94568` and `0x08A94638`. The accessors at `0x08A93D84` and `0x08A93DAC` use four- and eight-byte indexed halfword accesses. Those observations establish data use without proving all original array lengths or semantic fields. The review therefore classifies the encompassing configuration region as data and leaves its detailed type recovery open.

The word at `0x08A94668` points back to the already-known initializer `menu:0x08A93DF4`. It remains data; finding a code pointer adds no new function or matched source credit. The validated instruction and function maps contain no code within either reviewed data region.

## Inventory and progress

Menu now accounts for its 1,555,864-byte executable-flagged payload as follows:

| Classification | Bytes |
| --- | ---: |
| Decoded instructions | 1,410,324 |
| Defined/reviewed data | 28,711 |
| Reviewed zero padding | 5 |
| Still unclassified | 116,824 |

The first three categories plus unknown bytes partition the entire payload without overlap. Function bodies remain **6,277 candidates / 1,405,676 bytes**. The project-wide inventory remains **21,192 candidates / 4,711,248 function-body bytes**. Unknown executable-section bytes across main/menu/battle total **241,864**.

Matching source remains **24 C functions / 2,116 bytes**, all in extra, which is **9.28%** matching by reviewed code bytes. The full existing extra build still passes the retail comparison. These data classifications add no C recovery. Whole-game audited completion and feature percentages remain unavailable.

## Tooling and checks

`config/inventory-overrides.json` contains the hash-pinned review metadata. `tools/audit_menu_data.py` verifies the references and formats; `tools/build_inventory.py` refuses any reviewed data or padding that intersects decoded instructions or function bodies. Maintained `config/inventory/menu.json` includes the partial `data_island_audit` and revised ranges, while keeping `audited: false`.

All **45 inventory, progress and matching checks pass**. New rejection cases cover early/missing sentinels, the wrong sentinel field, changed reader steps, calls into a different reader, an interior code pointer, a data review swallowing the following function, and a mismatched main reference. The integration check confirms that the partial review adds no matched C or function-count credit.

```powershell
.\tools\run_inventory.ps1 -Module menu -ProjectName dissidia-menu-hooks-v1 -AuditReferences
.\.venv\Scripts\python.exe tools/build_inventory.py
.\.venv\Scripts\python.exe tools/progress.py
.\.venv\Scripts\python.exe -m unittest discover -s tools -p 'test_*.py'
```

Reference export is read-only and provides inspection evidence under ignored `analysis/`. The validator can regenerate maintained classifications from the pinned binaries, function maps and review metadata without needing that reference export. Close the chosen Ghidra project in the GUI before a headless export. Input binaries and existing annotations are preserved.

Revision 5's [overlay dispatch audit](overlay-dispatch-audit.md) establishes the menu prefix's caller and adds 63 candidates across main/menu/battle. Its current totals supersede this revision 4 snapshot. Next: trace deferred battle setup and callback registration, then continue data-gap classification using the newly recovered initialization graph.

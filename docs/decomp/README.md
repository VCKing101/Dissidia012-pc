# Dissidia 012 matching decompilation

Initial build target: **USA PSP release ULUS10566, version 1.00**.

**Project direction:** the main goal is now the [native PC port](../port/README.md). The matching-decompilation status that follows remains accurate and is the reference for what the original code does. (A PPSSPP-plugin mod and a new-character experiment were also built on this foundation; they are kept out of the public repository.)

The [working environment](getting-started.md) is ready: **the decrypted main executable and all three overlays rebuild byte-for-byte**, with **14,141 matching C/C++ functions / 1,393,272 bytes** integrated (29.53% of inventoried function bytes). Another **35 functions / 2,460 bytes** use the PSP's vector unit through inline assembly; they are built and verified the same way but [counted separately](scope-and-roadmap.md#what-the-percentages-mean). The [fast matching workflow](fast-matching.md) added almost all of these on 12–15 and 21–26 November 2026 (the 21–22 November run started on the texture path mapped in [texture-pipeline.md](../port/texture-pipeline.md), and the 25–26 November run on the text system mapped in [text-system.md](text-system.md)); it explains the tools, the agent process, the Sony GCC build profiles for main's SDK libraries and the compiler patterns learned. The last written-up milestone is [shared presentation services](presentation-services-milestone.md); the matches since then are written up run by run in the fast matching notes instead. The reviewed menu controller has **15/20 matching handlers / 35.61% of its code bytes**, including its complete four-handler savedata slice. **All 225 tests pass (7 skipped).** Source recovery remains the priority; emulator/data-install validation is deferred. Most source and full audits remain unfinished.

```powershell
.\.venv\Scripts\python.exe tools/build_extra.py
.\.venv\Scripts\python.exe tools/build_battle.py
.\.venv\Scripts\python.exe tools/build_menu.py
.\.venv\Scripts\python.exe tools/build_main.py
.\.venv\Scripts\python.exe tools/progress.py
```

The maintained [scope and roadmap](scope-and-roadmap.md) sets the whole-project milestones and progress rules. The [current progress report](progress.md) separates source recovery, build fidelity and feature evidence ranks. [Controller coverage](menu-controller-coverage.md) is 75% by handler count and 35.61% by bytes; this is one local scope, not the whole menu or game. Inventories cover **21,282 candidates**, including the [reviewed member callback](member-audit-milestone.md) and [deferred battle setup](deferred-battle-audit.md); unknown regions and untraced entries remain explicit. Extra is **9.28% matching C by reviewed code bytes**. The other module audits and the whole-game headline remain unfinished.

The reference disc is the USA v1.00 dump whose size and SHA-1 match Redump entry 22911 (hashes in `config/ulus10566.json`; disc metadata in [iso-inventory-usa.json](iso-inventory-usa.json), written by `tools/inspect_iso.py`). The European disc, ULES01505, decrypts to the same executable and has the same overlays ([other releases](../port/regions.md)).

All four scoped builds are verified and local test ISOs boot to the initial autosave notice. Retail encryption/signing, most recovered C/C++ source and the full three-case gameplay comparison remain unfinished.

The [post-tutorial runtime milestone](hub-runtime-validation.md) verifies both preserved slots load directly to the main hub on all three cases, Quick Battle reaches a fight and returns to selection, and Story Mode loads its prologue/world map with working movement. The human rebuilt-game test also passes controls/mechanics, graphics and sound by user report. Gateway traversal, victory results, new-progress reload and data-install scenarios remain pending; the original human saves and baseline are preserved.

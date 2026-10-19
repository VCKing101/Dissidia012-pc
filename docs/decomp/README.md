# Dissidia 012 matching decompilation

Initial build target: **USA PSP release ULUS10566, version 1.00**.

**Project direction:** the main goal is now the [native PC port](../port/README.md). The matching-decompilation status that follows remains accurate and is the reference for what the original code does. (A PPSSPP-plugin mod and a new-character experiment were also built on this foundation; they are kept out of the public repository.)

The [working environment](getting-started.md) is ready: **the decrypted main executable and all three overlays rebuild byte-for-byte**, with **278 matching C/C++ functions / 20,944 bytes** integrated. The most recent written-up milestone is [shared presentation services](presentation-services-milestone.md); 34 later matches in main, menu and battle are integrated and verified but have no milestone write-up. The reviewed menu controller has **14/20 matching handlers / 32.83% of its code bytes**, including its complete four-handler savedata slice. **171 checks pass.** Source recovery remains the priority; emulator/data-install validation is deferred. Most source and full audits remain unfinished.

```powershell
.\.venv\Scripts\python.exe tools/build_extra.py
.\.venv\Scripts\python.exe tools/build_battle.py
.\.venv\Scripts\python.exe tools/build_menu.py
.\.venv\Scripts\python.exe tools/build_main.py
.\.venv\Scripts\python.exe tools/progress.py
```

The maintained [scope and roadmap](scope-and-roadmap.md) sets the whole-project milestones and progress rules. The [current progress report](progress.md) separates source recovery, build fidelity and feature evidence ranks. [Controller coverage](menu-controller-coverage.md) is 70% by handler count and 32.83% by bytes; this is one local scope, not the whole menu or game. Inventories cover **21,282 candidates**, including the [reviewed member callback](member-audit-milestone.md) and [deferred battle setup](deferred-battle-audit.md); unknown regions and untraced entries remain explicit. Extra is **9.28% matching C by reviewed code bytes**. The other module audits and the whole-game headline remain unfinished.

Follow-up: the added USA ULUS10566 v1.00 ISO matches the size and SHA-1 in PPSSPP's Redump database and is the recommended starting reference. See [the USA comparison](usa-comparison.md) and [USA inventory](iso-inventory-usa.json). The EU research below describes the originally supplied image.

Read [the research and startup plan](project-research.md) for the workflow, tools, sources and game-specific findings. [The inventory](iso-inventory.json) records the supplied image's hashes, file extents and executable metadata.

**The supplied ISO is modified and truncated:** its EBOOT is a Prometheus loader, and 139,905,024 bytes of the declared ending movie are unavailable. A complete unmodified dump is needed as the retail reference. Preliminary investigation can use the fully present ELF overlays, subject to later comparison with that reference.

Reproduce the metadata inspection with Python 3:

```powershell
python tools/inspect_iso.py ULES-01505.ISO --output docs/decomp/iso-inventory.json
```

All four scoped builds are verified and local test ISOs boot to the initial autosave notice. Retail encryption/signing, most recovered C/C++ source and the full three-case gameplay comparison remain unfinished.

The [post-tutorial runtime milestone](hub-runtime-validation.md) verifies both preserved slots load directly to the main hub on all three cases, Quick Battle reaches a fight and returns to selection, and Story Mode loads its prologue/world map with working movement. The human rebuilt-game test also passes controls/mechanics, graphics and sound by user report. Gateway traversal, victory results, new-progress reload and data-install scenarios remain pending; the original human saves and baseline are preserved.

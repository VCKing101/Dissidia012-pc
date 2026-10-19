# Scope, roadmap and progress policy

**Superseded as the project's main direction, first by a PPSSPP-plugin mod (kept out of the public repository) and now by the [native PC port](../port/README.md).** This document still defines how matching progress is measured and remains the reference for that work.

Matching-decompilation plan, 12 October 2026. This supersedes the proposed milestones in the initial EU-image research. The fixed target is **USA ULUS10566 v1.00**. Current measured results are in [progress.md](progress.md); the machine-readable scope and feature assignments are in `config/progress.json`.

## Primary objective and boundaries

Recover readable C/C++ that reproduces the original decrypted main executable and the three game overlays, incrementally replacing assembly. Preserve exact addresses, calling conventions, data layout and behavior. The reference hashes are fixed; a change of game version requires a separate target.

Primary binary scope is `EBOOT.ELF`, `OVL_EXTRA_APP.ELF`, `OVL_MENU_APP.ELF` and `OVL_BATTLE_APP.ELF`. Each overlay gets its own address namespace because their load addresses overlap. Embedded runtime/SDK code remains counted in the initial module inventory until positively classified; moving it into a separate library scope must be documented and increment the scope version. Unclassified regions cannot disappear from the denominator to improve the score.

Separate PSP support PRXs are dependencies and an auxiliary analysis track. Their interfaces and provenance need documenting; their byte sizes are not added to the initial four-module progress denominator. Recover their source only when necessary or when the scope is explicitly expanded.

Asset work supports the code project: understand required archive/index formats, identify assets and prove extraction/repacking where needed. Models, textures, audio, movies and scripts retained from the user's reference are not recovered C. A native PC port, replacement assets, changed gameplay, other regional builds and separately obtained DLC are later scope decisions. The first target is an original-behavior PSP rebuild.

## Work order and acceptance criteria

| Phase | Work | Exit evidence | Current position |
| --- | --- | --- | --- |
| 1. Reference and environment | Pin USA inputs, extraction/decryption, analysis tools | Hash-checked inputs and reproducible setup | Established |
| 2. Build proof | Reassemble scoped modules, integrate representative C, test the verifier | Whole-file byte equality and independently checked relocations | Established for all four files; 278 C/C++ matches |
| 3. Whole-scope inventory | Map main/menu/battle, audit extra boundaries, split code/data, classify imports/libraries | Versioned function/range inventory covering all four modules, including unknown regions | Initial inventories built; boundary/classification audit remains active |
| 4. First complete subsystem | Finish the data-install call graph and state machine, retain the nonmatching loop candidate, test scenarios | Assigned subsystem scope; exact C for owned code; documented dependencies; retail/rebuilt runtime comparisons | Partial source/behavior recovery; runtime validation deferred at user request |
| 5. Broader build foundations | Establish assembly builds for main/menu/battle; recover shared I/O, allocation and overlay-loading services | Each module independently byte-identical; per-module compiler profiles and shared interfaces | All four file builds established; broader toolchain/interface recovery incomplete |
| 6. Feature expansion | Menus/configuration, saves, combat/AI/abilities, story/progression, stages and cutscenes | Feature-owned code/data inventories and exact source integration, with scenario evidence | Battle setup and selected menu configuration mapped in part; broader scopes incomplete |
| 7. Whole-game validation | Link all scoped modules, validate packaging, run a maintained scenario matrix | Full scoped build equality plus documented behavior coverage | Three-case post-tutorial save/load and Battle/Story entry established; human rebuilt combat passes; broader scenarios incomplete |
| 8. Public progress | Add object reports, CI history and a decomp.dev listing | Audited totals, valid report artifact and registered repository | Planned |

Phases overlap where dependencies allow. For example, module inventory should proceed before finishing every extra-overlay function. It establishes the project denominator and prevents repeated difficult matching attempts from consuming all effort while the rest of the game remains unmapped.

The immediate work queue is:

**The complete scoped build foundation is established.** Recovery stands at 278 functions / 20,944 bytes. The [presentation services milestone](presentation-services-milestone.md) covers the first 244; 34 later main, menu and battle matches are integrated and verified without a written milestone. Inventory revision 18 records 21,282 candidates, adding one reviewed entry each in main and menu. Ten new shared services cover presentation channel/event forwarding and drawing acquisition. One [reviewed controller](menu-controller-coverage.md) remains at 14/20 handlers and 2,688/8,188 matching bytes, with its four-handler savedata slice fully source-matched at 1,216 bytes. Save ownership and serialization remain incomplete at rank 2. Task dispatch matches; factory, registration and unlink remain unfinished. Main `0x088D99B0` and menu `0x089DEA74` remain explicitly untraced. Emulator/data-install validation remains deferred.

1. Continue presentation initialization/transition processing and the larger controller setup using the newly recovered shared services. Revisit task factory/registration/unlink and optional-resource wait when new compiler evidence is available. Six [menu-controller handlers](menu-controller-coverage.md) remain pending. Savedata polling now matches; investigate serialization separately. Audit consumed arguments/results as interfaces are recovered. Keep recorded failed variants bounded; revisit them when new evidence appears. Trace the two unreferenced entries when better evidence is available.
2. Maintain the versioned metadata inventory as boundaries and data classifications are checked. Extra's 100-function pilot denominator is reviewed; menu/battle hook entry points are established, while their broader layouts and feature ownership remain provisional. Keep unknown regions explicit.
3. Expand matching C from verified entry points and add progress-only source objects. Missing C units contribute zero matched bytes; assembly, opaque data and raw fallback words earn no C credit.
4. Map the data-install subsystem's complete owned function set and required shared helpers. Continue its 27-word loop mismatch as a bounded research item; keep the successful transfer recovery integrated.
5. Keep broader runtime checks deferred while source recovery advances. The [post-tutorial baseline](hub-runtime-validation.md) already covers both saves, Quick Battle endpoints and Story world-map entry. Gateway traversal, victory/results and new-progress reload remain queued. Data-install success/cancellation/storage/suspend checks are explicitly deferred; refresh package provenance before future emulator work.
6. Generate the first audited code-byte report and then add further features by dependency and impact. Dates for whole-game completion are not defensible from the current small sample.

## What the percentages mean

The eventual headline is:

```text
exact C/C++ matching % = verified matched source-built code bytes / all scoped target code bytes * 100
```

Larger functions carry more weight than small getters. Keep a secondary exact-function count. Data reconstruction, build fidelity, asset tooling and behavior tests receive separate measures. A byte-identical assembly-backed module is not 100% decompiled.

The [objdiff report schema](https://github.com/encounter/objdiff/blob/main/objdiff-core/protos/report.proto) distinguishes exact code, data and function measures from fuzzy matching and completed/linked measures. Our progress numerator credits only integrated C/C++ functions that pass both raw relocation resolution and final linked-byte comparison. Drafts and the table-loop candidate contribute zero exact matched bytes. Pseudocode, unchanged assembly and copied asset bytes do not increase it.

Current local reporting includes these explicitly different denominators:

- **Extra payload coverage:** exact source-built code bytes divided by the 24,312-byte loaded payload. This denominator includes data, so it is a conservative payload metric.
- **Extra reviewed code bytes:** source-built code bytes divided by the reviewed 22,796-byte code region. Revision 2 replaces the original provisional Ghidra-only denominator.
- **Extra reviewed function count:** matched functions divided by 100 reviewed function boundaries. This gives a different percentage because function sizes vary.
- **Reviewed menu controller:** exact callback bytes divided by 8,188 bytes in twenty checked table targets. Count coverage is also shown. Dispatcher and called dependencies are outside this local denominator; it is not all menu/UI code.
- **Four-module section proxy:** source-built code bytes divided by all file-backed executable-flagged section bytes across the scoped modules. This includes data in the overlay sections and import stubs. It is not the audited code-completion headline.

The whole-game audited percentage stays **unknown** until the scope inventory is complete. The current section proxy is available in the report to show scale without pretending its denominator is pure game code. BSS, ISO padding and the large movie/archive payloads are excluded from that proxy.

Progress reports include the scope version and reference/inventory hashes. Scope or boundary corrections may legitimately move a percentage backwards. Preserve the matched byte count and the changed denominator so history is interpretable. Do not average module percentages; sum matched bytes and total bytes across modules.

## Game-feature views and ranking

Feature groups can cover story modes, menus, combat/AI, character abilities, stages, cutscenes, saves and audio. They must be connected to functions, data and assets by evidence: call graphs, strings, archive identifiers, runtime breakpoints and overlay transitions. A menu overlay is not automatically the full menu feature, nor does a battle overlay necessarily contain all combat code.

Each function has one primary reporting owner, including a shared-services category and an unclassified category. It may have several secondary feature tags. Shared rendering, input, scripting or archive services are counted once in the global total, with dependency status shown in feature views. Secondary feature views may overlap; their percentages are not additive.

Feature code percentages are published only after the feature's target function set and code-byte denominator are established. The proposed ranking is an ordinal evidence ladder:

| Rank | Meaning | Required evidence |
| ---: | --- | --- |
| 0 | Unmapped | No supported feature ownership map |
| 1 | Mapped in part | Located entry points/regions and initial dependency evidence |
| 2 | Behavior explained in part | Documented control flow or data models; some source may match |
| 3 | Feature source recovered | Complete owned scope; readable exact C/C++ for its code, dependencies identified |
| 4 | Feature integrated | Owned code/data incorporated into an exact module build; dependency requirements satisfied |
| 5 | Runtime scenarios verified | Retail/rebuilt comparisons pass the feature's documented scenario matrix |

Rank 2 is **not 40% complete**. Ranks 3–5 require a complete ownership scope, while the first two are explicitly partial. `config/progress.json` stores the reviewed rank and notes; the generator derives matched counts from the verified build. Data installation and the savedata list/latest-load slice are rank 2; shared services and selected main/menu/battle helpers remain mapped in part at rank 1. Story/content ownership remains unmapped. Runtime checkpoints alone do not establish source ownership or a completed feature scope.

For stages, characters and cutscenes, also track content readiness independently: assets located, format understood, required runtime code mapped, loading/roundtrip checked and representative playback tested. A pre-rendered movie file has no C source to decompile; its playback engine does. A stage may be mostly geometry/scripts using shared engine code. These need a readiness matrix rather than invented per-level code percentages.

## Maintaining the local report

```powershell
.\.venv\Scripts\python.exe tools/build_extra.py
.\.venv\Scripts\python.exe tools/progress.py
```

The report is written to `docs/decomp/progress.md` and `docs/decomp/progress.json`. It rechecks the retail comparison and refuses stale build inputs using source/configuration fingerprints. Reference binaries remain local. All four module inventories are maintained as metadata under `config/inventory/`, with reviewed corrections under `config/inventory-overrides.json`. Extra is audited for boundaries and layout; main/menu/battle retain `audited: false`. Raw Ghidra exports and programs remain local under `analysis/` and can regenerate the inventories through the [inventory workflow](inventory-findings.md).

## Connecting to decomp.dev

The [integration guide](https://github.com/decompals/decompedia/blob/main/tools/decomp-dev.md) describes target objects, source-built base objects, and reports uploaded from GitHub Actions. [objdiff supports PSP MIPS](https://github.com/encounter/objdiff), and its units can carry progress-category tags. Include every scoped target unit; units without C can have no base object. Progress-only objects must exclude unfinished assembly fallbacks.

Once the inventory is audited, either generate a full `objdiff.json` and run `objdiff-cli report generate`, or adapt our exact verifier to the published report schema. Keep module and subsystem categories, plus separate library/data metrics. The current local JSON is our own schema and is **not yet a decomp.dev upload artifact**.

A future default-branch workflow will publish `report.json` in an artifact named `ULUS10566_report`. After that, a repository administrator can register the project on decomp.dev. No repository has been published, report uploaded or listing created during this planning work. A safe CI build must not embed or publish the user's game binaries, assets or local compiler packages.

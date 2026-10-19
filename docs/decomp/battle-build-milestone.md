# Second byte-identical overlay build

**Current source update:** [the source expansion](source-recovery-milestone.md)
brings battle to 34 matching functions / 1,172 bytes. This page retains the initial
battle build snapshot.

**Later milestone:** [menu now rebuilds exactly](menu-build-milestone.md), bringing current totals to three verified overlays and 45 C functions / 2,408 bytes. The figures below retain this second-build snapshot.

Major build-foundation milestone, 11 October 2026. **The entire USA battle overlay now rebuilds byte-for-byte**, with its first **10 matching C functions / 112 code bytes** integrated at their original addresses. Together with extra, **two of the four scoped modules** have verified builds. The project now has **34 exact C functions / 2,228 bytes**.

This establishes build fidelity for a second module. It does not mean half the game's source is recovered; main/menu builds, most C/C++, complete code/data audits and runtime scenarios remain unfinished.

## Verified output

Output: `build/ULUS10566/OVL_BATTLE_APP.ELF`.

| Measure | Result |
| --- | --- |
| Complete file | 1,596,312 bytes, identical to retail |
| SHA-256 | `1f96cfb3be771ff6e94741ae3916fab340df74e36ede07ca39545d015f4c49dc` |
| Loaded payload | 1,596,080 bytes at `0x089DD4C0` |
| Matching C | 10 functions / 112 bytes |
| Reassembled inventory instruction bytes | 1,508,764 |
| Explicit raw fallback words | 6 words / 24 bytes |
| Preserved original payload bytes | 87,180 |
| Preserved ELF header/metadata | 232 bytes |

The payload categories sum exactly to 1,596,080. Every assembly chunk and C unit has a linker assertion for its original start/end; the entire payload end is asserted as well. The resulting file, including original packaging, passes a complete retail-byte comparison.

## First battle C units

The eight units in `config/battle.matches.json` compile with SNC 1.2.7503.0, **`-O2 -g`**:

| Recovered behavior | Functions | Bytes |
| --- | ---: | ---: |
| Empty cleanup callbacks | 4 | 32 |
| Empty descriptor initializer | 1 | 8 |
| Store a value in the word at `0x08ABBDFC` | 1 | 12 |
| Clear two or three known prefix fields | 3 | 44 |
| Descriptor initializer sets main word `0x09B6B824` to one | 1 | 16 |

The sources are under `src/battle/`; `include/battle/state_prefix.h` models only known offsets and preserves padding. Original field meanings and complete object types remain open. The word setter is kept neutral rather than labeled as a callback or gameplay setting without evidence. Each pilot function has actual callers or descriptor references; no synthetic functions were created to increase progress.

The verifier compares each raw compiler function after resolving all relocations, then checks its linked address and bytes independently. No relocation operands are masked. Raw compiler objects remain unchanged; separate linker copies normalize only the historical PSP machine identifier. The pilot profile matches these functions and does not establish the toolchain for the entire module.

## VFPU assembler compatibility

The installed modern Allegrex assembler rejected the VFPU mnemonics. The already installed PSP GCC 1.3.1 toolchain contains an older assembler with VFPU support, so no new software installation was necessary. It assembles the generated scalar and VFPU code after selecting compatible register and prefix syntax.

Six words remain explicit `.word` fallbacks: two unsupported/reserved VFPU prefix encodings and four invalid words inside Ghidra's provisional instruction ranges. The latter may be data misclassified by automatic analysis. Their addresses, original words and reasons are listed in local `build/ULUS10566/battle.prepare.json`; they receive no source credit. The preserved 87,180 payload bytes include both known data and still-unclassified regions.

Neither the byte-identical build nor the successful assembler run proves that all provisional instruction ranges are game code. Battle's audited code percentage therefore remains pending. Its source contribution is counted independently from assembly, raw words and copied data.

## Deferred battle mapping

The [deferred setup audit](deferred-battle-audit.md) establishes registration of setup/update/teardown callbacks, their shared member dispatcher, and the explicit mode-1 barrier before battle construction. It resolves `battle:0x089DD4D0`, a 100-byte constructor-like routine, and extends the initialization graph used by the build.

Inventory revision 6 contains **21,279 module-qualified candidates / 4,717,900 function-body bytes**. The battle review added 24 candidates and resolved 1,812 unknown bytes. Twenty-two additions have checked call paths; two remain explicitly provisional and untraced. The [comparison metadata](deferred-battle-changes.json) keeps those groups separate.

Combat's evidence rank is now **1 — mapped in part**, reflecting its setup entry points and shared-main dependency. The ten source helpers have their own reporting category, **Battle lifecycle and state helpers**, while gameplay ownership remains incomplete. No combat, mode, arena or cutscene percentage is invented from those observations.

## Validation and provenance

**74 checks pass**, covering inventory partitions, callback/mode binding, relocations, both module builds and progress integrity. New build checks reject a changed retail byte, incorrect raw C behind an unchanged candidate file, wrong linked C addresses and stale source proof. They ensure that raw/opaque bytes receive no C credit and that battle's whole-code headline remains unknown.

`tools/battle_build_provenance.py` fingerprints the C units/header, compiler, assembler, wibo, build scripts/configuration and canonical instruction layout. Updating inventory confidence or source labels leaves that layout digest unchanged; changing instruction ranges invalidates the build proof. This avoids a cycle between compilation and the matching-C inventory update. A read-only review of the build/verifier found no blocking issue.

The source ISOs, extracted reference binaries and existing annotated Ghidra projects are preserved. No ISO packaging, emulator gameplay validation, repository publication or decomp.dev upload was performed.

## Reproduce

```powershell
.\.venv\Scripts\python.exe tools/build_battle.py
.\.venv\Scripts\python.exe tools/build_inventory.py
.\.venv\Scripts\python.exe tools/progress.py
.\.venv\Scripts\python.exe -m unittest discover -s tools -p 'test_*.py'
```

For an independent verification of existing output, run `tools/verify_battle.py`. The compiler-only experiment is `tools/probes/battle_pilot.py`; it is supporting evidence and cannot replace whole-file integration.

Next major build milestone: establish the menu and main builds using the same explicit preservation and verification policy. Continue code/data audits and source recovery alongside those foundations; runtime scenario baselines follow once the packaging workflow is ready.

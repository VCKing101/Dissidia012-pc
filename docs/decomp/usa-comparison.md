# USA versus supplied EU image

Inspected 10 October 2026. Recommendation: use **USA ULUS10566 v1.00** as the initial matching-decompilation reference. The original [EU research](project-research.md) remains useful for the workflow and overlay layout.

## Why this USA copy is a better starting point

The USA image's measured size and SHA-1 exactly match the USA v1.00 entry, Redump ID 22911, in [PPSSPP's bundled Redump database](https://github.com/hrydgard/ppsspp/blob/master/assets/redump.csv):

- Size: `1754529792` bytes.
- SHA-1: `ec23f5beac89312a2a25654e202ae1c997999c9e`.
- Measured SHA-256: `df94177d3e194e82800a574b50b61ee3c7be6caa877c8e82242a41af75d1ab6e`.

SHA-1 was computed by the Python inspector and independently verified with PowerShell Get-FileHash. This verifies agreement with that published reference; an emulator boot has not been tested.

| Check | Supplied EU image | USA image |
| --- | --- | --- |
| Disc ID | ULES01505 | ULUS10566 |
| Version | 1.00 | 1.00 |
| Actual size agrees with declared volume | No | Yes |
| Declared file extents beyond image end | END.PMF, missing 139,905,024 bytes | None |
| Main EBOOT | Prometheus replacement loader | `~PSP` game module, name `FFSTJP` |
| Added EBOOT.OLD / prometheus.prx / PACK.bin | Present | Absent |
| Retail reference comparison | Not established; visibly modified | Size and SHA-1 match published USA entry |

**Clarification to the earlier EU investigation:** BOOT.BIN is all zeros in the verified USA image too. An all-zero BOOT.BIN alone does not demonstrate modification. The EU patch evidence is the Prometheus loader, extra module and added files; truncation is established independently by the file extents.

## Does the code itself become easier?

The USA and supplied EU copies have identical SHA-256 hashes for all three game overlays:

- `OVL_EXTRA_APP.ELF`: `f6b7be5e15c28774985417f1ec2c64f6655e79cc3c9dce7600fa7f6bacabca01`.
- `OVL_MENU_APP.ELF`: `f021f96fa05da5907563a28c232966aa8a344526d6b8bdee728053ec75224d0a`.
- `OVL_BATTLE_APP.ELF`: `1f96cfb3be771ff6e94741ae3916fab340df74e36ede07ca39545d015f4c49dc`.

Both support-library files also match between these images. The overlay observations from the EU research therefore transfer to this USA reference. This does not establish that the encrypted main executable matches EU's EBOOT.OLD, or that every other file matches. A clean EU retail image has not been compared.

The USA main EBOOT is 4,641,296 bytes, a `~PSP` container with tag `0xD91613F0`. Its header declares a 4,640,948-byte ELF. Obtain the decrypted ELF before static analysis of the main game module. PPSSPP documents its decrypted EBOOT dump option under Settings → Tools → Developer Tools. [PPSSPP documentation](https://www.ppsspp.org/docs/development/developer-tools/).

The overlays are already ELF files, so their initial analysis can begin without decrypting the main executable. `OVL_EXTRA_APP.ELF` remains the smallest candidate for the first build/match experiment. The original game compiler is still unidentified; support-library GCC comments do not settle it.

There is a practical USA tooling advantage: [dissidia_filelist](https://github.com/hearhellacopters/dissidia_filelist) explicitly targets tested US version ULUS10566 and provides a largely recovered filename list. That helps archive analysis and filename recovery. It does not recover executable source or remove the compiler-matching work.

## Next step

Preserve this verified USA ISO, extract the smallest overlay locally, validate its mapping in Ghidra/PPSSPP, and establish an assembly-backed rebuild before replacing a function with C. Keep the EU image for comparison. A USA starting target does not require deciding now whether to support EU later.

The USA metadata is recorded in [iso-inventory-usa.json](iso-inventory-usa.json). Neither ISO was modified or extracted during this comparison.

```powershell
python tools/inspect_iso.py 'Dissidia 012 - Duodecim Final Fantasy (USA) (En,Fr,De,Es,It).iso' --output docs/decomp/iso-inventory-usa.json
```

# Working starter environment

Target: **Dissidia 012 USA, ULUS10566 v1.00**. Set up 10 October; latest verification 11 October 2026.

## Current result

`OVL_EXTRA_APP.ELF` reconstructs **byte-for-byte**, including twenty-four integrated C functions, to the verified retail overlay's SHA-256:

`f6b7be5e15c28774985417f1ec2c64f6655e79cc3c9dce7600fa7f6bacabca01`

Its 24,312-byte payload combines 2,116 bytes compiled from C with the remaining assembly/data and is linked at `0x089DD4C0`. The remaining 232 bytes of original ELF header and metadata are preserved locally. The resulting 24,544-byte file is compared directly with the pinned retail input.

Extra recovered C functions: **24**, using SNC 1.2.7503.0 at `-O2 -g`. Debug generation changes instruction scheduling; twelve current reconstructions differ without it. See [compiler findings](compiler-findings.md) for the source, control experiment and linked verification. This does not identify the original whole-game toolchain, rebuild the full main executable, or recreate the entire ISO.

The [battle milestone](battle-build-milestone.md) rebuilds `OVL_BATTLE_APP.ELF` byte-for-byte, integrating **ten C functions / 112 bytes**. Battle uses the installed historical VFPU assembler, six explicit raw fallback words and preserved original data.

The [menu milestone](menu-build-milestone.md) adds a byte-identical overlay with eleven C functions / 180 bytes. The [main milestone](full-build-milestone.md) adds fifteen more / 836 bytes and completes the four-file build foundation. Current recovery is **60 exact functions / 3,244 bytes**. Most source, encrypted/ISO packaging and gameplay validation remain unfinished.

Ghidra successfully imported the overlay using the PSP ELF loader and Allegrex processor. Its first analysis identified **97 provisional functions** and exported pseudocode to the ignored `analysis/ULUS10566/extra/` directory. The later inventory review added three leaf routines, giving **100 extra-overlay functions**. Revision 2 reviews their boundaries and classifies the whole loaded payload, establishing **9.28% matching C by code bytes**. The [deferred battle audit](deferred-battle-audit.md) brings the four-module inventory to **21,279 candidates** in revision 6. Main/menu/battle remain provisional; two new battle candidates are explicitly untraced. See [inventory findings](inventory-findings.md) and the [extra layout audit](extra-layout-audit.md).

See the maintained [scope and roadmap](scope-and-roadmap.md) for the whole-project work order and [progress report](progress.md) for current coverage and feature evidence ranks. Refresh that report after a successful build with `.\.venv\Scripts\python.exe tools/progress.py`. It distinguishes code recovery from assembly-backed build fidelity and refuses stale source/configuration inputs.

The USA EBOOT decrypted successfully with `pspdecrypt` and is in `analysis/ULUS10566/main/EBOOT.ELF`. Its SHA-256 is:

`0bc8e779aad2814a3d1bd77346f69ca5e16665845ed053cd0ad44a9cbfaefe6d`

That is identical to `EBOOT.OLD` in the supplied modified EU-labelled image. Combined with the previously identical overlays, substantial code can be investigated against the verified USA reference. This finding does not certify the other image's regional provenance or asset contents. The decrypted main ELF has blank section names and no recovered compiler comments; those properties are present in the verified retail input rather than evidence of the Prometheus patch.

## Installed tools

Windows tools are portable under `tools/local/`; Python packages are in `.venv/`. The existing Ubuntu-24.04 WSL distribution supplies the Linux build environment.

| Tool | Installed version / provenance |
| --- | --- |
| Python environment | Existing Python 3.12; dependencies pinned in `requirements.txt` |
| splat | 0.50.0 |
| spimdisasm / rabbitizer | 1.42.4 / 1.16.2 |
| Ghidra | 12.1.3, selected for Allegrex-extension compatibility |
| ghidra-allegrex | v21.4 for Ghidra 12.1.3 |
| Java | Eclipse Temurin JDK 21.0.12.1+1 |
| PPSSPP | 1.20.4 Windows x64 |
| Allegrex assembler / objcopy | Community Binutils 2.42 package from sotn-decomp |
| MIPS linker | Ubuntu `binutils-mipsel-linux-gnu`, 2.42-2ubuntu1cross5 |
| pspdecrypt | Source commit `c156627db7634d395c380c0a9589130f603307fc`, built with GCC/G++ in WSL |
| SNC / wibo | SNC 1.2.7503.0 / wibo 1.2.0, used through WSL for matching C compilation |

Official ZIP digests and URLs are pinned in `config/tools.json`. All four ZIPs were verified before installation. The Allegrex tools archive SHA-256 is pinned in `tools/setup_wsl.sh`; this digest was measured from the community-hosted archive. OpenSSL development headers were installed in WSL, with its OpenSSL runtime updated to satisfy that package dependency. The system-wide Windows PATH was not changed.

Tool sources: [Ghidra](https://github.com/NationalSecurityAgency/ghidra/releases), [Allegrex extension](https://github.com/kotcrab/ghidra-allegrex/releases), [Temurin](https://github.com/adoptium/temurin21-binaries/releases), [PPSSPP](https://github.com/hrydgard/ppsspp/releases), [community Allegrex tools](https://github.com/Xeeynamo/sotn-decomp/releases/tag/cc1-psx-26), [pspdecrypt](https://github.com/hrydgard/pspdecrypt), [splat](https://github.com/ethteck/splat).

## Commands to use now

Run these from the project root in PowerShell. No virtual-environment activation is needed.

### Rebuild and verify the first overlay

```powershell
.\.venv\Scripts\python.exe tools/build_extra.py
```

Rebuild the second overlay with `.\.venv\Scripts\python.exe tools/build_battle.py`, then refresh the maintained maps and report with `tools/build_inventory.py` and `tools/progress.py`. The full [battle workflow](battle-build-milestone.md) records raw compiler/linker checks and explicit preserved-byte accounting.

Rebuild menu with `.\.venv\Scripts\python.exe tools/build_menu.py`. Its [workflow](menu-build-milestone.md) follows the same exact verification policy and records assembler fallbacks explicitly.

Rebuild the decrypted main ELF with `.\.venv\Scripts\python.exe tools/build_main.py`. The [complete workflow](full-build-milestone.md) reconstructs two initialized load segments, preserves all other file bytes and never emits the memory-only BSS region.

The command regenerates the assembly, compiles the matched C units, assembles the remaining instructions/data in WSL, links everything, preserves the ELF packaging, and checks the complete output. It writes `build/ULUS10566/OVL_EXTRA_APP.ELF`, the linker map, and `extra.verify.json`. Every mismatch causes failure. Negative instruction, C-field-write, and relocation-target checks reject incorrect candidates.

Check the relocation verifier using actual compiler output and retail bytes:

```powershell
.\.venv\Scripts\python.exe tools/test_matching.py
```

The generated `extra.splat.ld` is diagnostic only. The build uses `config/extra.link.ld`, which avoids the default 16-byte segment padding and preserves exact payload size. `add_set_gp_64: false` selects the correct register width. The hand-maintained `include/macro.inc` avoids a local-symbol bookkeeping incompatibility in the community assembler. The splat `compiler: GCC` setting controls disassembly formatting and is **not a compiler-identification result**.

### Open the analyzed overlay in Ghidra

```powershell
.\tools\open_ghidra.ps1 -ExtraProject
```

Open `OVL_EXTRA_APP.ELF` in the project window. The saved project is `analysis/ghidra-projects/dissidia-extra.gpr`. Do not run a new headless import while the project is open in the GUI.

To regenerate the analysis and provisional pseudocode, first close the GUI project, then run:

```powershell
.\tools\run_ghidra.ps1
```

This overwrites that program's analysis, so preserve manual annotations before using it once recovery work begins. Settings/cache stay under `analysis/ghidra-home/`.

### Open the USA game in PPSSPP

```powershell
.\tools\open_ppsspp.ps1 -Usa
```

PPSSPP is installed, but emulator startup/gameplay has not been exercised during this milestone. Keep the extracted overlay reference unchanged. Later, use a separate patched build or an extracted game directory for runtime testing.

## Recreate the environment

Portable Windows tools:

```powershell
python tools/setup_tools.py
python -m venv .venv
.\.venv\Scripts\python.exe -m pip install -r requirements.txt
.\.venv\Scripts\python.exe tools/setup_compilers.py
```

WSL dependencies and the pinned assembler/decryption utility (`/mnt/x/path/to/Decomp-Dissidia` stands for your checkout's path in WSL):

```powershell
wsl -d Ubuntu-24.04 --cd /mnt/x/path/to/Decomp-Dissidia -- bash tools/setup_wsl.sh
```

The setup script uses sudo inside Ubuntu. This initial installation was completed through WSL's root user; a future interactive run may request your Ubuntu password.

Verified selective extraction (preserves any existing differing file instead of overwriting it):

```powershell
.\.venv\Scripts\python.exe tools/extract.py 'Dissidia 012 - Duodecim Final Fantasy (USA) (En,Fr,De,Es,It).iso'
```

Six files are extracted to `orig/ULUS10566/`: the encrypted main executable, three overlays, a support library, and the archive index. The large asset archive is not needed yet. Initial extraction was independently checked using pycdlib against the inspection hashes. The maintained extractor uses the standard-library ISO reader to avoid a pycdlib/Python shutdown diagnostic observed during that cross-check.

Repeat the main executable decryption:

```powershell
wsl -d Ubuntu-24.04 --cd /mnt/x/path/to/Decomp-Dissidia -- bash tools/decrypt_main.sh
```

## First recovery work

The smallest overlay appears to include memory-stick data-install handling. Ghidra recovered strings such as `data_install_power` and `data_install_fat` in its first initialization routines. Treat these as clues until runtime behavior is checked.

Useful first candidates from provisional analysis:

| Address | Candidate behavior | Why inspect it |
| --- | --- | --- |
| `0x089DD648` | Writes two fields through a state pointer | Small 20-byte helper for an initial compile comparison |
| `0x089DD5F8` | Handles flag bits and changes a byte field | Control flow and delay-slot behavior can distinguish compiler settings |
| `0x089DD4D0` | Initialization with several main-program calls | Useful for validating structures, calling conventions and compiler code generation |

The pointer-update helpers, event and power handlers, notifications and adapters, capacity query, setup/status/buffer routines, callback lifecycle, and adjacent transfer/completion routines are now integrated. The main-program helper at `0x089A5490` establishes capacity and available-space fields in KiB; its arithmetic reserves two free clusters. The table-building routine at `0x089DD99C` has a readable candidate in `tools/probes/data_install_table.c`; it is the correct 492-byte size but differs in 27 instruction words and remains excluded from the build. See [data-install structures](data-install-format.md) for the 8 KiB alignment, 28-byte records and observed completion behavior. The current SNC profile is verified for the integrated functions; whole-game toolchain identification still needs broader evidence.

Inspect explicit main-executable ranges without starting Ghidra:

```powershell
.\.venv\Scripts\python.exe tools/disassemble_elf.py analysis/ULUS10566/main/EBOOT.ELF 0x089A5490 0x1E4 --output analysis/ULUS10566/main/query_helper.s.txt
```

The bounded disassembler records the input hash and decodes Allegrex instructions. The caller supplies the range; the tool does not infer function boundaries or treat zero-filled memory as file-backed instructions.

Reproduce the compiler-profile control experiment and main import inventory:

```powershell
.\.venv\Scripts\python.exe tools/probe_debug_profile.py
.\.venv\Scripts\python.exe tools/inspect_imports.py
```

The ISO files, extracted binaries, assembly, pseudocode, tool installs and build output are ignored by Git. Tracked content is tooling, configuration and research. Both source ISOs remain unchanged.

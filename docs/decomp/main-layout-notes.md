# Main executable layout audit

Observed directly from the pinned USA `analysis/ULUS10566/main/EBOOT.ELF` on 2026-10-11 using pyelftools. This is a file/virtual-address geometry review; it does not establish behavior, source ownership, or a complete code/data partition.

Reference SHA-256: `0bc8e779aad2814a3d1bd77346f69ca5e16665845ed053cd0ad44a9cbfaefe6d`.

## ELF structure

The file is 4,640,948 bytes (`0x46D0B4`), ELF32 little endian, `ET_EXEC`, `EM_MIPS`, flags `0x10A23001`, with the entry field equal to zero. It contains two 32-byte program headers beginning at file offset `0x34` and 26 40-byte section headers beginning at `0x46CCA4`. The section-header table ends exactly at EOF.

Every section's `sh_name` field is zero. Section 25 is the one-byte, zero-valued section-name string table at `0x46CB50`. Names such as `section_2` in the maintained inventory are synthetic labels.

All ranges below have exclusive ends.

## Load segments and memory-only space

| Segment | File range | Initialized virtual range | File bytes | Memory bytes | Flags |
| --- | --- | --- | ---: | ---: | --- |
| PT_LOAD 0 | `0x1018..0x1DA398` | `0x08804040..0x089DD3C0` | 1,938,304 | 1,938,304 | 7: R/W/X |
| PT_LOAD 1 | `0x1DB000..0x1DB100` | `0x08B63170..0x08B63270` | 256 | 16,833,424 | 6: R/W |

Both segments declare alignment `0x1000`. Their file-offset/virtual-address residues modulo that value differ: segment 0 has `0x18`/`0x40`, and segment 1 has `0x0`/`0x170`. Preserve the observed geometry; imposing a congruence check would reject this pinned input. Their physical-address fields are respectively `0x001C3208` and `0x08B63170`; this audit assigns no meaning to those values beyond recording them.

The file-backed spans do not overlap. The full memory spans also do not overlap. An initialized virtual address maps uniquely to file offset `p_offset + address - p_vaddr` inside its segment. The virtual gap between segment 0's end and segment 1's start is 1,596,848 bytes (`0x185DB0`); it is not supplied by these main PT_LOAD segments.

Segment 1's memory-only tail is 16,833,168 bytes:

- `0x08B63270..0x08B63280`: a 16-byte alignment gap without a nonempty section.
- `0x08B63280..0x09B70D00`: section 23, `SHT_NOBITS`, 16,833,152 bytes, flags 3: allocated and writable.

The segment's memory end and section 23's end agree at `0x09B70D00`. NOBITS occupies memory without corresponding file content; its `sh_offset` is not an instruction to copy that many bytes from the ELF. Seven other NOBITS sections have zero size, at virtual addresses `0x089DD3C0`, `0x089DD4C0`, `0x08B62F70`, `0x08B63170`, `0x08B63270` (twice), and `0x09B70D00`.

## Initialized sections and counts

Section 2 covers `0x08804040..0x089C51DC`, 1,839,516 bytes, with executable section flags. Section 7 covers `0x089C5770..0x089C60B0`, 2,368 bytes, also executable. The latter contains the 296 declared eight-byte PSP import stubs recorded in the maintained inventory.

The other initialized allocated sections (3 through 6, 8 through 13, and 18 through 20) lack the executable section flag. Segment-level execute permission alone does not establish that all its contents are instructions.

There is one four-byte, all-zero gap between initialized sections 11 and 12: virtual `0x089C696C..0x089C6970`, file `0x1C3944..0x1C3948`. Segment 1's three initialized sections have sizes 240, 8, and 8 bytes and cover its file-backed span completely.

| Quantity | Bytes | Interpretation |
| --- | ---: | --- |
| Entire ELF | 4,640,948 | Includes headers and bytes outside load segments |
| Initialized PT_LOAD payloads | 1,938,560 | Appropriate rebuild payload size for these two segments |
| Initialized allocated PROGBITS sections | 1,938,556 | Payload total minus the four-byte inter-section gap |
| Executable-flagged sections | 1,841,884 | Sections 2 and 7; not a proven code-only denominator |
| Decoded instructions in maintained inventory | 1,792,844 | Static analysis snapshot, still provisional |
| Function-body union in maintained inventory | 1,783,500 | Includes platform stubs and does not establish source completion |
| Unclassified executable-section bytes | 49,040 | Executable-section total minus decoded instruction coverage |
| Memory-only segment tail | 16,833,168 | Excluded from file rebuild bytes and C code credit |

The maintained inventory snapshot used here is `config/inventory/main.json`, source inventory SHA-256 `9d79a87e767a2c221af815f0be68e8b7fc69b127719b1a1a376eee029a313b3d`. Its main source-credit count was zero at this audit. Any later matched C additions must be credited from independently verified function builds, not from recreating entire segment/file bytes with preserved data and assembly.

## File bytes outside the load payloads

Exactly 2,702,388 file bytes lie outside the two PT_LOAD payloads. Preserve these bytes verbatim when packaging a rebuilt executable.

| File range | Bytes | Observed contents |
| --- | ---: | --- |
| `0x0..0x74` | 116 | ELF header and two program headers |
| `0x74..0x1018` | 4,004 | All zero |
| `0x1DA398..0x1DB000` | 3,176 | All-zero gap between load payloads |
| `0x1DB100..0x1DC000` | 3,840 | All zero |
| `0x1DC000..0x1DC018` | 24 | Section 1, `SHT_MIPS_REGINFO`, flags zero; five nonzero bytes |
| `0x1DC018..0x46CB50` | 2,689,848 | All zero |
| `0x46CB50..0x46CB51` | 1 | Zero byte of section-name string table |
| `0x46CB51..0x46CCA4` | 339 | All zero |
| `0x46CCA4..0x46D0B4` | 1,040 | Section-header table |

Section 1 records address `0x08804000`, but lacks the allocated flag and is outside both load payloads. It should remain at its original file offset. This review does not infer encryption, SDK provenance, or an additional purpose for the large zero-filled tail.

## Packaging requirements

Starting with an exact copy of the pinned ELF and replacing only each validated PT_LOAD file span is a sufficient preservation strategy. The replacement payloads must retain their original lengths, virtual addresses, and initialized data/gap bytes. Verify each payload independently and compare the final entire ELF byte-for-byte against the reference. Preserve all original ELF/program/section headers, the inter-segment file gap, register-info section, and remaining tail.

Address-to-file conversion must reject addresses outside initialized load ranges and requests crossing their bounds. Memory-only BSS and the virtual gap have no initialized payload offsets. Code/function extraction additionally needs executable-section membership and function-range validation: membership in writable/executable PT_LOAD 0 alone is insufficient.

An exact full-file rebuild establishes preservation and reconstruction of this baseline executable. It does not prove gameplay operation or imply that every preserved byte has been decompiled to matching C.

## Initial build review

The initial `tools/build_main.py` pipeline was reviewed against this geometry. Its two extracted payloads cover the initialized spans only, and packaging retains the original full ELF around them. A direct `tools/verify_main.py` check and the saved-build provenance check passed with 15 matching C functions / 836 bytes. The verifier checks raw relocation-resolved C object bytes, linked C addresses and bytes, both complete load payloads, and the entire packaged ELF. SDK import stubs are excluded from C credit.

Read-only boundary probes confirmed rejection of addresses at either initialized segment end, BSS addresses, a range crossing segment 0's end, and a negative size. The pipeline permits the reference's observed noncongruent offset/address alignment residues without flattening the virtual gap or materializing BSS. These checks establish the initial build foundation; runtime scenarios remain unverified.

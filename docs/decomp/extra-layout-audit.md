# Extra-overlay layout audit

Inventory revision 2, 11 October 2026. This review resolves the 1,105 previously unclassified bytes in the verified USA extra overlay. It establishes function boundaries and a static CPU-code/data/zero-fill partition. It does not establish complete feature ownership, original source declarations, runtime behavior, or a completed decompilation.

## Reviewed partition

| Classification | Bytes |
| --- | ---: |
| CPU code, 100 functions | 22,796 |
| Data, including the overlay descriptor | 1,476 |
| Padding or reserved zero fill | 40 |
| Unclassified loaded bytes | **0** |
| Loaded payload | **24,312** |

The surrounding ELF header and metadata are another 232 bytes, outside this loaded-payload denominator. The original file is unchanged.

All 97 original Ghidra boundaries agree with spimdisasm. The three independently reviewed leaf routines also agree with spimdisasm and end in their own returns/delay slots. The 100 functions cover two contiguous code regions:

- `0x089DD4D0..0x089DE744`
- `0x089DE7A0..0x089E2E38`

Endpoints are exclusive. All other loaded bytes belong to the reviewed descriptor/data/zero-fill regions. Complete coverage and non-overlap are enforced by `tools/audit_layout.py`; the maintained review is in `config/inventory-overrides.json`.

## Overlay prefix: descriptor, not a function

The 16-byte prefix at `0x089DD4C0` has the following observed layout:

| Offset | Width | Field |
| --- | ---: | --- |
| `0x0` | 4 | Tag; extra=3, menu=2, battle=1 |
| `0x4` | 2 | Initializer-hook count |
| `0x6` | 2 | Cleanup-record count |
| `0x8` | 4 | Initializer function-address table |
| `0xC` | 4 | Cleanup-record pointer table |

The tag's wider application meaning remains unassigned. The other fields are supported by the **main executable's consumers**, rather than by interpreting these words as instructions:

- Loader `0x089331FC` checks the ELF magic, type `0xFFA8`, a 32-byte program header and one load segment. After loading the payload at its declared address and performing cache operations, it passes the segment base to `0x0893F810`.
- `0x0893F810` reads the halfword at `+4`, the pointer at `+8`, and invokes that many four-byte function-address entries.
- Cleanup walker `0x0893F888` reads `+6` and `+12`. Each table word points to a record containing two arguments and a callback address; the walker calls that callback.

The menu header declares eight initializers and no cleanup records; its zero-length cleanup table points one past the payload. Battle declares 21 initializers and five cleanup records. Both nonempty tables lie inside their respective payloads. Extra has zero counts and null table pointers, so its all-zero hook fields are descriptor data rather than executable entry code.

The first word happens to decode as an inert MIPS shift instruction. That decoding alone was insufficient evidence of a function; the loader and hook walkers establish its place in the descriptor.

## Data islands

The first data island follows the initial code cluster. References confirm an ASCII file/path label, the Memory Stick device name, callback names, two eight-byte values and two pointer words. The matched lifecycle and transfer routines already consume these values.

The larger tail island contains:

- ASCII names/path strings and a UTF-16 format string that ASCII-only recognition partly missed.
- Binary configuration streams passed to main-program helpers. Their complete internal protocol is still unknown, but they are passed as data and have no flow destinations in the reviewed reference export.
- Halfword records: `0x089DF3D4` and `0x089DF424` read unsigned halfwords, advance eight bytes per record and stop at a `0xFFFF` marker. The aggregate record region is 632 bytes and ends in a complete sentinel record.
- A seven-entry jump table at `0x089E32A8`. The code at `0x089E13E0` bounds-checks the index below seven, scales it by four and jumps through the selected address. All seven destinations lie in reviewed code.
- String pointers and flag/pointer records ending in a sentinel, plus static values and a mutable context pointer.
- A 168-byte member-dispatch table at `0x089E3310`, containing 21 eight-byte records. `0x089DEB00` copies the table, indexes it by eight, reads signed halfwords at offsets 0/2 and an address at 4, resolves the member target and invokes it. In this table, both halfwords are zero and each address is a known function entry.

This distinguishes code addresses **stored in data tables** from instruction bytes. The strings, records and binary streams are not credited as recovered C. Detailed object/field meanings and the binary configuration format remain future work.

Forty zero bytes occupy gaps between code, string slots and aligned data objects. They are recorded as padding/zero fill rather than as new functions. Some gaps could contain reserved data fields; their exact semantic role is not required to establish that they are outside the reviewed CPU functions. The validator checks that every byte in those ranges is zero.

## Progress and checks

The numerator remains **24 exact C functions / 2,116 code bytes**. The extra overlay now has a reviewed code denominator of **22,796 bytes**, giving **9.28% matching C**. Function-count coverage is **24/100 = 24.00%**. This is an extra-module code percentage, not the whole game's percentage.

The other three module maps remain provisional, with 250,021 executable-section bytes still unclassified. The whole-game audited headline remains unavailable. The extra overlay continues to rebuild byte-identically with unfinished assembly and preserved data.

The new negative checks reject nonzero padding, a data address used as a jump target, an interior code address used as a member-function entry, a dropped partition byte and a nonempty descriptor table outside its payload. Together with existing inventory, progress and matching checks, **27 checks pass**.

## Reproduce

```powershell
.\tools\run_inventory.ps1 -Module extra -AuditReferences
.\tools\run_inventory.ps1 -Module main -AuditReferences
.\.venv\Scripts\python.exe tools/build_inventory.py
.\.venv\Scripts\python.exe tools/progress.py
.\.venv\Scripts\python.exe tools/test_inventory.py
.\.venv\Scripts\python.exe tools/test_progress.py
.\.venv\Scripts\python.exe tools/test_matching.py
```

Reference exports are read-only and saved under ignored `analysis/ULUS10566/<module>/audit-references.json`. The independent disassembly used by the boundary check is generated by the existing overlay build. Raw ELF bytes, instructions and string contents remain local; the maintained review records addresses, types and evidence.

That follow-up is completed in the [revision 3 overlay-hook audit](overlay-hook-audit.md): 26 missing entries added, both descriptors and all hook records validated. Its report supersedes the revision 2 unknown-byte and test totals above. Menu/battle data islands and function-boundary candidates remain the next work; feature ownership and runtime checks follow separately.

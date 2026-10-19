# Data-install structures and transfer path

Evidence from verified USA ULUS10566 v1.00, inspected 11 October 2026. These are recovered memory layouts and code behavior; a complete installed-file format and runtime state machine are not yet established.

## Source entry metadata

The main executable's entry helpers use a global structure at `0x09B26BC0`. Its entry count is at offset `0x8`, followed by 12-byte entries beginning at offset `0x10`. `func_8871684` computes the entry address with a 12-byte stride. The first two words of each entry remain unnamed.

The third word combines a byte length in its lower 28 bits with a category in its upper four bits. `func_88716C8` returns `entry_word & 0x0FFFFFFF`, or zero for a null entry. The table builder accepts a nonzero category only when it is no greater than its signed input argument.

`func_886FCDC` calls the length helper and rounds the result up to an 8 KiB multiple:

```text
aligned_length = (raw_length + 0x1FFF) & ~0x1FFF
```

The arithmetic is 32-bit. In the table builder, the raw length contributes to a running total before the aligned length is checked against the caller's buffer capacity at state offset `0x604`. Entries that do not fit are skipped. Accepted entries consume their aligned length in the 64-bit destination cursor. The meanings of the two unnamed entry words and the application's category values still need investigation.

## Destination table

The main helper `func_886FA78` returns a global table buffer at `0x08B63800`. `func_886FA84` returns its fixed byte length, `0x1BB78` (113,528). That equals a 128-byte header plus 4,050 records of 28 bytes each. The table builder starts the first payload at the next 8 KiB boundary: `0x1C000` (114,688).

`func_886FAA0` publishes these header fields after normal table construction:

| Header offset | Width | Observed value |
| --- | ---: | --- |
| `0x0` | 4 | Selected entry count |
| `0x4` | 4 | Category-limit argument |
| `0xC` | 4 | Accumulated raw byte length |
| `0x14` | 4 | Set to 1 on finalization |

Other header fields remain unclassified. `func_886FAC0` builds the selected record at `table + 0x80 + selected_index * 0x1C`:

| Record offset | Width | Observed initialization |
| --- | ---: | --- |
| `0x0` | 2 | Source entry index, narrowed to 16 bits |
| `0x2` | 2 | Zero |
| `0x4` | 4 | Low 32 bits of the destination byte offset shifted right by 13 |
| `0x8` | 20 | Cleared to zero |

The final 20 bytes may be populated later; their meaning has not been established. The offset field uses 8 KiB units. The byte offset is passed to the helper as a 64-bit argument, but only a 32-bit shifted result is stored in this record.

The overlay's own state stores selected source-entry addresses in a separate 4,050-word array at offset `0x628`, the selected count at `0x4570`, and the current transfer index at `0x4574`. These are distinct from the destination table's 28-byte records.

## Header write and asynchronous completion

Seven functions added in this milestone compile and link exactly from C:

| Overlay address | Behavior |
| --- | --- |
| `0x089DDF24` | Poll the first notification flag; zero result after notification sets status `0x80000014` and the error flag |
| `0x089DDF6C` | Write the supplied buffer through a support-library helper, record status, and advance the 64-bit cursor by a rounded result |
| `0x089DE020` | Clear completion state and start work for the current selected entry, passing a callback and state pointer |
| `0x089DE0B8` | Poll asynchronous completion, record its result, advance cursor/index, and return 1 when notified |
| `0x089DE164` | Store a word into one of four state slots, accepting indices 0 through 3 |
| `0x089DE6F4` | Forward completion arguments to `func_089DDE10`, adding the global context |
| `0x089DE71C` | Forward notification arguments to `func_089DDE20`, adding the global context |

The header helper `func_886FF7C` takes a 64-bit offset, buffer address and byte length. For a nonzero length, it calls main import `0x089C5D48`, library `scesupPreAcc`, NID `0xB03FF882`. The supplied support PRX exports that NID at relative address `0xA28`. Its call path reaches the PRX's `sceIoWrite` stub at `0x3684`, NID `0x42EC03AC`; nonzero-offset handling also reaches `sceIoLseek` at `0x36FC`, NID `0x27EB27B8`. Those NID names agree with [PSPSDK's I/O stubs](https://github.com/pspdev/pspsdk/blob/master/src/user/IoFileMgrForUser.S). The public name of the support export remains unknown.

The overlay's initial offset global at `0x089DE788` is zero in the reference file. A second 64-bit global at `0x089DE790` contains 96; its wider role is not assigned here. Dynamic writes to these globals still need tracing.

The asynchronous completion callback stores a result at state offset `0x4594` and then sets the flag at `0x459D`. The poll routine uses that original result to advance the cursor by an 8 KiB-rounded amount. A zero result becomes status `0x80000015`; a signed negative status sets the separate error flag at `0x459E`. Returning 1 signals that completion was observed, including error cases. The transfer index advances even on that path, and the poll routine does not clear the completion flag. The recovered code preserves this behavior; callers must still be traced to establish its sequencing.

Similarly, the synchronous header-write routine updates the cursor after recording a negative result. Its rounding uses the original unsigned 32-bit status bits. These details are included because replacing them with more conventional error handling would break the retail match.

## Verification and limits

The complete extra overlay remains byte-identical with **24 C functions, totaling 2,116 bytes**, and the remaining generated assembly/data. The 492-byte table-building candidate remains excluded because 27 instruction words differ in branch layout. Its size and readable logic are useful evidence, but they are not a completed C match.

These layouts were inferred from code and have not been tested by running a data installation in PPSSPP. No installed data file has been parsed or generated yet. Unknown header/record fields, support-library transformations, and callback sequencing remain follow-up work.

To reproduce support-library import/export inspection:

```powershell
.\.venv\Scripts\python.exe tools/inspect_imports.py orig/ULUS10566/PSP_GAME/USRDIR/DATA/MODULE/LIBSUPPREACC.PRX --output analysis/ULUS10566/main/support-imports.json
```

The main executable's SHA-256 is `0bc8e779aad2814a3d1bd77346f69ca5e16665845ed053cd0ad44a9cbfaefe6d`; the support PRX's is `aed4d30f80b1e83561bdfdff4ddfc47dead5b29694be314e253bee69f61f8778`. Use `tools/disassemble_elf.py` to inspect explicitly bounded ranges. Main helper ranges include `0x08871674..0x088716F0`, `0x0886FA78..0x0886FB98`, `0x0886FCDC..0x0886FD08`, and `0x0886FF7C..0x0886FFD0`, with endpoints exclusive. The support export spans `0xA28..0xE98`; its addresses are relative to the PRX image, not fixed runtime addresses.

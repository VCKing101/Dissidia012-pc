# Task runtime and first matched savedata handlers

12 October 2026. **22 additional functions / 2,768 source-built code bytes**
now match, bringing recovery to **231 C/C++ functions / 16,816 bytes**.
All four scoped files remain byte-identical to their pinned references;
**156 checks pass**. This batch used compiler, binary and host behavior checks.
Emulator and data-install validation remain deferred.

| Module | Before: functions / bytes | Added: functions / bytes | After: functions / bytes |
| --- | ---: | ---: | ---: |
| Main | 66 / 3,780 | 7 / 528 | 73 / 4,308 |
| Extra | 24 / 2,116 | 0 / 0 | 24 / 2,116 |
| Menu | 40 / 3,104 | 2 / 860 | 42 / 3,964 |
| Battle | 79 / 5,048 | 13 / 1,380 | 92 / 6,428 |
| Total | **209 / 14,048** | **22 / 2,768** | **231 / 16,816** |

The [machine-readable record](task-savedata-milestone.json) lists every addition,
source path, checked native caller or controller record, feature owner and
unchanged file hash. Inventory revision **15** updates source attribution;
the **21,280 candidate entries / 4,717,908 body bytes** remain unchanged.

## First save-owned source matches

The **464-byte list setup at `0x089DD760`** and **396-byte latest-slot selector
at `0x089DD9DC`** now compile exactly. These are controller records 2 and 4.
Their source is in [savedata_list_setup.c](../../src/menu/savedata_list_setup.c)
and [savedata_latest_slot.c](../../src/menu/savedata_latest_slot.c).

List setup generates twenty `GameData00`–`GameData19` names, prepares the SDK
list request and retains pointers into the execution state's name array, slot
list and data buffer. The constructor's checked clears supply the final empty
name record. Selection scans all twenty records, converts nonempty entries'
modification timestamps, and retains entries whose ticks are at least the
current best. Equal timestamps choose the later scanned entry. It copies both
the chosen name and timestamp, then starts the load request. An empty list or
failed request initialization takes the original fallback transition.

The state view now covers the observed **`0x3FAB0` allocation**, with an actual
`0x3ECC8`-byte buffer at `0xDD0` and a typed timestamp at `0x3FA98`. This avoids
expressing the buffer as an address beyond a short prefix view. The timestamp
fields agree with [PSPSDK's date layout](https://github.com/pspdev/pspsdk/blob/master/src/base/psptypes.h);
request field offsets agree with its
[savedata interface](https://github.com/pspdev/pspsdk/blob/master/src/utility/psputility_savedata.h).
This remains an observed layout view, not a claim about the original class name.

[PSP C/C++ layout checks](../../tools/test_savedata_layout.py) verify sizes, offsets,
capacity and an intentionally wrong offset. The
[host behavior check](../../tools/test_savedata_behavior.py) compiles both real
handlers and exercises retained pointers, fixed-slot scanning, 64-bit timestamp
ordering, ties, empty lists and SDK initialization failure. RTC conversion's
return status remains ignored, matching the native code; the harness supplies
successful conversions. These checks do not validate PSP firmware dialogs.

Save/profile source credit is now **2 functions / 860 bytes**, assigned once
to the save feature. Shared menu and main services retain their own owners.
Save evidence stays at **rank 2** because status polling, serialization and
complete ownership remain unfinished. The
[static request audit](savedata-request-mapping.json) checks bindings and
native observations; source credit comes separately from current build proofs.

## Recovered task support

Seven exact main routines cover task construction, task finalization, filtered
removal, remove-all, traversal count, owner finalization and callback lookup.
The constructor stores an unsigned filter word and signed priority, clears
the observed finalizer and auxiliary links, and returns the task pointer.
Filtered removal obtains the next matching task before removing the current
one. Task finalization invokes the optional finalizer before freeing storage;
owner finalization removes tasks before releasing the owner and allocator.

The [task view](../../include/main/task_runtime.h) preserves the observed `0x4C`
task and `0x18` owner sizes. Native task dispatch passes **`MainTask *`** to
the update callback; lifecycle callbacks retain their separate interface.
The earlier callback-forwarding wrapper and menu task entry keep their
matching bytes after these declaration corrections. The forwarding regression
also checks a negative priority and the delivered task pointer.

[Task tests](../../tools/test_task_runtime.py) compile native layout assertions and
exercise the real recovered source with allocator/unlink stubs. Removed task
storage is poisoned, checking that traversal cached its successor before
deletion. They also check callback delivery, finalizer/free order and allocator
lifetime. Inlined C++ iteration methods emit four unused compiler bodies totaling
136 bytes; those copies are explicitly audited and discarded, with **zero**
source credit. The seven original bodies contribute 528 bytes.

## Connected battle routines

Thirteen exact routines add 1,380 bytes across binding lookup, guarded update
paths, nested initialization/reset calls and small fixed-entry loops. Every
addition has a checked direct caller, including connections from existing
battle lifecycle paths. The two-slot lookup rejects unsigned indices at or
above two. Consumed drawing input is now declared as an unsigned slot index;
three ignored update results are declared as signed status words. Full-file
rebuilding verifies these refinements preserve existing source matches.

The battle overlay now has **92 functions / 6,428 bytes** recovered. These
remain shared battle helpers: character ability, AI and gameplay-mode ownership
are not yet established.

## Scope and next work

The [reviewed menu controller](menu-controller-coverage.md) advances from
**10 to 12 of 20 handlers**, and from **1,472 to 2,332 of 8,188 bytes**:
**60% by handler count / 28.48% by bytes**. This denominator excludes called
services and other controllers. Whole-menu and whole-game percentages remain
unknown; byte-identical assembly-backed builds do not imply full decompilation.

Task factory `0x088823C4`, dispatcher `0x0888245C`, registration `0x08882790`
and unlink `0x088828BC` remain nonmatching and uncredited. Guarded registration
and unlink probes avoid null member access but still differ from the native
instructions. Experimental volatile argument records and placement-new forms
receive no credit. Savedata poll handlers `0x089DD930` and `0x089DDB68` also
remain uncredited. The earlier main `0x088D99B0` and menu `0x089DEA74` entries
remain untraced.

Next, recover the two savedata polling phases and finish task queue
registration/unlink/dispatch with targeted compiler evidence. This would close
more of the connected save and runtime paths before moving to larger controller
handlers. Preserve recorded failed variants and revisit them with new evidence.

No emulator was launched, no ISO or runtime package was rebuilt, and no saves
were changed. Earlier runtime observations remain separate from this batch's
source and binary evidence.

# Savedata polling and task dispatch

12 October 2026. **Three additional functions / 700 source-built code bytes**
now match, bringing recovery to **234 C/C++ functions / 17,516 bytes**.
The main executable and all three overlays remain byte-identical to their
pinned references. **159 checks pass**; emulator and data-install validation
remain deferred.

| Module | Before: functions / bytes | Added: functions / bytes | After: functions / bytes |
| --- | ---: | ---: | ---: |
| Main | 73 / 4,308 | 1 / 344 | 74 / 4,652 |
| Extra | 24 / 2,116 | 0 / 0 | 24 / 2,116 |
| Menu | 42 / 3,964 | 2 / 356 | 44 / 4,320 |
| Battle | 92 / 6,428 | 0 / 0 | 92 / 6,428 |
| Total | **231 / 16,816** | **3 / 700** | **234 / 17,516** |

The [machine-readable record](save-poll-dispatch-milestone.json) includes every
new source path, checked native caller or member record, file hash and the
completed callback slice. Inventory revision **16** updates attribution without
changing the **21,280 candidate entries / 4,717,908 body bytes**.

## Four savedata callbacks now match

Controller records 2–5 form a contiguous **1,216-byte slice**, from
`0x089DD760` through `0x089DDC20`. All four original function bodies now
have exact source: list setup, list polling, latest-slot selection and load
polling. That is **4/4 handlers and 1,216/1,216 bytes** for this reviewed slice.
The denominator excludes called SDK/runtime services, serialization and other
save operations. The full save feature remains incompletely mapped at rank 2.

The new [list poll](../../src/menu/savedata_list_poll.c) contributes 172 bytes;
the [load poll](../../src/menu/savedata_load_poll.c) contributes 184 bytes. Their
native status handling is:

| Status from GetStatus | Both handlers | Load poll only |
| ---: | --- | --- |
| 0 | Advance the phase | Set the completion flag at `0xDCC` |
| 1 | Wait | No change |
| 2 | Call Update with argument 2 | No change |
| 3 | Call ShutdownStart | No change |
| 4 | Wait | No change |
| Any other value | Call ShutdownStart and set phase 6 | No change |

Earlier nonmatching drafts had incorrectly assigned advancement to status 4
and fallback to status 0. Calculating the native branch destinations exposed
the error: the branch for zero enters the advancement block, the branch for
four enters the empty block, and the out-of-range branch enters fallback.
Those drafts never received source credit. The corrected cases reproduce all
instructions, including switch dispatch and delay slots, without changing the
compiler profile or patching machine code.

Pinned main import records bind the three calls to savedata GetStatus,
ShutdownStart and Update. The
[official import definitions](https://github.com/pspdev/pspsdk/blob/master/src/utility/sceUtility.S)
and [SDK interface](https://github.com/pspdev/pspsdk/blob/master/src/utility/psputility_savedata.h)
support their signatures; the game's actual branches and argument 2 establish
its behavior. The completion flag does not by itself establish successful
payload loading: these two handlers do not inspect the savedata result field.

[Polling checks](../../tools/test_savedata_poll.py) pin the three library/NID
bindings and compile the real handlers with SDK stubs. They exercise all five
known statuses, negative and large unknown values, the polling cycle, unchanged
flags while waiting, and ignored shutdown return values. Earlier request,
timestamp and persistent-buffer checks continue to pass. These are host and
binary checks, not firmware dialog tests.

The larger [twenty-handler controller](menu-controller-coverage.md) advances to
**14/20 handlers / 2,688 of 8,188 bytes**: **70% by count / 32.83% by code bytes**.
Whole-menu and whole-game percentages remain unknown.

## Exact task dispatcher

The **344-byte dispatcher at `0x0888245C`** now matches in
[task_dispatch.cpp](../../src/main/task_dispatch.cpp). It selects tasks using the
current mask/value pair, publishes the next eligible cursor before invoking a
callback, installs the current task context, and restores that context afterward.
A task without an update callback is removed. A callback can change its mask
and value fields; the dispatcher reads them back and rechecks pending work.

The source uses a referenced cursor for in-place advancement, a local callback
pointer and a bounded scope for the saved context. These reproduce the original
register and branch sequence under the unchanged SNC `-O2 -g` profile. Three
emitted inline helper copies are independently audited and discarded; only the
original 344-byte dispatcher receives credit.

[Dispatcher checks](../../tools/test_task_dispatch.py) compile the real source and
exercise empty queues, removal of pending work, removal of a task without a
callback, changed filters and nested dispatch through another owner. Removed
storage is poisoned. The checks verify callback task pointers, pending cursor
updates, restored global context and cleared owner context. Unlink is supplied
by a stub implementing the inspected native contract, so these checks do not
claim that the still-nonmatching unlink source has been recovered.

## Remaining work

Task factory `0x088823C4`, registration `0x08882790` and unlink `0x088828BC`
remain nonmatching. A factory candidate and a registration candidate have the
correct sizes but different instructions; size equality earns no credit.
Guarded link accessors and standard member-pointer variants also differ.
Candidates that compute members through null task pointers remain excluded.

Next, finish those task queue operations or move to the remaining controller
bootstrap, resource-wait and drawing handlers when new compiler evidence is
available. The save callback slice is complete within its stated denominator;
save serialization and broader ownership still need investigation. Main
`0x088D99B0` and menu `0x089DEA74` remain explicitly untraced.

No emulator was launched, no ISO or runtime package was rebuilt, and no saves
were changed. Earlier retail comparison observations remain separate from
this batch's source evidence.

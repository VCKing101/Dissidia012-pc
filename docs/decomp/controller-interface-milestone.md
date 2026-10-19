# Controller interfaces and savedata request mapping

12 October 2026. **13 additional functions / 656 source-built code bytes** now
match, bringing recovery to **209 C/C++ functions / 14,048 bytes**. All four
scoped files remain byte-identical; **152 checks pass**. Emulator and
data-install validation remain deferred.

| Module | Before: functions / bytes | Added: functions / bytes | After: functions / bytes |
| --- | ---: | ---: | ---: |
| Main | 63 / 3,600 | 3 / 180 | 66 / 3,780 |
| Extra | 24 / 2,116 | 0 / 0 | 24 / 2,116 |
| Menu | 30 / 2,628 | 10 / 476 | 40 / 3,104 |
| Battle | 79 / 5,048 | 0 / 0 | 79 / 5,048 |
| Total | **196 / 13,392** | **13 / 656** | **209 / 14,048** |

The [machine-readable record](controller-interface-milestone.json) lists every
addition, source path, caller/callback-pointer evidence and unchanged file hash.
Inventory revision **14** changes attribution and interface interpretation only;
the **21,280 candidate entries / 4,717,908 body bytes** remain unchanged.

## Corrected callback forwarding

Main `0x08865088` was previously declared as a one-argument `void` wrapper and
described as release. Inspecting `0x088823C4` establishes that it consumes the
forwarded `a1`–`a3` arguments and returns a task pointer. The original wrapper
passes those registers through unchanged. Its old C declaration happened to
produce matching machine code, but omitted that source behavior.

The corrected source explicitly forwards both native words and the callback,
returns the callee's pointer, and accepts the observed unused label argument.
Its **28 bytes still match**, and no extra matching credit is awarded for this
correction. A [host C behavioral regression](../../tools/test_callback_interface.py)
compiles the real source with a checked native stub, verifies every forwarded
value, invokes the callback and checks the returned pointer. This supplements
PSP byte comparison; it is not an emulator/gameplay test.

State helpers now share `MainCallbackBlock`, retaining typed callback fields.
The menu context event at `0x089DE9EC` is declared as executable code rather
than a data array. All affected old source functions retain their exact bytes.

## Connected source recovered

Main adds the shared PSP dialog header initializer plus its language-table and
button-setting inputs. The initializer clears the common header, sets its size,
copies those settings and writes the observed priorities. Field meanings agree
with [PSPSDK's common dialog layout](https://github.com/pspdev/pspsdk/blob/master/src/utility/psputility.h).

Menu adds its storage allocation/free pair, completion-flag callback, callback
registration/context setup, three global-instance entry wrappers, the resource
event wrapper and a reinstallation path. The original storage label is
`TITLE::BUF`; complete UI/game-mode ownership is still unresolved.

The **52-byte `0x089DEA74` reinstallation path remains untraced**: its existing
extent and exact C bytes are verified, but no checked direct caller or absolute
pointer currently establishes runtime reachability. The earlier main
`0x088D99B0` entry also remains untraced. Neither receives a runtime-entry claim.

## Savedata behavior mapped, not source-complete

Pinned main import records bind `0x089C5858` to `sceUtilitySavedataInitStart` and
`0x089C5D78` to `sceRtcGetTick`, using the official
[utility imports](https://github.com/pspdev/pspsdk/blob/master/src/utility/sceUtility.S),
[RTC imports](https://github.com/pspdev/pspsdk/blob/master/src/rtc/sceRtc.S) and
[RTC interface](https://github.com/pspdev/pspsdk/blob/master/src/rtc/psprtc.h).
Native handler analysis and
[the savedata API layout](https://github.com/pspdev/pspsdk/blob/master/src/utility/psputility_savedata.h)
establish these observations:

- Setup formats twenty `GameData00`–`GameData19` names and retains a twenty-first
  empty name-list record. It prepares a list request for `GameData*` and the
  data filename `DISSIDIA.BIN`.
- The next selection phase scans nonempty entries, converts modification dates
  to ticks, retains entries whose value is at least the current best, and
  submits an AUTOLOAD request for the selected name. Ties select the later
  scanned entry; serialization contents remain unreviewed.
- The request view is `0x600` bytes; twenty `0x48`-byte slot entries occupy
  `0x5A0` bytes. The `0x3ECC8`-byte data buffer starts at state offset `0xDD0`;
  the following cached timestamp fits within the observed `0x3FAB0` allocation.

[The static audit](../../tools/audit_savedata_setup.py) repeats import, call,
mode/loop/clear, string and extent checks from
[review metadata](../../config/menu-savedata-setup-review.json).
[PSP C/C++ layout tests](../../tools/test_savedata_layout.py) check the actual
target offsets and reject an incorrect list offset. The
[generated mapping](savedata-request-mapping.json) remains explicitly static.

Save/profile evidence advances to **rank 2: behavior explained in part**.
Its owned handler source count remains **zero**. This is an ordinal evidence
milestone, not 40% completion or a complete save-format reconstruction.

## Remaining matching work

The twenty-member controller still has **10/20 matching handlers and
1,472/8,188 matching bytes**; this batch adds supporting paths, not new table
members. The [coverage report](menu-controller-coverage.md) remains unchanged.
Whole-menu and whole-game completion percentages remain unknown.

The list setup draft is three instructions away from exact equality. Slot
selection, optional-resource wait, bootstrap and drawing index/release drafts
also remain nonmatching and uncredited. A bounded fifteen-case comparison of
other optimization levels found no exact matches; production SNC `-O2 -g`
settings remain unchanged. No instructions were patched.

Next, audit consumed arguments/results of adjacent forwarding wrappers and
continue medium-sized request/state routines with targeted source/compiler
evidence. Avoid repeating the recorded failed variants without new evidence.
No emulator was launched and no ISOs, packages or saves changed. Runtime
packages still require refreshed source provenance before future testing.

# Menu dispatch, resource operations and battle cleanup

**Later audit/source batch:** [member callback and control services](member-audit-milestone.md)
brings current recovery to 177 functions / 10,992 bytes and resolves the unlisted
member target as an eight-byte empty callback. The figures below retain the
preceding dispatch/cleanup snapshot.

12 October 2026. **28 additional functions / 2,452 code bytes now rebuild from
matching C.** Total recovery grows from **128 / 7,180 bytes** to **156 / 9,632
bytes**. All four scoped files remain byte-identical and **116 checks pass**.
No emulator or data-install validation was run for this source batch.

| Module | Before: functions / bytes | Added: functions / bytes | After: functions / bytes |
| --- | ---: | ---: | ---: |
| Main | 37 / 1,948 | 11 / 576 | 48 / 2,524 |
| Extra | 24 / 2,116 | 0 / 0 | 24 / 2,116 |
| Menu | 21 / 1,240 | 5 / 732 | 26 / 1,972 |
| Battle | 46 / 1,876 | 12 / 1,144 | 58 / 3,020 |
| Total | **128 / 7,180** | **28 / 2,452** | **156 / 9,632** |

The [machine-readable record](dispatch-cleanup-milestone.json) lists additions,
source units, checked direct callers/member records and module hashes.
[Current progress](progress.md) is regenerated from verified builds.

## Newly recovered paths

Menu's 152-byte member dispatcher now has exact source. It copies twenty
eight-byte records, selects one through the state index, adjusts the receiver,
handles the nonzero-flags member-table resolution path and invokes the target.
The record representation is observed ABI evidence; original class names remain
unknown. Callback receiver types agree with the recovered handlers.

Four additional menu handlers initialize resource/state fields, choose the
observed resource identifier, optionally configure a recovered main resource
handle, or select state 9/11. They remain described by their observed fields and
calls rather than assigned a specific gameplay/menu purpose.

Main adds eleven manager/native-operation wrappers: acquisition/release,
command selection and normalization against `-1`, a byte-conditioned command,
native result forwarding and indexed slot-flag clearing. Three previously
external command return declarations now correctly agree with the recovered
`int` definitions; existing callers ignore those results.

Battle adds twelve cleanup routines: six root-subobject deleting wrappers,
a nested deleting wrapper, four-entry/resource cleanup, conditional manager
release/reset, and two small state resets. The resource-pointer views and reset
view now agree on the pointer at offset `+4`. Original ownership/classes are
still incomplete.

## Savedata interface mapping

The pinned main import-library records identify these bindings:

| Main stub | NID | API |
| --- | --- | --- |
| `0x089C5868` | `0x8874DBE0` | `sceUtilitySavedataGetStatus` |
| `0x089C5878` | `0x9790B33C` | `sceUtilitySavedataShutdownStart` |
| `0x089C5898` | `0xD4B95FFB` | `sceUtilitySavedataUpdate` |

The API names and signatures are corroborated by the primary
[PSPSDK import definitions](https://github.com/pspdev/pspsdk/blob/master/src/utility/sceUtility.S)
and [savedata header](https://github.com/pspdev/pspsdk/blob/master/src/utility/psputility_savedata.h).
Checked calls from `menu:0x089DD930` and `menu:0x089DDB68`, plus their member
records, are retained in [the mapping review](../../config/savedata-dialog-review.json).

This supports raising the save/profile feature from **unmapped to mapped in
part (rank 1)**. Its complete owned scope and serialization format remain
unknown. Both polling switches still differ under bounded C/C++ source trials,
so their drafts live under `tools/probes/` and earn **zero matching-C bytes**.
No completion percentage or runtime-verification rank is inferred from this.

## Validation and boundaries

All integrated functions pass fully resolved raw relocation comparison and
fixed-address linked-byte comparison with the established SNC `-O2 -g` profile.
Main, menu and battle full files were rebuilt; extra's current proof was checked
during aggregation. Full hashes remain unchanged. The 116-check suite includes
source-credit rejection, shared-header provenance, import exclusions, original
geometry, inventory and progress integrity.

Read-only review found no material source/prototype/layout issue. The optional
reset-pointer clarification was applied and the battle build reverified. SDK
import stubs, copied table/data bytes, assembly and unfinished probes receive no
C credit.

Inventory revision **11** changes source attribution only, preserving the
**21,279 candidates**, boundaries and unknown-byte regions. Other feature ranks
remain unchanged. The twenty-record table's final target, `0x089DE3A4`, has no
current inventory function entry; it remains outside C credit pending a boundary
audit.

Resource-wait, savedata-switch, allocator/scaled-command and battle-loop
mismatches remain bounded, preserved and uncredited. Package provenance still
needs regeneration before future emulator work; ISO files and saves were not
modified here.

Next, audit the unlisted member target and continue the connected state/resource
paths. Keep emulator/data-install validation deferred while source recovery
advances.

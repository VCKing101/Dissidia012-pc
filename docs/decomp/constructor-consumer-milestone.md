# Connected initialization and configuration consumers

**Later source batch:** [menu dispatch and cleanup](dispatch-cleanup-milestone.md)
brings current recovery to 156 functions / 9,632 bytes. The figures below preserve
the preceding connected initialization batch.

11 October 2026. **26 additional functions / 1,952 code bytes now rebuild from
matching C.** Recovery grows from **102 / 5,228 bytes** to **128 / 7,180 bytes**.
All four scoped files remain byte-identical; **116 checks pass**. This continues
the [previous source expansion](source-recovery-milestone.md). Emulator and
data-install validation remain deferred at the user's request.

| Module | Before: functions / bytes | Added: functions / bytes | After: functions / bytes |
| --- | ---: | ---: | ---: |
| Main | 27 / 1,360 | 10 / 588 | 37 / 1,948 |
| Extra | 24 / 2,116 | 0 / 0 | 24 / 2,116 |
| Menu | 17 / 580 | 4 / 660 | 21 / 1,240 |
| Battle | 34 / 1,172 | 12 / 704 | 46 / 1,876 |
| Total | **102 / 5,228** | **26 / 1,952** | **128 / 7,180** |

The [machine-readable record](constructor-consumer-milestone.json) lists every
addition, verified direct caller or member-table entry, function size, source
unit and unchanged module hash. [Current progress](progress.md) is regenerated
from current verified builds.

## The connected source paths

Battle's seven initialization targets called by recovered setup root
`battle:0x089DD4D0` now all have exact C: `0x089E06A8`, `0x089E3208`,
`0x089E68E0`, `0x089E7130`, `0x089E79A8`, `0x089E8E70` and `0x089ECF40`.
The batch also adds two nested initialization routines and three state resets.
Observed behavior includes member-table installation, nested helper calls,
field initialization and returning the object pointer. These support the
constructor interpretation without identifying original classes or mechanics.

Main adds the resource-handle constructor/deletion wrapper used by menu's four
subobjects, sentinel-guarded acquisition/release and three checked manager
commands. It also recovers the descriptor construction wrapper used by a menu
initializer, its field/subobject initialization and a byte/reset helper.
The resource's concrete purpose remains unresolved.

Menu adds two configuration consumers that select the default or supplied
index, reject a negative selection, read the two halfword fields and pass a
local buffer/output through existing shared services. These follow the accessors
already recovered from the reviewed numeric configuration region. Their buffer
contents and higher-level menu ownership remain unknown.

Two menu state-update routines now have source as well. They submit an observed
flag, advance the state word, or poll a result and advance on a nonzero response.
Their `{adjustment=0, flags=0, callback}` records at `0x089E09E8` and
`0x089E09F0` bind them to the existing member-table dispatcher at
`menu:0x089DD5DC`. That dispatcher itself remains assembly-backed.

## Shared types and verification

Review checked recovered cross-unit signatures and partial layouts. The main
descriptor wrapper returns its input pointer; menu's declaration now agrees.
Menu's state pointer and fields reproduce the observed `+0x1DC`, `+0x08`,
`+0x49` and `+0x4C` accesses.

The newly recovered main constructor exposed a layout improvement: menu's four
resource slots now contain `MainResourceHandlePrefix` plus four unknown trailing
bytes, retaining the observed `0x24` stride. Construction/teardown pass the typed
prefix address. The shared main header is included in menu build provenance.
All original menu byte matches remain intact after this change.

All new functions pass raw relocation-resolved comparison using SNC
**1.2.7503.0 `-O2 -g`**, followed by fixed-address linking and whole-file equality.
Main, menu and battle were rebuilt; extra's current proof was rechecked during
aggregation. The full 116-check suite passes, including source-credit rejection,
header provenance, original layout preservation and unknown completion totals.

Inventory revision **10** changes source attribution only: the **21,279 existing
candidates**, boundaries and unknown-byte regions are unchanged. Feature ranks
and the audited whole-game percentage are not promoted.

## Bounded unfinished work

Four battle loop-clear candidates still differ in one commutative ADDU operand
ordering word each; the main clamp candidate also remains nonmatching. They
retain assembly and receive zero matching-C credit. No instruction patching or
byte masking was used to force an apparent match.

The earlier ISO packages and saves were not modified or tested again. Package
provenance still refers to the earlier source inputs and must be refreshed before
future emulator work. The earlier runtime record remains a historical scenario
baseline, not exhaustive feature coverage.

Next, recover services called behind the now-source-built initialization and
configuration paths, including resource operations and remaining state/member
dispatch. Keep difficult matching attempts bounded and leave runtime/data-install
validation deferred while source recovery advances.

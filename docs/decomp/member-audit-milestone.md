# Member callback audit and connected control services

12 October 2026. **21 additional functions / 1,360 code bytes now rebuild from
matching C**, bringing recovery to **177 functions / 10,992 bytes**. This crosses
10 KiB of exact source-built code. All four scoped files remain byte-identical;
**122 checks pass**. Emulator/data-install validation remains deferred.

| Module | Before: functions / bytes | Added: functions / bytes | After: functions / bytes |
| --- | ---: | ---: | ---: |
| Main | 48 / 2,524 | 9 / 456 | 57 / 2,980 |
| Extra | 24 / 2,116 | 0 / 0 | 24 / 2,116 |
| Menu | 26 / 1,972 | 1 / 8 | 27 / 1,980 |
| Battle | 58 / 3,020 | 11 / 896 | 69 / 3,916 |
| Total | **156 / 9,632** | **21 / 1,360** | **177 / 10,992** |

The [machine-readable record](member-audit-milestone.json) lists each addition,
source unit, caller/member evidence, unchanged module hash and explicit untraced
entry. [Current progress](progress.md) is regenerated from verified builds.

## The unlisted member target

Menu `0x089DE3A4` is an eight-byte empty callback: `jr $ra; nop`. Its entry is
referenced by record 19 at `0x089E0A80`, with zero adjustment/flags. The recovered
dispatcher copies all twenty eight-byte records; the callback ends exactly at
the existing `0x089DE3AC` function boundary. It is a real existing table entry,
not a newly invented stub.

[The review metadata](../../config/menu-member-table-review.json) and
`tools/audit_menu_member_table.py` repeat those checks against the pinned file.
Rejection tests cover a redirected record, a copy that omits the last record,
a nonempty body and an incorrect neighbor boundary. Inventory attribution also
checks the review before accepting the additional function and any matching C.

The callback now has a plain empty C body at its original address. Menu gains
one function and eight decoded/code-body bytes; eight previously unknown bytes
become accounted code. The inventory reaches **21,280 candidates / 4,717,908
function-body bytes**. Menu unknown bytes decrease from 116,152 to **116,144**.
No other boundaries or byte classifications changed, and the full module audit
remains incomplete.

## Connected services recovered

Main adds four-byte record copy/setters, flag-bit changes with existing follow-up
calls, an owner-byte accessor, a matrix-pointer/field update and the context
flag routine used by menu. The four-byte setter agrees with the menu consumer's
four-byte output storage. The context flag parameter is `unsigned char`; its
menu declaration is now consistent and retains the exact call bytes.

One matched main routine, **`0x088D99B0` / 44 bytes**, remains explicitly
untraced. Its existing inventory extent and raw/linked C bytes match, but no
checked caller/member entry currently establishes runtime reachability. Its
source credit does not promote a runtime or owning-feature claim.

Battle adds active-state update gates, a `0/1` readiness predicate, bounds checks
for three flag slots, staged checks with negative error results and reset/bind
activation. All eleven additions have checked direct callers. Original class
names, detailed flag meanings and gameplay ownership remain unresolved.

## Verification and remaining work

The established SNC `-O2 -g` profile produced all exact matches. Raw relocations
are fully resolved, linked addresses/bytes checked and complete files compared
with pinned references. Main, menu and battle were rebuilt; extra's proof was
rechecked. Memory-only BSS, file geometry, data and fallback-byte policies remain
intact.

Read-only review found the new prototypes/layouts coherent. Menu provenance now
also pins the audit's file-image reader, inventory builder and canonical menu
override review. A regression check rejects changed entry-review provenance
behind unchanged output bytes. **122 total checks pass**.

Inventory revision **12** records the eight-byte correction and source
attribution. The audited whole-game percentage and feature ranks remain
unchanged. SDK imports, assembly, copied data and nonmatching probes receive no
matching-source credit. Savedata switch, transform/flag and loop experiments
remain bounded and uncredited.

No emulator was run, and ISO files/saves were not modified. Existing package
provenance remains a previous-source snapshot and needs regeneration before
future runtime work.

Next, continue the remaining member-table handlers and shared drawing/resource
services. Trace the unreferenced main entry when better dependency evidence is
available; do not present it as runtime verified.

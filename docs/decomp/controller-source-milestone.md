# Menu controller coverage and connected source services

12 October 2026. **19 additional functions / 2,400 code bytes rebuild from
matching C/C++**, bringing recovery to **196 functions / 13,392 bytes**. All
four scoped files remain byte-identical to their pinned references; **142
checks pass**. Emulator and data-install validation remain deferred.

| Module | Before: functions / bytes | Added: functions / bytes | After: functions / bytes |
| --- | ---: | ---: | ---: |
| Main | 57 / 2,980 | 6 / 620 | 63 / 3,600 |
| Extra | 24 / 2,116 | 0 / 0 | 24 / 2,116 |
| Menu | 27 / 1,980 | 3 / 648 | 30 / 2,628 |
| Battle | 69 / 3,916 | 10 / 1,132 | 79 / 5,048 |
| Total | **177 / 10,992** | **19 / 2,400** | **196 / 13,392** |

The [machine-readable milestone](controller-source-milestone.json) records every
addition, source unit, checked caller/member entry, module hash and excluded
inline definition. Every new function has a checked caller or member record.

## A reproducible local percentage

The recovered dispatcher copies twenty member records from `0x089E09E8`.
The [controller coverage report](menu-controller-coverage.md) now records **10
of 20 matching handlers: 50% by count**, and **1,472 of 8,188 matching bytes:
17.98% by code bytes**. Larger pending handlers account for most of the bytes.

[Scope metadata](../../config/menu-controller-scope.json) pins each target and
extent. The generator checks the copied table, zero adjustment/flags, unique
targets, contiguous bounds, terminal return/delay slot, next known entries,
bounded relative branches and nonoverlap against the pinned original. It then
uses current raw/linked/full-file source proof for the numerator.

This denominator covers one controller's twenty callbacks. Its dispatcher
matches separately; called services and other controllers are outside the
denominator. It does not establish whole-menu, gameplay-mode or whole-game
completion. The full menu inventory remains provisional, and the whole-game
audited percentage stays unknown.

## Connected behavior recovered

Menu adds context finalization (`0x089DDC20`), resource readiness/release and
counter-threshold handling (`0x089DDDA0`), and selected-resource release with a
presentation request (`0x089DE0A4`). These join already matched creation and
state-selection handlers. The optional-resource wait and savedata polling
switches remain nonmatching drafts.

Main adds the context resource-request service, resource status forwarding,
derived drawing flags, transform-field updates and integer-coordinate updates.
The status service reaches an already recovered manager routine. Drawing
transform declarations now agree across main/menu; the menu stack buffer is
typed as sixteen floats, retaining its original size and exact consumer bytes.

Battle adds activation/reset chains, bounded slot loops, readiness checks and a
drawing-flag update. The largest new activation routine is 272 bytes. The
drawing step compares a signed service result with its cached value; the
meaning or unit of that value remains unclaimed. Partial views do not establish
original class names or complete gameplay ownership.

## Exact C++ inline support

The established **SNC 1.2.7503.0 `-O2 -g` profile is unchanged**. Static in-class
accessors reproduce original flag conversions and store placement that earlier
C probes did not match. No instructions were patched or copied into C/C++.

SNC also emits weak out-of-line copies in separate linkonce sections.
[The audit](../../tools/compiler_inline_sections.py) permits only explicitly listed,
complete, read-only weak function bodies with no references from retained
allocated bytes, including across units. Helper names cannot be assigned
external original addresses or source credit. Raw compiler objects remain
intact; the linker discards the audited copies. **Ten emitted copies / 196
bytes receive zero matching-source credit.** Independent verification repeats
the audit and checks its saved evidence.

Review removed an unsupported battle base-to-derived cast; static accessors on
the established drawing prefix retain exact bytes. Rejection checks cover
live references, unlisted/strong/incomplete/writable bodies, altered evidence
and extra code. Scope checks reject changed identities, targets and extents.

Inventory revision **13** updates source attribution only: **21,280 candidates /
4,717,908 function-body bytes**, with no boundary/classification changes. All
four build-input proofs are current. Main `0x088D99B0` remains explicitly
untraced from the previous batch; feature ranks are unchanged.

No emulator was launched, and ISOs, runtime packages and saves were not changed.
Existing packages need refreshed build-input provenance before future runtime
work. Next, recover the optional-resource wait and larger setup/dialog handlers,
following their shared-service dependencies while keeping difficult attempts
bounded.

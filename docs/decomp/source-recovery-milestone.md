# Matching C source expansion

**Later connected batch:** [initialization and configuration consumers](constructor-consumer-milestone.md)
bring current recovery to 128 functions / 7,180 bytes. The figures below preserve
the preceding 42-function expansion.

11 October 2026. **42 additional functions / 1,984 code bytes now rebuild from
C at their original addresses.** Total exact recovery grows from **60 / 3,244
bytes** to **102 / 5,228 bytes**. All four scoped files remain byte-identical.
Data-install runtime validation is deferred at the user's request; this batch
focused on source recovery and static build checks.

| Module | Before: functions / bytes | Added: functions / bytes | After: functions / bytes |
| --- | ---: | ---: | ---: |
| Main | 15 / 836 | 12 / 524 | 27 / 1,360 |
| Extra | 24 / 2,116 | 0 / 0 | 24 / 2,116 |
| Menu | 11 / 180 | 6 / 400 | 17 / 580 |
| Battle | 10 / 112 | 24 / 1,060 | 34 / 1,172 |
| Total | **60 / 3,244** | **42 / 1,984** | **102 / 5,228** |

The [machine-readable record](source-recovery-milestone.json) lists every added
function's address, size, source unit, inventory confidence and module reference
hash. The [current progress report](progress.md) is regenerated from verified
builds; no invented functions or omitted unknown regions increase these totals.

## Recovered behavior

Main adds null-guarded registered callback runners, callback-block construction,
three overlay-mode ensure routines, controller phase wrappers, a state end
request and a resource-acquisition helper. This recovers executable pieces of
the callback/overlay path previously mapped in the
[deferred battle audit](deferred-battle-audit.md).

Battle adds its called setup root, guarded multi-subobject updates, setup/cleanup
dispatch, selected state transitions and called subobject wrappers. The root
initializes seven subobjects; other recovered routines guard active state,
update fields and invoke existing helpers. Direct call-site evidence supports
all 23 added non-root helpers, while the root has the already-reviewed main setup
call. Original class names and specific combat-mechanic ownership remain unknown.

Menu adds its 152-byte setup and 116-byte guarded teardown, two signed-halfword
configuration accessors, and the final two descriptor-referenced initializers.
All eight loader-descriptor initializers now have exact C. The setup models the
observed callback block, four subobjects and three table pointers. The teardown
reverses the subobject calls and conditionally releases the owner. This does not
complete the whole menu flow or establish full object sizes.

## Verification and review

All integrated routines use the established SNC **1.2.7503.0 `-O2 -g`** profile.
Raw compiler functions pass relocation-resolved comparisons without masking
operands; linked functions retain their exact addresses and bytes. No emitted
instructions were patched, and assembly/raw words receive no C credit.

Main, menu and battle were rebuilt and compared as complete files; extra's
existing verified build was rechecked during aggregation. Reference file hashes
remain unchanged, including main's memory-only BSS and preserved file regions.
**116 checks pass**, covering build identity, relocation/source attribution,
inventory/dispatch integrity, progress and existing runtime/package helpers.

Read-only source review found and resolved cross-unit battle prototype
conflicts. A shared partial state record/prototype header now gives callers and
definitions consistent types. Menu uses the real recovered callback-block member
type; both shared-header dependencies are fingerprinted. All byte matches were
retained after these changes.

The battle verifier now requires matching-C preparation ranges and unit names
to equal the C manifest, and rejects unknown part kinds. Three added rejection
checks prevent an assembly relabel or forged unit identity from retaining C
credit behind unchanged output bytes. Review of the fixes found no remaining
material issue.

Inventory revision **9** changes source attribution only. The **21,279 candidates**,
function boundaries, code/data layout and unknown-byte regions are unchanged.
Feature ownership is still incomplete, so evidence ranks and the audited
whole-game percentage are not promoted.

## Runtime and next work

No emulator was run for this batch. The earlier
[runtime milestone](hub-runtime-validation.md) remains evidence for its tested
bytes and scenarios; all newly source-built file bytes are identical to those
references. Existing ISO files and save profiles were not modified. Their package
manifest still records the earlier build-input fingerprints, so regenerate the
packages with `tools/prepare_runtime_packages.py` before a future launcher run.
That provenance refresh is deferred with runtime work.

Next, follow the now-recovered battle root into its called subobject constructors,
recover adjacent controller/member dispatch, and expand menu configuration
consumers. Keep difficult nonmatching main list candidates bounded and uncredited.
Source recovery is the current priority; data-install testing stays deferred.

# Three-case post-tutorial runtime milestone

11 October 2026. **Both preserved saves load directly to the main hub on retail,
original-decrypted and rebuilt.** Quick Battle reaches a live fight, its
post-fight menu and battle/character selection on all three. Scenario 012's
prologue loads its opening, in-engine cutscene, world-map onboarding, movement
and map overview on all three.

| Checkpoint | Retail | Original decrypted | Rebuilt |
| --- | --- | --- | --- |
| Slot 1 load to main hub after restart | Pass | Pass | Pass |
| Slot 2 load to main hub on separate launch | Pass | Pass | Pass |
| Quick Battle character/setup menus | Pass | Pass | Pass |
| Live fight on Old Chaos Shrine | Pass | Pass | Pass |
| Post-fight menu and return to selection | Pass | Pass | Pass |
| Path to Sanctuary opening/cutscene | Pass | Pass | Pass |
| World-map load and movement | Pass | Pass | Pass |
| Map overview | Pass | Pass | Pass |
| Actual gateway entry | Pending | Pending | Pending |
| Victory/result branch | Pending | Pending | Pending |

## Evidence and preserved inputs

The [machine-readable record](hub-runtime-validation.json) pins package and
emulator hashes, separate launch times for each slot, 43 visually reviewed
captures with verified hashes, seed provenance and final copied-save hashes.
The [scenario definitions](../../config/runtime-scenarios.json) retain partial
status for Battle/Story scenarios whose remaining acceptance checks are unrun.

All eighteen initial copied save files matched the six-file human baseline.
The first slot-2 launch left those bytes unchanged. Battle/Story play subsequently
changed the disposable copies; reloading that newly earned progress is still a
separate pending check. The original human profile and preserved baseline were
rehashed at the end and remained unchanged. Both original slots are therefore
usable for future tests without replaying the opening tutorials.

The user-reported physical controls/audio result remains a separate rebuilt-game
test. The [original human record](human-runtime-result.json) preserves that
earlier reporting boundary; this report adds the automated restart/load evidence.

## Battle and Story coverage limits

Quick Battle used Lightning Lv1 against Lightning Lv6 (Auto level), Maximum
CPU strength, Tactician behavior, Old Chaos Shrine and None rules. CPU assists
and music were automatically chosen and differed between cases. Fight states,
timing and frames were not synchronized. This is a semantic menu/load/outcome
comparison rather than a deterministic combat benchmark.

Brief movement, jump, attack and shoulder inputs were sent and recorded. Hit
interruption and different fight states prevent attributing every individual
response. The post-fight defeat menus and return path are covered; victory and
its result branches remain untested. World-map movement provides a separate
uninterrupted analog-input check.

Story coverage stops after world-map onboarding and movement, at an NPC's
light/safety tutorial. The first actual gateway, chapter completion and subsequent
progress save/reload are not verified. These observations do not establish
complete combat, story, character, stage or cutscene coverage.

## Harness and verification

The new `hold_analog` helper sends bounded stick coordinates and returns the
stick to neutral in `finally`, including interruption. The parameter contract
comes from [PPSSPP 1.20.4's input subscriber](https://github.com/hrydgard/ppsspp/blob/v1.20.4/Core/Debugger/WebSocket/InputSubscriber.cpp#L222-L243).
Eight debugger regression checks pass, including interrupted-input cleanup and
rejection of invalid/nonfinite coordinates or duration. All four static build
proofs were revalidated against the package manifest.

The first Windows administrator attempt was canceled; a retry successfully
applied the new executable-specific firewall rules. Each normal-user launcher
verified effective rules before enabling its debugger. After the usage pause,
the launchers had stopped automatically. Slow concurrent ISO hash reads were
staggered on restart; only verified pre-launch workers were interrupted.

Menu transitions and title attract playback required reviewing each case and
realigning inputs. Captures that show intermediate screens are retained locally
but are not promoted as completed checkpoints. Captures use the last complete
local video frame, which can slightly precede the CPU pause.

All three owned comparison emulators are now stopped, and remote startup is
reset off. Game content, videos and save copies remain local and Git-ignored.
Matching C remains **60 functions / 3,244 bytes**; feature ranks and the audited
whole-game percentage are unchanged.

## Next work

Use the established harness for data-install success/cancellation/storage and
suspend/resume scenarios while resuming the mapped subsystem's source recovery.
Keep gateway traversal, victory/result coverage and new-progress save/reload in
the broader runtime queue.

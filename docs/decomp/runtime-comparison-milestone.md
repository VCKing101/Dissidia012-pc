# Three-case profile baseline and live combat input

11 October 2026. **Retail, original-decrypted and rebuilt cases now pass the same
boot/title/profile/save-load route.** Each created a real PSP save, restarted as a
separate process, loaded its own save successfully and resumed the opening.
All three savedata files per case were byte-identical before and after that load.

Live tutorial encounters also run in all three cases. Direct input produced
paradigm changes and attack responses, with observed HP reductions in rebuilt and
original-decrypted captures. The original-decrypted case returned to moogle
dialogue and entered an additional encounter. We have not recorded an unambiguous
final outcome across all three cases at that earlier checkpoint. The user
subsequently reached the Start Menu hub in the rebuilt human session and reported
normal retail-like controls, Lightning mechanics, graphics and sound. The later
[post-tutorial comparison](hub-runtime-validation.md) verifies both saved slots
reach the hub on all three cases, a Quick Battle reaches its post-fight menu, and
the Story prologue/world map loads with working movement.
**The broader gameplay baseline remains incomplete.**

## Corrected interpretation

The [previous protected-runtime report](protected-runtime-milestone.md) interpreted
COM under both portraits as evidence of an automated demonstration. That inference
was wrong. Lightning's HUD cycles COM/RAV/MED with her paradigms; direct shoulder
input produced Ravager/Medic transitions. The
[official manual](https://support.na.square-enix.com/document/manual/2020/upp_manual_0001_r12.pdf#page=5)
also describes her three-role system. The old snapshots establish battle playback
but do not themselves establish automation or lack of player control.

The [current machine-readable comparison](runtime-comparison.json) records hashes,
reviewed checkpoints, live input evidence, test-save checks and explicit pending
items. Each case uses isolated settings/memory sticks, pinned PPSSPP 1.20.4 and
the same profile choices. These are semantic route comparisons; frames, saved
timestamps, play time and battle outcomes were not synchronized.

| Checkpoint | Retail | Original decrypted | Rebuilt |
| --- | --- | --- | --- |
| Initial notice and title | Pass | Pass | Pass |
| New-profile menus/settings | Pass | Pass | Pass |
| Initial PSP profile save | Pass | Pass | Pass |
| Load after process restart | Pass | Pass | Pass |
| Saved file bytes unchanged during load | Pass | Pass | Pass |
| Opening/tutorial battle playback | Pass | Pass | Pass |
| Live combat input | Observed | Observed | Observed |
| Completed Quick Battle / post-fight menu | Pass | Pass | Pass |
| Main Start Menu hub | Pass, both slots | Pass, both slots | Pass, both slots |
| Human timing/feel and audio | Pending | Pending | Pass, user report |
| Post-hub save slots | Copied baseline | Copied baseline | Two backed up; copied baseline |
| Post-hub load on separate launches | Pass, both slots | Pass, both slots | Pass, both slots |
| Story prologue/world-map load and movement | Pass | Pass | Pass |

## Human combat and main-hub milestone

The user played the isolated rebuilt package through the opening tutorials to
the main hub. They reported no input, graphical or sound issues and confirmed
Lightning's controls and mechanics behaved normally. They created a second save
in slot two and reported both slots should have similar main-hub progress. No
specific win/loss outcomes or post-hub restart/load result were reported.

After the emulator closed, both `ULUS10566GameData00` and
`ULUS10566GameData01` were found, each with `DISSIDIA.BIN`, `ICON0.PNG` and
`PARAM.SFO`. All six files were copied to a new local save-baseline directory;
source hashes before/after copying and destination hashes matched. The
[human test record](human-runtime-result.json) pins the package, backup path and
file hashes. Save contents were not decoded to prove progress. Future tests
should use copies in fresh isolated sessions, preserving this baseline.

The [fresh comparison sessions](hub-runtime-validation.md) have now completed
all six slot-load checks and the Battle/Story entry checkpoints above. The first
administrator attempt was canceled, but a retry succeeded. All comparison
emulators are stopped and the original human saves and baseline are unchanged.

This completes the human rebuilt tutorial/combat/main-hub smoke test. It does
not complete the three-case gameplay matrix or increase matching C progress.

## Capture/input fixes and limits

The growing AVI fallback caused a long scan and left a test CPU paused during
readback. A subsequent capture worker was stopped only after checking its exact
command line/process identity, and the owned test CPUs were explicitly resumed.
The revised reader fixes the file extent at capture start and reads at most a
1 MiB header plus 64 MiB tail. It recognizes complete FFV1 video chunks and finds
the most recent decodable keyframe rather than scanning/decoding a whole live
recording. Its byte offset is recorded; no encoded timestamp is fabricated.
Regression checks cover appended-file EOF, longer partial GOPs with pixel-exact
reconstruction, malformed input and CPU/input cleanup.

Short frame-counted presses did not consistently move the battle pause-menu
selection. Explicit held-button pulses did, and now have a maintained helper
which releases the button in `finally`. We record this observation without
assigning an unverified cause. Timed combat remains a human validation task.

The full-length lossless videos became large. They remain local and ignored by
Git. All comparison processes are stopped, with remote startup reset off. A
separate [human test launcher/checklist](human-combat-checklist.md) uses normal
keyboard/controller defaults, sound and no continuous recorder or remote service.
The user's completed combat/main-hub results are recorded above.

**111 checks pass** after the reader/input cleanup changes. All four static
build proofs remain current. The separate human session was launched visibly
with sound and standard controls, and has closed. Physical controls and audio
passed by user report. The later post-tutorial report verifies post-hub load on
separate launches for both slots on all three cases.

Matching C remains **60 functions / 3,244 bytes**. No feature ownership denominator
or rank is promoted by these observations. Source ISOs, annotated analysis and
existing user saves remain preserved; no repository or decomp.dev publication was
performed.

## Walkthrough-informed next checkpoint

The official manual's printed pages 16–18 put the main **Start Menu after the
opening tutorials**, whose route depends on prompt responses and can advance
after losses. It is separate from the initial New Game/Load Game title screen.
Both saved slots have now been validated after restart using disposable copies.
The same selected Battle Mode fighters/stage and Story prologue/world-map route
are covered in the [follow-up](hub-runtime-validation.md). Automatic CPU assists
and music differed, and actual gateway entry, victory results and new-progress
save/reload remain pending.
[Manual](https://support.na.square-enix.com/document/manual/2020/upp_manual_0001_r12.pdf#page=10),
[walkthrough](https://gamefaqs.gamespot.com/psp/605802-dissidia-012-duodecim-final-fantasy/faqs/68428).

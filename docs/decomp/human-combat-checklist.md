# Personal combat and main-hub test

Updated on 11 October 2026. **The rebuilt human combat/main-hub test passed by
user report.** Controls, Lightning mechanics, graphics and audio behaved normally,
the main hub was reached, and two save slots are now preserved in a hash-checked
local backup. The [automated follow-up](hub-runtime-validation.md) now verifies
both slots load directly to the hub on all three cases, plus Battle/Story entry.
Victory results and broader gameplay coverage remain pending.

## Launch the isolated rebuilt game

From a normal PowerShell terminal at the repository root:

```powershell
./tools/start_combat_test.ps1
```

The launcher opens a visible PPSSPP window and holds the owned process for up to
one hour, or until you close it. It validates the rebuilt ISO and current build
proofs. Its memory stick is separate from both your existing saves and the
automated runs, seeded only from the disposable PPSSPP-named test profile.
Subsequent launches preserve your progress in this human-test profile.

Sound is enabled. Remote debugger/disc-sharing/WLAN and continuous video recording
are disabled. Standard PPSSPP mappings are restored on first preparation; your
later control changes in this session are retained. Connect your controller and
check **Settings → Controls → Control mapping** before judging responsiveness.
The launcher has been used successfully, with normal responsiveness reported by
the user.

For the standard QWERTY keyboard mapping in the pinned PPSSPP 1.20.4:

| PSP control | Keyboard |
| --- | --- |
| Analog movement | I / J / K / L |
| Cross / Circle / Square / Triangle | Z / X / A / S |
| L / R shoulders | Q / W |
| Start | Space |
| Directional buttons | Arrow keys |

These bindings come from [PPSSPP's pinned default mapping](https://github.com/hrydgard/ppsspp/blob/v1.20.4/Core/KeyMapDefaults.cpp#L18-L38).
Different keyboard layouts or your custom mappings may differ. The automated
sessions deliberately had physical mappings cleared; this human session restores
them using the [documented source behavior](https://github.com/hrydgard/ppsspp/blob/v1.20.4/Core/KeyMap.cpp#L724-L775).

## Expected progression

1. Acknowledge the initial notice, choose **Load Game**, and load the PPSSPP test
   profile. The existing human session now contains two slots reported to be at
   the main hub; select either for the next restart/load check.
2. Continue the opening and moogle dialogue. Keep the beginner/first-time route
   for comparison with the recorded runs.
3. Play through the tutorial encounters and advance the subsequent dialogue.
   The **Start Menu** is the main hub reached afterward; tutorial victory is not
   required, and prompts can change the encounters. This is confirmed by the
   [official manual, printed pages 16 and 18](https://support.na.square-enix.com/document/manual/2020/upp_manual_0001_r12.pdf#page=10).
4. At that hub, confirm the game-mode menu is usable, use the game's save option,
   then restart the isolated session and confirm loading returns to your saved
   progress.

The opening moogle tutorials are distinct from Story Mode's **012: Treachery of
the Gods → Prologue: Path to Sanctuary**. The walkthrough describes that story
prologue as world-map/gateway onboarding without fights; **Chapter 1: A New
Threat** follows with Lightning's combat gateways. [link3895's walkthrough](https://gamefaqs.gamespot.com/psp/605802-dissidia-012-duodecim-final-fantasy/faqs/68428).
No story chapter needs to be completed merely to reach the Start Menu.

## Report these results

| Check | What to report |
| --- | --- |
| Movement / jump / dodge | Responsive, delayed, missed input, or another problem |
| Bravery and HP attacks | Attacks execute and hits change the appropriate values |
| Lightning's L+R role change | COM / RAV / MED transitions respond |
| Encounter outcome | Win/loss/other end state and successful return to dialogue/menu |
| Main hub | Start Menu reached; game-mode/options entries usable |
| Save and reload | Progress preserved after closing/reopening PPSSPP |
| Graphics / sound | Missing visuals, corruption, stutter, missing audio, or crashes |

COM on Lightning's HUD denotes her Commando role; it is not a computer-control
classification. The prior report's interpretation was corrected after direct
paradigm-switch tests and consulting the manual.

A short response is enough: **Pass/fail for each check, plus the exact screen or
action where anything went wrong.** The first rebuilt-game report is recorded in
[human-runtime-result.json](human-runtime-result.json); use this checklist for
additional checks without overwriting the preserved save baseline.
If responsiveness seems wrong, the same launcher supports `-Case retail` for a
separate original-ISO control run with its own disposable profile.

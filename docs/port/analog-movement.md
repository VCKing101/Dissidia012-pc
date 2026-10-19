# Analog movement on the world map

> 19 October 2026. A port enhancement: in Story Mode the walker on the world map goes where the left stick
> points, from a small radial dead zone, instead of one of eight ways from about 78% deflection. On by default;
> `D012_NO_SMOOTH_STICK=1` gives the game's own functions. "V:" is verified in a run, "I:" is inferred.

## 1. Cause

The owner's report: on a real controller the world map walk is stiff, strictly eight directions.

**The stick never steers the walk.** Nothing on the world map reads the stick vector. V: with every pad accessor
overridden and logged (`D012_TRACE_PAD_CALLS=1`), the world map and battle make no `getStick` call (vtable +0x28)
while the stick is pushed. The walker's input is battle overlay `0x08B38A0C` (the world map runs on the battle
overlay), called every frame from `0x08B36D2C` with `(out, walker, pad, camera)`:

1. Four `isHeld` queries on the walker's LogicalPad (`walker+0x5A0`) for the logical "walk" bits, masks at
   battle `0x08B3ACD0..0x08B3ACEF`: up `{0x20000000, 0}`, down `{0x40000000, 0}`, left `{0x80000000, 0}`, right
   `{0, 1}`.
2. A camera-space vector from them: x = +1 (right) else -1 (left) else 0, z = -1 (up) else +1 (down) else 0. That
   is the eight-way step: **every direction is one of eight**, whatever the stick says.
3. The camera's matrix (main `0x0885EA88`, inverted), the y component dropped, the VFPU normalisation to length 1
   (w = 1), and, when `walker+0x198` bit 0 is set, a second pass through the walker's own matrix. The result goes to
   `walker+0x550`; the walk speed does not depend on it (V: 0.28 units a frame on open ground at every push).

The walk bits come from the stick only through the pad module (docs/research/pad-module.md): action table B
(`0x08B7F380`, V: dumped at run time) maps the raw analog-direction bits (`{0, 4}` up, `{0, 0x10}` down,
`{0, 0x20}` left, `{0, 8}` right) to the walk bits, and those raw bits are set by `PadMgr_applyDeadZone`
(main `0x08872138`) only when an axis is more than 100 of 127.5 from the centre: **past 78.4% on each axis
separately.** Its float vector (`{dir x, dir y, 0, min(len, 1)}`, rescaled from the dead-zone edge, normalised)
exists but the walk never reads it.

With PortableKit's circle-to-square stick (patch 0028) a full push at angle θ (0 to 45 degrees) reaches the nub at
(1, tan θ), so the vertical bit appears only from tan θ > 0.784, about 38 degrees: everything from 0 to 38 degrees
walks straight right, 38 to 52 diagonally, and so on. A push short of 78% on its longer axis does not walk at all.
I: the same holds on a PSP, whose nub also reaches its corners.

**No further snapping.** No atan2 quantisation and no lookup table: the eight ways are the four bits themselves.

**Battle uses the same function.** V: in a Quick Battle the fighter under the player's pad calls `0x08B38A0C`
too (walker = fighter 0 of the battle object `*(0x089D6A10)`), so battle movement has exactly the same eight-way
input. The other battle stick readers are direction gates (battle `0x08B32AC4` and its neighbours: "is up held",
for direction-modified actions) and an "any input" test (`0x08B23A48`); they are rules on the bits and are left
alone. See section 4 for why battle stays eight-way by default.

## 2. Measurements before and after (V)

World map, Story Mode (Prologue, Cornelia Plains), `build/port/data-smooth-test`, scripted virtual pad
(`axis leftx/lefty`), each push 30 frames, then its opposite. `D012_TRACE_WALK=1` logs, per frame, the port's
stick, the stick vector the game made, the camera-space direction the walk took (the eight-way bits it left in
`walker+0x5B0`, or the port's stick) and the walker's world direction and position. Summarised with one row a push
(`build/port/smooth/logs/final_wm_before.log`, `final_wm_after.log`):

| Stick (deg, full push) | Before: walk direction | After: walk direction |
| ---: | ---: | ---: |
| 0 | 0 | 0 |
| 10 | 0 | 10.2 |
| 22.5 | 0 | 22.5 |
| 30 | 0 | 30.0 |
| 40 | 45 | 39.9 |
| 45 | 45 | 45.0 |
| 60 | 90 | 60.1 |
| 67.5 | 90 | 67.3 |
| 90 | 90 | 90.0 |
| 180 + each of the above | the mirror (180, 135, 90 ...) | the stick's angle |
| half push at 0 (port magnitude 0.38) | **no movement** | walks at 0 |
| quarter push at 0 (0.03) | no movement | walks at 0 |
| 0.6 push at 30 (0.51) | no movement | walks at 30.4 |

The stick angles are what the port's stick gives after PortableKit's rounding to the nub's 256 steps (10 becomes
10.2). The walk speed is the game's own and unchanged: 0.28 a frame in both runs where nothing is in the way.

Battle (Quick Battle, `battle_before.log`, `battle_after.log`, `battle_optin.log`): before and after are
**identical frame for frame** (22.5 and 30 walk at 0, 45 at 45, half pushes do not move, the same positions); with
`D012_SMOOTH_STICK_BATTLE=1` the fighter walks at 22.5, 30, 45 and moves on a half push.

## 3. The fix

Two game functions are replaced by decompiled C (`port/decomp/pad.c`, `port/decomp/pad_walk.c`, types and
addresses in `port/decomp/pad_types.h`), registered by `port/host/d012_decomp_pad.cpp`:

- **`pad_apply_dead_zone`** (main `0x08872138`). It does exactly what the game's does: the same per-axis dead zone,
  the same analog-direction bits in `$v0:$v1`, the same vector written, the VFPU normalisation reproduced as the
  recompiler computes it. Beside it, host-side, it keeps the port's stick for that pad: the bytes taken from the
  nub's resting centre 0x80 (128 steps below, 127 above, as PortableKit fills them), how far it is pushed as the
  longer axis (the nub's range is a square), a **radial dead zone** of 0.1 of the nub's range on that, the length
  rescaled to 0..1 past it, and the stick's own direction. Nothing the game can see changes: menus, the direction
  gates, replays and netplay get exactly the bits and the vector they had. That is why menus are safe by
  construction rather than by a separate menu dead zone.
- **`wm_walk_input`** (battle `0x08B38A0C`). The game's function, with one change: when the walker's pad takes the
  walk bits from the stick (its table B maps each of the four walk masks from the matching analog-direction bit,
  which is the game's "move with the analog stick" control option), the pad is a LogicalPad, not muted, its stick
  vector is the one the dead-zone routine made this frame, and the port's stick is past the dead zone, the
  camera-space input is the port's stick direction instead of the eight-way step. The walker's direction flags
  (`walker+0x5B0`) are set as the eight-way bits would be (each axis within 67.5 degrees of it). Everything after
  (camera, normalisation, the walker's transform, length 1, w = 1) is the game's, so the output has the original's
  format and range. The D-pad, a pad whose movement is mapped to the D-pad, and replays (whose stick is rebuilt as
  0/127/254) take the game's path unchanged.

The radial dead zone is 0.1 of the nub's range (`D012_STICK_DEAD_ZONE`, 0 to 0.9). PortableKit already applies its
own radial dead zone (`input.dead_zone`, 0.15 of the stick) and rescales before the nub, so the walk starts at about
0.15 + 0.1 x 0.8 = **0.23 of the stick's travel**; the owner's suggested 0.2 applied on top would have meant 0.31.

**Speed.** The game's walk has one speed (the function's output is a unit direction). The port keeps it so:
past the dead zone the walker walks at the game's speed in the stick's direction. The magnitude only decides when
the walk starts, which is now at 0.23 of the stick's travel instead of 78% of an axis.

**Reaching the functions.** Both are called from their own 16 KiB unit, where the generated code calls them with a
plain `goto` that a registered override never sees. A framework change (PortableKit `dissidia` commit `ed844ca`,
patch 0029): `psp_recomp --dispatch-call ADDR` makes every `jal` to ADDR a call through the runtime's lookup, so
the override takes it; with nothing registered the original runs, so the off switch is unaffected. The addresses
are listed in `port/config/dispatch-calls.txt`; `tools/port/overlay_meta.py --recompile` passes the overlay rows
itself and docs/port/recompiled-build.md step 2 has the main one. V: the regenerated corpora differ from the old
ones at exactly those call sites (one in main unit 27, `0x08871E10`, plus the existing cross-unit call in unit 66
now through the lookup; two in battle unit 86, `0x08B36DEC` and `0x08B36E90`). A corpus generated without them
leaves the walk eight-way; the port then logs once
`[pad] smooth stick: the dead-zone routine ... never reached its override`.

The harness got two small additions for this: `Returns::Integer64` (compares `$v1` as well, for the bits) and probes
that start with the VFPU prefixes at identity (a zeroed context reads every source lane as x; it made the first
dead-zone probes differ although the function was right).

## 4. Switches

| Variable | What it does |
| --- | --- |
| `D012_NO_SMOOTH_STICK=1` | Off: neither override is registered; the game's own functions run (through the lookup, as above) |
| `D012_STICK_DEAD_ZONE=F` | The radial dead zone, a fraction of the nub's range; default 0.1 |
| `D012_SMOOTH_STICK_BATTLE=1` | Also in battle. **Off by default**: a battle records its replay as the eight-way bits only (main `0x08889628` -> `0x088CA680`, 15 bits a frame), so a fighter steered by the stick's real angle walks elsewhere when the saved replay is played back. Netplay would be safe (both pads are fed the same synchronised 0/0x80/0xFF bytes) |
| `D012_TRACE_WALK=1` | Logs every moving frame of the walk (and the pad's action table once) |
| `D012_TRACE_PAD_CALLS=1` | Puts the pad accessors in as decompiled C and logs each caller and mask once |
| `D012_DECOMP_PAD=1` | The accessors in as well; with `D012_NO_SMOOTH_STICK=1` also the faithful dead zone and walk |

Battle is a fighter of the battle object (`*(0x089D6A10)+0x38/+0x3C`); on the world map the object has no
fighters (V: both zero), so the test is exact.

## 5. Evidence (V)

All runs `PSPRECOMP_NO_INTERPRETER=1` (no fallback in any run), `D012_UNTHROTTLED=1`, data in
`build/port/data-smooth-test` (a copy of `build/port/data`, which was not touched); logs in
`build/port/smooth/logs/`.

- **Differential test** (`final_verify.log`, smooth on, `D012_DECOMP_PAD=1 D012_DECOMP_VERIFY=1
  D012_DECOMP_VERIFY_LIMIT=300`, world map with pushes): all SAME, 0 different, 0 inconclusive:
  `pad_apply_dead_zone` 300 at rest + 200 with the stick pushed (bits `{0,4}`, `{0,8}`, `{0,0xC}`, `{0,0x10}`,
  `{0,0x20}`, `{0,0x30}` among them, compared in `$v0:$v1`), `wm_walk_input` 300 + 200 pushed (the faithful
  version, `check`), `logical_pad_held` 300, `logical_pad_pressed` 300, `logical_pad_repeat` 290,
  `pad_base_held` 300, `pad_base_pressed` 300. `verify_off.log` (switch off): the same counts, all SAME. With the
  switch on the game runs the port's walk; the test compares the faithful decompiled walk with the original, and
  the intended difference is only the camera-space input of section 3.
- **World map** before/after: section 2.
- **Battle** before/after: identical by default (section 2); opt-in analog.
- **Menus**: main menu, half, full and diagonal pushes, captured with the switch off and on
  (`build/port/smooth/menu_before`, `menu_after`): pixel-identical outside the performance overlay (the main
  menu's cursor did not move for the stick in either run, so this shows no change rather than stick navigation).
  The real guarantee is by construction: the pad's bits and stick vector are the game's in both (the differential
  test above).

## 6. Not verified

- A real gamepad (all input came from the input script's virtual pad); the owner's DualSense was connected but the
  script's pad takes precedence.
- The "move with the D-pad" control option (the stick then is not mapped to walking and the port leaves the walk
  alone), the vertical-walk and no-camera flags of the walker (`walker+0x5B1` bits 0x20, 0x40), and
  `walker+0x198` bit 0 with the port's stick (all go through the game's code after the input).
- Replays: saving and playing one with `D012_SMOOTH_STICK_BATTLE=1` (expected to diverge, hence the default).
- Ad hoc netplay.
- Seen while testing, not from this change: 5 of about 30 runs ended with an access violation at quit after the
  stall watchdog's "no frame for 6-11 s" message, every one with the right-stick camera code present; none in 5 long
  runs with `D012_NO_RIGHT_STICK_CAMERA=1`; two of the five had `D012_NO_SMOOTH_STICK=1`.

## Reproduce

```powershell
# Corpus (once, after a framework rebuild of psp_recomp)
tools\local\port\build-d012-recomp\portablekit\psp_recomp.exe build/port/game/EBOOT.ELF `
    --auto build/port/recomp/generated 0x08804000 --dispatch-call 0x08872138
.\.venv\Scripts\python.exe tools/port/overlay_meta.py --recompile build/port/ovl/OVL_BATTLE_APP.ELF
.\tools\port\pk_cmake.cmd --build tools/local/port/build-d012-recomp -j4

Copy-Item -Recurse build/port/data build/port/data-smooth-test   # once
# To the world map (main menu, Story Mode, continue), then a full push at 22.5 degrees and back
$route = "460:pad a;1080:pad a;1500:pad a;1650:pad a;1900:pad a;2100:pad a;3000:pad a;" +
         "3400:axis leftx 0.92388;3400:axis lefty -0.38268;3430:axis leftx 0;3430:axis lefty 0;3500:quit"
.\.venv\Scripts\python.exe tools/port/run_d012.py --seconds 90 --build-dir tools/local/port/build-d012-recomp `
    --data-dir build/port/data-smooth-test --log build/port/smooth/logs/run.log --env PSPRECOMP_NO_INTERPRETER=1 `
    --env D012_UNTHROTTLED=1 --env D012_MSGDIALOG_ANSWERS=ok --env D012_TRACE_WALK=1 --env "D012_INPUT_SCRIPT=$route"
# add --env D012_NO_SMOOTH_STICK=1 for the game's walk; --env D012_DECOMP_PAD=1 --env D012_DECOMP_VERIFY=1 to test
```

The route's frame numbers depend on load times; the world map appears around frame 3200. The Quick Battle route
used is in `build/port/smooth/battle_prefix.txt` (without the 1650 press, which picks Story Mode when the menu comes
early).

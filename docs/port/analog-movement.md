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
| `D012_NO_SMOOTH_STICK=1` | Off: neither override is registered; the game's own functions run (through the lookup, as above). Also turns off the camera follow's dead zone (section 7) |
| `D012_STICK_DEAD_ZONE=F` | The radial dead zone, a fraction of the nub's range; default 0.1 |
| `D012_SMOOTH_STICK_BATTLE=1` | Also in battle. **Off by default**: a battle records its replay as the eight-way bits only (main `0x08889628` -> `0x088CA680`, 15 bits a frame), so a fighter steered by the stick's real angle walks elsewhere when the saved replay is played back. Netplay would be safe (both pads are fed the same synchronised 0/0x80/0xFF bytes) |
| `D012_TRACE_WALK=1` | Logs every moving frame of the walk (and the pad's action table once) |
| `D012_TRACE_PAD_CALLS=1` | Puts the pad accessors in as decompiled C and logs each caller and mask once |
| `D012_DECOMP_PAD=1` | The accessors in as well; with `D012_NO_SMOOTH_STICK=1` also the faithful dead zone and walk |
| `D012_CAMERA_FOLLOW_DEADZONE=<deg>` | The camera follow's angular dead zone each side of straight ahead (section 7); default 22.5, 0 to 60. 0 is the game's follow at the walker's true angle |
| `D012_TRACE_CAMERA_FOLLOW=1` | Logs every free-camera input call: yaw, the follow's angle, what it was fed, the turn, the stick, the walker's position. Puts the free input in as decompiled C even with the zone off |

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

## 7. Camera follow: an angular dead zone

> 20 October 2026. The owner's report: with the analog walk, a push a few degrees off straight up makes the world map
> camera turn, so the walker veers; with the eight-way walk a small angle snapped to straight up and the camera stayed.

### 7.1 How the game's follow works (V)

The world map camera is the battle overlay's free camera (mode 0, `docs/port/right-stick-camera.md` section 5), with
auto-follow on (`cam+0xF4 = 1`). Its input function, battle `0x08B3F7F4`, called once a frame from the mode's update
`0x08B3F40C`, ends with the follow (`0x08B3FAA4..0x08B3FC08`):

- **Gates:** `cam+0xF4 == 1`, the owner's `+0x542` is 0 or 1 (main `0x0883739C`), and the owner moved this frame
  (`owner+0x5B0` bit 3, battle `0x08B2F9BC`; I: set by the move routine `0x08B2D2CC` while the walker's velocity is
  not zero). So no
  follow while standing.
- **Input:** the owner's **facing** vector `owner+0x90`, not the stick, the walk bits or `+0x550`:
  `target = atan2(-facing.x, -facing.z)` (main `0x089A457C`), the yaw that puts the camera behind it.
- **Turn:** `diff = wrap(target - yaw)` (fmodf with 2 pi, main `0x089941C0`), then
  `yaw += clamp(diff, -row[0x20], +row[0x20])`, wrapped. `row[0x20]` is 0.05 rad (2.865 degrees) **a frame**. There is
  no dead zone: the follow turns by the whole angle up to that cap, which it reaches at 2.9 degrees.
- **The facing** follows the walk direction quickly (it turns up to 24 degrees a frame), and the walk direction is
  camera-relative, so the two chase each other. Measured (`D012_TRACE_CAMERA_FOLLOW=1`, game's follow): a push
  5 degrees off straight up settles at `diff` = 1.12 degrees and turns the camera 1.12 degrees a frame for as long as
  it is held (273 degrees in 4 s); at 15 degrees and more it settles past the cap and turns 2.865 degrees a frame
  (172 degrees a second; 685 degrees in 4 s). Straight up gives `diff` = 0: no turn. With the eight-way walk only 0,
  45, 90 ... occur, so a near-straight push never turned it.
- **Moving toward the camera: it turns, it does not flip.** With the eight-way walk (`D012_NO_SMOOTH_STICK=1`, stick
  held down, `orig8_back.log`) the facing swings round (24 degrees the first frame), the follow chases it, and the
  camera then turns at the cap for as long as down is held: `diff` settles at -170.24 degrees (the facing 9.76 degrees
  behind the camera's turn), 685 degrees in 4 s, the walker running a circle. The way it turns is whichever way the
  facing swung. At 175, 180 and -170 degrees with the analog walk it is the same, at the same rate. Straight back is
  therefore not a still direction that small deviations disturb, and a dead zone there would stop the turn the game
  makes at straight back itself. **There is no dead zone around straight back**; near it the port does what the game
  does.

### 7.2 The fix

The follow is fed a reduced angle; the walker's movement, its facing and everything else are untouched.

- **Battle `0x08B3F7F4` is decompiled** (`camera_free_input`, `port/decomp/battle_camera.c`): D-pad turn, distance,
  follow, recentre and distance clamp, with the game's wraps through its own fmodf and atan2. The only change is
  where the follow's clamped angle comes from: `fed = camera_follow_angle(diff, cap, zone)` in place of `diff`.
- **`camera_follow_angle`**: `|diff| <= 22.5` degrees: 0. From 22.5 to 33.75 degrees (the zone plus a ramp of half the
  zone, at least the cap) it rises linearly from 0 to the cap: `cap * (|diff| - 22.5) / 11.25`, with diff's sign. Past
  that, `diff` itself, which the game's clamp caps. A plain `|diff| - 22.5` would reach the cap 2.9 degrees past the
  edge, nearly a step; the ramp makes the steady turn rate grow with the push (table below). The ramp ends short of
  where the game's follow settles for a diagonal push (35.5 degrees: 45 less the facing's 9.76-degree lag at the cap),
  so the diagonal, the side and straight back turn exactly as in the game; straight up does not turn in either.
- **Only for a walker the port's stick steers**: the zone applies when the camera's owner had its last walk input
  (battle `0x08B38A0C`) through the analog path's checks (`d012_decomp_pad.cpp`, `analog_walker`): pad mapped to the
  stick, not muted, the stick vector current, and not a battle fighter (unless `D012_SMOOTH_STICK_BATTLE=1`). Battle
  (whose free camera also follows) and the "move with the D-pad" option keep the game's follow.
- **Reaching it.** `0x08B3F7F4` is called inline from its own unit, but its only caller, the free update `0x08B3F40C`,
  is already decompiled and reached through the mode's vtable (right-stick camera). The decompiled update calls the
  input through the runtime's lookup, so the override `camera_free_input` takes it. With `D012_NO_RIGHT_STICK_CAMERA=1`
  the free update is still put in (without the stick hook) while the zone is on. **No corpus regeneration and no
  framework change were needed.** `D012_NO_SMOOTH_STICK=1` registers neither (V: 4 overrides in all, the BGM
  playlist's and the camera updates).

### 7.3 Measurements (V)

World map, Cornelia Plains, `build/port/data-follow-test`, scripted left stick (full push), 240 frames a push, yaw
logged every frame (`build/port/follow/logs/*.log`, as CSV in `build/port/follow/yaw/`). Where a push followed a
180-degree right-stick turn of the camera, the first 15 frames are the walker turning round (the follow chases the
swinging facing, 10 to 14 degrees); the zone column is frames 15 to 240 there (`a_zone`, `c_zone`); in
`angles_zone`, with no camera turn before, all 240 frames of 5, 15 and 20 degrees gave the same 0.

| Stick (deg off straight up) | Game's follow: camera turn in 240 frames | Zone 22.5: camera turn | Zone: largest turn a frame |
| ---: | ---: | ---: | ---: |
| 0 | 0 | 0 | 0 |
| +5 / -5 | -273.5 / +253.7 | **0.000 / 0.000** | 0 |
| +15 / -15 | -684.7 / not run | **0.000 / 0.000** | 0 |
| +20 / -20 | -684.7 / +684.7 | **0.000 / 0.000** | 0 |
| 25 | -684.7 | -86.5 | 0.67 |
| 27.5 | -684.7 | -168.3 | 1.31 |
| 30 | -684.7 | -247.9 | 1.94 |
| 35 | -684.7 | -416.5 | 2.865 |
| 40 | -684.7 | -583.7 | 2.865 |
| 45, 90, -45 | -684.7, -684.7, +676.4 | -684.7, -684.7, +675.1 (the same) | 2.865 |
| 180, 175, -170 | +684.7, -684.7, +684.7 | the same | 2.865 |

- With the zone the walker still walks at the stick's angle: the facing holds at -4.99, -15.11, -20.07, +20.33 and
  +4.96 degrees from the camera for 5, 15, 20, -20 and -5, and where nothing was in the way the per-frame walk was
  15.1, 20.0, -20.4, -5.1 and -15.0 degrees from the camera's forward at the full 0.28 a frame: it drifts sideways
  while the camera stays.
- **Right stick on the world map** (`c_zone`): 0.1047 rad (6 degrees) a frame, as before.
- **Battle** (`battle_zone`, `battle_orig`, Quick Battle in free mode with pushes at 10, 30, 45 and 175 degrees): the
  battle free camera follows too, and in 350 followed calls the zone never applied (every call the game's follow):
  battle fighters are not analog walkers. (The two runs reached different battles, so they are not compared frame for
  frame.)
- **Differential test** (`verify_on`, `verify_spin`, `verify_off`, `D012_DECOMP_VERIFY=1`, limit 300): all SAME, 0
  different, 0 inconclusive. `camera_free_input` 300 (the faithful decompiled input against the original), a probe
  `camera_free_input (following)` 300 checked only where the follow's gates are open (in `verify_spin`, while the
  camera spun walking toward it: 124 of them through the angle's wrap and 2 through the yaw's), `camera_free_update`
  300, and the pad module's as before. `verify_off` is the same with `D012_NO_SMOOTH_STICK=1`. No run logged an
  interpreter fallback (`PSPRECOMP_NO_INTERPRETER=1`).

### 7.4 Not verified

- A real gamepad (the script's virtual pad only).
- The recentre (SELECT) and the free input's own D-pad turn masks on the world map: decompiled as the original, and
  in the differential test only as far as its calls took those paths.

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

Camera follow (section 7): the routes are in `build/port/follow/route_*.txt` (the same prefix, then `axis` steps;
`axis rightx 1` for 30 frames turns the camera round). Run as above with `--data-dir build/port/data-follow-test`
and `--env D012_TRACE_CAMERA_FOLLOW=1`; `--env D012_CAMERA_FOLLOW_DEADZONE=0` gives the game's follow.

The route's frame numbers depend on load times; the world map appears around frame 3200. The Quick Battle route
used is in `build/port/smooth/battle_prefix.txt` (without the 1650 press, which picks Story Mode when the menu comes
early).

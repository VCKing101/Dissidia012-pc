# The right-stick camera

State, 19 October 2026: **on by default in the port.** In battle, the right stick turns the camera and moves it
closer or further away, in proportion to how far it is pushed. R3 recentres the camera. The same code also turns the
Story Mode world map camera. This is the PPSSPP plugin's right-stick camera (`private/mod/src/right_stick_camera.c`,
the owner's own code), moved into the port. It keeps the plugin's logic, gates and tuning.

V = seen in a run or in the disassembly, I = inferred.

## 1. What the game does

The battle camera is described in [battle-camera.md](../research/battle-camera.md) and summarised here:

- **One camera object per character**, the opponent's included. Each frame the camera manager (task
  `OBJ_CAMERA_MANAGER`, battle `0x08B3C760`) calls the update of the camera's current mode through the mode's vtable
  (slot `+0x1C`).
  - Mode 0 is the **free camera**: battle `0x08B3F40C`, vtable `0x08B3FCD8`.
  - Mode 1 is **lock-on**: battle `0x08B40014`, vtable `0x08B41560`.
  - Modes 2 and 3 are automatic and scripted, and read no pad.
- **Each update first calls its input function**: `0x08B3F7F4` for free, `0x08B40290` for lock-on. Then it builds the
  look-at and eye from the mode's state, then runs collision.
- **The input is digital.** Four logical camera actions come from the D-pad, or from the nub's digital directions with
  the other control type.
  - The turn speed is 2.0, 3.0 or 4.5 degrees per time-step unit, picked by the camera-speed option, from the `camr`
    chunk of `battle.bin`.
  - Vertical input changes one distance value, which also sets the height. There is no separate pitch.
  - SELECT (logical `hi 0x4`) recentres.
- **Time steps.** Battle logic runs at 30 Hz, and the camera's time step is 2.0 (units of 1/60 s). So the stock turn is
  6, 9 or 13.5 degrees per battle frame, which is 180, 270 or 405 degrees a second.

## 2. The hooks

The input functions are in the same generated 16 KiB unit as their callers (battle unit 88, `0x08B3D4C0`..`0x08B414BC`).
Their `jal`s are inline `goto`s in the recompiled code, so an override at the input function would never run.

The two mode updates are reached **only through their vtables** (battle `0x08B3FCF4` and `0x08B4157C`; `re_query
callers` finds no `jal`). An indirect call goes through dispatch, so an override registered there takes every call.

| Override | Original | Decompiled | Runs in the game |
| --- | --- | --- | --- |
| `camera_free_update` | battle `0x08B3F40C` (244 bytes) | `port/decomp/battle_camera.c` | the decompiled update with the stick hook |
| `camera_lock_update` | battle `0x08B40014` (340 bytes) | the same | the same |

- **What the updates do.** Both are small. Each works out the `camr` row, calls the input function, then the look-at,
  eye and collision functions (lock-on also runs its two target checks first and its height step last). Last, it copies
  two spring words of the row into `cam+0xE0`.
  - The decompiled versions keep the original's stack frame: `0x10` bytes for free, `0x20` for lock-on.
  - Each takes a **hook** that runs right after the stock input and before the view is built.
- **How they are registered.** The harness registers them like the BGM playlist's overrides
  ([bgm-playlist-design.md](bgm-playlist-design.md), section 2.2): `Override.run` is the hooked update, and
  `Override.check` is the same update with no hook. They go back in after every battle-overlay install.
- **The hook** is the plugin's code (`port/host/d012_camera.cpp`).
  - **Gates.** It acts only on a camera that has manual input on (`+0x126`) and that listens to logical pad 0
    (`+0x120 == *(0x089D6008) + 0xC0`). It also needs the pad manager to show no muted input and no replay playback
    (`+0x264 & 3`), and raw pad 0 fed by hardware (`+0x50 != 1`). The opponent's camera, replays and ad-hoc play are
    never touched.
  - **Free mode.** `yaw += turn × π/180 × row[0x14 + 4·speed] × dt`, wrapped once into (−π, π]. Then
    `distance = clamp(distance − closer × row[0x08] × dt, row[0x00], row[0x04])`.
  - **Lock-on.** The game's own per-frame turn `mode+0x14` gets `+= turn × π/180 × row[0x6C + 4·speed] × dt`. The
    stock input has just written it from the D-pad, and the game applies it with its own rate cap. Then
    `distance = clamp(distance − closer × row[0x4C] × dt, row[0x44], row[0x48])`.
  - **dt** is the camera's own time step, read as battle `0x08B3B4FC` does.
  - **Direction.** `turn > 0` is what D-pad right does and `closer > 0` is what D-pad up does, after the player's two
    camera direction options (`*(u16*)0x09AFD960`, bits 2 and 1). So the stick moves the camera the way the D-pad does
    in every option setting.
- **The stick.**
  - It is read from the pad the game itself reads. The profile wraps `sceCtrlReadBufferPositive`, the game's only pad
    read, and takes bytes 10 and 11 of the sample. PortableKit already fills those with the right stick, and the game
    never reads them.
  - PortableKit's input layer has already applied the radial dead zone and the menu's inversion. The hook then applies
    the plugin's per-axis dead zone, 0.18, with the response rescaled from its edge (as the plugin added its own on top
    of PPSSPP's).
  - While HOLD is on, the stick counts as centred.
  - Unlike the plugin, the centre is 0x80 with ±127 travel (PortableKit's encoding), not 127.5.
- **R3.** PortableKit already reports the stick clicks as extra buttons (patch 0026). The profile now names R3 too:
  `extra_buttons = L2 | R2 | R3`.
  - While R3 is held, the wrapper ORs **SELECT** into the sample, so the game's own recentre runs, as in the plugin.
  - It does this only while a **battle** combatant's camera (time-step source 0) has passed the gates within the last
    four pad reads. So R3 is SELECT nowhere else: not in menus, not on the pause screen, not on the world map.

No framework change was needed. PortableKit already has:

- the right stick in the SceCtrlData sample (fed by the gamepad and by the camera keys);
- R3 through `hle_extension::extra_buttons()`;
- the input script's `axis rightx|righty <v>` and `pad rightstick` steps.

## 3. Controls

| Input | Battle | Story Mode world map |
| --- | --- | --- |
| Right stick left/right | turns the camera, proportional; a full push is one stock D-pad step (the camera-speed option applies) | turns the camera, the same way |
| Right stick up/down | closer/further as D-pad up/down, within the row's limits (lock-on 2..5 on row 0, free 3..6) | nothing: row 5's distance is 6..6 |
| R3 | recentres (the game's SELECT) | nothing |
| Keyboard I / J / K / L | PortableKit's camera keys push the right stick fully, so they do the same as the stick | the same |
| Keyboard Backspace (SELECT) | recentres, as in the stock game | the game's SELECT |
| D-pad, L (lock-on toggle), L/R on the world map | unchanged, the game's own | unchanged |

R3 has no keyboard key by default. The menu's Keyboard rows now list it (`input.bind.r3`), and SELECT already
recentres from the keyboard.

Limits, all from the stock game:

- In lock-on the visible turn is capped at the stock D-pad speed.
- Lock-on swings back when the target leaves the frame. V: in `run1` the yaw stopped at about 57 degrees while the
  stick was still held right.
- No mode has a pitch; vertical input is the distance, which also sets the height.

## 4. Switches and settings

| Variable | Effect |
| --- | --- |
| `D012_NO_RIGHT_STICK_CAMERA=1` | The game's own camera: no overrides, no pad wrapper, and R3 does nothing |
| `D012_CAMERA_SENSITIVITY=<f>` | Full deflection as a share of the stock D-pad step; default 1.0, from 0.05 to 4. Lock-on still caps at 1 |
| `D012_CAMERA_DEAD_ZONE=<f>` | The per-axis dead zone; default 0.18 (the plugin's), from 0 to 0.9 |
| `D012_CAMERA_INVERT_X=1`, `D012_CAMERA_INVERT_Y=1` | Turn the other way / move the other way vertically; added to the menu's inversion and the game's options |
| `D012_TRACE_STICK_CAMERA=1` | Logs each frame the stick moves a camera: mode, turn, closer, the new yaw (or lock-on turn) and the distance |
| `D012_CAMERA_VERIFY_HOOK=1` | With `D012_DECOMP_VERIFY=1`, the differential test checks the hooked updates themselves, not the plain decompiled ones |

PortableKit's controls page already has the entries that apply:

- **Right stick**: Camera / D-pad / Off. D-pad presses the game's digital camera instead, and Off does nothing.
- **Invert camera horizontally / vertically**, applied to the gamepad stick before the port sees it.
- **Dead zone**, the radial dead zone, shared with the left stick.

The "Analog camera" and "Camera speed" rows appear only for a profile with a `CameraDriver`. This port does not
register one: those rows would turn the camera in degrees a second in place of the game's speed option, and they say
"quest camera". So sensitivity is the variable above. Mouse look was skipped.

## 5. Other modes

From a static read of `analysis/ULUS10566/*/research/pseudocode.c` and the disassembly (V unless marked I):

- **Story Mode world map: player-controlled, already covered.**
  - It runs with the battle overlay loaded. The player's piece is a battle character on logical pad 0, so its camera
    is a battle camera in mode 0.
  - Main `0x08918E90` sets it up: time-step source 1, manual input on, row 5, auto-follow on.
  - L/R turn it by 3 degrees × dt: main `0x08919A14`, through mode vtable `+0x44`/`+0x3C`.
  - The free input function also runs there.
  - So the free-camera override turns it too, with no extra code. V in `worldmap`: the stick turned it about 6
    degrees a frame, about 360 degrees in 60 frames; L still turned it; vertical did nothing (distance 6.000); R3
    pressed no SELECT.
  - V in `worldmap_verify`: 60 checks of the hooked update with the stick centred on the world map, all SAME.
- **Gateway board (menu overlay, `DUNGEON::MEMORY`): automatic.** The camera (`0x08B32FF4`, updated in `0x08B3329C`)
  switches between overview and following the piece from game logic (`0x08B342D4`). There is no pad read in
  `0x08B32000`..`0x08B34400`. Not touched.
- **Battle replay: fixed camera positions.** D-pad up/down cycles the target and left/right steps through
  `POS_replay_camera%02d` (battle `0x08B25B88`). There is no free turn and no clean hook. Not touched. The stick hook's
  replay gate keeps the battle camera out of replays.
- **3D model viewer (menu overlay, I: the Museum): player-controlled.**
  - The nub turns it ±0.1 rad a frame (menu `0x08A33FFC`); the D-pad pans and confirm/△ zoom.
  - A pitch field (`+0x10`) exists but is never written.
  - A clean hook exists: menu `0x08A35078`, called through a member-function pointer from another unit.
  - **Not implemented.** The left stick already turns it, and the route there was not run. This is the next candidate.
- **Main menu, hub, character select, extra overlay:** no camera input found.

## 6. Evidence (V)

All runs used the recompiled build with `PSPRECOMP_NO_INTERPRETER=1` and `D012_UNTHROTTLED=1`, data in
`build/port/data-camera-test` (a copy of `build/port/data`). Logs are in `build/port/camera/logs/` and captures in
`build/port/camera/<run>/`. No run logged an interpreter fallback.

- **`run1`** (`run1_sheet.png`): Quick Battle, Lightning against Lightning, default options, starting in lock-on.
  - Right stick full right in lock-on: the hook's turn is −0.1047 rad a frame, which is 3 degrees × dt 2.0, the
    D-pad's. The view yaw measured from the GE (`D012_TRACE_CAMERA`) went 79.0 → 64.0 → 56.9.
  - Stick up: the distance went 4.5 → 6.0 (the maximum) and the view's pitch rose to 44 degrees.
  - R3 in lock-on: yaw 56.1 → 83.0, pitch 44.4 → 18.8 (recentred).
  - L switched to free mode. Stick full left: yaw +0.1047 a frame through the full circle (1.4753 → 3.0461 in 15
    frames).
  - Stick down: the distance went to 4.0.
  - R3 in free mode: yaw 18.5 → 63.8.
  - The captures `lock_before`, `lock_turning`, `lock_after_right`, `free_before`, `free_after_left` and the rest show
    the arena turning.
- **`off`** (`D012_NO_RIGHT_STICK_CAMERA=1`, same input): the stick was held right from frame 8520 and the yaw stayed
  between 87.3 and 89.1. No `[camera]` lines; only the playlist's two overrides.
- **`verify_hook_centred`** (`D012_DECOMP_VERIFY=1 D012_CAMERA_VERIFY_HOOK=1`, limit 100, stick never touched, L
  toggle to free): `camera_lock_update` 100 SAME and `camera_free_update` 100 SAME. With the stick centred, the hooked
  code is the original. The BGM functions were also all SAME, **0 DIFFERENT, 0 INCONCLUSIVE**.
- **`verify_decomp_stick`** (`D012_DECOMP_VERIFY=1 D012_DECOMP_BGM=1`, limit 100):
  - The stick was pushed at 0.6 in lock-on, then full left and down in free mode, while the game ran the hooked code.
  - 0 DIFFERENT, 0 INCONCLUSIVE, with the two camera updates and all twelve BGM functions checked: camera 100 + 100,
    selector_update 100, selector_chosen 101, selector_make_name 100, table_name 100, and the rest as called.
- **`menus`** (`menus_sheet.png`): the right stick held diagonally with R3 pressed on the main menu, the battle setup
  screen and the battle's pause menu.
  - The cursor and selections did not change.
  - No camera hook ran while paused and no SELECT was pressed.
  - After unpausing, R3 recentred as usual.
- **`worldmap`, `worldmap_verify`**: as in section 5.

Shutdown crash, not the camera's (fixed 19 October 2026):

- **Some runs ended with an access violation after the script's `quit`**, once the game had already stopped. The
  fault was in `amdvlk64.dll`, called from `VulkanRenderer::Impl::destroy_target` <- `VulkanRenderer::shutdown` <-
  `~VulkanRenderer`, run by the C runtime's atexit destructors.
- The cause was PortableKit freeing render targets' descriptor sets from a descriptor pool it had just destroyed. It
  happened on every quit and faulted in about 1 run in 13, with the camera on or off. Patch 0030 fixes it; see
  [quit-crash.md](quit-crash.md).

## 7. Reproduce

```powershell
.\tools\port\pk_cmake.cmd --build tools/local/port/build-d012-recomp -j4
Copy-Item -Recurse build/port/data build/port/data-camera-test   # once; never run against build/port/data
$route = "460:pad a;1080:pad a;1500:pad a;1650:pad a;1900:pad dpdown;2000:pad a;2400:pad a;3200:pad a;3700:pad a;" +
         "4400:pad a;4900:pad a;5600:pad dpdown;5750:pad dpright;5800:pad dpdown;5850:pad dpleft;5900:pad dpleft;" +
         "5950:pad dpleft;6000:pad dpleft;6050:pad dpup;6100:pad dpup;6200:pad a;7250:pad a;8150:pad a;" +
         "8160:speed normal;8500:shot lock_before;8520:axis rightx 1;8550:shot lock_turning;8580:axis rightx 0;" +
         "8640:axis righty -1;8700:axis righty 0;8740:pad rightstick;8800:shot lock_after_r3;8830:pad leftshoulder;" +
         "8910:axis rightx -1;8960:axis rightx 0;8980:shot free_after_left;9080:pad rightstick;9200:quit"
.\.venv\Scripts\python.exe tools/port/run_d012.py --seconds 90 --build-dir tools/local/port/build-d012-recomp `
    --data-dir build/port/data-camera-test --log build/port/camera/logs/run1.log --env PSPRECOMP_NO_INTERPRETER=1 `
    --env D012_UNTHROTTLED=1 --env D012_TRACE_STICK_CAMERA=1 --env D012_TRACE_CAMERA=1 `
    --env D012_SCREENSHOT_DIR=build/port/camera/run1 --env D012_SCREENSHOT_EVERY=100000 --env "D012_INPUT_SCRIPT=$route"
# The differential test: add --env D012_DECOMP_VERIFY=1 (and D012_CAMERA_VERIFY_HOOK=1 to check the hooked code).
```

For the world map, from the same save: `460:pad a;1080:pad a;1500:pad a;1650:pad a;2000:pad a;2600:pad a;3200:pad a;
3800:pad a;4100:speed normal`. Story Mode, Continue, Yes: Lightning on Cornelia Plains from about frame 4000. Then
`axis rightx 1` and so on.

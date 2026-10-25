# Local split-screen versus: scope

> Written 25 October 2026, after a Reddit reader asked for local split-screen 1v1. A scope only: nothing here is
> built or run yet. V = seen in the disassembly, the Ghidra pseudocode or PortableKit's source; I = inferred.
> All addresses are main unless marked `battle:` or `menu:`.
>
> **Route chosen by the owner (25 October 2026): one copy of the game, drawn twice (section 1 onward).** Two copies
> linked by the game's own ad hoc versus is kept for later online play (section 8).
>
> **Spike, 25 October 2026:** the second view works and costs about 1.5 ms of guest time a frame (section 7.1).

The request: a **Quick Battle (Player vs Player)** entry under Battle Mode on the main menu. It works like 1-on-1,
except that a second controller picks the opponent in place of the CPU. In battle the screen is split vertically,
and player 2 has their own character, camera and controls on their own controller.

## 0. Summary

The work splits into two halves of very different size.

- **Player 2's controls: small.** The game already gives the opponent its own pad (logical pad 1), its own camera
  with manual input on, and an AI object that a single call switches to human. The menus all read pad 0, so the
  port can hand controller 2 to the game while the opponent is picked.
- **The second view: large.** The battle frame separates update tasks from draw tasks by priority, so the port can
  run the draw tasks a second time with player 2's camera into a second render target. The cost is in the state that
  drawing reads or changes once per frame for one camera: lights, the effect camera, fades, screen captures, HUD
  markers and a model countdown. All of it has to be saved, rebuilt for view 2 and restored.

With split screen, Dissidia's camera-relative movement is right for both players, since each steers by the camera
they see.

Rough size, in working sessions like the camera or BGM work: phase 1 (controls) 1 to 2, phase 2 (second view) 5 to
7, phase 3 (smooth 60 fps on both halves) 1 to 2, phase 4 (menu row and polish) 3. See section 6.

### 0.1 The first release (decided with the owner, 25 October 2026)

In:

- **Battle type:** Quick Battle 1-on-1 only, assists as normal.
- **Entry:** a new Battle Mode row, "Quick Battle (Player vs Player)". It appears only when everything below works.
- **Characters:** player 2 picks from player 1's save on their own controller. Level and equipment come from Quick
  Battle's opponent settings.
- **Menus:** player 1 runs every menu but player 2's pick: the settings page (CPU strength and behaviour rows
  hidden), the Opponent Info and BGM screen, and the post-battle menu.
- **Pause:** either player can pause; the pause menu answers to whoever paused.
- **Results:** the screen says which player won, in new text. Then Quick Battle's own retry, change characters and
  quit, driven by player 1. Change characters goes through both picks again.
- **HUD:** drawn once across the whole window, laid out as in the stock game (player 1's gauges left, player 2's
  right), with 1P / 2P labels in place of COM.
- **World-pinned HUD pieces** (lock-on cursor, damage and Brave numbers): per view, each worked out from that half's
  camera.
- **Views:** each half shows the middle of a full 16:9 render, at full height. A thin dark line between them.
- **Cutscenes:** while a scripted or event camera is in charge (intro, EX Burst, finishing blow, victory), one full
  16:9 view fills the window; the split returns with control.
- **Frame rate:** both halves at 60 fps.
- **Controllers:** player 1 keeps the keyboard and the first gamepad; the second gamepad is player 2. With no
  second gamepad, the row says "Connect a second controller" and goes nowhere.
- **Disconnects:** a controller lost mid-battle pauses the battle until a gamepad reconnects.
- **Options and sound:** player 2 shares player 1's game options and controller settings. The sound listener stays
  on player 1's camera.

Out:

- Party Battle and every other mode.
- Online or ad hoc play (section 8, later).
- A separate player 2 save, or player 2's own control options.
- Rewards: a versus battle writes nothing to the save (no experience, KP, PP, items or records).
- Replays: not offered after a versus battle.
- A wider view per half. Cropping keeps the height and halves the width, so in lock-on the opponent can be out of
  frame.
- The Steam Deck: not blocked, but untested and unsupported.

How the work lands: on the long-lived branch `local-versus` (this repository) and a branch of the same name in the
PortableKit clone, cut from `dissidia`, merged when the first release is done. The patches in
`port/portablekit-patches/` are regenerated at the merge. First step: the second-view spike (section 6).

## 1. Player 2's controls

### 1.1 Making the opponent human

| Fact | Where | |
| --- | --- | --- |
| The field set-up builds player 1 on logical pad 0 and the opponent on **logical pad 1**: `battle:0x08B39E5C(fighter, logical(i), data, slot, 0)` | `0x0888BA0C` | V |
| The fighter constructor gives **every** fighter an AI object (`fighter+0x5C4`, constructor `battle:0x08A637B8`) and sets it to mode 1 | `battle:0x08B39E5C` → `battle:0x08A64078(ai, 1)` | V |
| AI modes: **1 = human**, 0 = CPU, 2 = the CPU plays for you (strength forced to 15). In mode 1 the AI update only does bookkeeping | `battle:0x08A64400`; wrappers `battle:0x08B2F2E4` (set), `0x08B2F36C` (is 1) | V code, I names |
| After both fighters exist, the set-up switches slot 1 to CPU when its block says so: `0x09AFB8A0 + slot*0x1060`, field `+0xF34` (1 = CPU with behaviour `+0xF36` and strength `+0xF38`; 2 = CPU from saved AI data) | `0x0888BA0C`, calls at `0x0888BE5C` / `0x0888BE90` | V |
| Movement reads the fighter's own pad and its own camera only in mode 1; otherwise the AI's move vector | `battle:0x08B36D2C` → `battle:0x08B38A0C(out, fighter, pad, camera)` | V |
| About 50 command checks OR the AI's virtual buttons with the fighter's own pad | `battle:0x08B309C8`..`0x08B3315C` | V |
| In ad hoc versus (FIELD mode 5) the remote fighter is human because `+0xF34` is 0 and the AI stays in mode 1 | `0x0888BA0C` | I |

**The change:** after `0x0888BA0C`, call `battle:0x08A64078(*(fighter1 + 0x5C4), 1)`, with fighter 1 at
`*(*(0x089D6A10) + 0x3C)`. Setting `+0xF34` to 0 instead is not enough: a second loop then switches the slot to
mode 2 when option bit 0x40 is clear. The battle intro and outro (`battle:0x08B0DF38` / `0x08B0E04C`) already hand
any mode-1 fighter to the CPU and back, for either slot.

### 1.2 Feeding controller 2

- In battle, write controller 2 into `raw[1].inject` and set `raw[1].mode = 1` every frame (`0x08C15BF0 + 0x40` and
  `+0x50`), as the ad hoc code does. `RawPad_poll` reads the injected stick bytes, so player 2 keeps full analog.
  Not through `0x088720B4`: it switches `raw[0]` to injection too.
- PortableKit opens only the first gamepad and ignores a second (`open_gamepad`, `host/gpu/vulkan_renderer.cpp`).
  It needs a second pad slot, a setting for which controller is player 2, and the right stick and R3 for each.
- The keyboard stays player 1's.

### 1.3 Picking the opponent

- **All 587 menu-overlay sites that fetch a logical pad pass index 0** (V). So the port routes controller 2 into the
  game's pad read (`raw[0]`) while the opponent is being picked, and controller 1 the rest of the time.
- The character room (CROOM) object is `*(u32*)menu:0x08AA3F44`, its update `menu:0x08A97DA8`, its work area
  `w = *(obj+0x154)`, its state `*(w+0x910)` (table `menu:0x08AA3F48`, 53 states). V:
  - states 4 to 7: player 1's character (`menu:0x08AA0D48(obj, 0)`, result at `w+0x804C`);
  - **states 8 to 11: the opponent** (`menu:0x08AA0D48(obj, 1)`, result at `w+0x8050`);
  - states 12 to 15: the battle settings page (stage, CPU strength, rules). I.
- The CPU strength and behaviour rows mean nothing in versus and are hidden (0.1).

### 1.4 Player 2's camera

- Battle start (`battle:0x08B0E200`) turns manual input on for **both** fighters' cameras, and each camera reads its
  own fighter's pad (V). Player 2's D-pad camera already works.
- The port's right-stick hook only acts on the camera listening to logical pad 0 ([right-stick-camera.md](
  right-stick-camera.md) section 2). It needs a second gate for logical pad 1 with controller 2's right stick, and R3
  as SELECT on raw[1].
- Each fighter's camera already updates every frame; only the on-screen choice is single (`cam+0x125`).

### 1.5 Left on player 1, and pause

The results screen, rewards and the post-battle menus read pad 0 or slot 0 (`battle:0x08B07500`, `0x08B09468`,
`0x08B0AA58`, `0x08B0C5F4`; rewards to `FIELD+0x54` at `battle:0x08B0B71C`). V. They stay player 1's; rewards are
skipped in versus (0.1). The L+R+START+SELECT abort only acts with a network object, so it does nothing locally (V).
Slot 1's control options (camera inversion and the like) are copied from player 1's (`0x088FAEF8`, V), as decided.

Pause is not located yet; probably a pad 0 reader (I). Either player pauses (0.1): the port watches START on
controller 2 as well, and while paused hands the pausing controller to `raw[0]`, as for the opponent pick. The same
pause carries the "controller disconnected" message.

## 2. The second view

### 2.1 How a battle frame is drawn

V, from the task registrations (factory `0x088823C4`, sorted insert `0x08882790`, dispatcher
`0x0888245C(owner, mask, value)` runs a task when `(task+0x28 & mask) == value`). The system list each frame:

| Priority | Task |
| --- | --- |
| 5000 | `GRAPHICS::DlSendTask`: sends the list built last frame (double-buffered) |
| 6000 | `DlStartTask` (`0x0886598C`): starts the list (0x58400 bytes), sets the draw buffer (`0x08866AE0`), view and projection (`0x08866BA8`), lights from the camera at `gfx+0x720` |
| 15000 | `PAD_MANAGER::ReadTask` |
| 35002 | `MENU_MANAGER::ExecuteUpdate` |
| **70000** | **the FIELD game mode** (its own task list, below) |
| 70035 / 70041 | `MENU_MANAGER::Draw` / `DrawAfter`: **the HUD, drawn once after all battle drawing** |
| 90000 | `DlFinishTask` |
| 110000 | `VSYNC` (`0x08883F9C`): sync, vblank wait, swap |

The FIELD list: update tasks (filters 1, 3, 9, 0x27, 0x2F; priorities 100 to about 57500; camera update at 32000)
then **draw tasks (filter 0xFFFFFFFF, priorities 58000 to 100200)**: OBJ `AddDrawTask` 58000,
`OBJ_CAMERA_MANAGER::DrawTask` 59000, effect `DrawFirst` 59400, `MAP::CullingTask` 59500, `MAP::DrawTask` 60000,
shadows 69000, effect `CaptureBefore` 69600, OBJ `DrawTask` 70000, and on to `SCREEN_FADE::DrawBeforeMenu` 100200.
A few 0xFFFFFFFF tasks are logic and sit below 58000 (`ExecPadInitByChange` 100, `SetFlagToPlayer` 29998, and
others); a priority floor of 58000 leaves them out. V.

The camera: the update `battle:0x08B3CA24` (32000) steps all 8 cameras, picks the on-screen one (`+0x125`, or the
scripted or event camera), applies screen shake and builds the view into the camera manager (`mgr+0x00..`,
projection `mgr+0xC0`). The draw task `battle:0x08B3C0D8` copies that into `gfx+0x720..0x7F0` and loads it. V.

### 2.2 The plan

A port-side hook after the FIELD game mode's dispatch, in the recompiled code:

1. Save the state view 1 left behind: `gfx+0x720..0x8B0` (view, projection, view×projection), the effect
   manager's camera copy, and `mgr+0x00..0xD0`.
2. Build view 2 from player 2's camera: the select-and-build tail of `battle:0x08B3CA24`, re-implemented with its
   own cut history, without stepping the cameras again. While a scripted or event camera is in charge (intros, EX
   Bursts), there is no view 2: the window shows view 1 full screen (0.1).
3. Point the screen target (`0x08866AE0`, which hard-codes 480x272 at framebuffer offset 0 / 0x44000) at a second
   target, and redo the lights.
4. Dispatch the FIELD list again with mask/value `(0x10, 0x10)`, which only the 0xFFFFFFFF tasks match (I: no
   update filter uses bit 0x10), from priority 58000.
5. Restore everything from step 1, so the next frame's update and lights see view 1's state.

The host then shows the second target as the right half.

**Considered and rejected: redrawing on the host.** PortableKit's frame interpolation already records every draw of
a frame and draws it again with changed matrices, so the host could draw the frame a second time with player 2's
view matrix. But `MAP::CullingTask` culls the stage to player 1's view, and player 2's camera mostly looks the other
way. The effects use their own camera copy, and screen-facing sprites are turned to player 1's camera on the CPU. The
second view has to come from the game's own draw code.

### 2.3 What has to be per view

| State | Why | |
| --- | --- | --- |
| Model draw countdown `model+0x128` | `0x0886ACCC` counts it down on every draw and frees resources at zero; two draws a frame halve its time. Draw 2 must skip the count | V |
| OBJ draw list | `DrawTask` (`0x08841144`) clears it; re-running `AddDrawTask` (58000) rebuilds it | V |
| Effect manager's camera copy | Set once in the camera update (`0x088A5F1C`); every effect draw loads it (`0x088A60B8` and siblings) | V |
| Near-camera fade of fighters | `char+0x380` bit 0x200000, `char+0x490`, set by `battle:0x08B3C1E0` for the on-screen camera; read by `DrawAlphaScreenTask` `0x088419EC` | V |
| Lights | Set at frame start from the previous frame's camera | V |
| Sound listener | `0x088A08CC`; stays player 1's | V |
| Shadows | Drawn to the scratch target `0xCC000`, composited back by `0x088763B8` with fixed screen strips | V |
| Blur and glow chain, 64-wide pass, render-to-texture with its own view | `0x08861208`, `0x0885E150`, `0x08870DBC` | V |
| EX Burst background | Copies the shown framebuffer once to RAM `0x095CD140` and redraws it for 56 frames; view 2 needs its own copy | V (census) |
| Glass crack | `battle:0x08ACD138` reads the shown framebuffer with sceDisplayGetFrameBuf and copies 480x272 | V |
| EX Revenge greyscale, Kuja's trance layer | Read the whole framebuffer through `0x040CC000` | V (census) |
| World-to-screen markers | `0x08866E04` reads `gfx+0x830` with 480x272 hard-coded; one position per marker | V |
| GE list size | 0x58400 bytes for one view; two views may not fit, so view 2 may need a list of its own | I |

Not yet determined: where bone poses are evaluated (if in a draw task, they would run twice), what the map call
every third frame does (`0x0889ACA8` / `0x0889B37C`, from the eye position), and whether any draw task fires sounds.

### 2.4 The HUD

The HUD is menu-manager layouts drawn once, after the battle, into view 1's frame (V). Its sides follow the local
slot `FIELD+0x54`, not the camera, so the stock HUD already shows player 1 left and player 2 right (V:
`0x0888D210..D2AC`).

Decided (0.1): **the HUD is drawn once across the whole window**, laid out as the stock game lays it out, so player
1's gauges sit at the bottom left and player 2's at the bottom right.

- Built in the spike (7.1): after the second pass, before `MENU_MANAGER::Draw` (system list, 70035), a port marker
  has the renderer put the middle half of each view side by side back into the frame buffer. The game then draws
  its stock HUD over both at full width, with its own blending, and the frame is shown as any other. No separate
  HUD layer is needed.
- **1P / 2P labels** replace COM under the portraits. Text and layout ids not mapped yet.
- **World-pinned pieces** (lock-on cursor, damage and Brave numbers, `0x08866E04` → `0x088D9C60`) are placed from one
  view over a full 480x272 frame, so on the full-width layer they would land in the wrong place. Per view: player
  1's set is moved into the left half's crop, and a second set placed from view 2's matrix goes in the right half.
  Where these sprites are placed (update or draw) is not traced; the second set may mean running their placement
  again with view 2's `gfx+0x830`. L.

### 2.5 The screen

- Each view is a full 480x272 target, since every pass assumes one view at the origin (V).
- Decided (0.1): **each half shows the middle of its view** (guest x 120 to 360 of 480, full height), the same
  picture a tall 8:9 camera with today's vertical field of view would give, with no projection change. On a
  1920x1080 window at 4x that is 960x1088 per half. A thin dark line marks the split.
- Guest time per frame in 3D scenes is 6.5 to 7.3 ms ([performance.md](performance.md)); the draw half of it runs
  twice. Fine on the PC within the 33 ms of a 30 Hz frame; the Steam Deck is not measured.

## 3. PortableKit

- **Composition.** The renderer shows the framebuffer the game displays. It needs to show the middles of two guest
  targets side by side (view 1 left, view 2 right), the divider, and the HUD layer over both; and one full view while
  a scripted camera is in charge.
- **Frame interpolation.** It blends only perspective draws into the displayed framebuffer
  (`interpolation::mark_eligible`), so view 2 would stay at 30 fps. Matching already keys draws by target
  (`DrawSummary::target`); it needs two eligible targets with a camera estimate each.
- **Two gamepads**, as in 1.2.
- **Render-target snapshots** for interpolated frames and the one-shot screen captures have to handle a second shown
  target.

## 4. The menu entry

- The main menu's Battle Mode handler is `menu:0x089E4A60` (submenu `state+0xA88` = 1, cursor `+0xA5C`). It writes a
  destination to `0x09B6D838`, and `menu:0x089E0CEC` launches it. Rows 0..3 go to destinations 2..5. Row 0 is Quick
  Battle 1-on-1; destination 2 opens the character room. V.
- The rows are at `menu:0x089E6EA4` (4 rows of 8 bytes and a 0xFFFF terminator, pointed to from `menu:0x089E7544`);
  each row's last u16 indexes the row-type table `menu:0x089E7488`, whose first u16 is "enabled". V.
- A fifth row needs the array moved, a cursor-4 case in `menu:0x089E4A60` and a destination case in
  `menu:0x089E0CEC`. Its text and layout ids are not mapped yet. I.
- When the room exits, `menu:0x08A9D72C` sets FIELD mode 4 (Quick Battle) from the table at `menu:0x08AA3F30`. V.
  Versus stays mode 4, plus a port-side flag.

## 5. Battle rules in versus

- **Replays: not offered** (0.1). A replay stores the battle set-up, including slot 1's CPU block, so played back
  slot 1 would be CPU-driven and fight differently (I). The save-replay choice is hidden after a versus battle.
- **Rewards: none** (0.1). A Quick Battle gives player 1's save experience, KP, PP and items (V: rewards go to the
  local slot, `battle:0x08B0B71C`). Versus skips all of it, and the battle records.
- **Results: versus wording** (0.1). The stock results screen is worded from player 1's side; versus shows which
  player won. Text and layout ids not mapped yet.
- **EX Burst.** A human fighter gets the button sequence, through the bursting fighter's own pad as far as can be
  seen (I). Untested for slot 1. The EX Burst itself is a scripted camera, so it plays full screen.
- **Disconnects.** A controller lost mid-battle pauses the battle with a message until a gamepad reconnects (0.1).

## 6. Phases

Work on the branches `local-versus`; the switch `D012_LOCAL_VERSUS=1` reaches versus until the menu row exists.

0. **Second-view spike (first).** View 2 drawn from the opponent's camera into a second target and shown beside
   view 1, nothing else fixed; the opponent stays CPU. Shows how much breaks. 1 session. **Done (7.1).**
1. **Controls.** Controller 2 picks the opponent; slot 1 is set human; controller 2 feeds `raw[1]`; the right-stick
   hook covers player 2's camera; either player pauses; disconnects pause. Needs PortableKit's second gamepad. 1 to 2
   sessions. **Done but for two pieces (7.2):** the on-screen "controller disconnected" message, and player 1's
   gamepad lost.
2. **The second view.** The per-view state of 2.3, the cropped halves and divider, the full-width HUD layer, full
   screen during scripted cameras, world-pinned pieces per view, then the screen-capture effects one by one (glass
   crack, EX Revenge). 5 to 7 sessions.
3. **60 fps on both halves.** Frame interpolation on two targets. 1 to 2 sessions.
4. **The menu row and polish.** Quick Battle (Player vs Player) under Battle Mode, "Connect a second controller",
   CPU rows hidden, no replays or rewards, versus results wording, 1P / 2P labels. 3 sessions.

## 7. Decisions

All open questions were settled with the owner on 25 October 2026; section 0.1 lists them.

## 7.1 The second-view spike (25 October 2026)

**The second view works.** The battle drawn a second time from the opponent's camera, beside player 1's view, from
the first battle frame on, with no crash and no interpreter fallback.

How it is built (branches `local-versus` here and in the PortableKit worktree `tools/local/port/PortableKit-versus`):

- **Game side**, `port/host/d012_split_view.cpp`, on only with `D012_LOCAL_VERSUS=1`:
  - An override of the camera manager's draw-task thunk (`battle:0x08B3C43C`) runs the original and, once per
    battle, adds one task to the battle's own task list: filter 0xFFFFFFFF, priority 100300, after every draw task.
    The running task (`0x089D69B0`) names the list's owner. The task's function is a native override at
    `0x08800200`, below the EBOOT, where no code is ever loaded.
  - The task writes a port marker into the display list, saves the camera manager's view (`mgr+0x00..0x110`) and the
    graphics manager's camera (`gfx+0x720..0x8B0`), and rebuilds the view from the opponent's blended camera slot
    (`mgr + slot*0xB0 + 0x110..0x140`) with the game's own view and projection builders. It hands the effect manager
    that camera, resets the screen target and clears the screen (`sceGuClear(0x17)`). Then it calls every
    0xFFFFFFFF task of priority 58000 to 100200 as the dispatcher does, puts everything back, and writes a second
    marker. No view 2 while a scripted or event camera is in charge.
- **PortableKit side**, game-agnostic:
  - `GeState` passes a NOP whose data has the top byte 0xA5 to a marker sink.
  - The renderer's `kMarkerSplitLeft` copies the target drawn so far (view 1) aside. `kMarkerSplitCompose` puts the
    middle half of view 1 and of the target as drawn now (view 2) side by side back into the target, with a dark
    line between them. The HUD, drawn next by the game, lies over both.
- **Spike limits:** 30 fps with `D012_NO_PRESENT_THREAD=1`. Interpolated presents redraw the frame's recorded
  draws, which don't include the two copies, so they don't show the split yet. Nothing in 2.3 is handled.

Runs: recompiled build `tools/local/port/build-d012-versus`, data `build/port/data-versus-test` (a copy of
`build/port/data-camera-test`), the right-stick camera's Quick Battle route (Lightning against Lightning), window
captures in `build/port/versus/spike1/` (HUD over view 2 only) and `spike2/` (HUD over both), local only.

What the captures show:

| | |
| --- | --- |
| Works | The stage, both fighters, shadows, particles, hit sparks, the HP damage number and impact shards, all in player 2's half, from player 2's camera; lock-on and free mode on player 1's side unchanged |
| Works (second run, `build/port/versus/spike2/`) | The stock HUD over both cameras, as in the owner's mock-up: player 1's portrait, Brave and HP over the left view, player 2's over the right, the tutorial banner across the top |
| Expected, not yet built | World-pinned pieces are placed from view 1 over the full width, so they land in the wrong place: the lock-on cursor sits on the split line when the opponent is in the middle of player 1's view (2.4, per view) |
| New finding | The opponent's camera goes into walls (the CPU's camera at `s8700`). Player 1's camera fades the stage pieces close to it, but nothing does that for the second camera. I: the eye-position map call every third frame in the camera update (`0x0889ACA8` / `0x0889B37C`) is that fade, run for the on-screen camera only. It goes on the per-view list (2.3) |
| New finding | Player 1's half is sometimes darker than player 2's (`s9200`). I: the lights, set at frame start from the previous frame's camera, or a darkening pass that only one view gets. Not traced |
| Not seen | Models vanishing early (the per-draw countdown, 2.3), doubled glass-crack captures. About 40 seconds of battle; not ruled out |

Cost, the same route with `D012_PERF=log`, battle seconds only, on (1) and off (0):

| | Off | On |
| --- | --- | --- |
| Draws a frame | about 2,350 | about 4,400 |
| Guest CPU a frame | 7.2 to 9.4 ms | 8.3 to 11.7 ms |
| Render (CPU) a frame | 1.8 to 2.2 ms | 3.0 to 3.8 ms |
| GPU a frame | 0.9 to 1.1 ms | 2.0 to 2.3 ms |

The second pass costs about 1.5 ms of guest time and 1.2 ms of GPU time a frame on this PC, well inside 33 ms.

Reproduce:

```powershell
.\tools\port\pk_cmake.cmd --build tools/local/port/build-d012-versus -j4
$route = "460:pad a;1080:pad a;1500:pad a;1650:pad a;1900:pad dpdown;2000:pad a;2400:pad a;3200:pad a;3700:pad a;" +
         "4400:pad a;4900:pad a;5600:pad dpdown;5750:pad dpright;5800:pad dpdown;5850:pad dpleft;5900:pad dpleft;" +
         "5950:pad dpleft;6000:pad dpleft;6050:pad dpup;6100:pad dpup;6200:pad a;7250:pad a;8150:pad a;" +
         "8160:speed normal;8500:shot s8500;8520:axis leftx 1;8640:axis leftx 0;8700:shot s8700;8830:pad leftshoulder;" +
         "9000:pad a;9200:shot s9200;9300:quit"
.\.venv\Scripts\python.exe tools/port/run_d012.py --seconds 90 --build-dir tools/local/port/build-d012-versus `
    --data-dir build/port/data-versus-test --log build/port/versus/logs/spike1.log --env PSPRECOMP_NO_INTERPRETER=1 `
    --env D012_UNTHROTTLED=1 --env D012_LOCAL_VERSUS=1 --env D012_NO_PRESENT_THREAD=1 `
    --env D012_SCREENSHOT_DIR=build/port/versus/spike1 --env "D012_INPUT_SCRIPT=$route"
# data-versus-test's settings.ini: video.frame_rate=30, video.present_mode=immediate, video.performance=off
```

Next, in phase 2 order: the per-view state of 2.3, starting with the camera's wall fade and the lights, then the
world-pinned HUD pieces per view.

## 7.2 Player 2's controls (25 October 2026)

Phase 1, on the same branches and switch (`D012_LOCAL_VERSUS=1`). V unless marked.

**PortableKit** (game-agnostic):

- **A second player's gamepad.** `VulkanRenderer::set_second_pad_wanted(true)` opens a gamepad other than player 1's
  as player 2's, read with the same bindings and tuning into `second_pad()`, never into the game's own pad. Off by
  default, so without a port asking a second pad stays a spare for player 1 as before.
- **Extension interface 6:** `hle_extension::want_second_pad(bool)` and `second_pad()` (buttons, extra buttons, both
  sticks, connected).
- **Input script:** `pad2`, `axis2`, `detach2` and `attach2` steps drive a second virtual gamepad.
  - It is named `PortableKit input script 2` and has its own vendor and product ids, because with the first pad's
    ids SDL gave it the first pad's mapping and gamepad name.
  - The renderer now tells script pads apart by their joystick names.

**Game side**, `port/host/d012_versus.cpp`:

- **The opponent's pad.** At every pad read, player 2's pad goes into raw pad 1's embedded SceCtrlData and its mode
  is set to 1, as the ad hoc code does. Logical pad 1, the opponent's fighter's and camera's, reads it.
- **The opponent is player 2's.** An override of `battle:0x08B2F2E4` turns the field set-up's "slot 1 to the CPU"
  call into "slot 1 to the player" in a Quick Battle (`FIELD+0x4C == 4`, caller inside main `0x0888BA0C`, fighter
  `FIELD+0x3C`). The battle intro hands both fighters to the CPU and back, as it does player 1's: the trace shows AI
  mode 0 through the intro, then 1.
- **Picking the opponent.** While the character room is in states 8 to 11, the game's pad read gets player 2's pad.
  - The room object is `*(menu 0x08AA3F44)`, its state `*(*(obj+0x154)+0x910)`.
  - Its state table at `menu:0x08AA3F48` has 8-byte entries; the first three functions tell the menu overlay from
    the battle overlay.
- **Player 2's camera.** The right-stick hook (`d012_camera.cpp`) also acts on the camera listening to logical pad 1,
  with player 2's right stick and slot 1's direction options. R3 on player 2's pad presses SELECT on raw pad 1 while
  that camera takes input.
- **Pause.** Nothing was needed. The battle checks START on each fighter's own pad (`battle:0x08B20458`, through
  `FIELD + slot*4 + 0x38`, then `+0x5A0`), and the pause menu answers only the pad that opened it.
- **A disconnect.** When player 2's pad goes away while the battle overlay is loaded, START is pressed on raw pad 1
  for one read. The game's pause opens as player 2's and waits for that pad.
- **`D012_TRACE_VERSUS=1`** logs both fighters' positions, the opponent's AI mode and what raw pad 1 was given,
  every 15 pad reads.

Runs (recompiled, `build/port/data-versus-test`, two virtual pads, every menu press sent on both so the route works
whichever pad the game listens to; logs in `build/port/versus/logs/`):

| Run | What it showed |
| --- | --- |
| `controls2` | Player 2's pad opened as player 2's. "player 2 picks the opponent" logged while the room was in its opponent states, then "player 1's pad again". The opponent was made player 2's |
| `controls3` (trace) | No input: both fighters stand still. Player 2's left stick alone moves player 2's fighter: x −10.0 to −3.2, z 0 to −19.0, and off a ledge, y 23 to 6. Player 1's stick alone moves player 1. Player 2's circle makes their fighter lunge (a Brave attack); cross makes them jump (y 6.0 to 8.9 and back) |
| `controls2` | Player 2's right stick, the only stick input at that moment, turned the lock-on camera: player 2's half changed view |
| `pause2` | Player 2's START paused; player 1's confirm left the menu up; player 2's confirm closed it, and player 2 moved again. The other way round, player 1's pause took only player 1's confirm. The pause menu draws over the split screen |
| `disconnect1` | `detach2` in battle paused the game. Player 1's confirm left the menu up. `attach2`, then player 2's confirm, closed it, and player 2 moved again |
| `off1` | `D012_LOCAL_VERSUS` unset: no second pad opened, no versus lines, and player 1's right stick still turned the camera |

Not done or not verified:

- **No on-screen message** when a pad goes away; only the pause. PortableKit has no notice facility yet.
- **Player 1's gamepad lost** is not handled. Player 1 still has the keyboard.
- **Real controllers** were not used: only SDL virtual gamepads. Two real pads, or a real pad and the keyboard,
  need the owner.
- **Scope of the switch:** while it is on, every Quick Battle is versus, and any character-room opponent pick
  listens to player 2. The menu row (phase 4) scopes it.
- **Untested:** EX Burst button sequences for player 2, assists called by player 2, and the results screen after a
  versus battle.

## 8. For later: two copies over the game's ad hoc versus

The owner wants this route for online play and matchmaking, much later. What was found:

- Dissidia 012 has human against human battles over ad hoc (Communication Mode, FIELD mode 5, `menu:0x08AB753C`).
  Each PSP draws its own player's camera and HUD. The game sends 16 bits of pad a frame per player and waits until
  every player's word for the frame has arrived, then injects them into `raw[i]` (`0x0890D664` sender,
  `0x0890CE58` / `0x0890D090` pack and unpack, `0x0890C640` inject mode on). V.
- Two copies of the same build compute identical floats, so they should not desync. Against a real PSP the VFPU and
  PRNG differences in the [gap analysis](portablekit-gap-analysis.md) (section 3) matter.
- PortableKit has PDP, PTP, adhocctl, discovery and a built-in server (`host/adhoc/`), and expects several copies on
  one machine. Missing: the 10 `sceNetAdhocMatching` functions. Its server listens on every interface, which pops the
  Windows Firewall prompt.
- The 16-bit packet carries the left stick as 8 directions only, so online play would be digital unless both ends
  change the packet ([pad-module.md](../research/pad-module.md) section 6).

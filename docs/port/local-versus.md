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
  right), with 1P / 2P labels. (The "COM" under Lightning's portrait is her Commando paradigm, not "computer";
  the labels sit over the portraits instead. 7.6.)
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
| Lights | Set at frame start from the previous frame's camera. Now set again from view 2's camera (7.3) | V |
| libGu render state | The list start task sets the frame's state (scissor test, stencil, fog, lighting, blend, culling and more); view 1's last passes change it. Now replayed before view 2 (7.3) | V |
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
- **1P / 2P labels** over the portraits (built, 7.6). "COM" under Lightning's portrait is her paradigm.
- **World-pinned pieces** (lock-on cursor, damage and Brave numbers, `0x08866E04` → `0x088D9C60`) are placed from one
  view over a full 480x272 frame, so on the full-width layer they would land in the wrong place. Per view: player
  1's set is moved into the left half's crop, and a second set placed from view 2's matrix goes in the right half.
  Where these sprites are placed (update or draw) is not traced; the second set may mean running their placement
  again with view 2's `gfx+0x830`. L.

### 2.5 The screen

- Each view is a full 480x272 target, since every pass assumes one view at the origin (V).
- Decided (0.1): **each half shows the middle of its view** (guest x 120 to 360 of 480, full height), the same
  picture a tall 8:9 camera with today's vertical field of view would give, with no projection change. On a
  1920x1080 window at 4x that is 960x1088 per half. A thin dark line marks the split, over one guest pixel of each
  view's inner edge (7.9).
- Guest time per frame in 3D scenes is 6.5 to 7.3 ms ([performance.md](performance.md)); the draw half of it runs
  twice. Fine on the PC within the 33 ms of a 30 Hz frame; the Steam Deck is not measured.

## 3. PortableKit

- **Composition.** The renderer shows the framebuffer the game displays. It needs to show the middles of two guest
  targets side by side (view 1 left, view 2 right), the divider, and the HUD layer over both; and one full view while
  a scripted camera is in charge.
- **Frame interpolation.** It blends only perspective draws into the displayed framebuffer
  (`interpolation::mark_eligible`), so view 2 would stay at 30 fps. Matching already keys draws by target
  (`DrawSummary::target`); it needs two eligible targets with a camera estimate each. **Built differently: both
  views draw into one target, and each draw carries its view (7.9).**
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
  `menu:0x089E0CEC`. Its text and layout ids are not mapped yet. I. **Built differently; see 7.6.**
- When the room exits, `menu:0x08A9D72C` sets FIELD mode 4 (Quick Battle) from the table at `menu:0x08AA3F30`. V.
  Versus stays mode 4, plus a port-side flag.

## 5. Battle rules in versus

- **Replays: not offered** (0.1). A replay stores the battle set-up, including slot 1's CPU block, so played back
  slot 1 would be CPU-driven and fight differently (I). The save-replay choice is hidden after a versus battle.
- **Rewards: none** (0.1). A Quick Battle gives player 1's save experience, KP, PP and items (V: rewards go to the
  local slot, `battle:0x08B0B71C`). Versus skips all of it, and the battle records.
- **Results: versus wording** (0.1). The stock results screen is worded from player 1's side; versus shows which
  player won. Text and layout ids not mapped yet. **Built as a banner over the game's own ending (7.8).**
- **EX Burst.** A human fighter gets the button sequence, through the bursting fighter's own pad as far as can be
  seen (I). Untested for slot 1. The EX Burst itself is a scripted camera, so it plays full screen.
- **Disconnects.** A controller lost mid-battle pauses the battle with a message until a gamepad reconnects (0.1).

## 6. Phases

Work on the branches `local-versus`. Versus is reached from the Battle Mode row; `D012_LOCAL_VERSUS=1` still makes
every Quick Battle a versus one, for the scripted tests (7.6).

0. **Second-view spike (first).** View 2 drawn from the opponent's camera into a second target and shown beside
   view 1, nothing else fixed; the opponent stays CPU. Shows how much breaks. 1 session. **Done (7.1).**
1. **Controls.** Controller 2 picks the opponent; slot 1 is set human; controller 2 feeds `raw[1]`; the right-stick
   hook covers player 2's camera; either player pauses; disconnects pause. Needs PortableKit's second gamepad. 1 to 2
   sessions. **Done (7.2, 7.7).**
2. **The second view.** The per-view state of 2.3, the cropped halves and divider, the full-width HUD layer, full
   screen during scripted cameras, world-pinned pieces per view, then the screen-capture effects one by one (glass
   crack, EX Revenge). 5 to 7 sessions. **Done (7.5, EX Revenge in 7.8).**
3. **60 fps on both halves.** Frame interpolation on two targets. 1 to 2 sessions. **Done (7.9); not tried on the
   Steam Deck.**
4. **The menu row and polish.** Quick Battle (Player vs Player) under Battle Mode, "Connect a second controller",
   CPU rows hidden, no replays or rewards, versus results wording, 1P / 2P labels. 3 sessions. **Done (7.6, 7.8):
   the results wording is a winner banner over the game's own ending.**

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
  draws, which don't include the two copies, so they don't show the split yet (now they do, 7.9). Nothing in 2.3
  is handled.

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

## 7.3 Order's Sanctuary blown out on player 2's half (fixed 25 October 2026)

The owner, playing with a real controller, saw Order's Sanctuary very bright on player 2's half. V:

- **Reproduced** with the graphics tour's Quick Battle route to stage 11. Player 2's water was solid white: mean
  brightness of the middle band 251 against player 1's 187 (`halves.py`, captures `build/port/versus/order_v1b/`).
- **Bisected** with `D012_SPLIT_ONLY=lo:hi` and `D012_SPLIT_SKIP=p,...` (new test switches; the first frame also
  logs every task the second pass runs).
  - Only the scene's tasks (58000 to 61000): still white.
  - Without `MAP::DrawTask` (60000): black.
  - So the map draw itself drew the water white.
- **The display list of one battle frame** (`D012_TRACE_GE_LIST=8440:1:600000`, split at the two markers), compared
  pass by pass with `ge_list_summary.py`:
  - In pass 1, the half-size frame copies the water samples (into `0x04168000` and `0x04178000`, and a 120×68 pass
    into `0x000CC000`) are drawn with their own scissor, 256×128 and 120×68.
  - In pass 2 the same draws had the full-screen scissor: the draw buffer was set (`D2`, `9D`, `9C`), but no scissor
    or region (`D4`, `D5`, `15`, `16`) was sent.
- **Cause.** libGu's scissor (main `0x089A204C`) records the rectangle in its context but sends it only while the
  context's scissor-test flag (`ctx+0xA8`) is on. The list start task turns it on at each frame's start, and view 1's
  last passes turn it off. Pass 2 started from what pass 1 left, not from the frame's starting state.
- **Fix** (`d012_split_view.cpp`, `frame_start_state`). Before view 2, replay the list start task's state block
  (main `0x088659F0..0x08865E24`) call for call, with the same arguments:
  - screen target, scissor test on, stencil function and op, the clears, the camera;
  - fog from `gfx+0x8B0`, lighting, the two lights that follow the camera (from view 2's camera);
  - depth test, blend, culling and the rest of the enables.
  - After view 2, the lights are set again from player 1's camera.
- **Checked.**
  - Order's Sanctuary: player 2's half 164 to 179 against player 1's 177 to 183 over five captures, the water
    drawn on both (`build/port/versus/order_fix/`).
  - Old Chaos Shrine and Lunar Subterrane: both halves alike (`stage1`, `stage4`).
  - Orphan's Cradle: not checked, the run ended on its Opponent Info screen.
  - Setting only the camera lights for view 2, tried first, changed nothing on its own.

Also found on the way: with a real gamepad connected, the scripted runs gave the real pad player 2's place. When the
port asked for a second pad, PortableKit's scan stopped at the first spare pad. The scan now offers every pad while
a second player is wanted, so the input script's second pad takes player 2's place from a real one, as its first
pad does player 1's.

## 7.4 The lock-on cursor and the damage numbers per view (25 October 2026)

V unless marked. Found with `D012_TRACE_MARKERS=1`, which logs the projections and the HUD's sprite placements.

- **The world-to-screen function** (main `0x08866E04`, `(out, in)`) is the HUD's only use of the graphics manager's
  view×projection (`gfx+0x830`). Now an exact native copy (an override), for the trace and for the popups below.
  In a fight it is called only at hits.
- **The lock-on cursor** is the camera manager's part at `mgr+0x878` (`battle:0x08B41BDC`). The camera update runs
  it for the on-screen camera's fighter: it projects the lock target through the manager's own view and
  projection, and places one HUD sprite (`*(mgr+0x880)`) through main `0x088D9C60` from `battle:0x08B418C8`.
- **Damage and Brave numbers** come from main `0x08858CB8` on `*0x089D4DC8 + 0x190`.
  - At a hit it projects a point once and keeps the screen position in a ring of 10 (start `+0x330`, count
    `+0x334`, 0x50 bytes each, position `+0x20`), with offsets `+0x33C/+0x340` and a jitter of up to 15 px on one
    path.
  - The popups animate in screen space and are drawn by battle draw task 83000 (main `0x0888819C`), so in both
    passes.
- **The HUD** is drawn in 2D only (through mode) by the menu manager (`*0x089D7830`, draw main `0x088CED50`):
  - a list of layouts at `+0x24`, linked through `+0x24`;
  - a layout draws while flag byte `+0x14` has bit 1 and not `0x0C` or `0x20`;
  - its sprites are linked through `+0x3C`, and one draws while `u16 +0x20` has `0x8000`.

What the port does (`d012_split_view.cpp`):

| Piece | How |
| --- | --- |
| Player 1's cursor | Its placement (main `0x088D9C60`, now an override, called from `battle:0x08B418C8`) is moved into the left half: x − 120, or off screen outside player 1's crop (guest x 120 to 360) |
| Player 2's cursor | A cursor part of its own, 0x40 bytes below the EBOOT (`0x08800400`), set up as the camera manager sets up its own: `battle:0x08B41A54`, then a sprite of the cursor's layout (`battle:0x08B41638` with `*0x08B422A8`, layout 0x222), and the manager's part's two flags. Each split frame, the split task runs the game's cursor update (`battle:0x08B41BDC`) for player 2's fighter while the camera manager holds view 2, so player 2's cursor has its own target, bone, scale, animation, visibility and lock-on sounds. That update shows the cursor only for the local player (vtable `+0x144`, `battle:0x08B2EF74`: fighter `+0x5B0` bit 4, set by `battle:0x08B2EF44` on player 1's), so player 2's fighter has that bit for the call only. Its placement goes into the right half. Overrides of `battle:0x08B41B40` (cursor shown or hidden: intros, EX Bursts) and `battle:0x08B41B24` (released at the camera manager's teardown) do the same to player 2's part. Hidden on full-screen frames. Both sprites are in the same layout, so the HUD's own draw draws both |
| Damage numbers | When the spawner projects, the projection records the world point for the ring slot the spawner fills next, which is exactly the slot it then fills. Before the second pass, each live popup moves to its place in view 2, keeping its own jitter and rise; afterwards it goes back. The records are cleared when the ring is empty. Player 1's half needs nothing: its popups are drawn into player 1's frame and cropped with it |
| Lens flare | Nothing needed: `battle:0x08B53C20` draws the sun's flare itself from the graphics manager's camera during the battle's draw tasks, so the second pass draws it from player 2's |

Checked (`build/port/versus/cursor2/`, `popups1/`, local):

- **Order's Sanctuary**, both fighters in view: player 1's cursor rings player 2's fighter in the left half, and
  player 2's rings player 1's in the right half.
- **A fight with hits:** the Brave numbers float over the fighter that was hit, in each half where that fighter is
  in view.

Also checked (`build/port/versus/cursor3/`, `sweep1/`, `popups2/`):

- **Player 2's own cursor** rings player 1 at the game's own aim point.
- **Independent cursors:** player 1 switched to the free camera and their cursor went away, while player 2's stayed
  on player 1.
- **The marker sweep:** a fight with the free camera, assist calls by both players, attacks and EX Mode
  (`D012_TEST_EX_FULL`). It found no world-pinned HUD piece other than the cursors, the popups and the lens flare.
  The only other placements came from the loading screen's pages.

Not done:

- **Not swept:** EX Bursts, summons, Kuja's or other fighters' special HUD pieces, other stages.
- **The lock-on sound** now plays for player 2's lock-on events too, through the game's own cursor update. Not
  listened to.

## 7.5 Phase 2: walls, fades, the draw count and full-screen events (25 October 2026)

V unless marked. Local runs on the walls route (each player backs into a wall on Old Chaos Shrine), the EX Burst,
Kuja and EX Revenge routes of `gfx_tour.py` played in versus, and a sweep of the 14 stages.

**View 2's eye is pulled back as the game pulls back the on-screen camera's.** After blending the slot cameras, the
camera update (`battle:0x08B3CA24`) moves the eye back along the view by the on-screen slot camera's `+0xD4` (one
unit here), in every camera mode but 2 and 3, before the screen shake. View 2 was built from the blended slot alone,
so with player 2 backed against a wall its eye sat inside player 2's fighter: the near plane and back-face culling
removed the whole fighter, faded or not. The split task now does the same pull-back, from player 2's slot camera,
into a vec4 below the EBOOT (`0x08800600`). With `D012_SPLIT_SAME_VIEW=1` both halves now match to within 0.6 of 255
in mean brightness (`build/port/versus/pull_same/`), which also settles the "player 1's half darker" finding of 7.1:
neither pass darkens its half, the two views simply differed.

**Fighters near the camera fade per view.** The game's step (`battle:0x08B3C1E0`) reads each fighter's bounding
sphere, which holds only during the update: run from the split task it never saw a fighter close, and player 2's
values stayed at 1. Player 2's values are now stepped right after the camera manager's update, through an override
of its task thunk (`battle:0x08B3C41C`), from player 2's slot camera with the game's own mode check; the split task
only applies them to the fighters (`+0x380` bit `0x200000`, alpha `+0x490`) for the second pass and puts player 1's
back after. DrawAlphaScreenTask (main `0x088419EC`) then draws the faded fighter into the alpha screen
(`0x040CC000`) from view 2's camera (main `0x0886AED0`) and composites it. Checked: each player backed into a wall
sees their own fighter faded in their own half (`pull_p2/`).

**The stages' near-camera fade per view** (the MAP slot table, 7.1's finding) is built: player 2 keeps a table of
its own, aged and filled by the game's two eye queries on the update's three-frame phase, and swapped in for the
second pass with the parts switched. Old Chaos Shrine has no object that fades; see the sweep below.

**A model's buffer count.** A model that lets go of its buffer (main `0x0886CA58`) keeps it for two draws, counted
at `model+0x128` in the model draw (main `0x0886ACCC`), and frees it at zero (main `0x0886CAA0`), so the list still
being drawn can read it. The model draw is now a native copy (an override) that leaves the count alone in the second
pass: two draws a frame no longer free a buffer a frame early. Calls from main `0x0886AED0`, in the same 16 KiB unit
(a `goto`, not dispatch), still count; that is the alpha-screen draw of a faded fighter. The log line "a model's
buffer count ... left alone in the second pass" shows it working.

**EX Bursts and other battle events are full screen.** An EX Burst does not change the camera mode or the manager's
scripted-camera flags; it sets bit `0x80` of `+0x5B1` on both fighters at its start (`battle:0x08A3EE44`, and
`0x08A3EB70` just before it, I: the glass crack) and clears it at its end (`0x08A3F448`). `battle:0x08AB6348` sets it too, for
another event between the two fighters (I: a grab). The split task now keeps the frame one full view while either
fighter has the bit, as it does for scripted cameras. Checked: Lightning's and Kuja's EX Bursts, glass crack to the
last hit, full screen, and the split back afterwards (`exburst6/`, `kuja/`).

**Screen-capture effects:** with the EX Burst full screen, its background copy (`0x095CD140`) and the glass crack
are drawn once, from player 1's frame, as in the stock game. Kuja's EX Mode trance draws in both halves.
EX Revenge's greyscale was not reached: the route needs player 1 hit at the moment of R+□, and no scripted timeline
gave that. Untested.

**The GE list.** The display list (libGu's context `*0x09AF6830`: 4, the list's start, the write cursor, its size
0x58400; two lists in turn) has to hold both views. Largest use with both, in the sweep's fights: Old Chaos Shrine
0x1CF14 (33%), Pandaemonium 59%, Interdimensional Rift 61%, Ultimecia's Castle 65%, Crystal World 61%, **Planet's
Core 86%**. To keep a heavier frame from running past the end, the split task leaves a frame one full view when the
first view alone has used more than half the list (less 8 KiB); the log says so, five times at most. It never did in
the sweep. A list of its own for view 2, or a larger one, would remove the limit (not done).

**The stage sweep** (`build/port/versus/sweep2/`, the marker sweep's fight on each of the 14 stages, both players
moving and attacking): every stage draws both halves with nothing missing or misplaced that the captures show. The
per-view stage fade shows its own changes in view 2 on Pandaemonium, Ultimecia's Castle, Crystal World, Dream's End
and Orphan's Cradle (`D012_TRACE_MAP_FADE`); not looked at in captures. Five stages first stalled on the
opponent-info screen in the unthrottled route (the NEXT press came before the loading ended); they passed with
later presses.

Test switches added: `D012_TRACE_VIEWS=1` (both views' eye and look-at, the fighters' positions and camera modes,
and the list in use, every 15 frames). `D012_TEST_EX_FULL=2` keeps player 2's EX gauge full as well.

## 7.6 The Battle Mode row (phase 4, 25 October 2026)

**The switch.** `D012_LOCAL_VERSUS` now means: `0`, the game as it was (no row, no overrides); unset or anything
else, the mode built in, reached from the row; `1`, as before, every Quick Battle (1-on-1) a versus one without the
row (the scripted tests). Everything versus does (player 2's pad into raw pad 1, the opponent's pick, the opponent
made human, the second view, player 2's camera, the disconnect pause) acts only while versus is active: chosen from
the row until the start menu comes back, or forced. Player 2's gamepad is looked for on the start menu and while
versus is active; at other times a second gamepad stays a spare for player 1, as before.

**How the start menu builds Battle Mode** (V, static; menu overlay):

- The submenu table at `0x089E7518` has 0x18 bytes a main-menu row; Battle Mode's (row 1) is at `0x089E7530`:
  `+0x0C` its parts table, `+0x10` its texts table, `+0x14` its rows table.
- Every part of the start menu is an instance of the menu's layout 9 (`main:0x088DAA68`, from the list at
  `0x089E6606`: 70 parts). A part's look is an animation of that layout. Battle Mode's parts table (`0x089E682A`
  for three rows, `0x089E6862` for four, chosen by the save's flag `0x400000` in `menu:0x089E3DB8`) gives each
  row's two parts (looks 0x12 and 0x11), their parent (part 0x16, the submenu's frame) and the parent's slot (4, 3,
  2, 1 from the top). The Communication submenu's five rows use slots 4 to 0 of their frame.
- The texts table (`0x089E6CCE`) puts text objects (`state+0x8B8`, made from messages by `main:0x088D3F7C`; 77 of
  them, up to 0x4C) on the parts: the row's label on the first part, two copies of it on the second.
- A row is `{part, part, the hand pointer's slot, row type}`. The row type indexes `0x089E7488`: enabled, and the
  help text's index when enabled and when not. Battle Mode's handler (`0x089E4A60`) plays sound 4 and does nothing
  more for a row whose type is not enabled; otherwise sound 1 and the destination by row index.
- When Battle Mode opens (`0x089E45E8`, case 1), its cursor gets its last row (2, or 3 with four rows) from
  `main:0x088D46E4`; then `0x089E23C8` shows the parts and texts of the tables and lights the current row. Moving
  the cursor (`0x089E56F8`) lights and puts out rows and swaps the help line (`0x089E2A24`: show the new row's help,
  hide the old row's, `main:0x0887B924` / `0x0887B944`, bit `0x400` of the text's `+0x10`).

- The menu's texts array (`state+0x8B8`) has 0x59 entries. 0x4D to 0x4F are made apart from the table (the
  player's name, the play time); entries from 0x52 up are made and replaced as the menu goes. 0x50 and 0x51 stayed
  free in every run.
- Two ends: leaving for a destination (mode 4, then 5: `0x089E133C` releases the layouts, `main:0x088D3A04`, which
  takes their parts, then every text) and `0x089E17F8` (parts by table, `main:0x088D3C24`, then layouts, then
  texts). Mode 6 (`0x089E1428`) then waits until the font has no text objects left (`main:0x0887E718`): a text the
  menu does not release keeps the game on a white screen for ever.

**What the port does** (`port/host/d012_versus_menu.cpp`): the row is one more row of the same kind. Its two parts
(ids 71 and 72) are made from layout 9 with looks 0x12 and 0x11 and attached to the frame at the fourth row's slot
(three rows) or slot 0 (four rows). Its label ("Quick Battle (Player vs Player)") and the label's copy on the bar
are text objects made from UTF-16 strings (`main:0x0887D868`, as the menu makes the player's name), menu texts 0x50
and 0x51, so the menu shows, hides and releases them with the others. Its help ("Fight a friend on a second
controller, on a split screen.") is kept apart (`0x08800A80`), put on the help line's part through that word, and
released by the port after the layouts. Battle Mode's three tables are copied below the EBOOT (`0x08800800`) with
the row added, and the submenu is pointed at them when it opens. The row's type is 19, which reads as "not
enabled, help text 0": the handler leaves the row to the port, and text 0 ("Story Mode") is no Battle Mode row's
help, so the help line's swap of text 0 becomes the row's help and its highlight. Chosen with player 2's gamepad
connected, the row leaves for Quick Battle (1-on-1) (destination 2) with versus on; without one, it plays the "not
allowed" sound and shows "Connect a second controller" for three seconds. Versus goes off when the start menu comes
back. The hooks are main functions the menu calls, told apart by their callers (the module comment lists them);
nothing in the menu overlay can be overridden.

**The labels.** "1P" and "2P" tags over the portraits, drawn by the port over the game's picture (as the BGM
playlist draws its box), while player 1's fighter is played in a versus battle. The stock HUD has no player label to
change: the "COM" under Lightning's portrait is her Commando paradigm (`docs/decomp/runtime-comparison-milestone.md`).

Checked (`build/port/versus/menu8/` to `menu_final/`, `menu_nopad/`, `plain/`, local, test save with three Battle
Mode rows):

- The row under Arcade Mode in the game's style; the cursor reaches it, it lights, and the help line reads its help.
  Moving off it puts it out and brings back the other rows' help; "Story Mode" stays.
- Chosen: Quick Battle (1-on-1)'s room, player 2's pick, the versus screen and a split battle, with the 1P / 2P tags.
- Chosen with player 2's gamepad unplugged: the "not allowed" sound and the notice; the menu stays.
- Quick Battle (1-on-1) chosen as usual with the mode built in: a stock battle against the CPU, full screen, no tags.

Left open at the time, all done since (7.8):

- **Four Battle Mode rows** (the save's flag `0x400000`, the "CROOM_T" room unlocked): the row goes to slot 0 of the
  frame, as the Communication submenu's fifth row does; not seen then.
- **The row's lit label** was drawn dark on the bar: the game's rows put a second copy on the bar at locator 1, and
  the row had one copy at locator 0.
- **Results wording, no rewards, no replays, CPU rows hidden:** a versus battle ended as a Quick Battle does, with
  rewards, and the fallen fighter's HP was not found yet.
- **The loser's outro was split:** player 1's fighter stays played after a loss, and no battle-event bit is set, so
  the frame stayed split with player 2's half on an empty floor.
- `D012_TEST_LOW_HP=1` sets player 1's maximum HP to 50 once a battle, for reaching endings fast.

## 7.7 Phase 1's leftovers (25 October 2026)

- **The notice.** A box in the middle of the game's picture, over the pause the loss opened: "Controller 2
  disconnected / Connect it again to go on", "Controller 1 disconnected / Connect it again, or play on the keyboard",
  or both; gone as each comes back. Drawn by the port (`portablekit::ui::add_game_overlay`, no new PortableKit
  interface needed). Checked (`disconnect2/`): unplugged, the battle pauses with the notice; plugged in, the notice
  goes and the pause stays until that player presses Start.
- **Player 1's gamepad lost.** PortableKit's extension interface 7 adds `first_pad_connected()` (player 1 has a
  gamepad, not only the keyboard). Losing it in a versus battle presses Start on the game's own pad for one read, as
  player 2's loss does on raw pad 1. Neither presses Start when the other's loss has paused the battle already
  (Start would resume). The input script gained `detach` / `attach` for its first virtual pad. Player 2's gamepad
  never takes player 1's place: the scan keeps it as player 2's.
- **Player 2's EX Burst** (`p2burst/`, `D012_TEST_EX_FULL=2` keeps both gauges full): EX Mode, the burst landing on
  player 1, its stages full screen (7.5), player 1's defence prompt, and the damage.
- **A battle player 2 wins** (`p2results4/`): the finishing blow full screen, then the game's own ending for player 1
  losing. See 7.6 for what is still missing there.

## 7.8 Endings, rewards, the setup screen and EX Revenge (phase 4, 25 October 2026)

V unless marked. Runs local, in `build/port/versus/`.

**A fighter's HP.** The status block (`fighter+0x52C`) holds the maximum HP at `+0x0C` and the damage taken at `+0x08`;
the HP is the difference, as battle `0x089E0110` and main `0x08847964` read it. A fighter has fallen when it is 0 or
less (with a maximum that is not 0).

**The ending.**

- Once a fighter has fallen, the split task keeps the frame one full view, as for an EX Burst, so the finishing blow
  and the loser's outro are the game's own, full screen. 7.6's empty right half is gone.
- The port draws a banner over the game's ending: "Player 1 wins", "Player 2 wins" or "Draw". The 1P / 2P tags go
  at that point. The game's own ending follows as before, worded from player 1's side (the results screen, or
  "Return to Battle Setup / Rematch"). The banner is the versus wording.
- Checked: player 1 winning (`p1end3/`) and player 2 winning (`p2results5/`), both with
  `D012_TEST_LOW_HP=1` or `=2` (now: `=2` lowers the opponent's maximum HP instead of player 1's).

**No rewards.** The ending (`battle:0x08B07324`) gives each fighter's side its rewards (`0x08B0AD88`, once per
fighter: experience, AP, gil, PP, items) and then lists the local player's (`0x08B0B71C`). In versus the first is
skipped. The second still starts and shows its list, empty, as it would (list `*0x08B1D820`, `0x08B1CF0C`,
`0x08B1D314`, then `0x08B1D544`), because the ending waits for it. Skipping that as well stalled the ending. The log
says "[versus] no rewards for a versus battle". Checked: a versus win lists nothing, and a stock Quick Battle still
gives its rewards (`stockend/`). **Replays:** neither Quick Battle ending offers to save one, so there was nothing to
hide. **Not checked:** whether the battle records (win counts) still change after a versus battle.

**`decomp::call_original`** (harness). An override that only sometimes replaces a function now runs the
recompiled original otherwise. It is the function as registered before the override, taken at `register_set` with
PortableKit's new `Runtime::registered_function` and run through `run_nested`. The two reward overrides and the
setup hooks below use it.

**The setup screen without the CPU rows.** Quick Battle's setup (the character room's setup object; start
`menu:0x08AA6F0C`, redraw `0x08AA637C`) picks its tables by mode (`state+0x58C`):

| | Quick Battle (mode 0) | Mode 2 |
| --- | --- | --- |
| Row layout | `0x08AA83E0`: Stage, Strength, Level, Type (CPU), Rules | `0x08AA8588`: Stage, Rules, Judgment, its own header with "L View Ruleset", a ruleset icon (part 6) |
| Texts on the parts | `0x08AA7EF8` | `0x08AA8048` |
| Display row to setting | `0x08AA8790`: 0, 3, 1, 2, 5 (count 6 with Enter Battle) | `0x08AA87A8`: 0, 5, 4 (count 4) |
| Name plates | `0x08AA86F0`: part `0x3D` look 2, a red "CPU" | `0x08AA8710`: part `0x3D` look 1, a "GUEST" badge |

A layout entry is four u16: part, look, parent part, parent locator. The rows table at `state+0x71C` is indexed by
cursor − 1, and its count is the byte at `+0x720`. The settings are the same in both modes: Rules is setting 5,
Judgment setting 4.

What the port does (`d012_versus_menu.cpp`): the main functions the tables are handed to (`0x088D3CF0` attach,
`0x088D3F00` attach again, `0x088D44EC` place texts) are overridden. In versus they run the original with these
tables in place of Quick Battle's, all built below the EBOOT:

- A row layout: Quick Battle's header and its first six entries, with mode 2's three rows on Quick Battle's first
  three row places (locators 5, 4, 3), then mode 2's entries for the rows' own pieces (`0x08800C00`).
- A texts table: Quick Battle's first entry (player 1's plate), then mode 2's rows (`0x08800B00`).
- The name plates with the "GUEST" badge (`0x08800D00`).

After the start's plate attachment, the rows table becomes mode 2's with count 4. The screen then reads Enter
Battle, Stage, Rules, Judgment, with Quick Battle's header and plates, and "GUEST" over player 2's plate
(`setup3/`). The battle is still a Quick Battle: the cursor, the settings, Enter Battle and the split battle after it
work as before. **Not checked:** setting Judgment to anything but None, and what it does in a Quick Battle.

**The row's lit label** (7.6's "drawn dark"): the label's two copies are now menu texts 0x50 (on the label part) and
0x51 (on the bar at locator 1, as the game's rows have it). The help, which needs no slot of its own on a part, moved
to the private text at `0x08800A80` (`kPrivateHelp`). Texts on a row's parts are drawn only from the menu's own
array, so the private one could not carry the label. Checked lit and unlit (`rows_lit2/`).

**Four Battle Mode rows** (`D012_TEST_FOUR_ROWS=1` sets the save's flag `0x400000`): the fourth row is "Labyrinth".
The versus row goes to slot 0, under it, and lights, chooses and helps as with three rows (`rows_four/`).

**EX Revenge** (`D012_TEST_REVENGE=1`: R + □ on player 1's pad the moment their Brave, the u16 at status `+0x14`,
drops, which is when EX Mode becomes EX Revenge): the greyscale and slow motion show in both halves, each from its own
camera (`revenge4/`). The 2.3 row is settled.

## 7.9 60 fps on both halves (phase 3, 25 October 2026)

PortableKit's frame interpolation draws the older of two recorded frames again with its transforms blended towards
the newer one's. Before this, a split frame broke it in three ways:

- the replay had no copies at the two markers, so it drew view 2 over view 1;
- the draws of both views paired by mesh in drawing order across the views;
- one camera motion, the median over all the frame's draws, was taken for both views.

What changed (PortableKit, game-agnostic):

- **Each draw knows its view** (`DrawSummary::split_view`, `ReplayGroup::split_view`): 0 before the left marker, 1
  between the markers, 2 after the compose (the HUD). `take_split_left()` and `compose_split()` move the recording
  frame's view on, and a frame whose compose happened is marked `split`.
- **The matcher pairs draws within a view** (the view is part of a draw's key) and finds **a camera motion per view**
  (`Matching::views`). The own-motion guard and the blend of each draw use its view's camera. The cut limits apply to
  every view, each continuing its own previous motion. The top-level camera fields are the largest turn and move
  over the views; with one view they are as before.
- **The composite is a move, not a scale.** Each half is its view's middle moved a quarter of the width (left view
  left, right view right). The dark line now covers one guest pixel of each view's inner edge instead of squeezing
  the halves by one pixel.
- **The replay draws each view into its half.** View 1's draws go with their viewport moved left a quarter of the
  width, and view 2's moved right. Their scissors are cut at their half's edge. The dark line is cleared before the
  first draw that lies over both. No copies are needed: the in-between frame comes out as the compose would make it.
- **A frame that starts or ends the split** against its neighbour is a cut ("split changed"), shown as is.

Checked (local, `build/port/data-versus-60`, a copy of `data-versus-test` with `video.frame_rate=60`):

- **The replay check** (`D012_CHECK_REPLAY=1`): a split frame drawn again from its recording with nothing blended
  against the frame itself. 424 and 555 of 2,088,960 pixels differ, none by more than 8 of 255 (`sixty1/`). The
  halfway frames in a fight sit between the two frames in both halves.
- **Rate** (`sixty2/`, about 25 seconds of battle with both players moving and turning their cameras): 60 of 60 fps
  throughout, 99 to 100% of about 5,000 draws a frame matched, 0.35 to 0.5 ms a blended present. Two cuts, at quick
  camera turns. Captures show both halves and the HUD whole.
- **A stock Quick Battle** at 60 (`stock60/`): every replay check 0 pixels different, 60 of 60 fps, so one view
  interpolates as before. The framework's tests pass, with a new one for two views (`split_views`).

Not checked:

- **The Steam Deck.** A split frame has about twice the draws to record and replay.
- **The governor under load.** With the replay check on (a GPU wait every 150 frames), the governor stepped down to
  45 and then 30 for the rest of that run. That is the check's cost, not the game's, but the split frame's extra
  work leaves less room on a slower machine.
- **The intro's white flash** blends one frame towards the white frame without a cut (`sixty1/replay_8250_*`). It
  is not specific to the split.

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

# The BGM playlist selector: the game side

> Written 19 October 2026 for the native port (`port/`). The owner wants the loading screen's battle music choice
> to become a two-level playlist: left/right picks the source game (series), up/down picks a track of it, and mods
> add series and tracks. Mods only **add**; nothing of the game's is replaced. This page maps the game's BGM code,
> lists what was decompiled and how it was checked, and designs the hooks. The host side (mod folder, catalogue,
> decoding) is [mod-folder.md](mod-folder.md). **Built the same day: section 10** (controls, switches, evidence),
> then **section 11**: the series in the box, a track wheel under it, the screen's pages moved down to make room.

Conventions as in the other port notes: "V:" verified (by a run, a trace or a capture), "I:" inference. Every
address names its module: **main** (`EBOOT.ELF`) or **battle** (`OVL_BATTLE_APP.ELF`, loaded at `0x089DD4C0`).

---

## 0. Summary

- **The track table is data.** `bgm_entry.bin` (in `general_archive/main/main.bin`) is a `BENT` file of 131 rows
  of 0x2C bytes; names are strings of `bgm_name.bin` (in `general_archive/main/EN/main_lang.bin`). main `0x0889EE60`
  loads both at boot into one table object (`*0x089D6C30`) **with room for 256 rows**: the game appends DLC rows
  (`dlc/bgm/dlc_000..019.bin`) at run time with main `0x0889F040`. There is no series field: the series is only in
  the name ("FF VII - Fight On!") and in the row order.
- **The selector** (battle `0x08AF8CA4`, struct at loading screen `+0x6AC`) offers every row with a non-zero random
  weight whose unlock flags are set (battle `0x08ADE4E0`), in table order, **up to 256**; 38 with the test save.
  Only **D-pad left/right** (with key repeat) moves it, wrapping both ways. D-pad up/down already scroll the page
  beside it, and L/R change that page.
- **Selection to playback:** the loading flow keeps the chosen id (battle `0x08AF5AC0` into flow `+0x74`); main
  `0x0888C4C8` copies it into the field's BGM id (`FIELD+0x804`) and the replay record; at the start of the fight
  main `0x08888868` plays it with main `0x0889F234`, which finds the row and starts streaming
  `DISC0:/PSP_GAME/USRDIR/DATA/BGM/<file>` (main `0x0889FD38`). The stream layer knows only the file name.
- **Decompiled:** 12 functions in `port/decomp/` (C), 10 of them running in the game behind `D012_DECOMP_BGM=1`.
  **V:** with `D012_DECOMP_VERIFY=1` a differential harness compared 2,888 calls in one run against the recompiled
  originals from the same state: all the same (memory, results, registers, system calls). With the switch off and
  on, the same route gives the same selector screens and the track shown is the track the game streams.
- **Mod tracks fit the game's own mechanism**: they become extra table rows (the DLC path), with an id from a
  reserved range, a file name the host serves, weight 1 and no stage bits. Then every existing path (list, name,
  play) handles them. The 2D selector replaces two functions (the list and the per-frame update).
- **Built** (section 10): L2/R2 (the triggers, or 1 and 3 on the keyboard) switch the loading screen's D-pad
  between the screen's menu and the music; in BGM focus left/right pick the series and up/down the track. On by
  default; `D012_NO_BGM_PLAYLIST=1` restores the game's selector. V: a mod track chosen this way plays in battle,
  and the differential test still finds the decompiled originals the same as the game's (1,849 calls).
- **Then** (section 11): the box shows the series and the track's place in it ("FINAL FANTASY I · 1/3"), a track list
  under it shows the series as a wheel round the chosen track, and the right half's three pages (Equipment,
  Accessories, Battlegen) move down and get shorter, by 436 checked edits of the screen's layout data in memory (no
  code replaced). `D012_BGM_PLAYLIST_FLAT=1` keeps section 10's selector; `D012_NO_BGM_PLAYLIST=1` restores the
  game's screen pixel for pixel. V: 1,999 differential comparisons, all SAME.

---

## 1. Map

### 1.1 The track table

| What | Where | V/I |
| --- | --- | --- |
| Loader | main `0x0889EE60` (from `0x0893354C` at boot): reads `bgm_entry.bin` (`prologus_bgm_entry.bin` when `*0x089D5F20 == 1`) from the main archive, allocates `size + (256 - count) * 0x2C`, copies, allocates `(256 - count) * 0x84` bytes of DLC names (slot/row `0xFFFF`), builds a text table from `bgm_name.bin` | V (code) |
| Table object | `*0x089D6C30`, struct `BgmTable` in `port/decomp/bgm_types.h`: `+0x04` file, `+0x08` rows, `+0x0C` names, `+0x1C` in-memory player (victory themes), `+0x40` disc row count, `+0x44` DLC names | V |
| Row | `BgmEntry`, 0x2C bytes: `s16 id`, `s16 name_text`, `s16 unlock[4]` (save flags, all required, -1 none), `u8 flags` (bit 2: DLC, stream from an ms0 EDAT), `s8 volume` (percent), `s8 weight` (random choice; 0 = never offered), `u32 stage_mask[2]`, `u32 mask18`, `char file[16]` | V (code and file) |
| Rows on the disc | 131 rows, ids 0..130 (no gaps above 130), version word `0x20100831`. 68 rows have a weight; the others are menu, event, story and victory music. Row 130 (id 70) is "No BGM" (empty file name, weight 1) | V (file) |
| Names | `bgm_name.bin`: 132 strings, the game's message-table format (`u16 count`, offsets, hidden with the key in `tools/character_text.py`), 16-bit units. Lookup main `0x0887AFBC` | V |
| Lookups | main `0x0889F5E8` row of id, `0x0889F3E4` row pointer of id, `0x0889F434` name of row (disc rows from `bgm_name.bin`, DLC rows from their own 0x84-byte slot), `0x0889F4AC` name of id | V |
| DLC rows | main `0x08861C64` scanner over `ms0:/PSP/GAME/ULUS10566/<hash>.edat` of `dlc/bgm/dlc_%03d.bin` (slots 0..19, the same `BENT` layout); each row appended by main `0x0889F040` (refuses past 256), its name taken from `text/jp/dlc/bgm_%03dt.bin` by main `0x0889F18C` (63 units) | V (code); no DLC on the test stick |
| Unlocks | `unlock[]` are bits of the save's flag field (save root `+0x1DF88`, pointer at `0x089D726C`): flags 1047..1071 (the 25 "BS" and later tracks, PP Catalog), 400 (Chaos's last battle themes), 727, 730 | V (file); I: which catalogue items set them |
| Series | **No field.** Rows are grouped by series in file order (FF I to FF XIII, then Cosmos, Chaos 1-3, Cantata Mortis, DFF Opening); the series is the name's prefix before " - "; `stage_mask[0]` pairs bits per series' stages (FF I `0x6`, FF II `0x18`, ...) but is 0 for the Dissidia rows | V (file) |

Selectable rows, by series (id, then `*` = needs an unlock flag): FF I 0 1 71 2* 86*; FF II 3 4 72 5* 87*; FF III 73 6
7 8* 88*; FF IV 74 10 9 11* 89*; FF V 13 12 75 14* 90*; FF VI 76 15 16 17* 91*; FF VII 19 77 18 20* 92*; FF VIII 21
78 22 23* 93*; FF IX 24 25 79 26* 94*; FF X 28 27 80 29* 95*; FF XI 30 81* 31* 96*; FF XII 32 82 33* 97*; FF XIII 83 84
98*; Dissidia 38 34* 35* 36* 85* 37; and 70 "No BGM".

### 1.2 The default and the loading screen

- **Default id**: main `0x0888B544` sets `FIELD+0x804` per battle mode (`FIELD+0x4C`): a weighted random row for the
  stage (battle `0x08ADE458`..`0x08ADF22C`, rows filtered by unlocks, `stage_mask`, `mask18`, flags bits 0/1, seeded
  from `sceKernelGetSystemTimeLow`), a fixed id from an opponent preset (`enmp` row `+0x22`, battle `0x08ADE648`) or
  from the battle rules block (`0x09AFDE48 + 6`), or the replay's (`+0x22E4`). V: two runs of the same route started
  on "FF I - Chaos Shrine" and "FF XIII - Saber's Edge": the default is random, from the clock.
- **Loading flow** (battle, vtable `0x08AFB520`, built in main `0x0888C4C8` by `0x08AF2C10`/`0x08AF2CBC` with the
  default id at `+0x6C`): when the loading screen (`+0x24`, battle `0x08AF5C88`, 0xF70 bytes) ends, battle `0x08AF3938`
  stores the selector's choice (or the default) at `+0x74`.
- **Selector struct** `BgmSelector` at screen `+0x6AC` (0x820 bytes): three layout elements (`+0` the name's anchor
  and sprite list, `+4` left arrow, `+8` right arrow), the name sprite `+0x14`, `entry[256]` `+0x18`, `name[256]`
  `+0x418`, `count` `+0x818`, `index` `+0x81C`. Shown unless the rules mode byte (`0x09AFDE50`, screen `+0xF54`) is
  0 or 7. V: in Quick Battle it is the "BATTLE BGM" box at the top right of the Opponent Info pages.
- **Build** (battle `0x08AF590C`, once): the list from battle `0x08ADE4E0` into `entry[]`/`name[]`, `index` = the row
  whose id is the default, three layout elements from the screen's layout.
- **Per frame** (battle `0x08AF94D8`, the screen's update, then battle `0x08AF8CA4`):
  - Input, only while screen `+0xF6D == 1` (cleared when the player confirms): logical pad `0x80` (D-pad right) or
    `0x40` (D-pad left), tested with **is-repeat** (vtable slot `+0x20`; repeat after 30 then every 4 frames, pad
    module doc). Right plays the right arrow's animation 4 (rest 3), left the left arrow's 2 (rest 1), and the cursor
    sound (main `0x088A24D4(0, 10000, 1, 0)`). `index = (index + count ± 1) % count`.
  - On a change: if there is no name sprite, make one (battle `0x08AF5AE8`: text sprite from the game's text system,
    main `0x0887D868`, appended to element 0's sprite list); otherwise unlink the old one, give it an end callback
    (battle `0x08AF5C6C`, which makes the new name) and delete it. The box is blank for two frames at each change;
    the playlist avoids it (11.7).
  - Place the name at anchor 0 and the arrows at anchors 1 and 2 of element 0.
- **What else the screen takes**: L/R (`0x10000`/`0x20000`, pressed) switch the info pages, **D-pad up/down
  (`0x10`/`0x20`, repeat) scroll the page's list** (battle `0x08AF3E60`), confirm (`0x4`) ends the screen (battle
  `0x08AF32C0`), cancel (`0x8`) held skips it (battle `0x08AF33D0`). The analog stick's directions are other bits
  (`0x400`, `0x800`, ...): the selector does not take them.

### 1.3 From the choice to the music

1. main `0x0888C4C8` (field set-up) reads the flow's `+0x74` with battle `0x08AF2D08`; if not -1 it becomes
   `FIELD+0x804`, then main `0x088C990C` copies the battle set-up and the id into the replay record (`+0x22E4`).
2. main `0x08888868` (fight start): `0x088A18B0(sound, 100, 0)`, then `0x0889F234(table, FIELD+0x804)`.
3. main `0x0889F234` → `0x0889F3E4` (row) → `0x0889F534`: volume = `row.volume * 0.01`, then
   `0x0889FD38(volume, slot 0, row.file, open now, row.flags & 4)`.
4. main `0x0889FD38` (the stream start): stops the current stream (`0x0889FF8C`), keeps the request at
   `0x09B4CEF0` (name, slot, volume, ms0 flag), builds the path (`0x088A04A8`: `DISC0:/PSP_GAME/USRDIR/DATA/BGM/%s`
   or, for a DLC row, `ms0:/PSP/GAME/ULUS10566/<hash of "bgm/<file>">.edat` with `sceNpDrmEdataSetupKey`, then
   upper case), opens it through the game's file layer (main `0x08862E08`), **reads the first 0x18000 bytes** into
   the stream buffer (`*0x089D6C38`, 0x18002 allocated) synchronously, hands them to the sound library
   (`0x088A1380` → `0x0899016C` → `sceAtracSetDataAndGetID`), plays (`0x088A1400`) and sets the volume
   (`0x088A14D0`). State `*0x089D6C50` = 4.
5. The rest of the file is streamed by SOUND_MANAGER::ExecTask (main `0x088A19B0` → state machine `0x088A0098`,
   playing state `0x088A0264`): when the library asks (`0x08990578`, `sceAtracGetStreamDataInfo`), it seeks and reads
   with the file layer's asynchronous read and adds the data (`0x08990680`, `sceAtracAddStreamData`). The loop
   points are in the file's RIFF header and the ATRAC library uses them itself (I: no game code reads them; the
   bring-up log shows `sceAtracSetLoopNum`). Fade-outs: `0x0889FF8C(frames)`.

V: `D012_TRACE_IO` on the route: `open DISC0:/PSP_GAME/USRDIR/DATA/BGM/B01_02.AT3`, a 98,304-byte read, then
reads of 49,140, 560, 48,160, ... bytes with `async result` lines.

### 1.4 What is in the game's saves

- `DISSIDIA.BIN`: the unlock flags above (PP Catalog purchases). V (static): no code found writing a BGM id into
  the save image; the default and the choice live in the field and battle set-up blocks (BSS). Not verified at run
  time.
- `REPLAY.BIN` (a save of its own): the replay record copies the battle set-up and **the BGM id** (`+0x22E4`).
  A replay of a battle fought to a mod track holds the mod track's id.
- The Museum player (menu `0x08A55D54`, up to 128 rows with flags bit 0 or 1 and not DLC; playlist ids at its
  state `+0x510`): I: not checked whether its playlist is saved.

---

## 2. Decompiled functions and their check

`port/decomp/` holds C written from the machine code (`bgm_types.h` structs and addresses, `guest.h` the interface,
`bgm_table.c`, `bgm_selector.c`). Behaviourally equivalent, not matching. `port/host/d012_decomp_harness.*` runs
them; `port/host/d012_decomp_bgm.cpp` lists them.

| Function | Original | In the game | Checked calls (V, one run) |
| --- | --- | --- | ---: |
| `bgm_table_find_row` | main `0x0889F5E8` | yes | 6 SAME |
| `bgm_table_find` | main `0x0889F3E4` | yes | 9 SAME |
| `bgm_table_name` | main `0x0889F434` | yes | 131 SAME |
| `bgm_table_name_of` | main `0x0889F4AC` | yes | 6 SAME |
| `bgm_play_id` | main `0x0889F234` | yes | 1 SAME |
| `bgm_play_entry` | main `0x0889F534` | yes (ends with a tail call to the original stream start) | 1 SAME |
| `bgm_stream_path` | main `0x088A04A8` | yes | 7 SAME (disc and ms0 paths) |
| `bgm_stream_start` | main `0x0889FD38` | **no**: checked only (below) | 1 SAME |
| `bgm_list_offered` | battle `0x08ADE4E0` | yes | 1 SAME (38 rows) |
| `bgm_selector_update` | battle `0x08AF8CA4` | yes | 908 SAME (presses right, right, left, left, left) |
| `bgm_selector_chosen` | battle `0x08AF5AC0` | yes | 909 SAME |
| `bgm_selector_make_name` | battle `0x08AF5AE8` | no: only reached inline | 908 SAME |

V: run `verify4` (`build/port/bgm/logs/verify4.log`): 2,888 comparisons, 0 different, 0 inconclusive.

### 2.1 The harness

`D012_DECOMP_VERIFY=1` (with `D012_DECOMP_BGM=1`): at each call of an override, before it runs in the game, a
second `psprecomp::Runtime` (the reference) gets a copy of guest RAM and VRAM, the caller's registers, the main
corpus and the overlay library resident in the game (found by its identity hash). It runs the **original** to its
return (return address set to an unregistered magic address, chaining off), keeps the RAM, restores the copy, runs
the **decompiled** function (its guest calls run nested in the same reference), and compares:

- `$v0`/`$v1` or `$f0`, `$s0`-`$s7`, `$gp`, `$sp`, `$fp`, `$f20`-`$f31`;
- every byte of RAM except the 64 KiB of dead stack below the caller's `$sp`;
- the system calls made, in order, with `$a0`-`$a3`. In the reference each of the executable's 296 imports is a
  recorder answering 0, except that vcount and system-time reads advance, so frame waits end. Guest functions can be
  stubbed the same way (`ReferenceStub`): the file layer's open/read/close are, because a synchronous read waits
  frames for I/O the reference does not do.

`verify_probe()` checks a helper from the state of the current call with arguments of its own (the selector's update
probes `chosen` and `make_name`; the list probes the lookups and paths; play probes the stream start). The game is
never touched by the test. `D012_DECOMP_VERIFY_LIMIT` (25) caps checks per function; `D012_DECOMP_VERIFY_ONLY=1`
tests without putting the decompiled code in place.

Rules learned (they make or break a check): a decompiled function must reserve the **same stack frame** as the
original (`g_push`), because callees and system calls see `$sp` (the first `make_name` check differed only in
that); a guest call made from host code must run with **chaining and thread dispatch off**, since a chain boundary
or a system call may otherwise switch threads under the host frame.

### 2.2 How an override runs in the game

`Runtime::register_function(address, live_entry, "d012_decomp_<name>")`. The name does not start with
`recomp_unit_`, so the entry is not chainable and its unit is taken off the dense chain table: every call from
another unit reaches it through dispatch. Its guest calls run nested (`invoke_isolated_aot` loop to a magic return
address) with chaining off (the corpus runtime's protected `chain_depth_limit_`, reached from a derived class) and
`Kernel::set_dispatch_enabled(false)`; a thread switch during one aborts with a message (never seen).

- **Same-unit calls bypass overrides.** A call between functions in the same 16 KiB unit is an inline `goto` in the
  generated code. V: battle `0x08AF8F58` calls `0x08AF590C` (build) and `0x08AF5A80` (show) inline (unit 70), so
  those cannot be overridden from their caller; `0x08AF8CA4` is called from unit 71 and `0x08ADE4E0` from unit 70 into
  unit 64, so they can.
- **Overlay reinstalls drop overrides.** Installing an overlay unregisters its range. The port registers its
  battle overrides again after each install: PortableKit's code-miss hook (asked before every install) marks it, a
  post-dispatch hook re-registers (and checks the window's identity every 65,536 dispatches). Main overrides are
  registered once, at the first code miss. All of it from a static initialiser in `d012_decomp_bgm.cpp`: no change to
  the profile files.
- **Blocking callees cannot run nested.** The stream start's callees stop the previous stream and read the file
  synchronously, which may wait for a vblank on the calling thread. So `bgm_play_entry` ends with a **tail call** to
  the original `0x0889FD38` (equivalent: that function answers only 0 or 1 and stores the same volume itself), and
  `bgm_stream_start` is checked but not put in the game.

### 2.3 Real runs (V)

Same route, switch off (`off1`, `off2`) and on (`on1`, `on2`); captures every 100 frames, `D012_TRACE_IO`:

- `off1`/`on1` (presses right, right, left, right, right): both defaulted to "FF XIII - Saber's Edge"; the nine
  captures of the loading screen (frames 7200 to 8000) are **pixel-identical** between the two runs outside the
  performance overlay (Cosmos at 7400 and 7600, No BGM at 7800 and 8000), and neither opens a BGM file in battle.
- `off2`/`on2`: random defaults differed (FF I - Chaos Shrine / FF XIII - Saber's Edge); after right, right, left,
  left, left each lands one row before its start (FF I - Dungeon / FF XIII - Blinded by Light) and streams that file
  (`B01_02.AT3` / `U_13BATTLE.AT3`).
- `verify4` also played the shown track (`B01_02.AT3`), with the decompiled play path.

---

## 3. Hook points for the 2D selector

| Hook | Reachable | Role in the playlist |
| --- | --- | --- |
| battle `0x08ADE4E0` list | yes (from unit 70 into 64) | Replace: build the offered rows grouped and ordered by series (game series, then mod series by `order`), still ≤ 256 rows, entries + names arrays as now. Index stays a row index, so the build function's own "find the default" keeps working |
| battle `0x08AF8CA4` update | yes (from unit 71) | Replace: left/right step **series** (first or last-chosen track of the next series), up/down step tracks within it; arrows as now; a second text sprite for the series line; the cursor sound as now |
| battle `0x08AF5AC0` chosen id | yes | Keep: the id of `entry[index]`. A mod row's id flows on through the field, replay and play paths |
| battle `0x08AF590C` build, `0x08AF5A80` show, `0x08AF5AE8` name | **no** (inline from their unit) | Keep; they call the list (overridable) and draw `name[index]` |
| main `0x0889EE60` table load | yes (from main `0x0893354C`, other unit) | Append mod rows after the game's (and after any DLC rows), or do it lazily before the first list |
| main `0x0889F234`/`0x0889F534` play | yes | Nothing to change when mod rows are table rows. Hook here only to prefetch (`d012::mods::prefetch`) when a mod row starts |
| main `0x088726B0` pad translate, or the profile's sceCtrl wrapper | I: yes | Up/down conflict, below |

**The up/down conflict.** The screen's update (battle `0x08AF94D8`) scrolls its page list with D-pad up/down
before the selector runs, in the same frame. Options, in order of preference:

1. The profile wraps `sceCtrlReadBufferPositive` (as `d012_hle.cpp` wraps the vblank waits) and, while the selector
   override has run in the last two frames, removes UP/DOWN from what the game reads and passes them to the
   selector (port-side state). The page list then no longer scrolls on the loading screen (it is a view of the
   equipment; nothing is chosen there).
2. Up/down for tracks only while a modifier is held (e.g. L2 or R2, which the game never sees today), otherwise
   the page scrolls as now.
3. Tracks on the right stick or L2/R2. Least intrusive; not what the owner asked for.

The selector must stop taking input when `+0xF6D` is cleared (confirm), as now.

## 4. Ids for added tracks

- The game's ids are 0..130; the field and the selector hold an id as **s16** (`lh`), sign-extended; -1 means none.
  DLC rows bring their own ids (unknown range; none on the stick).
- **Mod rows take ids in `0x4000..0x7FFE`**: `0x4000 + (number % 0x3FFF)` where `number` is the track's FNV-1a
  number from the catalogue, probing upwards (wrapping in the range) past ids already used, tracks taken in id-text
  order so the result depends only on which tracks are present. Never 0..0x3FFF (game and DLC), never negative.
- The id is a session handle: the port keeps `id → track id text`. It is the same from run to run while the same
  mods are on. What can hold it beyond the session is only `REPLAY.BIN` (section 1.4).
- Capacity: 256 rows in all, so with the 131 disc rows **125 mod tracks**, fewer with DLC. More needs the list
  override to offer rows that are not table rows (then `0x0889F3E4`/`0x0889F234` must be overridden too).

## 5. How a selected mod track reaches playback

Mod rows are ordinary rows: `{id, name_text = 0, unlock = -1 x4, flags = 0, volume = 100 (or the mod's), weight =
1, stage_mask = 0, mask18 = 0, file}`. Weight 1 puts it in the selector's list; no stage bits keep it out of the
random default; flags 0 keeps it out of the Museum and off the ms0 EDAT path. Names go in the row's DLC name slot
(`table+0x44`, 0x84 bytes each, 63 text units) in the game's 16-bit text encoding.

The game then opens `DISC0:/PSP_GAME/USRDIR/DATA/BGM/<FILE>` (upper case) and streams it like its own; the host
serves that path (PortableKit file provider, [mod-folder.md](mod-folder.md) section 5).

**Coordination needed:** the row's file name is at most **15 characters** (`char file[16]`, NUL included; the next
byte is the next row's id). `d012::mods::bgm_path()` first named `MOD_<8 hex>.AT3`, 16 characters. Changed on 19 October 2026 to
`MD<8 hex>.AT3` (14), e.g. `MD7D9EC9A2.AT3`, compared without case (the game upper-cases the path). `MD` rather
than `M` keeps it visibly apart from the disc's `M01.AT3`-`M05.AT3`.

Insertion: from the table-load hook (or lazily before the first list), host code writes the rows and slots directly
and raises `file.count`; or it calls the game's own `0x0889F040` (which also fills the slot header) and then
`0x0889F18C` for the name. Rows must go after the disc rows and after DLC rows (DLC loads later, at
`0x08861C64`; I: the order of the two at boot is to be checked, or mod rows inserted at the first list call).

## 6. What never goes into the game's save

- Nothing in `DISSIDIA.BIN` changes: no flags are set for mod rows (they need none), no ids are written.
- Mod choices (last series and track per player, favourites, a "random among mods" choice) live in a port file, e.g.
  `<data dir>/mods/bgm-choices.json`, keyed by **track id text** (`example-tones:intro-loop`), never by row index or
  session id. A missing mod's entries are ignored on load and kept in the file.
- `REPLAY.BIN` holds the BGM id. A mod row's id there is harmless when the mod is gone (`0x0889F3E4` finds no row,
  `0x0889F534` returns 0: the replay plays without music); with the same mods it plays the same track. A later
  step can replace a mod id with a game id (or -1) when the replay is saved (`0x088B0AF8` writes `REPLAY.BIN`; I:
  not traced).
- The mod rows exist only in memory, from the port, at each boot; removing a mod changes nothing persistent.

## 7. Drawing the series and track names

- **The game's text system (recommended).** The name is already a text sprite (`0x0887D868(text system, 0,
  string, -1, 0...)`) appended to layout element 0 and placed at its anchor 0 each frame. A second sprite for the
  series line, placed at anchor 0 offset by one line, uses the same font, colours, fades and depth as the rest of the
  screen and scales with the port's rendering. Strings must be in guest memory in the game's encoding; series names
  can sit in unused DLC name slots at the end of the table's slot array (no allocation). PortableKit draws the
  game's fonts from host TrueType, so non-ASCII text depends on the host font (mod-folder.md advises ASCII).
- **A port overlay (ImGui).** Easier to lay out, any text; but it is drawn over the game at host resolution, does
  not follow the screen's fades or the "BATTLE BGM" frame, and needs to know where the box is. Keep it for debugging.
- The box is sized for one line (V: captures); a series line may need the box's layout element enlarged
  (layout data, not code) or the series shown in place of the "BATTLE BGM" caption. To be decided with the owner on
  a capture.

## 8. Framework requests (for the PortableKit clone's owner)

1. A callback after an overlay library is installed (and after one is dropped), so a port can re-register
   overrides without the code-miss/post-dispatch pair.
2. A supported way to run a guest function to completion from host code (chaining and dispatch off, a return stub),
   and to call the original of an overridden address (today `lookup_function` is private, so verification needs a
   second runtime).
3. The chain-depth limit as a public scoped setting instead of a protected member.

## 9. Not verified, risks

- The replay sanitising (section 6) is a design, not code. The 2D selector and the mod rows are built: section 10.
- The default being random means two runs rarely start on the same row; the comparisons are of behaviour (same
  steps, same track for the shown name), and the differential test compares the same state.
- The harness's nested calls rely on callees that never block; the stream start is the one found that can.
- A thread switch inside a nested call aborts the game (by design); none was seen.
- The rows' flags bits 0/1 meanings beyond "random choice groups / Museum" are not decoded; `mask18` neither.
- DLC rows (ms0 EDAT) were not exercised.
- Built against a snapshot of the PortableKit clone (`tools/local/port/PortableKit-bgm`: `0c2f5b7` plus the working
  tree of 19 October 2026 morning), because the clone was being changed while this was built. Since merged: the
  decompiled functions build into the normal recompiled build, `build-d012-recomp`, and the snapshot is gone.

## 10. What was built (19 October 2026)

The playlist is the port's normal behaviour now: `port/host/d012_bgm_playlist.cpp` (with `.hpp`), wired in through
`d012_decomp_bgm.cpp` (the overrides) and `d012_hle.cpp` (the pad wrapper and the overlay). Two framework changes
on the clone's `dissidia` branch serve it, neither naming the game: **extra buttons** (patch 0026: L2/R2/L3/R3,
which a PSP lacks, read by a port through `hle_extension::extra_buttons()`) and **game overlays** (patch 0027:
`ui::add_game_overlay`, `ui::game_picture`, `ui::game_text_font`). The profile names L2 and R2 in its new
`extra_buttons` field.

### 10.1 Controls

| | Menu focus (when the screen opens) | BGM focus |
| --- | --- | --- |
| L2 or R2 | to BGM focus | back to menu focus |
| D-pad left/right | the previous/next track of one list, ordered series by series (as the game, with its key repeat and arrows) | the previous/next **series**, landing on the track last shown in it on this screen (the first at first) |
| D-pad up/down | scroll the page beside the selector (the game's) | the previous/next **track of the series**, wrapping; the page does not see them |
| L/R, confirm, cancel | the game's | the game's |

On a gamepad L2 and R2 are the triggers: with the profile naming them, the triggers press L2/R2 instead of
copies of L and R (V: the info page no longer turns when a trigger is pressed). On the keyboard they are **1**
and **3**, the number keys above Q (L) and E (□), free in both of the framework's layouts; the menu's Keyboard and
mouse rows rebind them (`input.bind.l2`, `input.bind.r2` in `settings.ini`). Every change plays the game's
cursor sound; a series change also plays the arrow's animation. When the player confirms (screen `+0xF6D`
cleared) the selector stops taking input, as the game's does, and the focus returns to the menu.

What shows the focus (V: captures below):

- **Menu focus**: the game's screen, plus a small "L2/R2 BGM by series" hint under the box.
- **BGM focus**: the track name turns a dark rust red (the game's own text sprite: its colour, at sprite `+0x0C`,
  is copied from the box's anchor each frame together with the screen's fade, and the playlist changes only the
  colour bytes after that copy); a gold outline goes round the box; the series has a line of its own under the box
  (◀ FINAL FANTASY XIII ▶, the track's place in it, 2/4); a key line under that ("▲▼ Track  L2/R2 Done").

The outline, series line and hints are a **port overlay**, drawn with ImGui over the game, placed from the box's
anchor 0 (the name's centre, `(348, 16)` in the PSP's pixels on this screen) and faded with the anchor's alpha,
in the face the game's own text is drawn with. Extra text sprites of the game's own were looked at and not used:
the screen's teardown (battle `0x08AF605C`) runs inline in its unit and deletes only the selector's name sprite
(`0x08AF5A04`) and its own five slots, and the text system keeps sprites in a fixed pool (main `0x0887DB1C`
returns them to a free list), so a sprite the playlist made would leak on every screen it could not see end.

### 10.2 Series

The offered rows are exactly the game's: the playlist's list calls the decompiled `bgm_list_offered` (unlocked,
non-zero weight, in table order) and then groups them. Each disc row's series comes from its **name, read from the
game's text at run time** (`0x0889F434`): the part before " - " when it starts with "FF" (or "FINAL FANTASY")
gives the port's label "FINAL FANTASY" plus the rest ("FFXIII - Saber's Edge" → FINAL FANTASY XIII); any other name
is **DISSIDIA** ("Cosmos", "DFF - Opening"). Series keep the order the table has them in. Then come DLC rows
(DOWNLOADED, none on the test stick), the mods' series in the catalogue's order (manifest `order`, then id), and
last the row with no file, "No BGM", as **NO MUSIC**.

V (test save, mods off): 38 tracks in 15 series, FINAL FANTASY I to XIII (3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 2, 2),
DISSIDIA (2), NO MUSIC (1). The log line `the flat list ... is in the game's own order` confirms that with no mods
menu focus steps through exactly the game's list; with mods the mod series sit before NO MUSIC, so the list is
regrouped. The verification of the game's own list in the same run: `list_offered ... SAME ... v0=0x00000026`.

### 10.3 Mod tracks as table rows

At the first list call the enabled mods' tracks are appended to the table, after the disc's rows and any DLC rows (I: the game's DLC scan runs before any battle; DLC was not exercised)
(section 5): id from `0x4000..0x7FFE` (section 4: `0x4000 + number % 0x3FFF`, probing upwards, tracks in id-text
order), file `MD<number>.AT3`, `unlock = -1 x4`, flags 0, weight 1, no stage bits, and the name written into the
row's DLC name slot (`dlc_slot = -2`, neither a DLC slot nor free; ASCII, anything else as `?`, 63 units). The
volume is the **median of the game's own battle rows** (37 to 80 percent, 56 with this table), so a track mastered
like any music file plays about as loud as the game's own (V: the test tone at about 4,600 against the file's
8,192; at 100 it was 8,190). Rows are added in display order while the table has room (256 rows: 125 mod tracks
with no DLC); the rest are logged and left out. The rows are checked at each list and added again if the table no
longer holds them. When a mod track is shown the playlist calls `d012::mods::prefetch`, so it is decoded before
the battle opens it (V: `[host-audio] decoded example-tones:intro-loop ... in 102 ms` at the selection, seconds
before the battle).

Example (`build/port/data-playlist-test`, three mods on): rows 131 to 141, ids such as `0x4025`
(`example-tones:intro-loop`, `MD7D9EC9A2.AT3`).

### 10.4 The overrides and the pad

- `d012_decomp_bgm.cpp` registers, **by default**, two overrides: battle `0x08ADE4E0` runs `playlist::run_list`,
  battle `0x08AF8CA4` runs `playlist::run_update`. The harness's `Override` has a new field, `check`: the game runs
  `run` (the playlist), and the differential test compares `check` (the decompiled original) with the recompiled
  original from the same state, so `D012_DECOMP_VERIFY` keeps checking all twelve functions. `D012_DECOMP_BGM=1`
  adds the other ten as before.
- The list is called by the selector's build (battle `0x08AF590C`) with buffers on its own stack; the build's
  `$s0` is the selector, which is how the playlist knows the screen. The build then finds the default's index in
  the grouped list itself. The update keeps the game's structure: the pad's is-repeat for left/right, the arrows'
  animations 4/3 and 2/1, the cursor sound, the name sprite's remake through its end callback
  (`bgm_selector_name_changed`, the part of the decompiled update now shared), then the placing.
- **Up/down in BGM focus.** The profile wraps `sceCtrlReadBufferPositive` (the game's only pad read): it keeps the
  buttons as read, and while BGM focus is on and the selector took input within the last four reads, it clears
  D-pad up and down (`0x10`, `0x40`) in what the game reads (design option 1, scoped to that focus). The playlist
  reads them from the kept buttons with the pad module's own key repeat (30, then 4, counted down by the frame
  step at main `0x089D5EAC`, at least 2).
- **Last choice.** The game has no "last chosen BGM" to mirror: its default is random per battle (section 1.2) and
  the choice goes only into the field and the replay. So nothing is remembered across screens and no file is
  written; within a screen each series remembers the track last shown in it.

### 10.5 Switches

| Variable | What it does |
| --- | --- |
| `D012_NO_BGM_PLAYLIST=1` | The game's own selector: no overrides (unless `D012_DECOMP_BGM`), no mod rows, no pad wrapper, no overlay. V: the screen as before, `1` does nothing, up/down scroll the page |
| `D012_NO_EXTRA_BUTTONS=1` | PortableKit's: the profile's extra buttons are ignored, the triggers press L and R; L2/R2 never reach the playlist |
| `D012_TRACE_BGM_PLAYLIST=1` | Logs every mod row, every choice with its series, id, volume and file, and the box's anchor |
| `D012_TEST_BGM_START=<id or mod track id>` | Test only: the selector starts on that track instead of the game's random default (for repeatable runs), e.g. `0` (FFI - Battle) or `example-tones:intro-loop` |
| `D012_DECOMP_BGM=1`, `D012_DECOMP_VERIFY=1` | As in section 2; with the playlist on, the list and the update are checked through `check`. `D012_DECOMP_VERIFY_ONLY=1` leaves every original in the game, the playlist's two included |

Always logged: the series and counts at each screen, each focus change and each change of track.

### 10.6 Evidence (V)

Recompiled build, `PSPRECOMP_NO_INTERPRETER=1` (no fallback in any run), `D012_UNTHROTTLED=1`, the route of the
Reproduce section, data in `build/port/data-playlist-test` (a copy of `build/port/data` with the test mods of
`build/port/data-mods-test/mods`: example-tones, format-test, the broken-fields mod's one good track, off-test off).
Logs and captures in `build/port/playlist/<run>/`.

- `run1`, `run5` (start pinned to id 0): menu focus, D-pad down scrolled the page (the hand moved to Leather Hat),
  right stepped to FFI - Dungeon; the left trigger gave BGM focus; down went to FFI - Chaos Shrine 3/3 and the page
  stayed; right went to FINAL FANTASY II 1/3; left, left, left, left, left went FINAL FANTASY I (back on 3/3), NO
  MUSIC, TEST SWEEPS, FORMATS, TEST TONES 1/3; down, up moved within TEST TONES; the right trigger gave menu focus;
  confirm. The battle opened `DISC0:/PSP_GAME/USRDIR/DATA/BGM/MD7D9EC9A2.AT3`, served by the host (74,100 bytes),
  and the audio dump (`D012_AUDIO_DUMP`, scanned as in mod-folder.md section 7) has 440 Hz for 2 s, then 660 and
  880 Hz alternating each second to the end of the run, never 440 again: `example-tones:intro-loop` with its loop.
- `run2`: the keyboard's 1 gave BGM focus, right and down chose FFII - Battle Theme 2, 3 gave menu focus, confirm;
  `D012_TRACE_IO`: `open DISC0:/PSP_GAME/USRDIR/DATA/BGM/B02_02.AT3`, a 98,304-byte read, then the streamed reads.
  The same run with `D012_DECOMP_BGM=1 D012_DECOMP_VERIFY=1`: **1,849 comparisons, all SAME**, 0 different, 0
  inconclusive: list_offered 1, selector_update 558, selector_chosen 559, selector_make_name 558, table_name 142,
  table_find 9, table_find_row 6, table_name_of 6, stream_path 7, play_id 1, play_entry 1, stream_start 1.
- `run3_nomods` (`D012_NO_MODS=1`): 38 tracks, 15 series, the game's own order (10.2).
- `run4_off` (`D012_NO_BGM_PLAYLIST=1`): no playlist line in the log, the game's selector, no overlay.
- `run6`: the captures `build/port/playlist/menu_focus.png`, `bgm_focus.png` and `bgm_focus_mod_series.png`
  (TEST TONES 2/3, "Chord, whole file looped").

### 10.7 Not verified

- A real gamepad (the triggers and the keyboard came from the input script's virtual pad and keys), fullscreen and
  the Fill aspect for the overlay's placement, a window other than 960x544.
- A mod series name or track title outside ASCII on the game's band (titles become `?` there; the overlay's series
  line draws any character the game's face has).
- DLC rows together with mod rows; more mod tracks than the table has room for; a replay saved with a mod track.
- The selector on other modes of the loading screen (it is shown unless the rules mode byte is 0 or 7), and the
  cancel-held skip path ending the screen in BGM focus (the pad wrapper stops after four reads without the
  selector, so the page gets up/down back either way).
- macOS and Linux builds of the framework changes.

## 11. The track wheel and the moved pages (19 October 2026)

The owner's mock-up of the right half: the BATTLE BGM box shows the **series**, a track list under it always shows
the series' tracks as a **wheel**, and the screen's own Equipment heading and panel move down and get shorter. Built
as drawn. The static mock-up made first (a real capture plus the planned overlay, native and 2x) is
`build/port/playlist/wheel_mockup.png`; the captures in 11.5 match it.

### 11.1 Layout

All positions in the PSP's pixels (480x272). The overlay scales them to where the game's picture is drawn
(`ui::game_picture`), so it follows any window, aspect or internal resolution; font sizes are not settable yet.

- **The box** stays where the game has it. Its text is now the series and the track's place in it,
  `FINAL FANTASY I · 1/3` (the middle dot is U+00B7, which the game's font draws), through the game's **own** name
  sprite, so it has the game's face, colour and fades; in BGM focus it turns rust red as before. Its ◀ ▶ arrows are
  the previous/next series in BGM focus. How: every `name[]` pointer of the selector points at one string in the
  selector's own memory (the last 25 words of `entry[]`, past the choices: 49 units), rewritten before the name
  sprite is made again on each change. The build shows the default choice right after the list returns, so the list
  already writes the text of the choice whose id is the screen's default (screen `+0xF60`, the last match, else the
  first). With more than 231 choices there is no room and the box shows the track, as before (logged).
- **The track list**, a box at (247, 27)-(460, 86), the width of the pages' frames: five rows 10 apart, the chosen
  track on a highlight bar in the middle row (y 53), its neighbours in the series above and below, dimmer and
  smaller further out (8.5 then 7.5 px). A series with fewer tracks shows each once: one track the middle row only,
  two the middle and the next, three ±1, four −1 to +2. Up/down wrap from last to first; a step of one rolls the rows
  (an ease of about 60 ms; any other change shows the new place at once). A small key line at the bottom right:
  `L2/R2 BGM` in menu focus, `◀▶ Series ▲▼ Track L2/R2 Done` in BGM focus. Style after the game's frames (lavender,
  darker to the left, a light line, the box's white-to-pale-blue bar), in the face the game's text is drawn with.
- **Titles**: a game track's name without its series prefix (`FFI - Battle` is `Battle` under FINAL FANTASY I; names
  without an FF prefix are shown whole), a mod track's own title (UTF-8: any character the face has). Too long a
  title is **cut with an ellipsis**, not scrolled: the list already rolls vertically, a second motion in the same box
  would pull the eye, and a cut title reads in place. With the prefixes gone only long mod titles are ever cut.
- **The pages** (the right half has three, switched with L/R: Equipment, Accessories, Battlegen):

| | Was | Now |
| --- | --- | --- |
| Headings' tops | 23 (Accessories, Battlegen), 48 (Equipment) | 90 for all three |
| Frames | 41..248 and 64..242 | 108..248 and 106..242 (the bottoms stay) |
| Accessories, Battlegen rows | from 54, 20 apart, 10 shown | from 121, 20 apart, **6 shown**, the game's scrolling |
| Equipment rows (4 slots and 2 extras, no scrolling) | from 80, 25 apart (the extras a little more) | from 122, about 20.5 apart |
| Scroll arrows (up, down) | 39, 249 | 106, 249 |
| L/R page arrows | 130 | 172 |

The left half, the NEXT button and the BATTLE BGM box are untouched. The hand cursor follows its rows by itself.

### 11.2 How the pages are moved

**Where the screen places them** (battle overlay; read from the code and dumped from memory at run time with
`D012_TRACE_BGM_LAYOUT=1`):

- The loading screen's init (battle `0x08AF8F58`) makes nine layout elements from one layout (`*(screen[0x3CC] + 4)`):
  element 0 is the screen (animations 17 then 18), 4 the hand cursor, 5 and 6 the L/R page arrows (animations 22,
  24), 7 and 8 the scroll arrows (26 up, 20 down). Each page is an object with its own element of the same layout:
  Equipment at screen `+0x50` (vtable `0x08AFB5A0`), Accessories `+0xB0` (`0x08AFB5E8`), Battlegen `+0x144`
  (`0x08AFB630`); vtable slot `+0x14` plays the page's rest animation, `+0x1C` places its rows, `+0x24`..`+0x3C`
  the slides, `+0x44` the cursor. A page's first words are `{visible rows, count, cursor, top}`; its scroll (battle
  `0x08AF3E60`) and the scroll arrows use `visible`.
- **The layout system** (main): an element is `+0x00` frame, `+0x0C` animation, `+0x10`/`+0x14` an offset,
  `+0x18`/`+0x1C` a scale, `+0x20` flags, `+0x24` the loaded layout, `+0x28` a colour. The layout's data has
  pictures (`+0x00`: a UV rectangle and four vertex colours), parts (`+0x04`: quads of 10 bytes, a flag byte, the
  picture and a rectangle relative to the node), and animations (`+0x0C`, 0x1C bytes each: nodes of 8 bytes, the
  part (255 = an **anchor**, a point the game places its own sprites at; at or above the part count, a nested
  animation), the keyframe count and an offset table). A keyframe is fixed (flag `0x40`: its place at `+0x0C`) or
  moves from `+0x18` to `+0x20` (with the place last computed at `+0x0C` and its time at `+0x08`). Anchor n is node
  (count − 1 − n): main `0x088DA0A8`; drawing main `0x088D16F4` with the node walk `0x088DBE64`.
- Each page's animations hold its **heading** (a picture), the heading's line (part 11), its **frame** (part 8 for
  Equipment, part 7 for the other two: gradient quads, no texture) and its row **anchors** (7, 10 and 20). The rows
  are the game's text sprites placed at the anchors each frame; the cursor goes to the chosen row's anchor (main
  `0x088D9C60`). Equipment is animations 12 to 16 (16 at rest), Accessories 2 to 6, Battlegen 7 to 11. The page
  arrows are placed at anchors 3 and 2 of the screen's animations 17/18 (battle `0x08AF64E4`); the scroll arrows sit
  where their animations put them.

**The change is data, not code**: no function is decompiled or replaced for it. `port/host/d012_loading_layout.cpp`
changes the layout as it is in memory, once per screen, from the selector's build (the list override: every element
and page exists by then and nothing has been drawn): the pages' animations (all their keyframes, the slides
included), the up arrow's, the page arrows' two anchors and the two frames' lower quads, **436 values**. Every value
is checked against what it must be before anything is written; on any other value nothing is written and the
pages stay (logged), and a layout already moved is left alone. Moving keyframes have their cached time set to NaN so
that the game computes their place again. The pages that scroll get `visible = 6`, and each frame, after the
pages placed their rows, the selector's update hides the rows below the sixth (main `0x0887B944`), because the
game's row loop draws ten from the top whatever `visible` says. Equipment has no scrolling in the game (fixed anchors
for at most six rows), so its rows are drawn closer instead.

The elements' own offsets (`+0x10`) were the other candidate: they move a whole element and its anchors, but the
pages' elements draw with flag `0x800`, which passes the offset through a matrix (main `0x089D5EC0`), and the frames'
height needs the data anyway.

**Fallback, designed but not exercised:** on an unexpected layout the pages stay where they are and the track list
is drawn over the top of them.

### 11.3 Controls

As built (10.1). L2/R2 switch the focus. **BGM focus**: left/right the series (the box), up/down roll the wheel, and
the list is lit (a gold outline, a bright bar, the title in the game's selected blue). **Menu focus**: the game's
controls; the list stays, dimmed; left/right step through every track as one list, series by series, and the box
and the wheel follow. After confirm the list stays, dimmed, while the screen fades.

### 11.4 Switches

| Variable | What it does |
| --- | --- |
| `D012_NO_BGM_PLAYLIST=1` | The game's selector **and layout**: nothing of this section. V: the three pages captured with it are pixel-identical to the previous build's (outside the random BGM name) |
| `D012_BGM_PLAYLIST_FLAT=1` | The one-line selector of section 10 (the track in the box, the series on a line under it), the pages where the game has them |
| `D012_TEST_BGM_UNLOCK_ALL=1` | Test only: the selector offers every row with a weight, locked or not (five tracks a series; the test save has three at most). Nothing is written to the save |
| `D012_TEST_OVERLAY_FIXED_STEP=1` | Test only: the wheel's roll advances by 1/60 s per drawn frame instead of by the clock, so frame captures (which pause the game while saving) show the motion. Used for the README animation |
| `D012_TRACE_BGM_LAYOUT=1` | Logs the screen's elements, the layout's animations, parts, pictures and anchors, 60 frames into the screen |
| `D012_TRACE_BGM_NAME=1` | Logs the box's name sprite and the text system's live sprites and text blocks at each change and for three frames after it (11.7) |

### 11.5 Evidence (V)

Recompiled build, `PSPRECOMP_NO_INTERPRETER=1` in every run (no fallback), `D012_UNTHROTTLED=1`, the Reproduce
route, data in `build/port/data-wheel-test` (a copy of `build/port/data` plus the test mods of
`build/port/data-mods-test/mods`). Runs in `build/port/playlist/wheel/<run>/`, captures copied to
`build/port/playlist/`:

- Layout: `[playlist] the loading screen's pages moved down for the track list: 436 of 436 layout values changed`.
  Captures `wheel_menu_focus.png`, `wheel_page_accessories.png`, `wheel_page_battlegen.png`: headings, frames,
  rows, cursor and arrows where 11.1 says. `wheel_cursor_down.png`: D-pad down in menu focus moves the hand to the
  second row, on it. The left half and the NEXT button of a wheel capture are pixel-identical to the previous build's
  (same opponent).
- BGM focus: `wheel_bgm_long_series.png` (FINAL FANTASY I 3/5, five rows, with `D012_TEST_BGM_UNLOCK_ALL`),
  `wheel_bgm_mod_series.png` (TEST TONES 2/3), `wheel_bgm_mod_one_track.png` (TEST SWEEPS 1/1, one row),
  `wheel_wraparound.png` (FORMATS 1/6, up: 6/6), `wheel_wraparound_game.png` (FINAL FANTASY I 1/5, up: 5/5);
  `wheel_menu_flat_step.png`: right in menu focus from FORMATS 6/6 goes to TEST SWEEPS 1/1, the wheel following.
- Scale: `wheel_2x_bgm.png`, `wheel_2x_menu.png`, a 1920x1088 window (internal scale 4): the list and the moved pages
  scale with the picture.
- Playback: `final_verify`, FINAL FANTASY II chosen in BGM focus with right, down, up, up (a wrap to 3/3), then
  confirm: `open DISC0:/PSP_GAME/USRDIR/DATA/BGM/U_02PANDEMO.AT3`. `modtrack`: TEST TONES 1/3 chosen in BGM focus
  and confirmed in it: the host serves `MD7D9EC9A2.AT3` (74,100 bytes), and a Goertzel scan of the `D012_AUDIO_DUMP`
  has 440 Hz for 2 s, then 660 and 880 Hz alternating each second to the end, never 440 again.
- Differential test, `final_verify` with `D012_DECOMP_BGM=1 D012_DECOMP_VERIFY=1`: **1,999 comparisons, all SAME**
  (list_offered 1, selector_update 608, selector_chosen 609, selector_make_name 608, table_name 142, table_find 9,
  table_find_row 6, table_name_of 6, stream_path 7, play_id 1, play_entry 1, stream_start 1). The originals are
  compared from the live state, which has the moved layout and the names pointing at the box's text. The layout
  edit itself replaces no function, so the harness has nothing of it to compare; the unchanged parts were compared
  by pixels instead (above, and the off switch in 11.4).
- Switches: `off` (`wheel_off_switch.png`, the game's screen; `1` does nothing), `flat` (`wheel_flat_switch.png`).

### 11.6 Not verified

- **Scrolling past six rows** on Accessories and Battlegen, and so the hiding of the rows below the sixth: the test
  save has one row on each, and the route's CPU settings did not give more. Equipment's six rows at the closer
  spacing were not seen either (the save shows two). The scroll itself is the game's own, with `visible = 6`.
- The fallback on an unexpected layout (by code only). Other languages' layouts and the European disc.
- A real gamepad, fullscreen and the Fill aspect.

### 11.7 The box's blank frames at each change (fixed 19 October 2026)

**Cause** (V: `D012_TRACE_BGM_NAME`, run `build/port/playlist/flicker/trace1`). At a change the update unlinks the old
name sprite, gives it the end callback battle `0x08AF5C6C`, hides it and deletes it (main `0x0887BA5C`: the sprite
goes back to the text system's pool at once, its text block's count 1 → 0), and sets the selector's sprite to 0. The
callback belongs to the old sprite's **text block** (sprite `+0x00`: the glyph buffer, `+0x14` callback, `+0x28` count),
and the text system's frame (main `0x0887DBD4`, once a game frame) frees a block two passes after its count
reaches 0: 0 → −1 at the first, and at the second (the GPU no longer reads it) it frees the buffer (main
`0x0887D828`), returns the block and runs the callback, which makes the new sprite (main `0x0887D868`). So two
updates draw no name: the one with the change and the next; the third draws the new one. A new sprite draws in the
frame it is made (its glyphs are built in `0x0887D868`). The game has the same gap (`D012_NO_BGM_PLAYLIST=1`, run
`off1`).

**Fix** (`swap_name` in `d012_bgm_playlist.cpp`, the playlist's update and every change of choice): the new sprite is
made first (the decompiled `bgm_selector_make_name`, with the old sprite set aside), then the old one is unlinked,
hidden and deleted in the same update, without the callback. Still one text sprite made and one deleted per change;
the old block is freed by the text system two frames later as before. If the text system cannot make a sprite now, it
falls back to the game's way (logged). The decompiled originals and `D012_NO_BGM_PLAYLIST=1` are
unchanged.

**Evidence** (V; recompiled build, `PSPRECOMP_NO_INTERPRETER=1`, data `build/port/data-readme-shots`, the README GIF's
route, runs in `build/port/playlist/flicker/<run>/`):

- `build/port/playlist/flicker_before.png` (the README GIF's captures): two blank captures after each of the 9 steps,
  18 of 99. `flicker_after.png` (run `after1`, the same route, a capture of every frame round each step): the new
  name in the next capture, **0 blank of 126**. `flat1` (`D012_BGM_PLAYLIST_FLAT=1`): 0 of 24.
- **No leak** (`leak1`): D-pad down and right held together for 400 frames in BGM focus, then left for 300 in menu
  focus: 431 changes, 180 of them on consecutive updates; the text system's live sprites 80 at every change and
  after, its live text blocks 81-82 while changing and back to 80 two updates after the last change.
- `verify1` (`D012_DECOMP_BGM=1 D012_DECOMP_VERIFY=1`): series and track changes in BGM focus, left/right in menu
  focus, FINAL FANTASY VIII 5/5 confirmed: opens `U_08PREMONI.AT3`; 1,857 comparisons, all SAME. `modtrack1`: eight
  series right, one left, one right, down, confirmed in BGM focus: the host serves `MDE64FADBE.AT3`
  (`ff-battle-collection:veiled-in-black`); 1,816 comparisons (`table_name` 139 among them), all SAME.

## Reproduce

```powershell
.\tools\port\pk_cmake.cmd -S port -B tools/local/port/build-d012-recomp -G Ninja `
    "-DD012_PORTABLEKIT_DIR=$PWD/tools/local/port/PortableKit" "-DD012_WORK_DIR=$PWD/build/port/recomp" `
    -DPSPRECOMP_GENERATED_JOBS=4
.\tools\port\pk_cmake.cmd --build tools/local/port/build-d012-recomp -j3
Copy-Item -Recurse build/port/data build/port/data-bgm-test   # once
$route = "460:pad a;1080:pad a;1500:pad a;1650:pad a;1900:pad dpdown;2000:pad a;2400:pad a;3200:pad a;" +
         "3700:pad a;4400:pad a;4900:pad a;5600:pad dpdown;5750:pad dpright;5900:pad dpup;6050:pad a;7100:pad a;" +
         "7300:pad dpright;7400:pad dpright;7500:pad dpleft;7600:pad dpleft;7700:pad dpleft;8000:pad a;" +
         "8020:speed normal;8400:quit"
.\.venv\Scripts\python.exe tools/port/run_d012.py --seconds 90 --build-dir tools/local/port/build-d012-recomp `
    --data-dir build/port/data-bgm-test --log build/port/bgm/logs/verify.log --env PSPRECOMP_NO_INTERPRETER=1 `
    --env D012_UNTHROTTLED=1 --env D012_TRACE_IO=1 --env D012_DECOMP_BGM=1 --env D012_DECOMP_VERIFY=1 `
    --env D012_DECOMP_VERIFY_LIMIT=1000 --env "D012_INPUT_SCRIPT=$route"
Select-String -Path build/port/bgm/logs/verify.log -Pattern "DIFFERENT|INCONCLUSIVE"   # nothing
```

Drop the two `D012_DECOMP_*VERIFY*` variables for a normal run with the decompiled code, and `D012_DECOMP_BGM` for
the original. That route's presses on the loading screen are menu focus's; the playlist is on in every run unless
`D012_NO_BGM_PLAYLIST=1`.

The playlist's runs (section 10.6), with `build/port/data-playlist-test` made as `data-bgm-test` plus the test
mods (`Copy-Item -Recurse build/port/data-mods-test/mods build/port/data-playlist-test/`): the same prefix to
frame 7100, then for example

```powershell
$steps = "7250:shot menu_focus;7300:axis lefttrigger 1;7320:axis lefttrigger 0;7370:shot bgm_focus;" +
         "7400:pad dpleft;7450:pad dpleft;7500:pad dpleft;7550:pad dpleft;7600:pad dpdown;" +
         "7660:shot bgm_focus_mod_series;7700:pad a;7720:speed normal;8800:quit"
# --env D012_TEST_BGM_START=0 --env D012_SCREENSHOT_DIR=... --env D012_AUDIO_DUMP=... --env D012_TRACE_IO=1
```

`key 1` and `key 3` press L2 and R2 on the keyboard instead of the triggers. The wheel's runs (section 11.5) use the
same prefix on `build/port/data-wheel-test`; `pad rightshoulder` turns the info page, and
`--env D012_TEST_BGM_UNLOCK_ALL=1` gives five-track series. The table dumps used above come from `general_archive/main/main.bin` and `EN/main_lang.bin` with
`build/scratch/assets/pkg.py`; they are not kept in the repository.

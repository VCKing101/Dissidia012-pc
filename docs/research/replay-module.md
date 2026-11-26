# Dissidia 012 (ULUS10566 v1.00) - battle replay module map

> 26 November 2026. Static analysis of the retail main, battle and menu code (`tools/dc.py context`, raw instruction
> scans of all four modules), checked against the four replays in the port's test data
> (`build/port/data-analog-replay`, copied to `build/scratch/replay/data`), decrypted offline with PortableKit's own
> save crypto (section 5). Nothing in the port, PortableKit or the analog-battle worktree was changed; the game was
> not run for this report.
>
> "V:" = seen in the code or in the decrypted files (the evidence is given). "I:" = inferred.
> The layouts are in [`include/main/replay_types.h`](../../include/main/replay_types.h) (`D012Replay`,
> `D012ReplayCamera`, `D012ReplaySave`), with the same marks. Pad terms are those of [pad-module.md](pad-module.md).
> FIELD is the battle manager object `*(0x089D6A10)`; its mode is `FIELD+0x4C`.
>
> One run on a copy of the test data checked section 5 (26 November, 23:27-23:33): replay 03 loaded, saved twice with
> no edit, then once after Camera Edit's Set Camera Data; the three new files decrypted and compared
> (`build/scratch/replay/`, shots `t08`-`t27`, log `replay1.err`).

## 0. Headline findings

1. V: **REPLAY.BIN has its own save key**, `80176be7efa35ae68c9d0d65571d6891`, not the game data's `5a802015…`.
   It is the 16 bytes at main `0x089CB708`, which the save set-up `0x088B0AF8` stores at save object `+0x2B94` (the
   field the matched source called `detail`); `0x088B04AC` copies it into the savedata parameters' key at `+0xDC0`
   (`params+0x5DC`). The earlier trace logs show it on every replay request
   (`build/port/analog-battle/logs/no_replay_bin_load.log`: `LISTLOAD … file="REPLAY.BIN" … key=80176be7…`).
   Decrypting the four test replays with it gives files whose PSP hash matches their `PARAM.SFO` file list exactly.
2. V: **The first 0xE8 bytes are a header and padding, not a date or checksum**: word 0 = 1, word 1 = 0xBEA0, then
   0xE0 zero bytes. The generic save code writes the descriptor's "enabled" and "size" there before every save
   (`0x088AEFA0`) and checks both after every load (`0x088AEF54`); nothing writes the rest. All four files are
   identical there.
3. V: **The plaintext is the replay object verbatim**: file offset = replay offset + 0xE8, for replay `+0x0000` to
   `+0xBDB8`. Replay `+0x0000` is a format word **0x20081106** (written by the clear `0x088C82E0`), not "a word of the
   battle". The last 0x18 bytes are replay `+0xBDA0..+0xBDB8`: two unknowns, the camera entry count, the **total
   frames recorded**, a flag byte and the player's side. V (run): saving a loaded replay again writes the same
   plaintext byte for byte; Camera Edit changes only the camera table and its count (section 5).
4. V: **There is no checksum anywhere in the game's own code.** The only integrity checks are the PSP save
   utility's (encryption and the hash in `PARAM.SFO`) and the two header words.
5. V: **The clock.** Recording: replay `+0xBDAC` counts every recorded frame; in all four files it equals the sum of
   each fighter's runs exactly. Playback: replay `+0xBDE4` counts every played frame. Per fighter the player keeps
   the mask being played (`+0xBDCC`), the frames left in its run (`+0xBDD4`) and the next record (`+0xBDDC`).
6. V: **Order in a frame**: the pad poll (`PAD_MANAGER::ReadTask`, system list priority 15000) runs before the FIELD
   list (70000); in the FIELD list `ExecPadInitByChangeTask` (100) runs, then `FIELD::ReplayRecordAndPlayTask`
   (300, filter 3), then the fighters' updates. So the recorder reads this frame's poll, and the player injects this
   frame's input after the real poll and before anything uses it.
7. V: **The load copies the file into the one replay object** (main `0x08889BAC` → `0x088C96E8`) and sets FIELD mode
   9. The battle start that the mode change triggers wipes the object first, and the menu copies the loaded data in
   afterwards (section 6).
8. I: **Replays travel no other way.** No code outside the battle ending, the replay menus and the replay module
   touches the replay object or `REPLAY.BIN` (section 7). Communication Mode (FIELD mode 5) records and offers a
   replay like any battle, saved locally.

## 1. Functions

| Address | Size | Name here | Role | C (26-27 Nov) |
| --- | ---: | --- | --- | --- |
| `0x0888948C` | 448 | `replay_task` | "FIELD::ReplayRecordAndPlayTask" (name at `0x089CA4DC`), registered by `0x0888C4C8` at `0x0888DDB8` (FIELD list, filter 3, priority 0x12C) | yes |
| `0x088CA680` | 808 | `replay_record_frame` | the recorder, once a frame unless mode 9 | yes |
| `0x088CA9A8` | 872 | `replay_play_frame` | the player, once a frame in mode 9 | yes |
| `0x088CA564` | 244 | `replay_write_record` | writes one record (a finished run) | yes |
| `0x088CA664` | 28 | `code_split` | splits a 24-bit record into mask (>> 8) and run (& 0xFF) | yes |
| `0x088CA658` | 12 | (auto) | `run + mask * 0x100`: builds a record | yes |
| `0x088C8434` | 48 | `store_last` | the next camera entry: `199 - camera_count`, or -1 when full | yes |
| `0x088CA528` | 24 | (inside the inventory's `0x088CA4EC`) | `!(flags & 1)`: the replay is not full | yes |
| `0x088CA510` | 24 | (inside `0x088CA4EC`) | flips the byte at `+0xBDB0` | yes |
| `0x088C82E0` | 284 | `replay_clear` | magic 0x20081106; counts, records (0x7E90 bytes), 200 camera entries and `+0xBDA0..+0xBDB8` zeroed | yes |
| `0x088C829C` | 68 | `replay_construct` | builds the two 0x1060-byte objects at `+0x4`, then clears | yes |
| `0x088C9564` | 60 | `replay_static_init` | constructor of the static object `0x08B99030` (guard `0x08B9902C`); also resets | yes |
| `0x088C95A0` | 188 | `replay_reset` | state 0, clear, cursors, `+0xBDE4` and flags zeroed | yes |
| `0x088C965C` | 140 | `replay_rewind` | state 0, cursors and `+0xBDE4` zeroed; keeps the data | yes |
| `0x088C96E8` | 520 | `replay_copy` | replay = replay (the load path), then `0x088FB028(+4)` | yes |
| `0x088C98F0` | 28 | `replay_apply_setup` | `0x088FB028(replay+4)` (I: applies the stored set-up to the battle) | yes |
| `0x088C990C` | 28 | `replay_store_setup` | `0x088FB1A4(replay+4, FIELD+0x804)` (I: stores the set-up and music) | yes |
| `0x088C9928` | 16 | (auto) | `+0xBDB4` = FIELD `+0x54` (the player's side) | yes |
| `0x088C93AC` | 224 | `replay_unedited_length` | `+0xBDAC` less the Camera Edit spans, at least 0 | yes |
| `0x088C948C` | 216 | `replay_in_edit_span` | 1 if a frame lies inside a Camera Edit span | yes |
| `0x088C913C` | 176 | `count_state1` | the number of span starts | yes |
| `0x088C9004` | 64 | `store_extra_index` | span entry n's table slot: `199 - camera_count - n` | yes |
| `0x088CA1A4` / `0x088CA42C` | 156 / 156 | | the camera data entry the play frame lies in (identical twins) | near (14 words) |
| `0x088AEF54` | 68 | `save_header_check` | after a load, the buffer's first two words against the descriptor (flag 4) | yes |
| `0x088C91EC` / `0x088C92CC` | 224 / 224 | `replay_edit_span_start` / `_end` | the n-th entry of kind 1 / 2 | yes |
| `0x088C9938` | 172 | `replay_stop` | state 1: both runs in progress written, state 3; state 2: state 4, the pad manager's replay flag cleared | yes |
| `0x088CA540` | 36 | (auto) | camera data at the play frame: `0x088C948C(replay, +0xBDE4)` | yes |
| `0x08889BAC` | 132 | `field_start_replay` | the load hand-off: pads cleared, `0x088C96E8(FIELD+0x7C8, data)`, flags bit 1 from its third argument, FIELD mode 9 | yes |
| `0x08889694` | 372 | `field_battle_start` | per battle: in mode 9 `0x088C95A0` or `0x088C965C`+`0x088C98F0`; otherwise `0x088C95A0`, `0x088C9928` | yes |
| `0x088892C8` / `0x088892E8` | 32 / 384 | `ExecPadInitByChangeTask` | outside mode 9, resets a fighter's pad edges when its pad's state changes (the source of bit 15) | yes |
| `0x0888AD50` | 708 | `replay_save_copy` | allocates the 60,000-byte save object (FIELD `+0x7CC`), copies the replay into its buffer, starts the save | yes |
| `0x088B0AF8` | 176 | `replay_save_init` | the replay's save descriptor | yes |
| `0x088B0BA8` | 96 | | stores the detail text's six words at save `+0xEA48`, then `0x088AFB08` | yes |
| `0x088B0CAC` → `0x088B0C08` | 28 / 192 | | the save's callback: titles and detail text (`0x088ADE90`, `0x088B0CC8`, `0x088ADF28`), then `0x088AFB30` | auto |
| `0x088ADE90` | 152 | `stamp` | date into the save's name (`+0x864`) | yes |
| `0x088ADF28` | 64 | `save_text_copy` | copies UI text 0x13 into the parameters' second title (`+0x8E4`) | yes |
| `0x088AFB30` | 52 | `save_manager_start` | the save manager starts its request: flags 0 and 2 cleared, then `0x088AF134` | yes |
| `0x088B0CC8` | 908 | `replay_detail_text` | builds the save's detail text at `+0x864 + 0x100` (0x400 bytes): the title if given, both fighters' names, the stage, the length; nothing if the six words are out of range | yes |
| `0x08872304` | 404 | `RawPad_inject` | buttons in, stick bytes rebuilt, a poll | near (11 words) |
| `0x08872970` | | `LogicalPad_injectAndUpdate` | called by the player per fighter | |

Camera Edit and camera data (main), all matched on 26-27 November:

| Address | Size | Name here | Role |
| --- | ---: | --- | --- |
| `0x088C9F2C` | 188 | `replay_set_camera_data` | Set Camera Data: overwrites the entry at the cursor (keeping its frame) or adds one |
| `0x088C9AEC` | 1088 | `replay_add_camera_data` | inserts camera data at the play frame in frame order (near: 5 words) |
| `0x088C9FE8` | 412 | `replay_remove_camera_data` | removes the entry in force at the play frame; a new first entry gets frame 0 |
| `0x088C854C` | 904 | `replay_normalize_span` | span edit at a frame: the range from the last open entry is bounded by a start and an end marker |
| `0x088C88D4` | 700 | `replay_remove_span` | removes the span edit at a frame |
| `0x088C8B90` / `0x088C8CC0` / `0x088C8DE8` | 304 / 296 / 380 | `replay_insert_span`, `replay_remove_span` (by index), `replay_insert_span_at` | span table helpers |

Battle and menu callers matched on 26-27 November (the brief's "if time is left"):

| Module | Address | Size | Name | Role |
| --- | --- | ---: | --- | --- |
| battle | `0x08B07B94` | 512 | `ending_update` | Save Replay on square (or the request byte) and the save's result |
| battle | `0x08B07974` | 360 | `ending_dispatch` | the ending's page update by battle mode (jump table) |
| battle | `0x08B07500` | 676 | `ending_pad_step` | the ending's pad step; page 0x11 for a replay with camera data |
| battle | `0x08B2337C` | 340 | `replay_camera_pick` | the camera data around the play frame for the replay camera |
| battle | `0x08B274AC` | 844 | `replay_cam_edit_input` | Camera Edit's input: set, remove and add camera data, span edits |
| battle | `0x08B2721C` | 656 | `replay_pause_step` | pausing and resuming a replay |
| battle | `0x08B2BEDC` | 580 | `replay_cam_label` | Camera Edit's "No.%03d" label |
| battle | `0x08B2C9D4` | 492 | `replay_cam_counter` | the pause screen's free camera slots (0-999) |
| battle | `0x08B2CF9C` | 740 | `replay_menu_step` | the save/load prompt's state machine |
| battle | `0x08ADDCF8` | 400 | `effect_slot_start` | an effect slot's start (calls main `0x088CAF10`) |
| battle | `0x08B1F538` | 1140 | (scratch) | the playback HUD's input step. The source matches over 1,140 bytes, but `config/inventory/battle.json` gives the function 972 bytes in two ranges and leaves out its case bodies `0x08B1F8D0..0x08B1F978`; integrating it needs that entry fixed |
| menu | `0x08A44504` | 1244 | `replay_menu_step` | a replay menu's step: builds the REPLAY.BIN save object, starts LISTLOAD or SIZES |
| menu | `0x08A40C98` | 620 | `replay_menu_exit` | hand-off into playback (`+0x1296 == 2` gives flags bit 1) |
| menu | `0x08A499A0` | 444 | `replay_list_exit` | hand-off into playback (state 5 gives flags bit 1; jump table) |
| menu | `0x08A505C0` | 144 | `field_mode12_battle_start` | starts a FIELD mode 12 battle |
| menu | `0x08A49D7C` | 148 | | the load set-up (near: 19 words) |
| menu | `0x08A49750` | 592 | | the Creation replay screen's enter (near: 7 words) |

## 2. The replay object (question 1)

V: one static object at `0x08B99030`, pointer at `0x089D7750`, copied into FIELD `+0x7C8` by the FIELD constructor
`0x088881FC`. It lives for the whole session; every battle resets it (`0x08889694`).

| Offset | Size | Field | Notes |
| ---: | ---: | --- | --- |
| `+0x0000` | 4 | magic | V: 0x20081106 (clear), in all four files |
| `+0x0004` | 0x20C0 | `sides[2]` | V: two objects of 0x1060 bytes (`0x08993560(+4, 2, 0x1060, ctor)`). I: the two per-side battle set-ups (main `0x09AFB8A0 + side*0x1060`, roster-and-save.md 1.7), stored by `0x088C990C` and applied by `0x088C98F0` |
| `+0x20C4` | 0x220 | | I: the rest of the set-up (rules: the files hold the same 100s, 0x7F pairs and 0xFF runs) |
| `+0x22E4` | 4 | `bgm` | V: the music (`FIELD+0x804` in playback, docs/port/bgm-playlist-design.md); 83, 83, 84, 83 in the files |
| `+0x22E8` | 2 × 0x3F48 | `records[2]` | V: 5,400 records of 3 bytes a fighter |
| `+0xA178` | 2 × 4 | `record_count[2]` | V |
| `+0xA180` | 200 × 0x24 | `camera[200]` | V: section 3.4 |
| `+0xBDA0` | 4 | `camera_data_count` | V (run): Camera Edit's entries, filled from the table's front; Set Camera Data made entry 0 and set this to 1. `0x088CA1A4`/`0x088CA42C` find the entry the play frame is in. 0 in the four files |
| `+0xBDA4` | 4 | `camera_count` | V |
| `+0xBDA8` | 4 | `span_count` | V: entries of kind 1 and 2 below the recorded events (`0x088C9004`: slot `199 - camera_count - n`); `0x088C913C`, `0x088C91EC`, `0x088C92CC` loop to it. I: the camera spans of section 3.4; not made by Set Camera Data. 0 in every file seen |
| `+0xBDAC` | 4 | `frame_total` | V: the recorder's clock |
| `+0xBDB0` | 1 | | V: flipped by `0x088CA510`; 0 in the files |
| `+0xBDB4` | 4 | `player_side` | V: FIELD `+0x54` at a recorded battle's start; 0 in the files |
| `+0xBDB8` | 4 | `state` | V: section 3.1. Not saved: the file ends here |
| `+0xBDBC` | 2 × 4 | `rec_last[2]` | V: the mask of the run being counted |
| `+0xBDC4` | 2 × 4 | `rec_run[2]` | V: its length, 1 to 255 |
| `+0xBDCC` | 2 × 4 | `play_mask[2]` | V: the mask being played |
| `+0xBDD4` | 2 × 4 | `play_left[2]` | V: frames left in that run |
| `+0xBDDC` | 2 × 4 | `play_index[2]` | V: the next record to read |
| `+0xBDE4` | 4 | `play_frame` | V: **the player's frame count**, +1 a played frame; the camera data is looked up by it |
| `+0xBDE8` | 4 | `flags` | V: bit 0 full, bit 1 camera playback (section 3.5) |

So for question 1: the frame count the track's clock should read is `+0xBDE4` in playback and `+0xBDAC` while
recording. They count the same frames: both start at 0 on the task's first call in the battle and add 1 per call.

## 3. Recording and playback (question 2)

### 3.1 The task, a frame

V, `0x0888948C`: only while FIELD `+0x48` (the battle phase) is 2, 3 or 4. It takes each fighter's pad
(`FIELD+0x38[i] -> +0x5A0`). In mode 9 it runs the player; otherwise the recorder, with:

- per fighter, a "pad changed" byte: 1 when the pad map set's kind changed this frame (`FIELD+0x7B8` against
  `+0x7BC`, by `0x08889468`: kinds 1, 2 and 0x10 against the rest) or the fighter's pad state changed (`FIELD+0x7C0[i]`
  against `+0x7C2[i]`). These are the values `ExecPadInitByChangeTask` (priority 100) set this frame, and in that
  case it has just reset that pad's edges (`0x08871BC4`, `0x08871BD0`);
- a "camera event" bit: `0x0888B0A4()` differs from last frame's (`FIELD+0x7C4`). I: `0x0888B0A4` is 1 while a
  fighter's action or the battle's state asks for a special camera (it scans both fighters' `+0x18 -> +0x2A8`, and in
  phase 3 battle `0x08A15A70`), so an entry marks each start and end of such a stretch (the files show pairs 32 frames
  apart).

States (`+0xBDB8`), V: 0 starts (recorder: set 1 and clear the clock; player: set 2); 1 records; 2 plays; 3
recording stopped; 4 playback stopped (the player sets 5); 5 and 6: the player does nothing; negative: nothing.
The stop is `0x088C9938` (`replay_stop`, called from main `0x08888CEC`, `0x0888C048` and `0x088C9A54`): in state 1
it writes both fighters' runs in progress and sets 3; in state 2 it sets 4 and clears the pad manager's replay flag.

### 3.2 The recorder

V, `0x088CA680(replay, pads[2], changed[2], camera_event)`, only in states 0 and 1:

1. State 0: state = 1, clock (`+0xBDAC`) = 0.
2. If either fighter's count has reached 5,400: flags |= 1.
3. For each fighter with a pad: mask = 0. If the pad is active (its vtable call at `+0x30/+0x34` returns 1), each of
   the 15 entries of the table at `0x089D7758` is tested on the pad's raw pad (`pad+0x40`, vtable `+0x8/+0xC`, the
   "held" test), giving bits 0 to 14. Bit 15 = the fighter's "pad changed" byte.
4. If mask equals `rec_last` and `rec_run` < 255: run + 1. Otherwise the finished run is written
   (`0x088CA564`), then `rec_last` = mask, `rec_run` = 1.
5. If the camera event bit is set: a camera entry at index `199 - camera_count` with zero vectors, the clock as its
   frame and zero bytes; `camera_count` + 1. If the index is not below 200: flags |= 1.
6. Clock + 1.

The table (V, `{lo, hi}`): bit 0 `{0x10,0}` UP, 1 `{0x40,0}` DOWN, 2 `{0x80,0}` LEFT, 3 `{0x20,0}` RIGHT, 4 `{0,4}`,
5 `{0,0x10}`, 6 `{0,0x20}`, 7 `{0,8}` (the four analog directions), 8 `{0x2000,0}` CIRCLE, 9 `{0x4000,0}` CROSS, 10
`{0x8000,0}` SQUARE, 11 `{0x1000,0}` TRIANGLE, 12 `{0x100,0}` L, 13 `{0x200,0}` R, 14 `{1,0}` SELECT. START is not
recorded (pad-module.md section 6 lists the same).

**A record** (V, `0x088CA564`): `code = run + mask * 0x100` (24 bits), stored big-endian at
`replay + 0x22E8 + fighter*0x3F48 + count*3`; count + 1. A run of 0 is not written. In the files the records read
`(0x0000, 172) (0x0080, 60) (0x0010, 60) …`: the scripted walks.

**When the slots fill** (V): a fighter's 5,401st record is dropped and flags |= 1; the frame is still counted and
the other fighter goes on recording, so the two parts no longer cover the same frames. A 201st camera event sets
the same flag. The ending then does not offer the save as usual (`0x088CA528` returns 0, and the offer function
passes 1 instead of 0 to battle `0x08B05040`; I: the "Save Replay" item shown unavailable). At 60 frames a second
and runs of up to 255 frames, 5,400 records last at least 90 seconds of constantly changing input and up to 382
minutes of held input.

**The last run.** V: the recorder writes a run only when the mask changes, so the run in progress is written by
the stop `0x088C9938` (section 3.1). In all four files each fighter's runs sum to the clock exactly (5,592, 2,849,
3,957, 3,415).

### 3.3 The player

V, `0x088CA9A8(replay, pads[2])`:

- State 0: state = 2; the pad manager's flag bit 0 (`*(0x089D6008)+0x264`, "replay playback") set; cursors and
  `play_frame` zeroed; if flags bit 1, `0x088FF7CC()` and `0x088FF868()`.
- State 0 or 2, each frame: if flags bit 1, `0x088FF8F8(0x088CA540(replay) != 0)` (the camera data at this frame).
  For each fighter that has frames left or records left: when the run is used up the next record is read
  (`play_mask` = code >> 8, `play_left` = code & 0xFF, `play_index` + 1); `play_left` - 1; the buttons are the OR of
  the table entries of the mask's bits 0 to 14, from a zero base (`0x089CBC70`), and go to
  `0x08872970(pad, …)`, which injects them through `RawPad_inject`; if bit 15, `0x08871BC4(pad)`,
  `0x08871BD0(pad)` (the same edge reset as when it was recorded). Then `play_frame` + 1.
- A fighter whose records run out gets no more injections.

`RawPad_inject` (`0x08872304`, V): buttons word to raw pad `+0x44`, both stick bytes (`+0x48` x, `+0x49` y) to the
centre (127.5 at `0x089C972C`, stored as 127) and moved by 127.5 (`0x089C9730`) for each analog direction bit
(`{0,4}` up and `{0,0x10}` down on y; `{0,0x20}` left and `{0,8}` right on x), giving 0, 127 or 254; then
`0x088720B4(1)`, the pad's poll `0x08871E24(pad, 0)`, `0x088720B4(0)`.

### 3.4 Camera entries

V: 200 entries of 0x24 bytes (`D012ReplayCamera`): two float vectors, a frame (`+0x18`), two halves, a kind byte
(`+0x20`) and three bytes. The table is shared:

- **Recorded events** fill it from index 199 downwards (`camera_count` at `+0xBDA4`), all zero but the frame; kind 0.
  The files hold 6 to 10: index 199 at frame 0, then frame 72 and pairs such as 995/1027 and 2963/2995.
- **Camera data** (Camera Edit's Set Camera Data) fill it from index 0 upwards, counted at `+0xBDA0`. V (run): one
  Set Camera Data at 20.13 s into replay 03 wrote entry 0: both vectors (`(30.34, 36.53, -1.19)` and
  `(24.86, 34.04, -2.45)`: I, the camera's position and target), frame 0, `+0x1C` = 50, kind 0; and `+0xBDA0` = 1.
  The pause menu's camera counter showed 192 before (200 less the 8 recorded events) and 191 after.
  `0x088CA1A4`/`0x088CA42C` return the entry whose frame range holds the play frame.
- **Spans** (kind 1 starts one, kind 2 ends one) sit below the recorded events, counted at `+0xBDA8` and reached
  through `0x088C90C0(replay, n)`. V: `0x088C91EC`/`0x088C92CC` find the n-th start and end, `0x088C913C` counts the
  starts, `0x088C948C(replay, frame)` says whether a frame lies inside a span (1) or not (0), and the player feeds
  that to `0x088FF8F8` each frame (with flags bit 1). `0x088C93AC` is the clock less every span's length (at least
  0). I: another Camera Edit command makes them; none was seen.

So a camera edit changes only the table and its counts (file `0xA268..0xBE88`, `0xBE88`, and `0xBE8C`/`0xBE90` if
the other kinds change).

### 3.5 Flags bit 1 and mode 12

- V: bit 1 is set only by the load hand-off `0x08889BAC` from its third argument: menu `0x08A40C98` passes
  `+0x1296 == 2`, menu `0x08A499A0` passes "its state is 5" (4 and 5 both start a playback). With bit 1 the player
  drives the camera functions above and the task, after a countdown in FIELD `+0x7C5`, starts the battle music
  (`0x088A18B0`, `0x0889F234(…, FIELD+0x804)`). `0x0888B080` reads it for the FIELD builder. I: bit 1 = play with the
  camera data (Play, and Convert to AVI); clear = Camera Edit, with the camera free.
- V: **mode 12** is set only by menu `0x08A505C0` (`0x0888964C(FIELD, 12, 0, 2, 0)`, after registering three
  callbacks `0x08A50160` through `0x08889ED4`; called from menu `0x08A502B8`). The ending withholds the save offer in
  mode 12 only when the battle request `*(0x089D8508)` has byte `+2` == 3 and word `+0x279C` bit 1. Which game mode
  that is was not identified (no strings nearby). I: a mode whose battles are scripted or replayed by the game
  itself.

## 4. The offer and the save (question 5, first part)

V, battle `0x08B07324` (the ending): the "Save Replay" item is set up only when `*(0x089D5F1F)` is 0, option 0x12
(Save Battle Replays) is on, the mode is not 9 and not the mode-12 case. Then battle `0x08B05040(item, 0)` when
the replay is not full, no other battle `0x08B1DBD0`/`0x08B21018` item is pending and `*(0x089D6A00)+0x18` is 0;
otherwise `0x08B05040(item, 1)`.

V, the save itself is started in only two places, both in the ending's results flow:

- battle `0x08B07B94`: when the pad's square is pressed (vtable `+0x10/+0x14` with the mask at `0x08B0CDC0`), or a
  request byte (`+0x3888`) is set, and the item allows it, and the save utility is idle (`*(*(0x089D7280))` == 0):
  `0x0888AD50()`;
- battle `0x08B07E28`: the same without the button (the request byte only). I: the replay pause menu's "Save data"
  leaves through it.

Both then wait on `0x0888B014`/`0x088AF29C` for the result.

V, `0x0888AD50` (the save copy), in order:

1. `0x08889060(FIELD)`; a 60,000-byte save object (`0x088909C8(60000)`) into FIELD `+0x7CC`; `0x088B0AF8` on it
   (descriptor, buffer cleared, `0x088B04AC` sets up the savedata parameters, key included); `0x088AF020` (the save
   manager's heap).
2. The copy into the buffer (save `+0x2BA8`): replay `+0x0` to buffer `+0xE8`, `+0x4..+0x22E8` (0x22E4 bytes) to
   `+0xEC`, both record arrays to `+0x23D0`, the counts to `+0xA260`, the 200 camera entries field by field to
   `+0xA268`, and `+0xBDA0..+0xBDB8` to `+0xBE88`.
3. The detail text's six words: `0x08889C38(FIELD, side, out)` (a fighter id, 1 to 0x20, and a variant, 0 to 2)
   for the player's side (FIELD `+0x54`) and for the other, the stage index
   (`0x088D5258(0x0889716C(FIELD+0x800))`) and the length in seconds (`frame_total / 30`), stored by `0x088B0BA8`
   at save `+0xEA48`, which starts the save (`0x088AFB08`: flags |= 0x90, then a SIZES request).
4. When the size check passes, the descriptor's callback `0x088B0CAC` → `0x088B0C08`: mode 5 (LISTSAVE), the name
   with the date (`0x088ADE90` into `+0x864`), the detail text (`0x088B0CC8(+0x864, +0x2A94)`), a 0x13-byte text
   into `+0x8E4` (`0x088ADF28`), then `0x088AFB30`.

The 60,000-byte object is freed when the result is taken (I: `0x0888B014`).

## 5. REPLAY.BIN (question 3)

V, from the save copy and from the four decrypted files (key above; the decrypter is
`build/scratch/replay/decrypt/decrypt_save.cpp`, built in WSL from PortableKit's `savedata_crypto.cpp`,
`aes128.cpp` and tiny-AES, unchanged):

| File offset | Size | Replay | What | In the four files |
| ---: | ---: | --- | --- | --- |
| `0x0000` | 4 | | descriptor "enabled" (`0x088AEFA0`) | 1 |
| `0x0004` | 4 | | descriptor size | 0xBEA0 |
| `0x0008` | 0xE0 | | never written (cleared by `0x088B0AF8`) | zeros |
| `0x00E8` | 4 | `+0x0000` | magic | 0x20081106 |
| `0x00EC` | 0x20C0 | `+0x0004` | the two 0x1060-byte set-ups | |
| `0x21AC` | 0x220 | `+0x20C4` | the rest of the set-up | |
| `0x23CC` | 4 | `+0x22E4` | music | 83, 83, 84, 83 |
| `0x23D0` | 0x3F48 | `+0x22E8` | fighter 0's records | |
| `0x6318` | 0x3F48 | `+0x6230` | fighter 1's records | |
| `0xA260` | 8 | `+0xA178` | record counts | (26, 22) (15, 12) (19, 16) (16, 14) |
| `0xA268` | 0x1C20 | `+0xA180` | camera entries | 10, 6, 8, 8 used |
| `0xBE88` | 4 | `+0xBDA0` | camera data count (Camera Edit) | 0 (1 after one Set Camera Data) |
| `0xBE8C` | 4 | `+0xBDA4` | recorded camera events | 10, 6, 8, 8 |
| `0xBE90` | 4 | `+0xBDA8` | span count | 0 |
| `0xBE94` | 4 | `+0xBDAC` | frames recorded | 5,592, 2,849, 3,957, 3,415 |
| `0xBE98` | 1 + 3 | `+0xBDB0` | flag byte, padding | 0 |
| `0xBE9C` | 4 | `+0xBDB4` | the player's side | 0 |

- **Checksum:** none (V: the game writes no word it could be in, and the load checks only words 0 and 1).
- **What changes on every save:** V (run): nothing. Replay 03 loaded and saved twice with no edit (to new slots
  Replay04 and Replay05) gave plaintext identical byte for byte to the original; the file holds no date or counter
  (the date goes into the save's name and `PARAM.SFO`). The encrypted files differ on every save (their hashes:
  `d3e8c877…`, `8544e110…`): the 16-byte header is random.
- **What a camera edit followed by Save data changes:** V (run): 26 bytes, camera entry 0 (`0xA268..0xA27F` and
  `0xA284`) and `0xBE88` (`camera_data_count`, 0 to 1). Nothing in `0x0000..0xA268`, the counts or the frames. The
  set-up does go round a trip during playback (V: `0x088C96E8` applies it through `0x088FB028`, and the FIELD
  builder `0x0888C4C8` stores it back through `0x088C990C` → `0x088FB1A4` at `0x0888DCCC` on both of its paths),
  and it came back byte for byte.

## 6. The load path (question 4)

V:

1. **Set-up.** Menu `0x08A49D7C` (called from menu `0x08A49750` and `0x08A4B5AC`) allocates a 60,000-byte save
   object if it has none, calls `0x088B0AF8` on it (the same descriptor as the save: `REPLAY.BIN`, key, buffer
   cleared) and `0x088AF020`. Menu `0x08A44504` does the same. The load request is started by `0x088AF2AC`
   (savedata mode 4, LISTLOAD), called at menu `0x08A44950`/`0x08A44988` and `0x08A4BB1C`/`0x08A4BBD4`. The same
   menus also call `0x088AF348` (flag 8, then a SIZES request; I: another of their requests, not traced). The trace
   logs show the load: `LISTLOAD (4) save="" file="REPLAY.BIN" bufSize=0x0000BEA0`, `list=["Replay00", …]`.
2. **Checks after the load.** The save manager's update `0x088AFDE4`, for a finished mode-4 request, calls
   `0x088AF0D0` → `0x088AEF54`: the buffer's first word must equal the descriptor's "enabled" (1) and the second its
   size (0xBEA0), else flag 4, and the result is 2 (broken data). There is no version check beyond that, and no
   size check beyond the utility's own. A failed utility result (`0x80110307` no data, `0x80110306` broken) takes
   the same way back to the Creation menu (V in the earlier runs, docs/port/replay-input-track.md 3.6).
3. **Hand-off.** Menu `0x08A40C98` (when its `+0x129A` is set) and menu `0x08A499A0` (states 4 and 5) call
   `0x0888964C(FIELD, 9, 0, 0, 0)`, which sets mode 9 and runs the battle start `0x08889694` at once. In mode 9 that
   resets the replay object (`0x088C95A0`: everything cleared, flags 0) when flags bit 1 is set or FIELD `+0x68` is 0,
   else only rewinds it (`0x088C965C`, `0x088C98F0`). Then `0x08889BAC(FIELD, save + 0x2C90, camera)`: the raw pad
   cleared through `RawPad_inject`, the loaded data (the buffer from file offset 0xE8) copied into the replay object
   (`0x088C96E8`, which ends with `0x088C98F0`), flags bit 1 from `camera`, mode 9.
4. I: on "Play from beginning" the battle start runs again with FIELD `+0x68` = 1, which rewinds without clearing
   when bit 1 is clear. The case "bit 1 set and FIELD `+0x68` = 1" clears the object, so either the restart takes
   another path or the menu copies the data in again; not traced.

So the game never reads more than 0xBEA0 bytes and never looks past file offset 0xBEA0: a longer plaintext is
ignored by the game itself.

## 7. Every caller (question 5)

V, from instruction scans of all four modules:

| What | Called from |
| --- | --- |
| save copy `0x0888AD50` | battle `0x08B07B94`, `0x08B07E28` (the ending) |
| save set-up `0x088B0AF8` | main `0x0888AD50`; menu `0x08A44504`, `0x08A49D7C` (the load) |
| `REPLAY.BIN`'s name `0x089CB718` | only `0x088B0AF8` |
| the replay key `0x089CB708` | only `0x088B0AF8` (the menu's other saves, `FCARD.BIN` at menu `0x08B58080` and `QUEST.BIN` at `0x08B58498`, have keys of their own) |
| load hand-off `0x08889BAC` | menu `0x08A40C98`, `0x08A499A0` |
| replay copy `0x088C96E8` | only `0x08889BAC` |
| the replay pointer `0x089D7750` | only the static initialiser and the FIELD constructor |
| recorder, player | only the task |

Calls into the replay module from outside it (`0x088C8000..0x088CB000`) are the task, the battle start, the FIELD
builder, the ending, the replay pause menu's battle functions (battle `0x08B2xxxx`) and the two replay menus
(menu `0x08A43CB0`, `0x08A4ACFC` call `0x088C93AC`). None is in the ad hoc or network code (the Communication Mode
set-up is menu `0x08AB74FC`, mode 5). I: there is no way to send a replay to another PSP; a Communication Mode
battle is recorded on each PSP from the injected pads and saved like any other.

## 8. What this changes in the replay plan (docs/port/replay-input-track.md)

1. **The key.** `PCREPLAY.BIN` must be encrypted with the replay key `80176be7…` (which the game passes in each
   request), not "the game's key". PortableKit already uses whatever key the request carries, so nothing changes in
   practice, but the plan's wording is wrong.
2. **The header.** File `0x00..0xE8` holds no date, name or checksum: words 1 and 0xBEA0 and zeros. A `PCREPLAY.BIN`
   with its track after byte 0xBEA0 passes the game's check, because the game reads only 0xBEA0 bytes.
3. **"A word of the battle" at 0xE8** is the format word 0x20081106. The plan's key (3.3) can keep it in.
4. **The clock** (3.1): read `+0xBDAC` while recording and `+0xBDE4` while playing; both are per task call, and
   the task runs once a frame only in battle phases 2 to 4. Rewinds zero `+0xBDE4` (`0x088C965C`), so a track keyed
   to it follows "Play from beginning" by construction. A track should also stop where `frame_total` stops.
5. **The last 0x18 bytes:** `+0xBDA0` (camera data) changes with Camera Edit (V, run), and `+0xBDA4`/`+0xBDA8`
   belong to the same table: leave all three out of the key, as the camera entries are. `+0xBDAC` (frames) and
   `+0xBDB4` (side) belong in it.
6. **The plan's key range holds** (V, run): a re-save, with or without a camera edit, leaves `0xE8..0xA268` and
   the counts unchanged, so a track keyed to them survives Edit and Save data. The camera table is the only part
   that moves.
7. **A full replay** (5,400 records for one fighter, or 200 camera events) is not offered for saving; the port's
   track can stop recording when flags bit 0 is set.
8. **Order:** the recorder reads the stick bits after this frame's poll (V), so the port's per-frame stick bytes
   should be taken from the same poll (the dead-zone routine's input of that frame).

## 9. Not determined

1. What mode 12 is.
2. The camera entries' halves and bytes besides the kind (`+0x1C` was 50 in a Camera Edit entry), which Camera
   Edit command makes spans, and what `0x0888B0A4` tests exactly (I: a fighter's special states, `0x08ADFE20` on
   `+0x2A8`, and a battle overlay state in phase 3).
3. `+0xBDB0`'s meaning; `+0x20C4..+0x22E4` (I: the option flags `0x09AFD960` and the rest that `0x088FB1A4` stores).
4. The restart path's case of section 6, step 4.

## 10. The plan's open questions (its section 5)

| Question | Answer |
| --- | --- |
| The file's first 0xE8 bytes and last 0x18 | **Answered** (V): 1, 0xBEA0, zeros; the last 0x18 are replay `+0xBDA0..+0xBDB8` (the camera data, event and span counts, frames, a flag byte, side). No checksum. Nothing changes on a re-save; Camera Edit changes only the camera table and its counts (V, run). |
| What mode 12 is, and what the ending shows for a full replay | **Partly**: mode 12 is set by menu `0x08A505C0` and withholds the offer under one condition; its identity is open. A full replay gets the offer item in its other state (I: unavailable). |
| What a PSP and PPSSPP show for a `PCREPLAY.BIN`-only folder | Not answered (needs PPSSPP; step 7). |
| Whether a PSP saving over such a folder keeps `PCREPLAY.BIN` | Not answered (step 7). |
| The player's cursor fields: which is the frame count | **Answered** (V): `+0xBDE4`; per fighter `+0xBDCC` mask, `+0xBDD4` frames left, `+0xBDDC` next record. Recording: `+0xBDAC`. |
| Whether the port plays a PSP-made replay to the same end | Not answered (step 7). |
| Which cheats and mods change a battle | Not answered (out of scope). |
| Whether Communication Mode can send a replay | **Answered** (I, static): no code path sends one; it is recorded and saved locally. |
| The detail-text line for PSP players | Not decided (owner). |

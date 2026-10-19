# Dissidia 012 (ULUS10566 v1.00): moveset archive semantics

> Static analysis, 14 October 2026. What each part of a fighter's moveset archive (`obj/p_<code>.objx`)
> means to the code that reads it, for authoring new attacks. Byte layouts are shared with
> `tools/moveset.py` (the compiler/decompiler), which takes its names from the two tables that go with
> this page: `build/scratch/moveset_semantics/fields.tsv` and `events.tsv` (git-ignored; regenerate
> with `make_fields.py` / `make_events.py` in that folder). Builds on
> [battle-moveset.md](battle-moveset.md); corrections to it are marked **Correction**.
> Nothing here was checked at runtime unless it says so.

Status words used throughout: **verified** = the reading code was followed to what it changes (address
given); **partial** = code read, meaning of the state it touches inferred; **flag** = the event only sets
a bit, the reader is named but not fully read; **inferred** = from data patterns only; **unknown**.

---------------------------------------------------------------------------------------------------

## 0. Short answers

* **One interpreter, no scripting.** An attack is an attack record (`plya`) that plays a motion; the
  motion's animation has an event track of *ranged* events (active over a frame window) and *instant*
  events (fire once). Each frame `main:0x08830444` advances the motion and runs both lists through
  `main:0x08829930` (ranged, 125-entry table) and `main:0x0882CEC0` (instant, 56-entry table).
  112 ranged and 48 instant types are implemented; every type has a fixed argument count in the data.
* **Frames are 1/60 s.** Motion length = GMO `FrameLoop` end x 60 / `FrameRate` (`main:0x0886B344`);
  the stock GMO motions are 30 fps, so one model frame is two track frames.
* **Only one stock teleport exists**: instant `0x26` (`warp_to_target`), used once (Chaos,
  `SPEAR_BOMB`). It puts the fighter on the *opponent*, not on a projectile or an arbitrary point.
  Nothing in the stock vocabulary moves a fighter to a projectile. A warp-strike needs one new event
  type; section 8 gives the hook point (the handler pointer at `main:0x08828ECC`/`main:0x08828EDC`), the
  free type numbers (instant `>= 0x38` is ignored by the stock game) and the functions to call.
* **Corrections to battle-moveset.md.** Instant `0x11` is "play sound if a model part is shown",
  not "attach effect to bone" (its first argument is a model part, the second a sound id);
  `plya +0x21` is the HUD name banner string, not a particle effect (`main:0x08856D48` drives the
  manager fed by the `wind` chunk; see `tools/character_text.py`); `plya +0x07` is an effect index
  (`main:0x088A9DE8`), 0 in all stock data; the hit event's second argument is a **shape id**,
  matched against the first byte of every kind-5 shape record (several records may share one id; the
  record's position in the chunk does not matter); `plya +0x28` is the *upper* range bound with `+0x24` the lower; `plya +0x1E`
  is the motion queued after the attack (idle 1 / air 101), not a second attack motion.

---------------------------------------------------------------------------------------------------

## 1. Units and conventions

| quantity | unit | basis |
|---|---|---|
| event frames, `comb` frame windows | 1/60 s ticks of the motion clock (motion+0x78 = char+0x1A8) | `main:0x0886B344`, `main:0x08833910` advances by `time scale x char+0x3BC` |
| speeds in events and records | world units per tick, multiplied by the time scale `main:0x08838300` (char+0x3C8 x global dt) | `main:0x08835034`, `battle:0x08A60838` |
| angles in event arguments | degrees (int or float), converted with pi/180 (`main:0x089C78F0` = pi) | every turn/launch handler |
| `plya +0x58` approach angle | radians (passed unconverted) | `battle:0x08B382B8` |
| percentages | integer / 100 | fade, shake, alpha |
| distances | world units; body pushbox radius about 1, Cloud's sword spheres 0.7 | kind-5 data |
| ids in events | absolute: sound id (bank x 100 + slot, the fighter's own bank), voice id, effect index in the own package, hit record id, projectile id, attack id | sections 3, 6 |

The fighter object is 0x12D0 bytes; its motion controller is at char+0x130, so "motion+X" in this
page is char+0x130+X. The current attack record is char+0x101C (`battle:0x08B30898` prefers char+0x1020,
an upgraded record). Attack target: char+0x2FC (vtable +0x124, `main:0x08836D8C`); lock-on target:
char+0x300 (vtable +0x134, `main:0x08836D9C`). Position char+0x80 (vec4), centre char+0x100, velocity
char+0xE0 (unit direction + magnitude), facing char+0x90.

---------------------------------------------------------------------------------------------------

## 2. Chunks of an objx

The archive is `ARC\x01`, u32 count, then 16-byte entries `{u32 kind, char tag[4], u32 offset, u32
size}`. `main:0x088459E0` finds kinds 7, 8, 5, 6, 9 by kind alone (`main:0x0885CA3C`) and the rest by
kind and tag (`main:0x0885CA98`, tags at `main:0x089C8948`), and fills the 0xEC-byte object-data
struct kept at char+0x390.

| kind / tag | objdata | role | layout and lookup | read by |
|---|---|---|---|---|
| 7 | +0x1C | motion table: motion id -> animation index + variant | `{u32 2, u32 count}`, 8-byte entries sorted by id; binary search | `main:0x0882F0D4` via `main:0x08934944` |
| 8 | +0x20 | event tracks, one per animation | `{u32 3, u32 count, u32 offset[count]}`; a track is found by its first u16 = animation index, linear | `main:0x088298E4`, interpreter section 4 |
| 5 | +0x18 | collision shapes (body, hurt, attack, clash ...) | `{u64 default-on mask, u32 count, u32 0}`, 0x20-byte records; switched by shape id = record byte 0 (ids 0..63; several records may share one) and grouped by class byte | `main:0x08831808`, `main:0x08837D18` |
| 6 | +0x24 | swing bones (capes, hair) | not decoded here | `main:0x0880D82C`; events R 0x16/0x17/0x27 |
| 9 | +0x30 | EX burst background (`hibg` / `defa`) | | `battle:0x08ACDA90` |
| 4 `effe` | +0x34.. | effect package (MPK) | effects are spawned by index | `main:0x0883EB10`; spawn `main:0x088A9DE8` |
| 14 `snd_` (x2) | +0x60.. | sound banks (SSCF) | | `main:0x088423A8` |
| 1 `plya` | +0x88 | attack records | `{u32 0x16, u32 count}`, 0x74 each; by index (`main:0x088480D0`) or id (linear, `main:0x0884808C`) | section 5 |
| 1 `comb` | +0x8C | ability command -> attack chain | `{u16 6, s16 count, u32 offset[count]}`; linear | `battle:0x08AB7F40`, `battle:0x08AB81E0` |
| 1 `atkp` | +0x90 | hit records | `{u32 0x0C, u32 count}`, 0x44 each, sorted by id; binary search, fallback to `battle.bin` | `main:0x08837F14` |
| 1 `magi` | +0x94..+0x9C | projectile emitters + float pool | `{'MAGI', u32 0x20101011, u32 count, u32 floats}`, 0xCC each sorted by id, then `floats` x f32 | `main:0x08828A98`, `main:0x08828AE8` |
| 1 `brat` | +0xA0.. | CPU attack weights per `comb` command | magic `atta`, f32 1.02 | `main:0x088105FC` |
| 1 `text` | +0xC8/+0xCC | HUD banner strings (`mlng`) | see `tools/character_text.py` | `main:0x0887AC24` |
| 17+3+13 `spec` | +0xD0 | EX burst UI | | `main:0x088CF188` |
| 3 `feat` `pris` `bubb` `cart` `mine` `wind` | global managers `*(main:0x089D4DC8)` +0x4E0, +0x6200, +0x65E0, +0x6B00, +0x52D0, +0x69C0 | textures for programmatic effects; `wind` feeds the name-banner manager | | `main:0x08854A5C`, `main:0x08859DA4`, `main:0x08851C78`, `main:0x08852378`, `main:0x08857824`, `main:0x08856ACC` |
| 2 + 18 `d_xx` | | small secondary model | | `main:0x08842F78` |

Collision shape classes (`+0x04`) are collected into per-class lists by `main:0x08831808`: class 0 ->
char+0x3A0 (body; used by the position integrator `main:0x0883B470`), 1 -> +0x468 (hurt: "has hurtbox"
`main:0x088375D4`), 2 -> +0x46C, 3 -> +0x470, 6 -> +0x474 (read only by the clash code
`main:0x08809F00`), 5 -> +0x478, 13 -> +0x47C, 14 -> +0x480, 0 -> +0x484, 8 -> +0x488, 4 -> +0x48C.
**Shape ids (corrected 14 October 2026).** A hit or `shape_on` event names a *shape id*, not a record:
each frame the code walks the records of each class list and keeps those for which
`main:0x08837D18(char, *(u8 *)record)` holds, so every record whose first byte equals the id is armed
together. The first u32 of a record (`id` in `fields.tsv`, formerly named `bone`) is that id; the bone
is the byte at +0x06 (formerly `bone2`). `main:0x08812664` builds the sphere: kind at +0x05 (3), the
u16 at +0x06 as the frame for `main:0x088369A4` (a model bone, or with `shape_flags` 0x40 the value
0x4000 | bone, where 0x4000 alone is the character's own frame: on the floor, +z forward), centre
`(x, y - w, z)` in that frame, radius at +0x18. Example, Squall: id 46 = records 30 and 31 (class-4
spheres on bone 46 `r_wep` at 0.5 and 1.0 along the blade, radius 0.7) plus record 32 (class 6);
ids 42 and 43 are grids of 16 class-4 spheres each in front of him (Blasting Zone). Hit events arm
the class-4 records of the id; other classes with the same id (5, 6, 7) ride along as their companions.
Counts over 32 fighters: class 4 x1228, 6 x903, 5 x471, 7 x91, 3 x73, 2 x69, 1 x50, 0/10/11/15 one
each per fighter. 21 fighters have more than 64 records; that is no limit in itself, since only the
ids (0..63, a 64-bit mask) are switched and records share them.
A shape is active when (default mask | char+0x458 | this frame's char+0x448) has the bit of its id and
neither char+0x460 nor this frame's char+0x450 does (`main:0x08837D18`); the per-frame masks are reset by
`main:0x08829848` before the ranged events run.

---------------------------------------------------------------------------------------------------

## 3. How the parts refer to each other

| from | field | to | how resolved |
|---|---|---|---|
| ability (`system.bin/ablt` +0x0C) | command id | `comb` entry `+0x00` or `+0x04` | `battle:0x08AB7F40` marks entries of equipped abilities (256-bit mask) |
| `comb` stage | `+0x00` attack id | `plya +0x00` | stage i is accepted when stage i-1 is the current attack and its frame is in `[min, max)` (`battle:0x08AB81E0`) |
| `plya` | `+0x1C` motion id | motion table -> animation index + variant | binary search; animation index = GMO motion block number = event track key |
| `plya` | `+0x0A` / `+0x08` | another `plya` (upgrade) | `main:0x0884A2B8`, while char+0x52C+0x14 > `+0x08` |
| `plya` | `+0x14` + flag 0x2000 | stance record | `main:0x0884A340` |
| `plya` | `+0x1E` | motion queued after | `main:0x0882F49C` (16-slot queue at motion+0xA0) |
| `plya` | `+0x21` | `text` string index | name banner `main:0x08856D48` |
| motion variant | `+0x04` | event condition byte `+6` / `+4` | events with a non-zero variant run only for that variant (two motion ids may share one animation) |
| ranged event `0x00`/`0x43`/`0x7B` | arg0 | `atkp` id | `main:0x08837F14` (sorted) |
| same | arg1 | shape id: every kind-5 record whose first byte equals it | bit in char+0x448, tested per record by `main:0x08837D18` |
| instant `0x04`, `0x29`, `0x2D`, `0x33` | arg0 | `magi` id | `main:0x08828AE8` (sorted) |
| `magi` | `+0x0A..` | `atkp` ids (inferred) | |
| instant `0x15` | arg0 | voice id: bank x 100 + slot (+10000 manikin, +30000 alternate, +50000 assist) | `battle:0x08B33F8C`; `tools/character_voice.py` |
| instant `0x03`/`0x11`/`0x12` | sound id | absolute id in the fighter's own SFX bank (voice bank number - 100); +50000 for an assist object (`main:0x08842828`) | `main:0x0883834C` |
| instant `0x19`, `0x2F`, `0x30`, ranged `0x66`, `plya +0x07`, `comb stage +0x06` | effect | index in the own effect package (objdata+0x5C) | `main:0x088A9DE8` |
| ranged `0x2A` | group 9999 / 8888 | weapon-trail effects of the package | `main:0x0884E660` |
| ranged `0x67`/`0x79` | arg1 | `plya` id offered as the next attack | `main:0x0884808C` then `battle:0x08B308D8` |

Id ranges seen over all 32 fighters: attack ids 17..3035 (1298 distinct in 1299 records), hit record
ids 2..3355 (all 2130 distinct), projectile ids 0..2277. The game only searches the fighter's own
tables, but the shipped numbering is global; new records should take ids above these.

---------------------------------------------------------------------------------------------------

## 4. Event tracks and the interpreter

Track: `{u16 animation, u8 ranged count, u8 instant count, u16 offset of instant list, u16 size}`,
ranged events from +8, then instant events.

| | ranged | instant |
|---|---|---|
| header | `u16 start, u16 end, u8 type, u8 argc, u8 variant, u8 mode, u8 form, u8 costume, u16 0` | `u16 frame, u8 type, u8 argc, u8 variant, u8 mode, u8 form, u8 costume` |
| args | argc x 32-bit words (int or float by type) | same |
| fires | every update while `start <= frame < end` (`end` 0xFFFF: to the end of the motion; `start == end` never fires) | once, when `previous < frame <= current`; frame-0 events fire when the motion starts |
| walker | `main:0x08828EEC`, called by `main:0x0882F30C` | `main:0x08829104`, called by `main:0x0882F17C` |
| handler | `main:0x08829930`, pointer loaded at `main:0x08828EA0`/`main:0x08828EB0` | `main:0x0882CEC0`, pointer loaded at `main:0x08828ECC`/`main:0x08828EDC` |
| bounds | `sltiu 0x7D` at `main:0x088299C4`; out of range -> return | `sltiu 0x38` at `main:0x0882CEE4`; out of range -> return |

Condition bytes (`main:0x08828EEC`, `main:0x08829104`; inputs from `main:0x0882F17C`/`main:0x0882F30C`):
`variant` 0 or the motion-table variant; `mode` 0 always, 1 normal, 2 EX mode, 6 assist, 3 a type-2
object with flag 0x20, 4 EX mode with char+0x5B6 bit 0x40 clear, 5 not EX or that bit set; `form` 0
or form index + 1 (char+0xD60, Lightning paradigms, Cecil); `costume` 0 or costume + 1 (char+0x1079).
Seen in data: variants 1..16, modes 1, 2, 4, 5, 6, forms 1..2, costumes 1..5 (costume-specific trails).

Order in an update (`main:0x08830444`): advance the clock; instant events over (previous, current];
`main:0x08829848` clears the per-frame state (shape masks, chain list, ...); ranged events at the current
frame. The handlers set bits in the flag array motion+0x60..0x6F which the state machine reads until the
next clear. `main:0x08829344` marks a motion as an attack (motion+0x54 |= 0x10) when its track has a
ranged 0x00 or 0x5B or an instant 0x04 for the current variant.

### Motion flag array (motion+0x60.., set by ranged events)

| byte | bits -> set by | main readers |
|---|---|---|
| 0x60 | 1 actionable 0x05/0x1A/0x1E/0x58/0x7A; 2 airborne 0x0A/0x18/0x48; 4/8 follow-up 0x01/0x67/0x78/0x79; 0x10 0x20; 0x20 0x06/0x47; 0x40 0x55; 0x80 0x0D | `main:0x08837B20`, `main:0x0883752C`, `main:0x088377D0`, `main:0x08837948`, `battle:0x08B3A5BC` |
| 0x61 | 1 camera 0x0C | |
| 0x62 | 2 intangible 0x13; 4 0x15; 8/0x10/0x40 0x18; 0x20 0x25; 0x80 0x1A | `main:0x088375D4`, `main:0x088379B8`, `main:0x08837A10` |
| 0x63 | 1/2 cancel 0x1E/0x58/0x7A; 4 armor-off 0x22; 8 0x29; 0x10 0x23; 0x20 0x24; 0x40 0x26; 0x80 0x34 | `main:0x0883A150`, AI functions |
| 0x64 | 1 0x31; 2 0x3E; 4 0x28/0x59; 8 0x2D; 0x10 0x2F; 0x20 0x30; 0x40 0x32; 0x80 0x36/0x3A | `battle:0x08B3A5BC`, `main:0x08837458`, `main:0x08837C44`, `main:0x088379FC` |
| 0x65 | 1 0x37/0x3A; 2 0x3A; 4 0x3D; 8 (fade latch) 0x40; 0x10 0x41; 0x20 0x45; 0x40 0x46; 0x80 0x49 | `main:0x08839D84`, `main:0x0883E3BC`, `battle:0x08ADA0A4` |
| 0x66 | 1 0x4B; 2 0x4A; 4 0x4C; 0x08..0x40 0x4D; 0x80 0x52 | `main:0x08838E24`, `main:0x08835418` |
| 0x67 | 1 0x51; 2 0x53; 4 camera paths; 8 0x62; 0x10 0x69; 0x20 0x6A; 0x40 0x6C; 0x80 0x71 | `main:0x0883A150`, `main:0x08831360` |
| 0x68 | 1 steer 0x18; 2 0x72; 4/0x40/0x80 0x1E/0x7A; 8 0x45; 0x10 0x74; 0x20 0x77 | `battle:0x08B382B8`, `battle:0x08B38A0C` |
| 0x69 | 1 0x7C; 2 0x3C; 4 0x1E/0x58/0x7A | `battle:0x08B2F5E0` |

---------------------------------------------------------------------------------------------------

## 5. Record fields

Machine-readable copy with value ranges: `fields.tsv`. Unknown fields keep their neutral names.

<!-- FIELDS -->
#### `plya` attack record (0x74 bytes after `{u32 version 0x16, u32 count}`)

| off | type | name | status | read by | seen | meaning |
|---|---|---|---|---|---|---|
| 0x00 | u16 | `id` | verified | main:0x0884808C | 17..3035 (1298 distinct) | Attack id; unique per fighter; comb stages and chain events name it |
| 0x02 | u8 | `hit_percent_bonus` | partial | main:0x08807860 | 0..200 (2 distinct) | Added to 100 in the hit instance (+0x2A) of hits made by char+0x1024 record; 0 or 200 |
| 0x03 | u8 | `kind` | verified | main:0x08848160, battle:0x08B3A5BC | 0..4 (5 distinct) | 0 opener, 1 follow-up, 2 chase, 3 counter chase, 4 combo-stage only (battle:0x08AB81E0) |
| 0x04 | u8 | `pad_04` | inferred |  | 0..0 (1 distinct) | Always 0 |
| 0x05 | u8 | `self_position` | verified | main:0x08847F10 | 0..2 (3 distinct) | 0 any, 1 attacker airborne, 2 attacker grounded (main:0x088374B0) |
| 0x06 | u8 | `target_position` | verified | main:0x08847F10 | 0..0 (1 distinct) | 0 any, 1 target airborne, 2 target grounded |
| 0x07 | s8 | `start_effect` | verified | battle:0x08B382B8 | 0..0 (1 distinct) | Effect id spawned at start (main:0x088A9DE8) when > 0; 0 in all stock data |
| 0x08 | s16 | `upgrade_at` | verified | main:0x0884A2B8 | -1..3999 (3 distinct) | Use record upgrade_to instead while char+0x52C+0x14 exceeds this (-1 none) |
| 0x0A | s16 | `upgrade_to` | verified | main:0x0884A2B8 | -1..2228 (37 distinct) | Attack id of the upgraded record (-1 none) |
| 0x0C | u8 | `field_0x0C` | unknown |  | 1..255 (7 distinct) | 0xFF or 1..6 |
| 0x0D | u8 | `field_0x0D` | unknown |  | 1..255 (3 distinct) | 0xFF, 1 or 5 |
| 0x0E | u8 | `field_0x0E` | unknown |  | 0..12 (13 distinct) | 0..12 |
| 0x0F | x8 | `slot_class` | verified | main:0x08848160 | 0x1 0x2 0x41 0x8 0x44 0x42 0x51 0x21 (8 distinct) | Selector class bits: 1 classes 0/1, 2 classes 2/3, 4 classes 4/5, 8 class 6; 0x20 needs height >= 0 |
| 0x10 | s16 | `follow_motion` | verified | battle:0x08B2EF88 | -1..20002 (13 distinct) | Motion played by instant follow_motion_after (-1/0 none) |
| 0x12 | s16 | `field_0x12` | unknown |  | -1..10067 (2 distinct) | Almost always -1 |
| 0x14 | u8 | `stance` | verified | main:0x0884A340, battle:0x08B382B8 | 0..8 (9 distinct) | With flag 0x2000: record used when char+0x106A equals this |
| 0x15 | u8 | `field_0x15` | unknown |  | 0..2 (3 distinct) | 0 or 2 |
| 0x16 | u16 | `pad_16` | inferred |  | 0..0 (1 distinct) | Always 0 |
| 0x18 | x32 | `flags` | verified | battle:0x08B382B8, main:0x08848160 | 0x10002 0x10012 0x10102 0x10006 0x10003 0x200002 0x10042 0x10812 (88 distinct) | 1 approach first; 2 no range test; 0x80 HP attack; 0x400 special action; 0x800 scripted (chrg) |
| 0x1C | u16 | `motion` | verified | battle:0x08B382B8 | 0..10172 (258 distinct) | Motion id (motion table); with flag 0x400 an action code 1..6 (states 0x1B..0x20) |
| 0x1E | s16 | `next_motion` | verified | main:0x0882F49C | -1..101 (3 distinct) | Motion queued after this one ends (1 ground idle, 101 air), -1 none |
| 0x20 | u8 | `field_0x20` | unknown |  | 0..3 (4 distinct) | 0..10 |
| 0x21 | s8 | `banner` | verified | battle:0x08B382B8 | -1..108 (25 distinct) | Name banner: string index in the text chunk (main:0x08856D48, wind manager), -1 none |
| 0x22 | s16 | `action_arg` | verified | battle:0x08B382B8 | 0..469 (2 distinct) | Argument of special action 4 (state 0x1E) |
| 0x24 | f32 | `range_min` | verified | main:0x08847E08 | 0.0..25.0 (22 distinct) | Chosen when horizontal distance to target >= this |
| 0x28 | f32 | `range_max` | verified | main:0x08847E08 | 0.0..100.0 (40 distinct) | ... and < this (units: 1 = about a body width; reach 4..100) |
| 0x2C | f32 | `height_min` | verified | main:0x08847E08 | -100.0..5.0 (32 distinct) | Target height minus own, lower bound (minus a selector tolerance) |
| 0x30 | f32 | `height_max` | verified | main:0x08847E08 | -6.0..100.0 (32 distinct) | Upper bound (plus the tolerance) |
| 0x34 | f32 | `aim_min` | partial | main:0x08847E08 | -1.0..1.0 (3 distinct) | Lower bound (exclusive) of the facing/target alignment ratio 0..1; ~0 or -1 |
| 0x38 | f32 | `aim_max` | partial | main:0x08847E08 | 1.0..1.0 (1 distinct) | Upper bound (inclusive); 1 |
| 0x3C | f32 | `next_motion_blend` | verified | battle:0x08B382B8 | 0.0..16.0 (3 distinct) | Blend frames for next_motion (16) |
| 0x40 | f32 | `approach_speed` | partial | battle:0x08A60BC8 | 0.0..1.5 (22 distinct) | flag 1: homing move speed per frame (move block p1) |
| 0x44 | f32 | `approach_v_speed` | partial | battle:0x08A60BC8 | 0.0..8.0 (19 distinct) | Vertical speed factor (p2) |
| 0x48 | f32 | `approach_frames` | partial | battle:0x08A60BC8 | 0.0..52.0 (21 distinct) | Frames the move runs (p3) |
| 0x4C | f32 | `approach_arrive` | partial | battle:0x08A60BC8 | -1.0..1.5 (3 distinct) | Arrival distance (p7), -1 never |
| 0x50 | f32 | `approach_total` | partial | battle:0x08A60BC8 | -1.0..52.0 (21 distinct) | p8, -1 or equal to approach_frames |
| 0x54 | f32 | `approach_height` | partial | battle:0x08A604AC | -6.0..5.0 (17 distinct) | Goal height offset (p4) |
| 0x58 | f32 | `approach_angle` | partial | battle:0x08A604AC | 0.0..1.5 (3 distinct) | Goal angle around the target, radians (p5) |
| 0x5C | f32 | `approach_distance` | partial | battle:0x08A604AC | -2.0..6.0 (21 distinct) | Goal distance from the target (p6) |
| 0x60 | x8 | `field_0x60` | unknown |  | 0xff 0x6 0x5 0x1 0x2 0x4 0xd 0x11 (25 distinct) | Four bytes, 0xFF-padded small numbers (AI hints?) |
| 0x61 | x8 | `field_0x61` | unknown |  | 0xff 0x2 0x1 0x0 0x15 0x6 0x5 0x14 (20 distinct) |  |
| 0x62 | x8 | `field_0x62` | unknown |  | 0xff 0x1 0x2 0x6 0x0 0x3 0x11 0x5 (13 distinct) |  |
| 0x63 | x8 | `field_0x63` | unknown |  | 0xff 0x3 0x11 0x1 0x0 0x2 0x5 0x7 (8 distinct) |  |
| 0x64 | char[16] | `name` | verified | main:0x08848E3C |  | Debug name; LIMIT_START is looked up by name |

#### `atkp` hit record (0x44 bytes after `{u32 version 0x0C, u32 count}`, sorted by id)

| off | type | name | status | read by | seen | meaning |
|---|---|---|---|---|---|---|
| 0x00 | u32 | `id` | verified | main:0x08837F14 | 2..3355 (2130 distinct) | Hit record id; sorted; fallback to battle.bin atkp (main:0x0880BE84) |
| 0x04 | u8 | `clash_class` | verified | main:0x08809F00, main:0x0884A454 | 0..15 (16 distinct) | Row/column of the 16x16 clash table main:0x089D4978; 4,5,6,9,12..15 projectile-like |
| 0x05 | u8 | `reaction` | partial | main:0x0884A53C | 0..19 (17 distinct) | Reaction handed to vtable+0xE4 (13 light, 1, 4, 5, 6 launch, 7 down ...) |
| 0x06 | s8 | `field_0x06` | unknown |  | -1..32 (6 distinct) | Almost always -1 |
| 0x07 | u8 | `pad_07` | inferred |  | 0..0 (1 distinct) | Always 0 |
| 0x08 | x32 | `hit_flags` | unknown |  | 0x0 0x80 0x4 0x92 0x12 0x400 0x20 0x2 (31 distinct) | Bits 0x2, 0x4, 0x10, 0x20, 0x80, 0x200, 0x400 seen |
| 0x0C | u8 | `field_0x0C` | inferred |  | 0..255 (62 distinct) | 0..20, 0xFF on HP attacks (brave power?) |
| 0x0D | u8 | `spark` | verified | main:0x08808C98 | 0..20 (11 distinct) | Hit spark kind (battle:0x08ABB690 / battle:0x08ABB60C) |
| 0x0E | u8 | `arc_start` | verified | main:0x088092E4 | 0..90 (2 distinct) | Clash arc start, degrees (0 or 90) |
| 0x0F | u8 | `arc_width` | verified | main:0x088092E4 | 90..180 (2 distinct) | Clash arc width, degrees (180 or 90) |
| 0x10 | u8 | `power` | inferred |  | 0..255 (36 distinct) | 0..30 (combo hits rise 5, 6, 8, 10) |
| 0x11 | u8 | `field_0x11` | unknown |  | 0..255 (3 distinct) |  |
| 0x12 | u8 | `rehit` | partial | main:0x0880AD28 | 0..120 (18 distinct) | Non-zero: multi-hit path main:0x0880AAE0 (2..20) |
| 0x13 | u8 | `pad_13` | inferred |  | 0..0 (1 distinct) | Always 0 |
| 0x14 | u8 | `field_0x14` | unknown |  | 0..14 (6 distinct) | 0, 6, 8, 10, 12 |
| 0x15 | u8 | `field_0x15` | unknown |  | 0..20 (3 distinct) | 0, 10, 20 |
| 0x16 | u8 | `field_0x16` | unknown |  | 0..100 (10 distinct) | Equal to +0x17 in every record |
| 0x17 | u8 | `field_0x17` | unknown |  | 0..100 (10 distinct) |  |
| 0x18 | f32 | `field_0x18` | inferred |  | -0.2..50.0 (25 distinct) | 0, 10..50 (knock-back?) |
| 0x1C | f32 | `field_0x1C` | inferred |  | -35.0..50.0 (113 distinct) | Knock-back speed? |
| 0x20 | f32 | `field_0x20` | inferred |  | -50.0..30.0 (100 distinct) |  |
| 0x24 | f32 | `field_0x24` | inferred |  | -1.0..1000.0 (59 distinct) |  |
| 0x28 | f32 | `field_0x28` | inferred |  | 0.0..1000.0 (46 distinct) |  |
| 0x2C | a32 | `field_0x2C` | unknown |  | 0x0 0xffff 0x3d4ccccd 0x3c23d70a 0x3f19999a 0x3e4ccccd 0x3dcccccd 0x3ecccccd (14 distinct) | 0xFFFF or a small float |
| 0x30 | a32 | `field_0x30` | inferred |  | 0xffffffff (1 distinct) | Always -1 |
| 0x34 | char[16] | `name` | verified |  |  | Debug name |

#### `comb` entry (`{u16 version 6, s16 count, u32 offset[count]}`; entry then `stages` x 8-byte stage)

| off | type | name | status | read by | seen | meaning |
|---|---|---|---|---|---|---|
| 0x00 | s16 | `command` | verified | battle:0x08AB7F40 |  | Ability command id (system.bin ablt +0x0C); entry enabled when equipped |
| 0x02 | s16 | `stages` | verified | battle:0x08AB81E0 |  | Stage count |
| 0x04 | s16 | `alt_command` | verified | battle:0x08AB7F40 |  | Second command that also enables the entry, -1 none |
| 0x06 | s16 | `prerequisite` | unknown |  |  | -1 in the data seen |

#### `comb` stage (8 bytes)

| off | type | name | status | read by | seen | meaning |
|---|---|---|---|---|---|---|
| 0x00 | u16 | `attack` | verified | battle:0x08AB81E0 |  | Attack id |
| 0x02 | s16 | `min_frame` | verified | battle:0x08AB81E0 |  | Previous stage frame from which this one is accepted |
| 0x04 | s16 | `max_frame` | verified | battle:0x08AB81E0 |  | ... until this frame (<0: to the end) |
| 0x06 | s8 | `start_effect` | verified | battle:0x08B382B8 |  | Effect id spawned when the stage starts, 0 none |
| 0x07 | u8 | `field_0x07` | unknown |  |  |  |

#### `magi` projectile record (0xCC bytes after `{'MAGI', u32 0x20101011, u32 count, u32 floats}`; float pool after the records)

| off | type | name | status | read by | seen | meaning |
|---|---|---|---|---|---|---|
| 0x00 | u16 | `id` | verified | main:0x08828AE8 | 0..2277 (1283 distinct) | Projectile id (binary search) |
| 0x02 | u16 | `bullet_class` | verified | battle:0x08AD43B4 | 0..4 (5 distinct) | 4: bullet class B (0x140 bytes, battle:0x08AD7838), else class A (0x110) |
| 0x04 | u16 | `behaviour` | inferred |  | 0..65535 (43 distinct) | Behaviour type (earlier report); reader not found |
| 0x06 | s16 | `effect` | partial | battle:0x08AD96F0 | -1..1 (3 distinct) | -1: own effect (battle:0x08AD3350); else library effect (main:0x0885A574) |
| 0x08 | s16 | `max_live` | verified | battle:0x08AD052C | 0..27 (12 distinct) | Bullets alive per emitter; extra ones are removed (flag 0x200000: newest) |
| 0x0A | s16[10] | `hits` | inferred |  |  | Hit record ids (-1 none) |
| 0x1E | s16 | `origin` | verified | battle:0x08AD3ECC | 0..12 (12 distinct) | Spawn point: 0 owner, 1/2 owner bone, 3/4 target, 8/9 lock target, 11/12 either, 13 partner |
| 0x20 | s16 | `origin_bone` | verified | battle:0x08AD3ECC | 0..402 (66 distinct) | Bone for origin 1/2 (main:0x08836648); arg for origin 6 and 10 |
| 0x22 | s16 | `direction` | partial | battle:0x08AD5D70 | 0..4 (5 distinct) | Initial direction mode (4 special) |
| 0x24 | s16 | `field_0x24` | partial | battle:0x08AD5D70 | 0..2 (3 distinct) |  |
| 0x26 | s16 | `target_mode` | partial | battle:0x08AD3ECC | 0..5 (5 distinct) | 5: aim at the bullet target |
| 0x28 | u32 | `field_0x28` | unknown |  | 0..3 (4 distinct) |  |
| 0x2C | f32 | `field_0x2C` | partial | battle:0x08AD5D70 | 0.0..10.0 (51 distinct) |  |
| 0x30 | f32 | `field_0x30` | unknown |  | 0.0..40.0 (31 distinct) |  |
| 0x34 | f32 | `field_0x34` | unknown |  | 0.0..1.0 (12 distinct) |  |
| 0x38 | x32 | `magi_flags` | partial | battle:0x08AD5D70, battle:0x08AD96F0 | 0x40503 0x40403 0x40603 0x10040008 0x10040408 0x4040b 0x13 0x104003b (431 distinct) | Bits 0x200 no own effect, 0x1000, 0x20000, 0x200000, 0x2000000, 0x10000000 |
| 0x3C | x8[4] | `field_0x3C` | unknown |  |  |  |
| 0x40 | u8[138] | `params` | verified | battle:0x08AD0694 |  | 46 x {min, max, step} indices into the float pool; value random in [min,max] |
| 0xCA | u16 | `pad_CA` | inferred |  | 0..0 (1 distinct) |  |

#### Motion table entry (kind 7; `{u32 version 2, u32 count}`, 8-byte entries sorted by motion id)

| off | type | name | status | read by | seen | meaning |
|---|---|---|---|---|---|---|
| 0x00 | u16 | `motion` | verified | main:0x0882F0D4 |  | Motion id (binary search, main:0x08934944) |
| 0x02 | u16 | `anim` | verified | main:0x0882F0D4 |  | Animation index in the model; also keys the event track (main:0x088298E4) |
| 0x04 | u8 | `variant` | verified | main:0x08829344, main:0x08828EEC |  | Events with a non-zero variant byte run only for this value |

#### Collision shape (kind 5; `{u64 default-on mask, u32 count, u32 0}`, 0x20-byte records)

| off | type | name | status | read by | seen | meaning |
|---|---|---|---|---|---|---|
| 0x00 | u32 | `id` | verified | main:0x08837D18 |  | Shape id: hit and shape_on/shape_off events name it; every record whose first byte equals it is switched (main:0x08837D18 tests *(u8 *)record); several records may share one id; 0..63 switchable |
| 0x04 | u8 | `class` | verified | main:0x08831808 |  | 0 body, 1 hurt, 4 attack (by data), 6 clash (main:0x08809F00); 2,3,5,7,8,10,11,13..15 |
| 0x05 | u8 | `kind` | partial | main:0x08812664 |  | Geometry type for the overlap test (main:0x08812D48); 3 = sphere, always 3 |
| 0x06 | u8 | `bone` | verified | main:0x08812664 |  | Model bone the sphere follows (u16 at +0x06 with flags: bone \| 0x4000; 0x4000 alone = the character's own frame, main:0x088369A4) |
| 0x07 | x8 | `shape_flags` | verified | main:0x088369A4 |  | 0x40: sets 0x4000 in the bone value (with bone 0: the offset is in the character's frame, on the floor, +z forward) |
| 0x08 | f32 | `x` | verified | main:0x08812664 |  | Sphere centre offset x in the bone frame |
| 0x0C | f32 | `y` | verified | main:0x08812664 |  | Offset y |
| 0x10 | f32 | `z` | verified | main:0x08812664 |  | Offset z |
| 0x14 | f32 | `w` | verified | main:0x08812664 |  | Subtracted from the centre height; always 0 |
| 0x18 | f32 | `radius` | verified | main:0x08812664 |  | Radius |
| 0x1C | f32 | `radius2` | inferred |  |  | Second radius, equal to radius in the data |

#### Ranged event header (12 bytes: `u16 start, u16 end, u8 type, u8 argc`, then the bytes below, then argc words)

| off | type | name | status | read by | seen | meaning |
|---|---|---|---|---|---|---|
| 0x06 | u8 | `variant` | verified | main:0x08828EEC |  | Run only when the motion table variant equals this (0 always) |
| 0x07 | u8 | `mode` | verified | main:0x08828EEC |  | 0 always, 1 normal, 2 EX mode, 3 type-2 flag, 4 EX without sub-flag, 5 not (EX w/o sub-flag), 6 assist |
| 0x08 | u8 | `form` | verified | main:0x08828EEC |  | 0 always, else form index + 1 (char+0xD60) |
| 0x09 | u8 | `costume` | verified | main:0x08828EEC |  | 0 always, else costume index + 1 (char+0x1079) |
| 0x0A | u16 | `pad_0A` | inferred |  |  | Always 0 |

#### Instant event header (8 bytes: `u16 frame, u8 type, u8 argc`, then the bytes below, then argc words)

| off | type | name | status | read by | seen | meaning |
|---|---|---|---|---|---|---|
| 0x04 | u8 | `variant` | verified | main:0x08829104 |  | As the ranged variant byte |
| 0x05 | u8 | `mode` | verified | main:0x08829104 |  | As the ranged mode byte |
| 0x06 | u8 | `form` | verified | main:0x08829104 |  | As the ranged form byte |
| 0x07 | u8 | `costume` | verified | main:0x08829104 |  | As the ranged costume byte |
<!-- /FIELDS -->

`plya +0x18` flag bits seen in code: 0x1 approach first (`battle:0x08A60BC8` with `+0x40..+0x5C`, then
state 0x0F); 0x2 skip the range test; 0x10 counts as an HP-type attack for slot class 4 and lets turn
events track a guarding target; 0x80 HP attack (selects slot classes 1/3/5); 0x100 passed to the
follow-up tests `battle:0x08B2FD90`/`battle:0x08B2FF2C`; 0x400 special action (`+0x1C` 1..6 -> states 0x1B..0x20,
no motion); 0x800 scripted `chrg` action (`+0x1C` id -> char+0xD40, `main:0x08811768`); 0x2000 stance
variant; 0x4000 allowed while the selector's +0x81 is set; 0x10000 no input-condition requirement,
otherwise one of 0x20000..0x400000 must match selector bits 0x2..0x40 (`main:0x08848160`);
0x8000000/0x10000000/0x20000000 approach options (no rise / no fall / 0x40).

`magi` parameters: the 46 triples at `+0x40` are `{min, max, step}` indices into the float pool; at
spawn each value is random in `[pool[min], pool[max]]` (`battle:0x08AD0694`), stored at emitter
+0x44 + 4 x i, and copied into each bullet (`battle:0x08AD46F0`): i = 10, 12, 11 (degrees) -> bullet
+0x40, +0x44, +0x48; 30 -> life frames +0x4C/+0x5C; 31 -> +0x58/+0x60; 13 -> +0xCC; 14..19 -> +0x8C..+0xA0
(19 in degrees); 20..29 -> +0xA4..+0xC8; 37..45 -> +0xD0..+0xF0. What the bullet update does with them
was not read; typical values: 25..27 = 1 (scales), 34/36/44 = 9999.

---------------------------------------------------------------------------------------------------

## 6. Events

Machine-readable copy (handler, argument names and types, status, per-fighter counts, longer notes):
`events.tsv`. Counts are over the 32 main archives; argument counts are constant per type.

<!-- EVENTS -->
### Ranged events (`main:0x08829930`, table `main:0x089C7910`, 125 entries)

| type | name (arguments) | handler | status | uses / fighters | what it does |
|---|---|---|---|---|---|
| 0x00 | `hit` (atkp, shape) | `main:0x08829A80` | verified | 1606 / 32 | Arm the collision shapes with id `shape` (every kind-5 record whose first byte is `shape`) as hitboxes dealing hit record atkp while the window is open |
| 0x01 | `follow_up` (buffer) | `main:0x08829C00` | flag | 354 / 29 | Follow-up input accepted (motion+0x60 \|= 4; \|= 8 when buffer != 0) |
| 0x02 | `turn_to_target` (max_deg, only_if_reeling, through_guard, not_if_reeling) | `main:0x08829EF8` | verified | 1567 / 32 | Turn toward the attack target (char+0x2FC) by up to max_deg per frame |
| 0x05 | `actionable` | `main:0x08829A58` | flag | 529 / 32 | Free: any action may be started (motion+0x60 \|= 1) |
| 0x06 | `invincible` | `main:0x0882A758` | flag | 520 / 32 | Invincible / guard state (motion+0x60 \|= 0x20) |
| 0x0A | `airborne` | `main:0x08829A6C` | verified | 1037 / 32 | Treat the fighter as airborne (motion+0x60 \|= 2) |
| 0x0B | `turn_to_lock_target` (max_deg, a1, through_guard, not_if_reeling) | `main:0x0882A08C` | verified | 131 / 32 | As turn_to_target, toward the lock-on target (char+0x300) |
| 0x0C | `camera_mode2` (a0, a1, a2) | `main:0x0882A818` | partial | 269 / 31 | Drive the fighter camera mode-2 object (battle:0x08B3E840(a0), battle:0x08B3E864(a1)); motion+0x61 \|= 1 |
| 0x0D | `freeze_state` | `main:0x0882A7BC` | verified | 0 / 0 | Suspend own state-machine update (motion+0x60 \|= 0x80) |
| 0x0E | `hold_target` | `main:0x0882A7D0` | verified | 0 / 0 | Suspend the attack target's state update (target obj+0x380 \|= 4) |
| 0x10 | `turn_to_input` (max_deg, flag) | `main:0x0882A3A8` | partial | 68 / 32 | Turn toward the stored direction char+0xF0 by up to max_deg (0..180) |
| 0x11 | `turn_or_camera_target` (max_deg, flag, camera, c0, c1, c2, c3, c4) | `main:0x0882A418` | partial | 173 / 32 | camera=0: turn toward attack target centre (max_deg); camera!=0: scripted camera on it |
| 0x12 | `shape_on` (shape) | `main:0x088299E8` | verified | 1337 / 32 | Enable the collision shapes with id `shape` (record byte 0) for this frame (obj+0x448), no hit record |
| 0x13 | `intangible` | `main:0x0882A984` | flag | 184 / 32 | No hurtbox: cannot be hit (motion+0x62 \|= 2) |
| 0x14 | `face` (channel, state) | `main:0x0882A998` | partial | 3096 / 26 | Set face texture animation: channel 0 left eye, 1 right eye, 2 lips; state index |
| 0x15 | `ground_follow` | `main:0x0882A9B4` | flag | 706 / 32 | Motion flag 0x62 bit 4 (main:0x088379B8) |
| 0x16 | `swing_angle` (chain, deg) | `main:0x0882A9C8` | partial | 592 / 24 | Swing-bone (kind-6) chain whose id is chain: drive it to angle deg |
| 0x17 | `swing_reset` (chain) | `main:0x0882ABAC` | partial | 5 / 3 (Sephiroth, Bartz, Firion) | Swing-bone chain chain: set +0x151 (reset/hold) |
| 0x18 | `air_control` (a0, a1, a2, a3, steer) | `main:0x0882ACAC` | partial | 98 / 19 | Enter air state 0x12; char+0x3D0 = a0, +0x3D4 = a1 (air motion parameters); flag bits |
| 0x19 | `match_target_height` (max_speed, offset) | `main:0x0882AE0C` | verified | 108 / 30 | Move vertically toward the target's height + offset at up to max_speed |
| 0x1A | `actionable_ex` | `main:0x0882AEA4` | flag | 1 / 1 (FeralChaos) | motion+0x60 \|= 1 and 0x62 \|= 0x80 |
| 0x1B | `prop_on` (part) | `main:0x0882AEC4` | partial | 298 / 11 | Attachment manager char+0x3E4: switch item part on (main:0x088476AC) |
| 0x1C | `prop_off` (part) | `main:0x0882AEE0` | partial | 247 / 17 | Attachment manager char+0x3E4: switch item part off (main:0x08847758) |
| 0x1D | `landing` | `main:0x0882AEFC` | partial | 128 / 32 | Unless in state 10: enter state 10 for the rest of the window |
| 0x1E | `cancel_window` (any, a1, into_other, into_same) | `main:0x0882AF34` | flag | 586 / 32 | Cancel window (0x63 \|= 1, 0x60 \|= 1) with what may cancel it |
| 0x20 | `follow_up_if_target` (atkp) | `main:0x08829E0C` | flag | 335 / 32 | motion+0x60 \|= 0x10 when the lock target is under hit record atkp (on-hit continuation) |
| 0x21 | `nop_21` | `main:0x0882B1C4` | verified | 0 / 0 | No effect (handler is an empty case) |
| 0x22 | `armor_off` | `main:0x0882B1CC` | flag | 851 / 32 | From here an armoured attack can be interrupted (motion+0x63 \|= 4) |
| 0x23 | `flag_63_10` | `main:0x0882B1F4` | flag | 54 / 32 | motion+0x63 \|= 0x10 |
| 0x24 | `slot_4ac` (a0, a1, a2, slot, a4) | `main:0x0882B208` | unknown | 11 / 5 | motion+0x63 \|= 0x20; once per slot write {a0,a1,float a2,a4} to char+0x4AC[slot] |
| 0x25 | `air_params` (a0, a1) | `main:0x0882ADE4` | partial | 3 / 1 (Squall) | char+0x3D0/+0x3D4 = a0/a1 without the state change; 0x62 \|= 0x20 |
| 0x26 | `motion_value` (value) | `main:0x0882B254` | unknown | 239 / 31 | motion+0x88 = value; motion+0x63 \|= 0x40 |
| 0x27 | `swing_angle_by_index` (chain, deg) | `main:0x0882AAE4` | partial | 947 / 21 | As swing_angle, chain found by main:0x0880CD9C |
| 0x28 | `late_cancel` | `main:0x0882B2D0` | flag | 758 / 32 | motion+0x64 \|= 4 (only on attacks, late in the motion) |
| 0x29 | `flag_63_08` | `main:0x0882B1E0` | flag | 226 / 32 | motion+0x63 \|= 8 (attacks only) |
| 0x2A | `trail` (slot, group, index, scale) | `main:0x0882A8B0` | partial | 1439 / 32 | Keep a weapon-trail type effect alive in slot (0..15) while open; group 9999 or 8888 |
| 0x2B | `burst_signal` (unused) | `main:0x0882B2BC` | partial | 1 / 1 (Squall) | EX-burst manager +0x280 = 1 (battle:0x08A3EAB4) |
| 0x2C | `feat_particles` (count, chance, bone0, bone1, bone2, bone3, ex_only) | `main:0x0882B4DC` | verified | 62 / 4 | Scatter 'feat' particles on up to 4 bones (-1 ends), count*dt/2 tries at chance % |
| 0x2D | `flag_64_08` | `main:0x0882B6E8` | flag | 279 / 32 | motion+0x64 \|= 8 |
| 0x2E | `list_76c` (id, value) | `main:0x0882AC74` | unknown | 4 / 3 (Cloud, Squall, WoL) | Fighter: entries of char+0x76C with id get value (main:0x0884BB00) |
| 0x2F | `flag_64_10` | `main:0x0882B6FC` | flag | 11 / 7 | motion+0x64 \|= 0x10 |
| 0x30 | `flag_64_20` | `main:0x0882B710` | flag | 27 / 10 | motion+0x64 \|= 0x20 |
| 0x31 | `flag_64_01` | `main:0x0882B294` | flag | 3 / 2 (Jecht, Squall) | motion+0x64 \|= 1 |
| 0x32 | `flag_64_40` | `main:0x0882B724` | flag | 108 / 32 | motion+0x64 \|= 0x40 |
| 0x33 | `battle_cue` (cue) | `main:0x0882B738` | unknown | 34 / 9 | If *main:0x089D85E8 == 1: battle:0x08AB40DC(cue) (scripted battle cue) |
| 0x34 | `burst_cue` (value) | `main:0x0882B274` | flag | 38 / 13 | motion+0x88 = value; motion+0x63 \|= 0x80 |
| 0x36 | `flag_64_80` | `main:0x0882B7A0` | flag | 0 / 0 | motion+0x64 \|= 0x80 |
| 0x37 | `flag_65_01` | `main:0x0882B7B4` | flag | 31 / 4 | motion+0x65 \|= 1 |
| 0x38 | `stop_vertical` | `main:0x0882B7F8` | partial | 39 / 4 | Zero the velocity vector at obj+0xC0 (main:0x08837A5C) |
| 0x39 | `pull_target` (speed, max_dist, min_dist, if_reeling, if_not_reeling) | `main:0x0882B808` | verified | 34 / 17 | Pull the lock target (and its partner) toward self when min_dist < distance < max_dist |
| 0x3A | `flags_65_3` | `main:0x0882B774` | flag | 138 / 32 | motion+0x65 \|= 1\|2, 0x64 \|= 0x80 |
| 0x3B | `set_3dc` (value) | `main:0x0882BAC4` | unknown | 4 / 4 | char+0x3DC = value |
| 0x3C | `brave_regen` (amount, percent) | `main:0x0882BAD4` | verified | 2 / 1 (Gabranth) | Add amount*dt bravery per frame (char+0x52C+0x18, 0..10000) unless ability 0x53 |
| 0x3D | `hide_shadow` | `main:0x0882BC04` | flag | 8 / 4 | motion+0x65 \|= 4 (main:0x08839D84 skips the drop shadow) |
| 0x3E | `flag_64_02` | `main:0x0882B2A8` | flag | 23 / 11 | motion+0x64 \|= 2 |
| 0x40 | `fade_out` (frames, hold, alpha_percent) | `main:0x0882BC18` | verified | 19 / 9 | Once per motion: fade the model to alpha_percent over frames (main:0x08834D30) |
| 0x41 | `break_lock_ons` | `main:0x0882BC9C` | verified | 0 / 0 | Every fighter targeting self loses its target (battle:0x08B2F22C) |
| 0x42 | `nop_42` | `main:0x0882BD34` | verified | 0 / 0 | No effect (empty case) |
| 0x43 | `hit_ex` (atkp, shape, x0, x1, has_more, x2, x3) | `main:0x08829B64` | partial | 120 / 14 | As hit, plus values stored in the hit instance (+0x20C, +0x210; +0x214/+0x218 if has_more == 1) |
| 0x44 | `tint` (r, g, b) | `main:0x0882B6A4` | verified | 0 / 0 | Model colour char+0x374 = (r,g,b,255) (battle:0x08B307BC) |
| 0x45 | `flag_65_20` (also) | `main:0x0882BD3C` | flag | 74 / 32 | motion+0x65 \|= 0x20 (and 0x68 \|= 8 if also) |
| 0x46 | `flag_65_40` | `main:0x0882BD74` | flag | 0 / 0 | motion+0x65 \|= 0x40 |
| 0x47 | `invincible_if_ability` | `main:0x0882A76C` | flag | 130 / 32 | motion+0x60 \|= 0x20 if ability 0x136 is equipped |
| 0x48 | `launch` (pitch_deg, yaw_deg, speed, no_target_only, homing_only) | `main:0x0882BDC4` | verified | 185 / 29 | Enter air state and add velocity along facing turned by pitch/yaw, speed*dt |
| 0x49 | `no_push` | `main:0x0882C380` | flag | 317 / 32 | No body push between fighters (motion+0x65 \|= 0x80, main:0x0883E3BC) |
| 0x4A | `flag_66_02` | `main:0x0882C394` | flag | 1 / 1 (Kain) | motion+0x66 \|= 2 |
| 0x4B | `heavy` | `main:0x0882C3A8` | verified | 49 / 16 | Push weight 1000 instead of 100 (motion+0x66 \|= 1, main:0x08838E24) |
| 0x4C | `flag_66_04` | `main:0x0882C3BC` | flag | 7 / 2 (Lightning, Jecht) | motion+0x66 \|= 4 |
| 0x4D | `flags_66_78` | `main:0x0882C3D0` | flag | 0 / 0 | motion+0x66 \|= 0x78 |
| 0x4E | `shape_off` (shape) | `main:0x08829A20` | verified | 0 / 0 | Disable the collision shapes with id `shape` (record byte 0) for this frame (obj+0x450) |
| 0x4F | `step` (yaw_deg, speed, no_target_only, homing_only) | `main:0x0882C1EC` | verified | 66 / 18 | Add velocity along facing turned by yaw_deg, speed*dt (main:0x08835034) |
| 0x50 | `turn_or_camera_lock` (max_deg, flag, camera, c0, c1, c2, c3, c4) | `main:0x0882A4CC` | partial | 0 / 0 | As turn_or_camera_target for the lock target (main:0x088364F4 / battle:0x08A62B04) |
| 0x51 | `armor` | `main:0x0882C408` | flag | 2 / 2 (Kain, Chaos) | Hits do not interrupt while attacking (motion+0x67 \|= 1, main:0x0883A150) |
| 0x52 | `flag_66_80` (a0, a1, a2, a3, a4, a5) | `main:0x0882C41C` | flag | 1 / 1 (Emperor) | motion+0x66 \|= 0x80 (args unused) |
| 0x53 | `shake` (frames, percent_a, percent_b) | `main:0x0882C488` | partial | 44 / 20 | Start one screen shake (main:0x088749A0) per window; defaults 30 frames, 100% |
| 0x54 | `to_ground` | `main:0x0882C544` | unknown | 0 / 0 | vtable+0x1DC(char, ground height char+0x3C0 - y) |
| 0x55 | `flag_60_40` | `main:0x0882A7A8` | flag | 7 / 2 (FeralChaos, Chaos) | motion+0x60 \|= 0x40 |
| 0x58 | `cancel_window_if` (any) | `main:0x0882B020` | flag | 2 / 1 (Jecht) | As cancel_window(any) when obj+0x384 & 0x40000 |
| 0x59 | `late_cancel_if_free` | `main:0x0882B2E4` | flag | 141 / 25 | As late_cancel while the lock target is not reeling |
| 0x5A | `sub_actor` (id, a1, a2, a3, a4, a5, a6) | `main:0x0882C570` | partial | 103 / 3 (Gilgamesh, Yuna, Bartz) | Spawn or drive attached sub-actor id (char+0xD64 manager, main:0x08850574 / main:0x0884F1C8) |
| 0x5B | `sub_actor_hit` (atkp, id, a2, a3) | `main:0x0882C868` | partial | 163 / 1 (Gilgamesh) | Sub-actor id: main:0x0884F324 (counts as a hit for main:0x08829344) |
| 0x5C | `camera_path_a` (a0, a1, a2, a3) | `main:0x0882C904` | partial | 0 / 0 | Scripted camera (battle:0x08A62660, char+0x1120); motion+0x67 \|= 4 |
| 0x5E | `sub_actor_cmd` (a0, id) | `main:0x0882C8A8` | partial | 217 / 1 (Gilgamesh) | Sub-actor id: main:0x0884F3E8(a0) |
| 0x5F | `sub_actor_trail` (slot, group, index, a3, id) | `main:0x0882A918` | partial | 194 / 1 (Gilgamesh) | As trail, on sub-actor id (battle:0x08B33CF4) |
| 0x60 | `sub_actor_set` (id, a1, a2) | `main:0x0882C8D4` | partial | 39 / 2 (Gilgamesh, Yuna) | Sub-actor id: main:0x0884F7C4(a1, a2) |
| 0x62 | `flag_67_08` | `main:0x0882CB9C` | flag | 33 / 9 | motion+0x67 \|= 8 |
| 0x63 | `camera_path_b` (a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) | `main:0x0882C980` | partial | 2 / 1 (Laguna) | Scripted camera (battle:0x08A627EC); fighters only |
| 0x64 | `camera_path_c` (a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) | `main:0x0882C9FC` | partial | 22 / 5 | Scripted camera (battle:0x08A62978); fighters only |
| 0x65 | `sub_actor_at` (id, a1, a2, a3, a4, a5, x, y, z, a9, a10, a11) | `main:0x0882C668` | partial | 23 / 2 (Yuna, Bartz) | As sub_actor, then place it at (x,y,z) in its owner frame |
| 0x66 | `effect_at_frame` (effect, life, frame, attach) | `main:0x0882CBB0` | verified | 53 / 9 | On frame: spawn effect (main:0x088A9DE8), +0xE4 = life; attach >= 0: main:0x088A6A38 |
| 0x67 | `chain` (buffer, plya, a2, a3) | `main:0x08829C38` | verified | 117 / 7 | Follow-up window that offers attack plya as the next attack (char+0x1028 list) |
| 0x68 | `global_7e0` (a0, a1) | `main:0x0882CC64` | unknown | 0 / 0 | Set two values of global +0x7E0 (main:0x08873594); reset on the last frame |
| 0x69 | `flag_67_10` | `main:0x0882B19C` | flag | 4 / 2 (Squall, Cloud) | motion+0x67 \|= 0x10 |
| 0x6A | `flag_67_20` | `main:0x0882B1B0` | flag | 69 / 17 | motion+0x67 \|= 0x20 |
| 0x6B | `sound_ramp` (from, to) | `main:0x0882CCA8` | partial | 1 / 1 (FeralChaos) | Interpolate a sound level from..to over the window (main:0x08896508) |
| 0x6C | `flag_67_40` (mode) | `main:0x0882B350` | flag | 9 / 1 (Kain) | motion+0x67 \|= 0x40 unless mode == 1 and not obj+0x380&0x100 |
| 0x6D | `turn_to_any_target` (max_deg, only_if_reeling, through_guard, not_if_reeling) | `main:0x0882A1E8` | verified | 110 / 31 | As turn_to_target, attack target or else lock target |
| 0x6E | `turn_or_camera_any` (max_deg, flag, camera, c0, c1, c2, c3, c4) | `main:0x0882A580` | partial | 1 / 1 (Ultimecia) | As 0x11/0x50 for whichever target exists |
| 0x6F | `camera_path_d` (a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) | `main:0x0882CA78` | partial | 0 / 0 | Scripted camera for assists (battle:0x08A627EC / battle:0x08A62978) |
| 0x70 | `ground_probe` | `main:0x0882B398` | unknown | 1 / 1 (Kain) | Fighter on the ground: ray test (main:0x0889BEB0), on hit battle:0x08B2F054 |
| 0x71 | `flag_67_80` | `main:0x0882CD4C` | flag | 0 / 0 | motion+0x67 \|= 0x80 (assist: also owner 0x197 \|= 0x80) |
| 0x72 | `flag_68_02` | `main:0x0882B00C` | flag | 6 / 1 (Exdeath) | motion+0x68 \|= 2 |
| 0x73 | `assist_turn_to_owner` (max_deg) | `main:0x0882A33C` | verified | 0 / 0 | Assist: turn toward its owner (char+0x1108) |
| 0x74 | `flag_68_10_if_ability` | `main:0x0882BD88` | flag | 128 / 32 | motion+0x68 \|= 0x10 if ability 0x41 equipped |
| 0x75 | `vector_1294` (a0, a1, a2, a3) | `main:0x0882CDA4` | unknown | 0 / 0 | Fighter: char+0x1298/0x129C/0x12A0/0x1294 = float(a0..a3) |
| 0x76 | `vector_1294_partner` (a0, a1, a2, a3) | `main:0x0882CDD8` | unknown | 0 / 0 | As 0x75 on the owner (assist) or partner (type 3) |
| 0x77 | `flag_68_20` | `main:0x0882CE70` | flag | 1 / 1 (Vaan) | motion+0x68 \|= 0x20 |
| 0x78 | `follow_up_if_hit` (buffer, atkp) | `main:0x08829CC0` | verified | 41 / 3 (Prishe, Firion, Bartz) | follow_up only once hit record atkp has connected (main:0x08839120); Firion (id 3) always |
| 0x79 | `chain_if_hit` (buffer, plya, a2, a3, atkp) | `main:0x08829D6C` | verified | 10 / 1 (Prishe) | As chain, only once atkp has connected |
| 0x7A | `cancel_window_ex` (any, not_ex, into_other, into_same) | `main:0x0882B090` | flag | 20 / 13 | As cancel_window; not_ex selects whether obj+0x380&0x100 must be clear or set |
| 0x7B | `hit_if` (atkp, shape) | `main:0x08829AE4` | verified | 3 / 1 (Exdeath) | As hit, only when obj+0x384 & 0x40000 |
| 0x7C | `flag_69_01` (value) | `main:0x0882B7C8` | flag | 128 / 32 | motion+0x69 \|= 1 and char+0x1270 = value |

Free: 0x03, 0x04, 0x07, 0x08, 0x09, 0x0F, 0x1F, 0x35, 0x3F, 0x56, 0x57, 0x5D, 0x61, >=0x7D (the first ones dispatch to the no-op default; the last line is the range above the bounds check).

### Instant events (`main:0x0882CEC0`, table `main:0x089C7B08`, 56 entries)

| type | name (arguments) | handler | status | uses / fighters | what it does |
|---|---|---|---|---|---|
| 0x00 | `end_action` | `main:0x0882CF1C` | verified | 256 / 32 | Return to a neutral state chosen by char+0x314+0xF4 (states 1,6,4,0xC,0xD,0xE,0xF) |
| 0x01 | `nop_01` | `main:0x0882CFFC` | verified | 0 / 0 | No effect |
| 0x02 | `end_motion` | `main:0x0882D004` | verified | 566 / 32 | Finish the motion now (motion+0x54 \|= 4) |
| 0x03 | `sound` (sound, not_3d, fixed) | `main:0x0882D014` | verified | 3933 / 32 | Play sound id (+50000 for an assist object); fixed != 0: at the current position, else follows |
| 0x04 | `projectile` (magi, param) | `main:0x0882D2DC` | verified | 842 / 31 | Spawn projectile emitter magi id (vtable+0x11C -> battle:0x08B304C4 -> battle:0x08AD0978) |
| 0x06 | `shake` (frames, percent_a, percent_b) | `main:0x0882D244` | partial | 308 / 31 | One screen shake (main:0x088749A0) |
| 0x08 | `direct_hit` (atkp) | `main:0x0882D5F0` | partial | 36 / 8 | Queue hit record atkp on the target without a collision test (table 0x09AF84F0) |
| 0x09 | `fade_in` (frames) | `main:0x0882D56C` | verified | 1 / 1 (Emperor) | Fade the model in over frames (char+0x3B4) |
| 0x0A | `fade_out` (frames, alpha_percent) | `main:0x0882D58C` | verified | 0 / 0 | Fade the model to alpha_percent over frames |
| 0x0B | `part_show` (part) | `main:0x0882D670` | verified | 52 / 3 (Vaan, Sephiroth, Emperor) | Show model part (main:0x0886CB64(model, part, 1)) |
| 0x0C | `part_hide` (part) | `main:0x0882D688` | verified | 573 / 2 (Vaan, Prishe) | Hide model part |
| 0x0D | `nop_0d` (a0, a1, a2) | `main:0x0882D6A0` | verified | 0 / 0 | No effect (main:0x0884E658 is empty) |
| 0x0F | `ground_contact` (kind, a1, a2, a3, a4, a5) | `main:0x0882D6D0` | partial | 1575 / 32 | Ground-contact sound and dust by stage material (main:0x0883DA88, main:0x08853E24) |
| 0x10 | `home` (speed, frames, height, angle_deg, distance, arrive, total, v_speed, no_up, no_down, f40, f80, to_owner) | `main:0x0882D8C4` | verified | 243 / 26 | Homing move toward a point near the target (battle:0x08A60BC8); state 0x0F when airborne |
| 0x11 | `sound_if_part` (part, sound, fixed) | `main:0x0882DC70` | verified | 146 / 2 (Squall, Cloud) | Play sound if model part is shown |
| 0x12 | `sound_if_set` (set, sound, fixed) | `main:0x0882DC98` | partial | 136 / 4 | Play sound if the model set index is set (main:0x0886B1DC) |
| 0x14 | `nop_14` (a0) | `main:0x0882DCC0` | verified | 0 / 0 | No effect (battle:0x08B33F84 is empty) |
| 0x15 | `voice` (voice, a1, a2) | `main:0x0882DCE8` | verified | 2304 / 32 | Say voice clip (bank*100+slot; +10000 manikin, +30000 alt, +50000 assist) |
| 0x16 | `nop_16` | `main:0x0882DD84` | verified | 0 / 0 | No effect |
| 0x17 | `cart_effect` (bone) | `main:0x0882DD8C` | partial | 66 / 2 (Laguna, Squall) | Programmatic 'cart' effect (Squall, Laguna cartridges), main:0x088525F8 |
| 0x19 | `effect` (effect, life) | `main:0x0882DFE4` | verified | 625 / 26 | Spawn effect from the own effect package, attached (main:0x088A9DE8); +0xE4 = life |
| 0x1B | `screen_fade` (in, hold, out, colour, unused) | `main:0x0882D05C` | verified | 18 / 7 | Full-screen fade: colour 0 white, 1 black, 2 red |
| 0x1C | `alpha` (percent) | `main:0x0882D5C4` | verified | 4 / 1 (Emperor) | Set model alpha now |
| 0x1D | `banner` (text, variant) | `main:0x0882DD24` | verified | 22 / 11 | Show the name banner with string text of the text chunk (main:0x08856D48) |
| 0x1E | `form` (form) | `main:0x0882DD5C` | verified | 9 / 2 (Cecil, OK) | Change form (battle:0x08B33DDC; Cecil, Onion Knight) |
| 0x1F | `target_fade_in` (frames) | `main:0x0882DDB8` | verified | 5 / 4 | fade_in on the lock target |
| 0x20 | `target_fade_out` (frames, alpha_percent) | `main:0x0882DE08` | verified | 6 / 4 | fade_out on the lock target |
| 0x21 | `target_alpha` (percent) | `main:0x0882DE70` | verified | 37 / 11 | alpha on the lock target |
| 0x22 | `foot_dust` (bone, a1, a2, a3) | `main:0x0882D87C` | partial | 1618 / 31 | Queue a ground dust/footprint at bone (main:0x08853E80, char+0xD08) |
| 0x23 | `go_airborne` (unless_target) | `main:0x0882E214` | verified | 123 / 32 | Enter air state 0x12 (not when unless_target and a target exists) |
| 0x24 | `hold_facing` | `main:0x0882E280` | partial | 2 / 2 (Cecil, Garland) | char+0x320 (wanted yaw) = char+0x9C (current) |
| 0x25 | `dash` (speed, frames, angle_deg, distance, arrive, total, f40, f80, to_owner) | `main:0x0882D9E8` | verified | 80 / 19 | Homing dash toward the attack target, state 0x0F |
| 0x26 | `warp_to_target` (angle_deg, distance, height) | `main:0x0882E290` | verified | 1 / 1 (Chaos) | Teleport to the opponent's position; offset from its facing set as velocity |
| 0x27 | `home_fixed` (speed, frames, height, angle_deg, distance, arrive, total, v_speed, no_up, no_down, f40, f80, to_owner) | `main:0x0882DAA0` | partial | 0 / 0 | As home with p11 = 1: goal from the origin, not from a target (no stock use) |
| 0x28 | `dash_fixed` (speed, frames, angle_deg, distance, arrive, total, f10, f20, f40, f80, to_owner) | `main:0x0882DBA4` | partial | 0 / 0 | As dash with p11 = 1 (no stock use) |
| 0x29 | `projectile_if_brave` (magi, param, low, high) | `main:0x0882D434` | verified | 0 / 0 | projectile when low <= brave (char+0x52C+0x14) < high |
| 0x2B | `face_lock_target` | `main:0x0882DECC` | verified | 14 / 14 | Set wanted yaw toward the lock target (main:0x08835BA4) |
| 0x2C | `follow_motion_after` (frames) | `main:0x0882E4C8` | partial | 1 / 1 (Chaos) | After frames: play the attack record second motion plya+0x10 (battle:0x08B2EF88) |
| 0x2D | `projectile_either` (magi_a, param_a, magi_b, param_b) | `main:0x0882D4C4` | verified | 5 / 1 (Tidus) | projectile a, or b when random_gate fired |
| 0x2E | `random_gate` (level, bit, one_in) | `main:0x0882E50C` | verified | 4 / 1 (Tidus) | With 1/one_in chance (and level, setup bit): motion+0x54 \|= 0x40 |
| 0x2F | `effect_either` (effect_a, life_a, effect_b, life_b) | `main:0x0882E02C` | verified | 2 / 1 (Tidus) | effect a, or b when random_gate fired |
| 0x30 | `effect_on_sub_actors` (effect, life, id) | `main:0x0882E0D4` | verified | 0 / 0 | effect on each sub-actor with id |
| 0x31 | `set_6dc` (value) | `main:0x0882E61C` | unknown | 18 / 1 (Lightning) | Fighter: char+0x6DC = value |
| 0x32 | `camera_reset` | `main:0x0882E640` | partial | 3 / 1 (Laguna) | Scripted camera update/reset (battle:0x08A62DD8) |
| 0x33 | `projectile_if_charge` (magi, param, level, at_most, equal, at_least) | `main:0x0882D340` | verified | 6 / 1 (Laguna) | projectile when charge level char+0x107F compares with level |
| 0x35 | `assist_effect` (effect, life, attach) | `main:0x0882E188` | partial | 0 / 0 | Sub-actor objects: effect from the owner package |
| 0x36 | `call_b34f40` (a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) | `main:0x0882E660` | unknown | 4 / 1 (OK) | battle:0x08B34F40(...) |
| 0x37 | `clear_target_188` | `main:0x0882E6B8` | unknown | 1 / 1 (Ultimecia) | Lock target obj+0x188 &= ~0x10 |

Free: 0x05, 0x07, 0x0E, 0x13, 0x18, 0x1A, 0x2A, 0x34, >=0x38 (the first ones dispatch to the no-op default; the last line is the range above the bounds check).

<!-- /EVENTS -->

Still unknown or only flagged (no effect established): ranged 0x15, 0x23, 0x24, 0x26, 0x29, 0x2D, 0x2E,
0x2F, 0x30, 0x31, 0x32, 0x33, 0x36, 0x37, 0x3A, 0x3B, 0x3E, 0x4A, 0x4C, 0x4D, 0x52, 0x54, 0x55, 0x62,
0x68, 0x69, 0x6A, 0x6C, 0x70, 0x71, 0x72, 0x75, 0x76, 0x77, 0x7C (most set one flag bit whose reader was
named but not read); instant 0x31, 0x36, 0x37. The meanings of 0x06 (invincible), 0x15 (ground follow),
0x22 (armor off), 0x28 (late cancel) rest on which motions carry them; they are the most common flags
and the best candidates for a runtime check.

---------------------------------------------------------------------------------------------------

## 7. What the data can express (with a stock example each)

| mechanic | how | stock example (fighter, record, animation) |
|---|---|---|
| melee hit window | R 0x00 `{atkp, shape}` over frames; shape = a shape id whose class-4 records sit on the weapon bone | Squall `NEAR` (plya 68, motion 10000, anim 43): R 0x00 [10..16) atkp 97 shape 46 in normal mode, R 0x43 atkp 1442 in EX |
| multi-hit | several R 0x00 windows; a window keeps one hit instance per (atkp, shape) (`main:0x0883E108`), and stock data repeats the same pair in separate windows to hit again (inferred: once per target per window); or `atkp +0x12` re-hit | Squall `NEAR_COMBO4` (72, anim 47): atkp 103 in [12..18) and [24..30), 104 in [42..48); Cloud `LAND_FINISH3` (1486, anim 61): six windows |
| combo chain | `comb` stages + R 0x01 follow-up window, or R 0x67 `chain` offering a specific next attack | Squall command 329 = attacks 68..72; Laguna `BR_A_SHOTGUN2` R 0x67 -> 2388 |
| continue only on hit | R 0x78 / 0x79 gate on a connected hit record; R 0x20 on the target's state | Squall anim 52 R 0x20 atkp 357 |
| projectile | I 0x04 `{magi}` spawns an emitter (max 32 live, list at `0x09AFB890`); origin and aim from `magi +0x1E/+0x22`; flight from the 46 parameters | Squall `MIDDLE` (77, anim 49) I 0x04 magi 68 at frame 32; `MAGIC_DEFO2` |
| conditional projectile | I 0x29 (bravery range), I 0x2D (random gate I 0x2E), I 0x33 (charge level) | Tidus anims 50/51; Laguna `BR_L_GRENADE1` (2386) |
| charge / hold | I 0x33 compares char+0x107F, reset at attack start, raised in `battle:0x08B3526C` (inferred: while the button is held) | Laguna `BR_L_GRENADE1`, `BR_A_LAUNCHER1` |
| approach / homing dash | `plya` flag 1 with `+0x40..+0x5C`, or I 0x25 `dash` / I 0x10 `home`; goal is a point at angle/distance/height from the target, reached at `speed` per tick | Squall `NEAR` I 0x25 at frame 0; Squall `AIR_NEAR` (84) I 0x10 |
| step, launch, rise | R 0x4F `step`, R 0x48 `launch` (pitch/yaw/speed), R 0x19 height match, R 0x18 air state | Squall `GROUND_UPPER` (1194, anim 53) R 0x48 40 deg; Ultimecia `SWORD_DEFO` R 0x18 |
| ground/air transitions | `plya +0x05/+0x06` requirements; R 0x0A airborne; I 0x23 go airborne; R 0x1D landing state; `+0x1E` queues idle 1 or air idle 101 | Squall `AIR_*` records |
| launch / chase | hit record reaction (`atkp +0x05`: 6 launch, 7 down) and chase records `kind` 2/3 | Squall `CHASE_ATTACK_UP/LO` (plya 95/96, motions 811/812) |
| pull / vacuum | R 0x39 pulls the lock target into a distance band | Squall `NEAR_FINISH` (75, anim 61) |
| grab / hold | R 0x0E suspends the target's state machine; I 0x08 applies a hit record without a collision test | I 0x08 in EX bursts (8102, 8500); R 0x0E has no stock use |
| armour, invincibility | R 0x51 armour, R 0x22 armour off, R 0x13 intangible, R 0x06 invincible | Chaos `BURST` (2055) R 0x51 |
| props, weapon swaps | I 0x0B/0x0C model parts; R 0x1B/0x1C attachments; R 0x5A sub-actors (Gilgamesh, Yuna, Bartz) | Vaan (572 x I 0x0C); Bartz `FF04_BR_A_THRUS` (3010) R 0x5A |
| presentation | I 0x03 sound, I 0x15 voice, I 0x19 effect, R 0x2A weapon trail, I 0x1B screen fade, R 0x53 / I 0x06 shake, R 0x14 face, I 0x1D / `plya +0x21` banner, fades and tint | throughout |
| forms / stances | I 0x1E form change; condition byte `form`; `plya +0x14` stance | Cecil, Onion Knight chase records |
| teleport | I 0x26 to the opponent only | Chaos `SPEAR_BOMB` (2056, motion 10081, anim 43): I 0x26 at 146 (height 8) then I 0x10 dive |

---------------------------------------------------------------------------------------------------

## 8. Moving a fighter to a projectile or a point (warp-strike)

**Stock:** no event moves a fighter to a projectile's position or to an arbitrary world point.

* Closest: **I 0x26 `warp_to_target`** (`main:0x0882E290`). It reads the attack target's position and
  calls `main:0x08835AA4(self, pos)`, which stores pos at char+0x2EC and sets char+0x388 bit
  0x10000000; the position integrator `main:0x0883B470` then moves char+0x80 there on the next physics
  step, shifts char+0x100 by the same delta and sets the camera's cut flag (camera+0x124 = 1). The
  offset (target facing turned by `angle_deg`, scaled by `distance`, plus `height`, or down to the
  ground when height <= -100) is then given to `main:0x08833AD8` as the new velocity; whether that
  ends as a one-tick offset or a drift was not settled. `main:0x08835AA4` has no other caller.
* Second closest: the homing move (I 0x10/0x25, `plya` flag 1, `battle:0x08A60BC8`): moves over
  several ticks to a point defined relative to a *fighter* (attack target, lock target or owner).
* Projectiles: an emitter is a 0x130-byte object from a pool of 32 (`battle:0x08AD0810`), linked from
  `0x09AFB890` through +0x120; owner char at +0x108, record at +0x24. Its bullets (0x110 or 0x140 bytes,
  vtable at +0xF8) hang from emitter +0x3C (head) / +0x40 (tail), next at +0xF4; a bullet's position is
  the vec4 at +0x00 (written by the spawn placement `battle:0x08AD46F0` -> `battle:0x08AD5D70`; previous
  position +0x10), its owner +0x68, its target +0x6C. Nothing in the event code reads them.

**Adding a new event type** (plugin, no table edit):

1. Type number: an instant type `>= 0x38` (or ranged `>= 0x7D`). The stock dispatchers bounds-check
   (`main:0x0882CEE4`, `main:0x088299C4`) and return, so an archive carrying the new event still runs
   on an unpatched game, just without the warp. Free entries inside the tables (instant 0x05, 0x07,
   0x0E, 0x13, 0x18, 0x1A, 0x2A, 0x34; ranged 0x03, 0x04, 0x07, 0x08, 0x09, 0x0F, 0x1F, 0x35, 0x3F, 0x56,
   0x57, 0x5D, 0x61) also no-op today, but a handler reached through the jump table runs inside the
   dispatcher's stack frame and must return through its epilogue (`main:0x0882E738` / `main:0x0882CE84`).
2. Hook: the walkers call the handler through a register loaded by two instruction pairs,
   `main:0x08828ECC lui t2,0x0883` + `main:0x08828EDC addiu t2,t2,-0x3140` (= `main:0x0882CEC0`,
   instant) and `main:0x08828EA0` + `main:0x08828EB0` (= `main:0x08829930`, ranged). Pointing them at a
   plugin function `handler(event *a0, motion *a1)` that handles its own types and otherwise calls the
   original is the whole hook. The owner is `*(motion+0x30)`; arguments are at event+8 (instant) or
   event+12 (ranged).
3. Warp to my projectile: find the newest emitter in the `0x09AFB890` list whose +0x108 is the owner
   (and whose record id `**(emitter+0x24)` matches an argument), take its tail bullet's +0x00 vec4, call
   `main:0x08835AA4(owner, &pos)`; optionally turn to the target (`main:0x08835F60(angle, owner, 0)`)
   and release the bullet (bullet vtable +0x0C with 3). Warp to a point: compute the point and call the
   same function. Applied by the stock integrator, so stage collision and the camera cut come for free.
4. To check at runtime before relying on it: the bullet position at +0x00 (inferred) and that the
   integrator accepts a warp into the air (the stock use warps onto the opponent then dives with I 0x10).

---------------------------------------------------------------------------------------------------

## 9. How to make a new attack (data side)

Everything an attack needs, outside code. Ids marked **unique** must not repeat within the fighter's
table; use ids above the stock maxima (attack 3035, hit record 3355, projectile 2277) so they are
unique game-wide as the shipped data is.

1. **Ability row** (`system.bin/ablt`, roster side): ability id (unique game-wide), command id at
   +0x0C, bravery/HP at +0x18, ground/air at +0x19, CP at +0x12; plus a learn-list row and the equipped
   set (save side). Without it no `comb` entry is enabled.
2. **`comb` entry**: command = the ability's command (unique per fighter; at most 256 entries), stage
   list = the attack ids of each step; frame windows `[min, max)` on the previous step, or `[0, -1)` and
   let the track's follow-up windows decide.
3. **`plya` record per step**: unique id (at most 192 usable records); `kind` 0 for the first step and
   1 for later steps; `self_position` matching the ability's ground/air; `slot_class` and flag 0x80
   agreeing with bravery/HP and the slot it is equipped to; `range_min/max`, `height_min/max`,
   `aim_min/max` admitting the situations it should start from (or flag 2); `motion`; `next_motion` 1
   (ground) or 101 (air) with blend 16; optional approach (flag 1 and `+0x40..+0x5C`); `banner` for HP
   attacks; a name.
4. **Motion table entry**: unique motion id (10000 and up for attacks, sorted), pointing at the
   animation index; the variant if one animation serves several records.
5. **Animation** in the model at that index (GMO motion block) and an **event track** keyed by the same
   index: hit windows (R 0x00 with a hit record id and a shape id < 64), turn toward target
   (R 0x02), follow-up window (R 0x01 or R 0x67) where the next step may start, cancel windows (R 0x1E,
   R 0x22, R 0x28) late in the motion, presentation events, and the end (end of motion, or I 0x00).
6. **Hit records** (`atkp`): unique ids, **kept sorted** (binary search); clash class, reaction,
   spark, arc, power and knock-back fields copied from a similar stock hit.
7. **Shapes**: class-4 records on the weapon bone (or in the character's frame) sharing a shape id
   below 64 as their first byte; the record count is free, the id is what must be unused or reused.
8. Optional: **projectiles** (`magi`, unique ids, sorted, plus float-pool entries), **effects and
   sounds** at indices/ids in the fighter's own package and bank, **voice** ids in its bank, **AI**
   group in `brat` for the command, **banner** string in `text`.

Consistency the game does not check: a missing motion id makes `battle:0x08B382B8` refuse the attack
(returns 0); a missing hit record id falls back to `battle.bin`'s table (`main:0x0880BE84`); a missing
projectile id spawns nothing; a missing event track means no events at all.

---------------------------------------------------------------------------------------------------

## 10. Not settled

* `atkp` beyond the clash class, reaction, spark, arc and re-hit bytes: damage, knock-back floats and
  hit flags are named by data pattern only. The damage code reads them through the hit instance
  (`main:0x08807860`, record pointer at +0x34); a trace of `main:0x0883A150` (take hit) is next.
* `magi`: the bullet update (bullet vtable `battle:0x08ADB2A0`, class B `battle:0x08AD7838`) and the
  meaning of `+0x04` and the 46 parameters.
* The flag-only events listed in section 6, and the selector fields (`main:0x08848160` param +0x78
  input bits, +0x7C slot class) that decide which equipped slot a button press maps to.
* I 0x26's velocity step.
* Suggested runtime checks (session `mod-camera-v1`): break on `main:0x08835AA4` during Chaos'
  `SPEAR_BOMB`; break in the instant dispatcher on a bullet-spawning attack and read emitter/bullet
  +0x00; flip R 0x06 / R 0x22 / R 0x28 windows in a copied track and watch guard, armour and cancels.

## 11. Scripts

`build/scratch/moveset_semantics/` (git-ignored): `fetch.py` (every fighter's archive from the ISO),
`common.py` (readers), `survey_events.py`, `survey_records.py`, `survey_shapes.py`, `where.py`,
`examples.py`, `track.py` (print a track by animation or motion id), `pcs.py` / `pchead.py` / `grepfn.py`
(Ghidra pseudocode queries), `flagreaders.py`, `atkp_reads.py`, `make_events.py`, `make_fields.py`,
`md_events.py`, `md_fields.py --splice` (regenerates the two tables of this page), `check_names.py`
(loads the names through `tools/moveset.py` and round-trips archives).

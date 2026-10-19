# Dissidia 012 (ULUS10566 v1.00): battle moveset system, static analysis

> Static-analysis report produced by a research subagent on 13 October 2026 and kept as written. Nothing in it has
> been confirmed at runtime.

Scope: how a fighter behaves in battle (class structure, attack definition, animation binding, input
mapping, EX mode / EX burst / assists, CPU AI, boss Chaos) and what a new moveset would cost.
Static analysis only: Ghidra pseudocode, `refs.tsv`, `tools/re_query.py`, raw reads of the two ELFs,
and read-only reads of retail data from the ISO. Nothing tracked was modified, no emulator was run.
Helper scripts and extracted samples are in `build/scratch/moveset/` (git-ignored); the package
reader in `build/scratch/assets/` (another investigator's) was imported read-only.

Addresses are prefixed `battle:` (OVL_BATTLE_APP.ELF) or `main:` (EBOOT.ELF).
"char" means the fighter object (0x12D0 bytes).

---------------------------------------------------------------------------------------------------

## 0. Answer in one paragraph

A fighter's moveset is **almost entirely data**. There is **one fighter class** for all 34
character IDs (plus a cosmetic "assist" subclass). Each fighter has a per-character container
`obj/p_xxxNNN.objx` (ARC) holding: an attack list (`plya`), combo/ability chains (`comb`), hit
parameters (`atkp`), projectile definitions (`magi`), AI weights (`brat`), a motion table (logical
motion ID to animation index), per-animation **timed event tracks** (hit windows, cancel windows,
effects, sounds, movement), collision shapes, effects and sounds. The animations themselves are
motions embedded in the character's own `.gmo` model. There is **no bytecode VM**: an attack is a
motion plus its event track, interpreted by two switch-dispatched functions in the **main
executable** (112 implemented "ranged" event types and 48 "instant" event types). Chaining between
attack stages is table-driven (`comb`). Character-specific *code* is small: a 32-bit feature mask
per character (EX-mode bonuses), a table-selected EX-burst mini-game type (20 selectable slots), a
data-driven "form/job" table (Lightning's paradigms, Cecil's jobs), and at least 16 overlay
functions that compare a hard-coded character ID. Bartz does not call other fighters' attacks at run time: his
"mimic" moves are separate records in his own files with his own animations and effect copies.
Skeletons differ between fighters, so animations are not portable without retargeting; the logic
data (tracks, hit parameters, attack records) is portable by copy and renumbering.

---------------------------------------------------------------------------------------------------

## 1. Character object and class structure (Q1)

### Verified

* **One fighter class.** `battle:0x08B39E5C` (size 1888) is the only fighter constructor. It calls
  the base object constructor `main:0x0883975C(obj, type, objentry)` and then stores the fighter
  vtable: `*(char+0x544) = 0x08B3A9A8` (data ref at `battle:0x08B39EF0`). Every creator allocates
  the same size, `main:0x08831320(0x12D0)`:
  `main:0x0888BA0C` (the two combatants; five `jal 0x08B39E5C` sites at `0x0888BAA8`, `0x0888BAF4`,
  `0x0888BB64`, `0x0888BBB4`, `0x0888BC00`), `main:0x08918E90`, `main:0x08919500`,
  `battle:0x08B43374`, and the assist creator below.
* **Vtable census** (vtable format: 8-byte entries `{s16 this_adjust, s16 pad, u32 fn}`, pointer
  stored at `obj+0x544`, 66 slots `+0x004..+0x20C`). Searching both modules for four base methods
  that are never overridden (`0x08839D84`, `0x08836D9C`, `0x08831690`, `0x08834274`) finds every
  vtable of this object family:
  * main: `0x089C6A20`, `0x089C7030`, `0x089C7248`, `0x089C76A8` (0x5A0-byte attached sub-objects
    such as weapons, constructor `main:0x08827854`), `0x089C7C78` (base), `0x089C8AD8`.
  * battle: **`0x08B3A9A8` fighter**, **`0x08A61CF8` assist**, `0x08AB71A0` (0x580-byte field
    object, base type 6, constructor `battle:0x08AB67DC`; not a character).
  So the battle overlay has **two character vtables**. The fighter overrides 51 of the 66 base
  slots.
* **The assist subclass is the fighter class with two extra flag bits.** `battle:0x08A61C3C`
  (92 bytes) calls `0x08B39E5C`, stores vtable `0x08A61CF8` and ORs `{0, 0x20}` (words at
  `battle:0x08A61F08`) into `char+0x388/+0x38C` (the fighter ORs `{0x4000, 0}` from
  `battle:0x08B3ACB0`). Its vtable differs from the fighter's in 12 slots: the destructor (`+0x0C`
  = `0x08A61C98`) and 11 accessors at `0x08A61BB0..0x08A61C38` which are **instruction-for-
  instruction copies** of the fighter's (`0x08A61BB0: jr ra / lwc1 f0,0xD3C(a0)` equals
  `0x08B30888`; `0x08A61BC0: ori a1,zero,1 / jr ra / sb a1,0x12A6(a0)` equals `0x08B34174`; and so
  on). No behaviour is overridden.
* **No factory, no per-character function-pointer table.** The character ID is read once in the
  constructor: `char+0x5A4 = main:0x088B70C8(*(s8*)(objentry+6))`, a binary search
  (`main:0x088B7080`, record size 0x90) in the `chrp` chunk of `system.bin` (loaded by
  `main:0x088BA88C`; tag bytes at `main:0x089CB898`). All later character-specific behaviour keys
  off that record:
  * `chrp+0x00` u8 character ID (1..34), `chrp+0x74` char[8] file stem (`P_SEV100`),
  * `chrp+0x04` row in the global stats table (`battle.bin/spec`, via `main:0x0884E58C`, result at
    `char+0x39C`),
  * `chrp+0x06/+0x07` EX-burst mini-game type and variant (section 6),
  * `chrp+0x08` form/job table index or 0xFF (section 3),
  * `chrp+0x0C` u32 **feature mask** (section 3),
  * `chrp+0x7C..` four u32 IDs in the character's effect/sound ID range.
* **Sub-objects built by the constructor** (generic, same for everyone): motion controller
  `char+0x130` (main), EX-mode controller `char+0x784` (`battle:0x08ACA490`), form table
  `char+0xD5C`, combo state `char+0xF70` (`battle:0x08AB8390`), AI brain `char+0x5C4` (0x1430
  bytes, `battle:0x08A637B8`), camera `char+0x5AC`.
* **Player action state machine is generic**: `battle:0x08A5D2D0` (2640 bytes) builds a table of
  33 state objects (IDs 0..0x20, one vtable each, for example `0x08A5C298`, `0x08A570C0`,
  `0x08A61710`); states are entered with `main:0x08834C10(char, stateId, arg)`.

### Character table (`system.bin/chrp`, 34 records, read from the ISO)

| ID | stem | who | `+0x0C` mask | `+6` burst type | `+8` form table |
|---|---|---|---|---|---|
| 1 | P_ONE100 | Warrior of Light | 0x00000002 | 3 | 3 |
| 2 | P_ONE200 | Garland | 0x00000010 | 0 | none |
| 3 / 4 | P_TWO100 / 200 | Firion / Emperor | 0x00000020 | 3 / 6 | none |
| 5 / 6 | P_THR100 / 200 | Onion Knight / Cloud of Darkness | 0 | 8 / 7 | 1 / none |
| 7 | P_FOR100 | Cecil | 0x00000408 | 4 | 0 |
| 8 | P_FOR200 | Golbez | 0 | 4 | none |
| 9 | P_FIV100 | **Bartz** | **0** | 3 | none |
| 10 | P_FIV200 | Exdeath | 0 | 7 | none |
| 11 / 12 | P_SIX100 / 200 | Terra / Kefka | 0x00000040 | 2 / 5 | none |
| 13 | P_SEV100 | **Cloud** | 0x00000801 | 0 | none |
| 14 | P_SEV200 | Sephiroth | 0x00000040 | 0 | none |
| 15 / 16 | P_EHT100 / 200 | Squall / Ultimecia | 0 | 9 | none |
| 17 | P_NIN100 | Zidane | 0x00000080 | 1 | none |
| 18 | P_NIN200 | Kuja | 0x00000002 | 1 | 5 |
| 19 | P_TEN100 | Tidus | 0x00000003 | 10 | 4 |
| 20 | P_TEN200 | Jecht | 0x00000004 | 10 | none |
| 21 | P_GST100 | Shantotto | 0x00000200 | 11 | none |
| 22 | P_GST200 | Gabranth | 0x00000002 | 12 | 2 |
| 23 | P_ORG200 | **Chaos** | 0x00008008 | 0 | 6 |
| 24 | P_FOR110 | Kain | 0 | 18 | none |
| 25 | P_FIV210 | Gilgamesh | 0x00010000 | 14 | none |
| 26 | P_SEV110 | Tifa | 0x00080000 | 15 | none |
| 27 | P_EHT110 | Laguna | 0 | 13 | none |
| 28 | P_TEN110 | Yuna | 0 | 16 | none |
| 29 | P_GST110 | Prishe | 0x00000002 | 17 | 8 |
| 30 | P_GST120 | Vaan | 0x00100000 | 12 | none |
| 31 | P_GST130 | **Lightning** | 0x00060000 | 3 | 7 |
| 32 | P_ORG210 | Feral Chaos | 0x00000002 | 19 | 9 |
| 33 | P_SEV120 | Aerith (assist only) | 0 | 0 | none |
| 34 | P_ORG800 | (no battle data) | 0 | 0 | none |

Character names are from the file stems (game knowledge); the numbers are read from the data.

---------------------------------------------------------------------------------------------------

## 2. How an attack is defined and executed (Q2)

### 2.1 Where the data lives (verified)

`main:0x08845230` loads, for an object entry (`objentry.bin`, 0x44-byte rows), the model
`obj/<name>.gmo`, costume `obj/<name>.cosx`, `obj/<name>.exex` and the container
`obj/<name2>.objx` (extension strings at `main:0x089C8850` "objx", `0x089C8898` "cosx",
`0x089C88A0` "exex", `0x089C88A8` "gmo"). `main:0x088459E0` (3144 bytes) parses the objx, an
`ARC\x01` container, into a 0xEC-byte "object data" struct that the fighter keeps at `char+0x390`:

| ARC kind / tag | object-data offset | meaning | evidence |
|---|---|---|---|
| kind 7 | `+0x1C` | **motion table**: sorted `{u16 motionId, u16 gmoAnimIndex, u8 variant, pad}` | `main:0x0882F0D4` bsearches it (`0x08934944`, 8-byte records) |
| kind 8 | `+0x20` | **event tracks**, one per GMO animation | `main:0x088298E4` finds the track whose first u16 equals the animation index |
| kind 5 | `+0x18` | collision shapes, 0x20-byte records | `main:0x0883E6E8` |
| kind 6 | `+0x24` | cloth/swing-bone physics | `main:0x0880D82C` (4596 bytes of bone math) |
| kind 9 (`hibg`) | `+0x30` | EX-burst background | used only by `battle:0x08ACDA90` from the burst code |
| kind 4 `effe` | `+0x34..` | effect package (MPK with `.fep/.feb/.fxseb`) | `main:0x0883EB10` |
| kind 14 `snd_` | `+0x60..` | sound bank (SSCF) | `main:0x088423A8` |
| kind 1 `plya` | `+0x88` | **attack list**, 0x74-byte records | `main:0x088480D0` |
| kind 1 `comb` | `+0x8C` | **ability to attack-chain table** | `battle:0x08AB7F40`, `0x08AB81E0` |
| kind 1 `atkp` | `+0x90` | **hit parameters**, 0x44-byte records | copied to `char+0x49C` by the base constructor |
| kind 1 `magi` | `+0x94..` | projectile definitions, 0xCC-byte records plus a float pool | `main:0x08828A98/0x08828AE8`, `battle:0x08AD0978` |
| kind 1 `brat` | `+0xA0..` | AI attack weights (magic `atta`) | `main:0x088105FC` |
| kind 1 `text` | `+0xC8` | attack names for the HUD | `main:0x0887AC24` |
| kinds 3/17/13 `spec` | `+0xD0` | EX-burst UI resources | `main:0x088CF188`; read by `battle:0x08A40684` |
| kind 3 `feat/pris/bubb/cart/mine/wind` | global managers at `*(0x089D4DC8)+...` | textures for programmatic effects | `main:0x08854A5C`, `0x08859DA4`, `0x08851C78`, `0x08852378`, `0x08857824`, `0x08856ACC` |
| kinds 2/18 `d_xx` | | a small secondary model | `main:0x08842F78` |

Tag bytes are at `main:0x089C8948..0x089C89BF`. Chunk lists read from the ISO for Cloud
(`p_sev100.objx`, 1,352,328 bytes), Bartz, Lightning, Warrior of Light, Feral Chaos, Chaos and two
assist containers all follow this layout.

**Animations** are motion blocks (GMO block type 0x000B) inside the character's own `.gmo`. Counted
from the ISO: Cloud 86 motions and 86 event tracks; Bartz 84 and 84; Chaos (NORMAL) 47 and 47;
Aerith's assist model 14 and 14.

### 2.2 The pipeline for one attack (verified end to end for Cloud's ground bravery combo)

1. **Ability** (equipped, section 5) has a *command ID*: `system.bin/ablt` record `+0x0C`.
   Cloud's ability 3329 has command 314 (0x13A).
2. **`comb`** maps the command to stages. Format: `{u16 version 6, s16 count, u32 offsets[count]}`;
   entry `{u16 commandId, s16 nStages, s16 altCommand, s16 prerequisite}` then 8-byte stages
   `{u16 plyaId, s16 minFrame, s16 maxFrame, u16 flags}`. Cloud's entry 0x13A is
   `3A 01 03 00 FF FF FF FF | BE 05 00 00 FF FF 00 00 | BF 05 .. | C0 05 ..` = attacks 1470, 1471,
   1472. `battle:0x08AB7F40` enables the entries whose command matches an equipped ability;
   `battle:0x08AB81E0` accepts a follow-up when `stage[i-1].plyaId == *(u16*)*(char+0x101C)`
   (current attack), `stage[i].plyaId == candidate`, and `minFrame <= char+0x1A8 (current frame)
   < maxFrame`.
3. **`plya`** record (0x74 bytes; header `{u32 version 0x16, u32 count}`), fields confirmed in code:
   `+0x00` u16 attack ID; `+0x03` u8 kind (0 opener, 1 follow-up, 2 chase, 3 counter-chase by
   name); `+0x05/+0x06` self/target ground-or-air requirement (`main:0x08847F10`); `+0x07` a number
   handed to `main:0x088A9DE8` when above 0 (not a voice clip, as first written here: it is 0 in
   all of Squall's records, and voice comes from the event track, see I 0x15 below); `+0x0F` slot-class mask; `+0x18` u32 flags; `+0x1C` u16 **motion ID**; `+0x1E` s16 second
   motion; `+0x21` s8 programmatic-effect ID; `+0x28` floats (reach, min/max height difference:
   7.5, -1, 1 for `LAND_NEAR1`); `+0x40..+0x5C` approach parameters; `+0x64` char[16] debug name.
   Cloud has 33: `LAND_NEAR1` (1470, motion 10000), `LAND_NEAR2` (1471, 10001), `LAND_NEAR3`
   (1472, 10002), `LAND_MIDDLE1..3`, `LAND_LONG1..4`, `AIR_NEAR1..3`, `LAND_FINISH1..3`,
   `AIR_FINISH1..3` (HP attacks), `LAND_DEFO`, `AIR_DEFO`, `CHASE_ATTACK_UP/LO`, `CHASE_ATTACK`,
   `CHASE_ABILITY`, and three assist entries (`BR_A_REN1/2`, `HP_A_METEO1`).
4. **Start**: `battle:0x08B382B8` (1876 bytes) stores the record at `char+0x101C`, then
   `main:0x0882FA10(0, 0, char+0x130, *(u16*)(plya+0x1C), 0)` plays the motion; if flag bit 0 is
   set and there is a target it calls `battle:0x08A60BC8` with the `+0x40..+0x5C` floats (approach
   / homing) and enters state 0x0F, otherwise state 0x12 (both gated by `main:0x088374B0`).
5. **Motion table** (kind 7): motion 10000 is `{10000, anim 43, variant 1}`; 10001 is `{anim 45,
   variant 1}`; 10004 (`LAND_NEAR2_2`, a different combo branch) is `{anim 45, variant 2}`.
6. **Event track** for animation 43 (decoded from the ISO; 22 ranged + 18 instant events, 952
   bytes). Excerpt:

   ```
   R [  0..    2] type 02  180 0 1 0        turn toward target, up to 180 degrees
   R [ 16..   24] type 00  861 48           hit window: atkp 861 "LAND_NEAR1", collision bit 48
   R [ 24..   54] type 01  0                follow-up input accepted (motion flag 0x60 |= 4)
   R [ 24..65535] type 22 ; R [44..65535] type 28   cancel / free-movement windows (flag bits)
   I @ 14 type 15  0x21B8E3 0 0             say voice clip 2210019 (Cloud's voice bank 22100, slot 19)
   I @ 14 type 25  0.48 12.0 0 1.5 -1 12.0  dash toward target, enter state 0x0F
   I @ 15 type 11  51 0x2191C2 0            attach effect to bone 51
   I @ 96 type 03  0x2191D4 0 0             play sound
   I @  2 type 22  12 0 0.1 0               footstep
   ```
   Animation 45 shows **variant-conditional events**: `R [16..22] type 00 cond 01: 862 48` and
   `R [16..22] type 00 cond 02: 1614 48`, so the same animation deals `LAND_NEAR2` (862) in one
   combo and `LAND_NEAR2_2` (1614) in the other.
7. **`atkp`** record (0x44 bytes; header `{u32 version 0x0C, u32 count}`): `{u32 id, class and
   reaction bytes, power bytes, knock-back floats, char[16] name}`. Cloud's 861 is
   `00 01 FF 00 .. 05 00 B4 08 ..  "LAND_NEAR1"`, 863 is `00 0C FF .. B4 19 .. 30.0 8.0 25.0
   "LAND_NEAR3"`. A second, global `atkp` (stage hazards: `STEAM_ATK`, `GEAR`) is in `battle.bin`.

### 2.3 The interpreter (verified)

There is no instruction pointer, stack or branching. Two functions in **main** evaluate the events
of the current animation's track each update:

* **Ranged events** `{u16 start, u16 end, u8 type, u8 argc, u8 cond, u8, u32, s32 args[argc]}`:
  `main:0x08829930` (13,708 bytes). Dispatch:
  `088299C0: lbu a1,0x4(s0)` / `088299C4: sltiu a2,a1,0x7D` / `088299D4: lui at,0x89C` /
  `088299DC: lw at,0x7910(at)`, so a 125-entry jump table at `main:0x089C7910`; 113 distinct
  targets, one of them the default shared by 13 unused types, hence **112 implemented**.
* **Instant events** `{u16 frame, u8 type, u8 argc, u32, s32 args[argc]}`: `main:0x0882CEC0`
  (6,308 bytes). `0882CEDC: lbu a0,0x2(s0)` / `0882CEE4: sltiu a1,a0,0x38` / `0882CF10: lw
  at,0x7B08(at)`: 56-entry table at `main:0x089C7B08`, **48 implemented**.
* Track header: `{u16 animIndex, u8 nRanged, u8 nInstant, u16 offInstant, u16 size}`
  (`main:0x08829344` walks both lists with strides `0x0C + argc*4` and `8 + argc*4`).
* The callers `main:0x0882F17C` / `0x0882F30C` pass four condition inputs to the evaluators
  (`main:0x08828EC0` / `0x08828E94`): a mode (1 normal, 2 when `char+0x5B2 & 4` = EX mode, 6 for
  object type 1, 3 for a type-2 object with a flag), a flag from `char+0x5B6 & 0x40`, the form
  index `char+0xD60`, and the costume byte `char+0x1079`. The motion-table variant is compared
  against event byte `+6` (`main:0x08829344`).

A handful of event types, from the handler code:

| type | effect |
|---|---|
| R 0x00 | `main:0x08833B80(obj, atkpId, bit)`: set bit in the 64-bit active-collision mask `obj+0x448`, call vtable `+0xBC` to arm the hit |
| R 0x01 | set "follow-up accepted" bits (`motion+0x60 \|= 4`, `\|= 8` if arg) |
| R 0x02 / 0x0B | rotate toward opponent (vtable `+0x124`) / lock target (`+0x134`) by at most N degrees |
| R 0x0C | drive the camera's mode-2 object (`battle:0x08B3E840/0x08B3E864`) |
| R 0x2C | scatter programmatic "feat" particles at random (`main:0x08854B14`), optionally only in EX mode |
| R 0x78 | follow-up window with a Firion-only branch: `**(char+0x5A4) != 3` (`main:0x08829CD8` area) |
| I 0x00 | end of action: return to a neutral state chosen from `*(char+0x314)+0xF4` |
| I 0x03 | play sound ID (`main:0x0883834C`) |
| I 0x04 | vtable `+0x11C(obj, a0, a1)` (fighter override `battle:0x08B304C4`): spawn a `magi` projectile |
| I 0x11 | attach effect to a bone (`main:0x088384A0`) |
| I 0x15 | play a voice clip (`battle:0x08B33F8C`, then the voice channel `main:0x0884EA08`): the argument is voice bank number x 100 + slot, plus 10000 for a manikin, 30000 for the alternate bank, 50000 for an assist (corrected 14 October 2026; first written as "spawn effect"; see `tools/character_voice.py`) |
| I 0x25 | dash toward the opponent with speed, angle and distance arguments, enter state 0x0F |

Most ranged types just set bits in the 16-byte flag array at `motion+0x60..+0x6F`; the generic
state code in the overlay reads those bits ("may cancel", "may move", "invulnerable", ...).

**Projectiles** (`magi`): 0xCC-byte records `{u16 id, .., u16 behaviour type, atkp IDs, parameter
indices into a per-character float pool}`; Cloud has 27 with behaviour types 1, 2, 4, 5, 7, 9,
10..16, 19, 20. They are run by overlay code (`battle:0x08AD0978` creates a 0x130-byte object; a
task named `MAGIC_2::Update` is registered at `battle:0x08AD0810`).

---------------------------------------------------------------------------------------------------

## 3. Code versus data per character (Q3)

### 3.1 What character-specific code exists at all (verified)

1. **EX-mode feature mask `chrp+0x0C`.** Eleven tiny helpers `battle:0x08B343D0..0x08B3476C` each
   return "in EX mode (`0x08B34378`) and bit set". Callers (from `refs.tsv`):

   | bit | owners | helper | used by | effect seen in code |
   |---|---|---|---|---|
   | 0x1 | Cloud, Tidus | `0x08B345F8` | `0x08B34244` | damage multiplier grows with missing HP (`spec+9`) |
   | 0x80000 | Tifa | `0x08B34650` | `0x08B34244` | multiplier from a gauge (`spec+10`) |
   | 0x400 | Cecil | `0x08B346B0` | `0x08B34244` | constant multiplier |
   | 0x4 | Jecht | `0x08B343D0` | `0x08B2FEA0` | extra follow-up acceptance for attacks with `plya+0x0F & 1` |
   | 0x200 | Shantotto | `0x08B3442C` | `0x08A7C1DC`, `0x08AE5BC8` | |
   | 0x20 | Firion, Emperor | `0x08B34488` | `0x08A7C1DC`, `0x08B33938` | |
   | 0x40 | Terra, Kefka, Sephiroth | `0x08B34540` | `0x08B2F4A0` | (glide, by game knowledge) |
   | 0x80 | Zidane | `0x08B3459C` | `0x08A54934`, `0x08A54CD0`, `0x08B2F794` | |
   | 0x20000 / 0x40000 | Lightning | `0x08B3470C` / `0x08B3476C` | `0x08B34A6C` / `0x08AE45B8` (damage) | |
   | 0x100 | nobody | `0x08B344E4` | `0x08AE4310`, `0x08AF07EC` | unused feature |

   Bits 0x2, 0x10, 0x800, 0x10000 are tested directly in the EX-mode enter/exit code
   (`battle:0x08ACA77C`: bit 0x2 calls the form change `0x08B33DDC(char, 0)`; 0x10 and 0x800 clear
   flags in `char+0x380`; 0x10000 calls `main:0x0884F564(char+0xD64)`, Gilgamesh). Bit 0x8 (Cecil,
   Chaos) is tested in the per-frame handler and 0x8000 (Chaos) in `battle:0x08B31108` (section 8).
2. **Hard-coded character IDs.** A scan for `lw rX,0x5A4(rY)` followed by a byte load and compare
   finds 16 overlay functions (29.5 KB if you count the whole functions, but each contains one or
   two short branches): IDs 21 Shantotto, 23 Chaos, 25 Gilgamesh, 31 Lightning, 32 Feral Chaos in
   `battle:0x089E4CC0`, `0x089E4EE0`, `0x089E7B9C`, `0x089E98F0`, `0x08A2FB70`, `0x08A3EE44`,
   `0x08A4AC08`, `0x08A56E10`, `0x08A78DBC`, `0x08A9DF74`, `0x08A9FF80`, `0x08ACB208`,
   `0x08AE45B8`, `0x08B06370`, `0x08B168E4`, `0x08B3526C`; in main, ID 3 (Firion) in the ranged
   event handler and ID 28 in `main:0x0888B70C`, and ID 23 in the loader (no `.exex` for Chaos).
3. **EX-burst mini-game class** chosen by `chrp+0x06` (section 6).
4. **Form/job table** chosen by `chrp+0x08` (data; below).

For scale: the overlay has 6,267 functions / 1.51 MB of code; the fighter class range
`0x08B2C000..0x08B3A9A8` is 228 functions / 58.7 KB, the AI `0x08A62288..0x08AB3400` is 1,300
functions / 324 KB, all of it shared by every character.

### 3.2 Lightning (paradigm shift)

* **Data.** `chrp`: form table 7, mask 0x60000. `p_gst130.objx`: 59 `plya` attacks, **48 `comb`
  entries** (commands 631..676, 1020, 1021) against Cloud's 18, 150 motion-table entries, 85 `atkp`
  records including `_EX` variants. `P_GST130_1P.exex` is 248 bytes = header + **3** x 80-byte
  blocks (Cloud: 1 block, 88 bytes; Onion Knight and Cecil: 2 blocks, 168 bytes), so it scales
  with the number of forms. The equipped-ability block passed in from the save side is indexed by
  form (`main:0x088B7A88(set + form*0x1E, ...)`), so each paradigm has its own slot assignment.
* **Generic form system (code shared with Cecil, Onion Knight, Gabranth, ...).** `battle:0x08B2EAC0`
  reads `chrp+0x08` and calls `battle:0x08ACF118(char+0xD5C, char, index)`, which looks up a
  0x70-byte record (`main:0x088C1B68`; `system.bin`, 10 records, the `jobp` chunk by size) and
  applies it with `battle:0x08ACF2A0`: per form (0x20 bytes) a model state (`main:0x0886B18C`), a
  stats row (`main:0x0884E58C` to `char+0x39C`) and up to four motion-ID remaps
  (`main:0x0882FDC4`). The current form is `char+0xD60`. Record `+6 == 1` means "player-switchable":
  `battle:0x08B3315C` tests the pad (two masks at `battle:0x08B3ACC0/0x08B3ACC8`) and
  `battle:0x08B33DDC(char, form)` performs the switch and plays the form's sound.
* **Lightning-only code.** In the per-frame handler `battle:0x08B3526C`, after a successful switch:
  `if (**(char+0x5A4) == 0x1F)` start programmatic effect 0x0D/0x0E/0x0F for the new form
  (`main:0x08856D48`) and stop the other two (`main:0x08856F54`), about 25 instructions. Two more
  ID tests (`battle:0x089E4EE0`, a HUD function; `battle:0x08B168E4`) and the two mask hooks above.

So the paradigm mechanic is roughly 95% data (form table, per-form ability sets, per-form motions
and events) and a few dozen instructions of Lightning-specific code.

### 3.3 Bartz (uses others' attacks)

* `chrp` mask **0**, no form table: **no Bartz-specific code path was found** (no ID 9 compare in
  either module).
* His attacks are ordinary records in **his own** `p_fiv100.objx`, named after the source game:
  `FF01_NEAR2`, `FF08_NEAR_COMBO`, `FF07_LAND_MIDDL..`, `FF02_LONG`, `FF09_AIR_UPPER_`,
  `FF10_AIR_MIDDLE`, `FF06_AIR_LONG`, `FF05_AIR_FINISH`, `FF04_BR_A_THRUS..`, `FF07_BR_L_RUSH1..4`,
  `FF12_BR_B_KATAN..`, `FF13_BR_L_SMASH`, `FF13_BR_A_IMPAC..`, `FF04_HP_A_HOLY1`,
  `FF08_HP_A_BLADE`, `FF10_HP_L_HELL1` (38 `plya` records, IDs 1933.. and 3010..3023).
* His hit parameters are **separate copies with their own IDs**: Bartz `atkp` 3244
  `BR_L_SMASH1_1` and 3248 `BR_A_IMPACT2_1`, Lightning `atkp` 1995 `BR_L_SMASH1_1` and 2006
  `BR_A_IMPACT2_1`.
* His animations are **his own**: `P_FIV100_1P.gmo` has 84 motions and 166 bones (the extra bones
  include `l_wep1..l_wep5` with `_acc` children, the borrowed weapons), and his objentry row lists
  16 extra prop objects (`40 41 4B 4C 4D 4E 4F 50 42 43 44 45 46 47 49 4A`) where Cloud's lists 1.
* His event tracks reference **his own** effect/sound ID range: 150 of 151 large IDs are in
  1,80x,xxx / 1,81x,xxx (Cloud's are 2,20x,xxx / 2,21x,xxx, Lightning's 21,70x,xxx / 21,71x,xxx);
  one effect ID is in Vaan's range (21,61x,xxx). His container also carries the `wind`, `bubb` and
  `feat` programmatic-effect textures.

Conclusion: a Bartz move is a **re-authored duplicate** (attack record, hit parameters, event
track, animation on Bartz's skeleton, effect copy), not a reference. That proves attack *logic
data* can be transplanted between characters, and equally that the developers did not reuse
animations across skeletons.

### 3.4 A plain veteran (Cloud)

Mask 0x801: bit 0x1 (damage multiplier from missing HP in EX mode, `battle:0x08B34244`) and bit
0x800 (a flag cleared on EX exit, `battle:0x08ACA77C`). No hard-coded ID 13 anywhere. Everything
else (33 attacks, 18 abilities, 63 hit records, 27 projectiles, 120 motion IDs, 86 event tracks)
is data.

---------------------------------------------------------------------------------------------------

## 4. Animation and effect binding (Q4)

### Verified

* An attack names a **motion ID** (`plya+0x1C`). The character's motion table maps it to an
  **index into that character's own GMO motion list** plus a variant byte. Motion IDs are a
  **shared vocabulary**: 78 IDs below 10000 are present in all five full fighters sampled (1, 2,
  10 idle; 100..103; 200..244; 300; 400..471; 500..611 hit reactions; 700..754; 800..822 chase;
  901, 911; 1100..1210; 3000; 5000; 8000..8600 EX burst), and attack motions use 10000+.
* Event tracks are keyed by **animation index** (not motion ID), so two motion IDs can share an
  animation and still differ through the variant condition.
* Effects and sounds are referenced by **global numeric ID** in a per-character range, resolved
  against the character's own `effe` package and `snd_` bank. Hit data is referenced by `atkp` ID,
  projectiles by `magi` ID, both looked up in the character's own tables.
* Skeletons differ: Cloud 108 bones, Lightning 112, Bartz 166, Chaos 148, Aerith (assist) 80. All
  four full models share a named humanoid core in the same order (`root, hip, l_femur, l_tibia,
  l_foot, l_toe, r_..., waist, chest, l_clavicle, l_uparm, l_foarm, l_hand, neck, head, ...`;
  34..38 names shared pairwise) and add character-specific bones (skirt, mantle, weapon, wing,
  tail). The engine itself looks bones up by these names (strings at `main:0x089C9470..`).

### Inference

* An attack's *logic* (plya + comb + atkp + event track + magi) can be reused on another character
  if the IDs it references exist there: copy the records, renumber effect/sound IDs into the new
  character's range, and bind the motion ID to an animation that exists in the new model.
* The *animation* cannot be reused directly across models because motions live inside a GMO and
  address that GMO's bones. Because the core bone names match, retargeting between humanoid
  fighters looks mechanical (remap by name, drop or synthesise the extra bones), but proportions
  differ, and frame-exact events (hit windows, effect bone attachments by bone index, such as
  `I 0x11` bone 51) would need re-checking.

---------------------------------------------------------------------------------------------------

## 5. Equippable attack list and input mapping (Q5)

### Verified

* **Ability table**: `system.bin/ablt`, 790 records x 0x28, sorted by s16 ability ID
  (`main:0x088B1D9C` -> `0x088B1C14`, loaded by `main:0x088B1CC0`, which also takes names from
  `ability.bin`). `+0x0C` s16 is the **command ID** matched against `comb`. Cloud's rows are
  ability IDs 3328..3342, 3345, 3346 with commands 313..328, 927, 928; byte `+0x18` is 0 for
  bravery and 1 for HP attacks, `+0x19` is 0/1/0xFF for ground/air/either, `+0x12` looks like the
  CP cost.
* **Learn lists**: `system.bin/chrl` (magic `CLEV`), 31 entries `{u32 charId, u32 count, u32
  offset}` (Warrior of Light 54 rows, ...). Keyed by character ID.
* **Equipped set**: the per-player battle setup block (0x1060 bytes per player at `0x09AFB8A0`,
  handed to the fighter through vtable `+0x104` = `battle:0x08B2EAC0` -> `main:0x08835468`, kept at
  `char+0x30C`). `main:0x088CBDFC` iterates it: set = `block + 0x10 + block[0x773] * 0xC6`; inside
  a set, a sub-block per form of 0x1E bytes; **12 primary slots** (`main:0x088B7A88`) and **6 x 3
  secondary slots** (`main:0x088B7AB8`).
* **Slot to attack**: `battle:0x08AB7F40(combMgr, comb, block, form, ...)` marks the `comb` entries
  for the equipped abilities (256-bit mask at `combMgr+0x24`); `main:0x08848160(sel, slotClass)`
  then builds a 192-bit mask of currently usable `plya` records by filtering on the record's
  slot-class mask (`plya+0x0F`: bit 1/2/4 with flag 0x80 distinguishing two families, bit 8 for
  class 6), ground/air requirement, reach and height window; follow-ups go through
  `battle:0x08AB81E0`. `battle:0x08B382B8(char, slotClass, ...)` starts the chosen record (classes
  0/1 increment one counter, 2/3 another: bravery and HP).
* Limits implied by the masks: at most 192 `plya` records and 256 `comb` entries per character.

### Not traced

The exact button+direction to slot-index mapping (which of the 12 primary slots is "circle +
toward, ground") sits in the per-frame handler `battle:0x08B3526C` and was not decoded. It is
generic code, not per character.

---------------------------------------------------------------------------------------------------

## 6. EX mode, EX burst and assists (Q6)

### EX mode (verified)

* Controller object `char+0x784`; "EX mode active" is `char+0x5B2 & 4`.
* Passive bonuses come from the `chrp+0x0C` mask (section 3.1): **code per bonus, data per
  character**. A new character can combine existing bits; a new bonus needs code.
* The look of EX mode is data: `.exex` (one 80-byte block of floats and RGBA values per form), the
  event-track mode condition (events that only fire in EX mode, such as the `_EX` hit records in
  Lightning's `atkp`), and the form table (bit 0x2 switches form on entering and leaving EX mode).

### EX burst (verified)

* Started by `battle:0x08A3EB70(mgr, attacker, ...)`; the manager is `*(battle:0x08A405E0)` and
  is fed `super_skill.bin` (`battle:0x08A3EAC0`).
* The **mini-game object is selected by data**: `battle:0x08A3D720(mgr, *(u8*)(chrp+6))` returns
  one of **20 embedded objects** (`mgr + n*0x18`, vtable pointer at `+0x10`, virtual calls through
  it at `+0x0C`, `+0x44`, `+0x4C`; the 20 vtables themselves were not enumerated), then
  `battle:0x08A40684(obj, .., attacker, victim, *(u8*)(chrp+7))` binds it and hands it the
  attacker's `spec` resources (`objdata+0xD0`). Types in use: 0 (Garland, Cloud, Sephiroth,
  Chaos), 1 (Zidane, Kuja), 2 Terra, 3 (Warrior of Light, Firion, Bartz, Lightning), 4 (Cecil,
  Golbez), 5 Kefka, 6 Emperor, 7 (Cloud of Darkness, Exdeath), 8 Onion Knight, 9 (Squall,
  Ultimecia), 10 (Tidus, Jecht), 11 Shantotto, 12 (Gabranth, Vaan), 13 Laguna, 14 Gilgamesh,
  15 Tifa, 16 Yuna, 17 Prishe, 18 Kain, 19 Feral Chaos.
* The burst's content is data in the attacker's objx: motions 8000, 8101.., 8500, 8600; hit
  records `LIMIT_ATK01..`; the `hibg` background; the `spec` UI chunks.
* Three hard-coded IDs in the burst code: `battle:0x08A4AC08` (an offset for Shantotto 21, Chaos
  23, Feral Chaos 32) and `battle:0x08A3EE44` (Feral Chaos).

### Assists (verified)

* Created by `main:0x08888354`: for each side, if an assist is set (`+0x7F0`), it looks up
  `main:0x088B3958(charId)` in `assist.bin` and constructs a **full 0x12D0-byte fighter** with
  `battle:0x08A61C3C(obj, pad, objentry, side + 2, ...)`, then loads default abilities
  (`main:0x088CB9E8/0x088CBA10/0x088CBA54`), sets `battle:0x08B2F2E4(obj, 3)`, AI strength 0
  (`battle:0x08A64100`), and cross-links owner and assist through `+0x1108`.
* `assist.bin`: version 0x15, **31 records x 0x68**, keyed by character ID (1..22, 24..31, 33;
  **no record for Chaos 23, Feral Chaos 32 or ID 34**), four 0x18-byte sub-records each.
* An assist uses a **separate, smaller asset set**: objentry rows `P_xxx_nP_A` (flag byte `+8` =
  0x08) with their own model and `p_xxxNNN_a.objx`. Sizes from the ISO: Cloud full model
  2,017,616 bytes and objx 1,352,328; Cloud assist model 580,748 and objx 309,532. The loader
  skips `.exex` when objentry flag 8 is set and picks the assist voice bank
  (`voice_%s_a%s.scd`).

### What a full fighter has that Aerith lacks (verified from the ISO)

| | Cloud (full) | Aerith (`p_sev120_a`, assist only) |
|---|---|---|
| objentry rows with a model | 1P/2P/3P, `_A`, `_W`, ZAKO | only `_A` (the `P_SEV120_1P` row has no gmo and no objx) |
| model motions | 86 | 14 |
| motion-table entries | 120 | 23 (idle, two moves, a few hit reactions, 6 attack motions) |
| `plya` attacks | 33 | 8 (4 chase stubs + `BR_L_CAREL1`, `BR_A_JAKI1`, `HP_L_STAR1`, `HP_A_HOLLY1`) |
| `comb` entries | 18 | 4 |
| `atkp` records | 63 | 15 |
| `brat` (AI weights) | yes | **absent** |
| kind 9 `hibg`, `spec` chunks (EX burst) | yes | **absent** |
| `.exex` | yes | none |
| `chrl` learn list, full `ablt` set | yes | not checked (roster side) |

So an assist-only character is missing locomotion, guard, dodge, most hit reactions, chase, EX
mode and EX burst content, AI data and the full ability list: roughly 85% of a fighter's content.

---------------------------------------------------------------------------------------------------

## 7. CPU AI (Q7)

### Verified

* Every fighter owns the same brain object: `battle:0x08A639F4(0x1430)` + `battle:0x08A637B8`,
  twelve sub-modules constructed at fixed offsets (`0x08AB2EF0`, `0x08A96398`, `0x08A870F0`,
  `0x08A80770`, `0x08AA4CB0`, `0x08A656D8`, `0x08A65E58`, `0x08A9AC18`, `0x08A9A538`,
  `0x08AA1ED0`, `0x08A9A1B8`, `0x08AA29F0`), wired together by `battle:0x08A63A2C`. Level and
  strength are set with `battle:0x08A640CC` / `0x08A64100` (called from `main:0x0888BA0C`).
* **Global parameter tables**: `battle:0x08AAD8F8` (1,644 bytes) parses a blob with tag `seco`
  (`battle:0x08AAE16C`) and float version 4.36 (`0x408B851F` at `0x08AAE174`) into about 45 arrays
  of fixed-size records. One of them (`+0x1CC` count, `+0x1D0` data, 0x1C-byte records) is
  **keyed by character ID**: `battle:0x08AACD30(table, charId)`, used from `0x08AB2108`,
  `0x08AB22B4`, `0x08AB2340`, `0x08A7EA90`.
* **Per-character data**: the `brat` chunk (magic `atta`, float 1.02): for Cloud 18 groups (one
  per `comb` command 0x139..0x148, 0x39F, 0x3A0) of six `{u16 index, u16 flag}` pairs followed by
  a byte matrix.
* **Attack metadata**: candidate selection `main:0x08848160` filters `plya` records by reach and
  height window and flags, so new attacks are usable by the AI without code if those fields are
  filled in.
* Hard-coded IDs inside the AI range: Chaos 23 (`battle:0x08A78DBC`), Feral Chaos 32
  (`battle:0x08A9DF74`, `0x08A9FF80`).

### Inference

AI is a generic engine (324 KB of shared code) driven by global tables plus per-character and
per-attack data. A new character needs a `brat` chunk matching its `comb` commands and a row in
the character-keyed global table; without the latter `0x08AACD30` returns null and the callers
return 0 (feature off), which looks survivable.

---------------------------------------------------------------------------------------------------

## 8. Boss Chaos (Q8)

### Verified

* **Same class, same data pipeline.** Character ID 23, `chrp` mask 0x8008, burst type 0, form
  table 6. Three objentry rows `P_ORG200_NORMAL`, `P_ORG200_HARD`, `P_ORG200_S_HARD` (variants 1,
  2, 0; objentry flag byte 0x06) each with its own model (1.6..2.1 MB) and container
  (`p_org200_00/01/02.objx`, 1.5..1.7 MB). The names are hard-coded in `battle:0x08B06370`.
* `p_org200_00.objx` has `plya` (28 attacks: `CLAW`, `V_WAVE_1..3`, `FANG`, `H_WAVE`, `KICK`,
  `BALL`, `ASSAULT`, `ASSAULT_COMBO_1/2`, `BURST`, `SPEAR_BOMB`, `BEAM`, `BEAM_SUPER_HARD`, `RUSH`,
  `TERA_FLARE`, `HANDLE_BALL`, `TEST_HUGE`, `HUGE`, plus the eight standard chase records), `comb`
  (17 entries), `atkp` (70), `magi`, `brat`, `text`.
* **He has the complete common motion vocabulary**: all 71 non-burst common motion IDs (idle,
  walk, jump, dash, guard, dodge, hit reactions, wall rush, chase) are in his motion table.
* **Missing compared with a selectable fighter**:
  * EX burst: no motions 8000..8600, **no `spec` chunks**, no `LIMIT_ATK*` hit records, kind-9
    chunk tagged `defa` instead of his own name.
  * `.exex` is not loaded: `main:0x08845230` skips it when `*(s8*)(objentry+6) == 0x17`; the files
    do not exist for NORMAL and HARD.
  * No `LAND_DEFO` / `AIR_DEFO` attack records (every sampled fighter has them).
  * No assist: no `_A` objentry row and no `assist.bin` record.
  * No `_W` (world map) model rows.
* **Chaos-specific code**: mask bit 0x8000 lets `battle:0x08B31108` accept the "special action"
  input outside EX mode (other characters need `char+0x5B2 & 4`); mask bit 0x8 (shared with Cecil)
  makes that action cycle his form (`battle:0x08B33DDC(char, -1)` in `battle:0x08B3526C`) when
  `main:0x088377C0(char)` returns 0, instead of starting a slot-class-4 action; ID tests in `battle:0x089E7B9C` (skips one per-player setup step),
  `0x089E98F0` ("is Chaos" query used by that module), `0x08A4AC08` (burst camera offset),
  `0x08A78DBC` (AI), `0x08B06370` (variant lookup).

### Inference: what would be glitchy under player control

Input handling is not missing (same class, same per-frame handler, same state machine), so he
would move, guard, dodge and attack if abilities are equipped. Expected problems:

* **Ability slots**: his 17 `comb` commands need `ablt` rows and a default set for the 12 slots;
  whether they exist and how they map to bravery/HP slots is the roster investigator's area. With
  no `LAND_DEFO/AIR_DEFO`, empty slots have no fallback.
* **EX mode and EX burst**: entering EX mode works through generic code, but an EX burst would
  reference motions, hit records and UI resources that are absent (the opponent's burst against
  him is fine, since the victim only needs hit reactions).
* **Assist call and being someone's assist**: no data.
* **Menus and save data**: no learn list, no world-map model (out of scope here).
* His attacks were tuned for the AI (long `TEST_HUGE`/`HUGE` sequences, reach metadata), so
  balance and camera framing would be off rather than broken.

---------------------------------------------------------------------------------------------------

## 9. Assessment (Q9)

Common groundwork for all three goals (engineering, no new art): a new character ID needs a `chrp`
row (0x90 bytes), objentry rows pointing at model/cosx/exex/objx files, a stats row, an
`assist.bin` row if it should be callable as an assist, `ablt` rows + a `chrl` learn list + a
default ability set, a row in the character-keyed AI table, voice bank names, and whatever the
roster/save investigators find. In the battle code itself nothing needs to change for a character
whose feature mask, burst type and form table reuse existing values, **provided** no table is
sized for exactly 34 characters (not checked here).

### (a) Exact copy of an existing fighter's moveset

* **Authoring**: none for battle. Point the new objentry rows at the donor's `.gmo`, `.objx`,
  `.exex` (or at a re-skinned model on the donor's skeleton), copy the donor's `chrp` row
  (mask, burst type, form table, stats row), `assist.bin` row and AI row, and reuse the donor's
  ability IDs or clone the `ablt`/`chrl` rows.
* **Reuse**: 100% of attack data, animations, effects, sounds, AI weights, EX burst.
* **Caveat**: if the donor is one of the five characters with hard-coded ID tests (Shantotto,
  Chaos, Gilgamesh, Lightning, Feral Chaos; Firion in main), the clone loses those branches unless
  each is patched to accept the new ID (16 overlay functions, small patches). Donors with only
  mask bits and tables (Cloud, Bartz, Squall, Kain, ...) clone cleanly.
* **A new visual model on the donor's rig** is an art task (mesh + skin weights on the same bone
  list) but needs no animation work.

### (b) Moveset assembled from several fighters' attacks (the Bartz approach)

Per borrowed attack, the following must be produced, and all but the animation are mechanical:

| part | work | kind |
|---|---|---|
| `plya` record(s), `comb` entry, `ablt` row | copy, assign new IDs, set slot class | tooling |
| `atkp` records | copy with new IDs, patch the IDs inside event tracks and `magi` | tooling |
| event track | copy; renumber effect/sound IDs; fix bone indices used by attach events | tooling |
| `magi` projectile records + float pool entries | copy and re-index | tooling |
| effects (`.fep/.feb/.fxseb` entries) and sounds (SSCF) | extract from donor package, merge into the new package under new IDs | tooling (formats not yet decoded here) |
| `brat` AI weights | copy the group for the command | tooling |
| **animation** | the donor's motion must exist in the new model's GMO | **retarget tool or animator** |
| weapon props | extra objentry prop rows and bones (as Bartz's `l_wep1..5`) | modelling + rigging |

* If the base body is donor A's skeleton and the borrowed attack is from donor B, B's motion must
  be retargeted to A's bones. The shared core bone names make an automatic name-based retarget
  plausible for humanoid donors; non-shared bones (capes, tails, wings, weapon chains) need manual
  attention. This is the main cost and the main risk.
* Square Enix did exactly this by hand for Bartz (84 own motions, copied effect assets).
* Tooling needed: ARC and MPK packers, encoders for the seven record formats in section 2, a GMO
  motion importer/retargeter, an effect-package merger, an SSCF sound merger, and ID allocation.
* Budget limits: 192 attack records and 256 `comb` entries per character; model memory (full
  models are 1.5..2.2 MB with 79..86 motions).

### (c) Newly designed attacks and animations

* **Data authoring** (tooling exists after (b)): new `plya`/`comb`/`atkp`/`magi` records and new
  event tracks composed from the 160 existing event types. Anything expressible as "play motion,
  open hit windows with given knock-back, spawn effects and projectiles of existing behaviour
  types, move toward the target, accept follow-ups" needs **no code**.
* **New content**: animations for every new motion (authored in a DCC tool and exported to GMO
  motions on the character's skeleton), plus the roughly 78 standard motions if the body is not
  an existing rig; effect assets in the game's effect format (new particle effects are a content
  pipeline of their own; reusing and recolouring existing effects is far cheaper); sounds and
  voice; HUD attack names (`text`); EX-burst cut-in resources (`spec`, `hibg`) if not reused.
* **Code only when the design leaves the existing vocabulary**: a new projectile behaviour type,
  a new event type, a new EX-mode bonus (mask bit), a new EX-burst mini-game (21st class), or a
  unique mechanic comparable to Lightning's effect switching. These are additions to shared code
  (plugin hooks at the dispatch points: the two jump tables in main, the mask helpers at
  `battle:0x08B343D0..`, the burst factory `battle:0x08A3D720`).
* A form-based mechanic (stances, paradigms, job change) is available as data through the form
  table without code.

---------------------------------------------------------------------------------------------------

## 10. Not determined statically, and runtime checks worth doing

1. **Event condition bytes.** The variant condition (event byte `+6` against the motion-table
   variant) is read directly in `main:0x08829344`; how the mode / form / costume inputs are matched
   against event bytes `+7` and `+8..+11` inside `main:0x08828E94/0x08828EC0` was not decoded.
   Check: break on `main:0x08829930` for Lightning in each paradigm and in EX mode.
2. **Meaning of most event types and `plya`/`atkp` fields.** About 20 of 160 event types were
   read; `atkp` bytes (damage multiplier, reaction, wall rush, EX gain) are only identified by
   position and name. A full table needs either reading every handler or diffing attacks with
   known in-game properties.
3. **Collision argument of the hit event** (second argument, 48/49 for Cloud): a bit index into
   the mask at `obj+0x448`; its link to kind-5 shape records is assumed, not traced.
4. **Effect and sound ID resolution.** That IDs sit in per-character ranges is measured; whether
   the range is enforced by code (derived from the character ID or `chrp+0x7C`) or is only a
   packaging convention inside `.fep/.fxseb` and SSCF was not traced. This decides whether a
   clone character can share a donor's effect package unchanged.
5. **Button+direction to slot index**, and the default ability set, in `battle:0x08B3526C` and the
   setup block at `0x09AFB8A0` (overlaps the roster/save investigation).
6. **Tables sized by character count** (per-character arrays indexed by ID - 1 in battle or main)
   were not searched for; `battle:0x08ADCFA0` indexes two arrays by character ID with strides 200
   and 0x30 and should be checked for bounds.
7. **`seco` AI blob**: which package file it is (main has the string
   `brain/brain_second_table_debug_data.bin` at `0x089CD780` with no code reference) and the
   meaning of its 45 tables.
8. **EX-burst classes**: the static constructor that stores the 20 mini-game vtables was not
   located, so "20 implemented types" means 20 selectable objects, not 20 verified distinct
   classes. Likewise the hard-coded-ID scan is pattern based and may have missed sites that reach
   the ID through another register path.
8b. **`magi` behaviour types**: the overlay code that switches on the behaviour type was not
   located, so the number of projectile behaviours implemented in code is unknown.
9. **The `chrg` and `scra/scrb` chunks of `battle.bin`** (`main:0x08811A58`, `0x0884A944`): used by
   attacks with `plya` flag 0x800 through `char+0xD40`; purpose not determined.
10. **`LIMIT_START`**: `main:0x08848E3C` looks for a `plya` record with that debug name (so the
    name field is functional, not just a comment), but of the containers sampled only Warrior of
    Light's contains the string; what it gates is unknown.
11. **Runtime experiments that would confirm the model cheaply** (PPSSPP, memory edits only):
    * overwrite `*(u16*)(plya+0x1C)` of one of Cloud's attacks with another motion ID and watch
      the attack play the other animation with that animation's events;
    * swap two `atkp` IDs in a hit event and observe knock-back change;
    * set Bartz's `chrp+0x0C` to 0x40 and check whether he gains the EX-mode ability of
      Terra/Kefka/Sephiroth;
    * change `chrp+0x06` for Cloud to 3 and check that the EX-burst mini-game changes;
    * point a player slot at `P_ORG200_NORMAL` to verify the Chaos predictions in section 8.

## 11. Sources

* Binaries and retail data only. Community searches (GBAtemp thread "Dissidia 012 - Help with
  attack data modifications", https://gbatemp.net/threads/dissidia-012-help-with-attack-data-modifications.566632/,
  not readable, HTTP 403; a Scribd "Dissidia Modding Suite" texture guide,
  https://www.scribd.com/doc/76104071/Mod) turned up no documentation of the `objx` chunk formats
  or event types, so nothing in this report rests on community material.
* Scripts used (all under `build/scratch/moveset/`): `objx.py` (chunk list/dump), `plya.py`,
  `track.py` (event-track decoder), `idranges.py`, `chrp.py`, `gmo.py`, `bones.py`, `vt.py`,
  `idscan.py`, `stats.py`, `pc.py`, `grepfn.py`, `dis.py`.

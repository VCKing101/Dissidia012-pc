# Bartz's original Dissidia moveset in 012: feasibility

Static comparison made on 21 November 2026, after a Discord request for Bartz's moves from the first Dissidia
(ULUS10437, USA v1.00) alongside his 012 moves. Nothing was built and nothing was run in the game.

Sources: the owner's ULUS10437 ISO and our ULUS10566 ISO, read with `tools/package_archive.py`,
`tools/moveset.py` and the GMO reader in `build/scratch/model/gmolib.py`. The original executable decrypts
with `tools/local/pspdecrypt` (tag C0CB167C); `BOOT.BIN` on that disc is zero-filled.

## Short answer

Yes. The original disc holds a complete, self-contained Bartz: model with all his animations, battle data,
effects, sound effects and voice bank. 012 is built on the same engine. The model, skeleton, effects and
event scripts fit as they are or with small fixes. Four battle tables are older versions with different record
sizes, and their field mappings are visible by lining up records that both games share. A converter is
ordinary work, not research.

We cannot ship any of it. The port's setup would build "Bartz (OG)" from the player's own copy of the original
game, the way it already builds everything else from their 012 disc.

## What matches

| Part | Original | 012 | Result |
| --- | --- | --- | --- |
| Archive | `PACKAGE.BIN` + `PACKAGE_INFO.BIN`, header `0x20090218 "pack"`, 5,741 files | same format, 11,043 files | same name hash; our reader works unchanged |
| Battle archive `obj/p_fiv100.objx` | `ARC\x01`, 21 chunks, 1,611,964 bytes | `ARC\x01`, 21 chunks, 1,559,788 bytes | same chunk kinds and order except `pris` (original) vs `brat` (012) |
| Skeleton | 83 bones | 83 bones in 1P, 2P, 3P and ZAKO | identical bone data in every 012 costume |
| Animations | 83 motions | 84 motions | 70 names shared, 61 byte-identical; 13 original-only, 14 012-only |
| Motion table (kind 7) | version 2, 103 entries | version 2, 108 entries | same format |
| Event tracks (kind 8) | version 3, 83 tracks | version 3, 84 tracks | same layout and type numbering |
| Collision shapes (kind 5), swing bones (6), EX burst background (9) | | | same formats; kinds 6 and 9 are byte-identical |
| Effects `.fep` | `FEP\xcd` 1.3 | `FEP\xcd` 1.3 | same version |
| Effect sounds `.fxseb` | `FXSE` 0x20070422, 320-byte records | same | same |
| Sound bank `snd_` | `SSCF` 10 02 07 02 | same | same |
| Voice bank | `voice/JP/battle/voice_P_FIV100_00.scd` (244,944 bytes; on the US disc) | `voice/EN/battle/...` | same `SSCF` format |
| `D_FIV100` camera rig, `spec`, `bubb`, `feat` | | | identical or same size |

## Conversion rules

Each rule was checked against every record the two games share, over the 22 fighters of the original game
(scratch scripts, `evidence.py` and its siblings, kept only in the session scratchpad). Where 012 changed a value
on purpose (balance), the original value is kept, since the point is the original moveset. Field names follow
`docs/research/moveset-format.md` and dissidia-fan's `charmod_objx.py` / `copy.js`.

| Part | Original | 012 | Rule | Evidence |
| --- | --- | --- | --- | --- |
| `plya` attacks | version 0x14, 0x70 bytes | version 0x16, 0x74 bytes | 0x00-0x0E as is; slot class 0x0F = (old flags >> 13) & 3 (bit 0x2000 = 1, 0x4000 = 2: which button the press answers); 0x10 as is; new s16 0x12 = -1; old 0x12-0x13 to 0x14-0x15; new pad 0x16 = 0; flags 0x18 = old flags without 0x6000; old 0x18-0x6F to 0x1C-0x73 | 771 shared records: structure matches in all; the rest are retuned ranges and approach speeds. field_0x12 is -1 in all 771 |
| `atkp` hits | version 9, 0x3C bytes | version 0x0C, 0x44 bytes | 0x00-0x05 as is; new 0x06 = 0xFF, 0x07 = 0; old u16 0x06 to u32 flags 0x08; old 0x08-0x0F to 0x0C-0x13 (contact effect bit, EX Force orbs, power, re-hit); new 4-byte hit-stop/shake block 0x14 (`shake` in dissidia-fan's names); old 0x10-0x3B to 0x18-0x43 | 1,337 shared records. 012 retuned EX Force orbs (+0x0D: 50 became 20 in 175 records, 0 in 125) and changed power in 30%. Shake is all zero in half of them; for new hits, copy it from a 012 hit of the same reaction |
| `comb` combos | `{u32 4, u32 count}`, fixed 0x48-byte entries: s16 command, s16 0, s16 -1, s16 -1, 8 stages (unused: attack 0xFFFF) | `{u16 6, s16 count, u32 offset[]}`, `{command, stages, alt_command, prerequisite}` + used stages | stages are the same 8 bytes; rebuild the headers; commands 408-419, 604, 607 become 162-175 slot by slot; 604 (air follow-up) gets prerequisite 171 as 012's 172 has | Bartz's 14 entries in both games |
| `magi` projectiles | `MAGI` 0x20080918, 0x268 bytes | `MAGI` 0x20101011, 0xCC bytes + float pool | 0x00-0x3F as is, except two codes. Origin +0x1E: codes 2 and up move up by one (012 put a new type at 2). Target +0x26: 2 becomes 3 (except with origin 7, which keeps 2), and with an old target of 2, origins 2, 3 and 8 become 012's combined types 11, 12 and 12. The 138 floats at 0x40 go into the pool (u8 indices, at most 128 values; Bartz uses 52 distinct values); 2 zero bytes pad the record | 817 shared projectiles: 97.6% of parameter values equal; the origin and target rule holds for 746 of 817. Bartz's 15 original-only projectiles fall in cases where it holds at least 77% of the time (weakest: origin 8 with target 2, 30 of 39, his ids 740-742) |
| `.feb` effect placement | `FEPA` 0x20080711, 248 bytes | `FEPA` 0x20100205, 260 bytes | insert a zero u32 at 0x54; the original's last u32 (0xF4, 1 in every record) goes to 0xF8 (1 in 84% of 012's records, 0 in 15%, 2 rarely); two new zero u32 follow at 0xFC and 0x100 (0 in all 3,312 records of the fighters checked). A 1 at 0x100 ends the effect after one frame (found in play, 21 November 2026) | 1,919 shared records; +0xF4 differs in 23% (unexplained) |
| Event arguments | | | append zeros: instant 0x10 `homing_dash` (12 to 13), 0x15 `voice` (1 to 3), 0x25 `ground_homing_dash` (8 to 9); ranged 0x11 `turn_or_swivel` (2 to 8), 0x18 `steer_during_move` (4 to 5), 0x1E `can_act_now` (1 to 4), 0x43 `hit_window_fire` (4 to 7), 0x45 `perfect_guard_window` (0 to 1) | in every matched pair the original's arguments are the first ones of 012's and 012 appends zeros (voice 1,213 of 1,235; the rest are retuned) |
| `brat` CPU weights | absent | 500 bytes | use 012 Bartz's as is: it is keyed by command, and the commands line up | |
| `text` banners | `mlng`, 7 tables | `mlng`, 6 tables | keep the original's chunk (the game picks a table by language) | |
| `pris` texture | present | absent in 012 Bartz | keep: 012's engine has the `pris` manager (`moveset-format.md` section 2) | |
| Voice | `voice/JP/battle/voice_P_FIV100_00.scd`, 52 slots | `voice/EN/...`, 71 slots | voice ids stay; 012 kept the original slot numbers (same-size clips in the same slots) and left some empty (slots 12, 20, 36 ...): fill those from the original bank | slot sizes compared |
| Model | | | take a 012 costume GMO and put the original's 83 Motion chunks in place of its own, in the original order, so animation indices match the original tracks | identical skeletons |

## Effects and sounds

The original Bartz's moves are other fighters' moves, and so are their effects: 32 effects his OG moves use are
missing from 012 Bartz's package. 28 of them are in 012 already, in the package of the fighter they belong to,
under that package's prefix:
- Cecil (`p_for100`, P0041_): Soul Eater, Paladin Force, the Holy sword set, Tuki Sonic, 16 in all.
- Terra (`p_six100`, P0061_): the water set and the finish glow, 5.
- Onion Knight, Firion, Cloud, Warrior of Light, Zidane: 2, 2, 1, 1, 1.

20 of the 28 have the same duration and node count as the original's copy; the rest are 012's reworked
versions. The other 4 are Bartz-only recolours of trails (Zidane's, Tidus's and Warrior of Light's two), close to
012's but not the same; those come from the original disc, which the moves need anyway.

`tools/effect_package.py` reads and writes an effect package (FEP) as a graph of blocks:
- The pointers are found by walking the structure the way main 0x0894351C relocates it. All 83 fighter and assist
  packages of both games are written back byte for byte, and the pointer set matches dissidia-fan's library on
  every one.
- `copy_effect` copies an effect with its body, nodes and the resources they use (textures, models, texture
  animations, vertex lists), renumbering resource indices in the nodes' parameters.
  - A resource already present byte for byte is shared.
  - Clashing resource ids are renumbered, and models' and animations' texture ids follow.
  - The effect gets a group of the target's own.
- Which parameter fields hold resource indices is dissidia-fan's finding (their `LINKS`). All 86,521 such fields
  in both games are in range.

Each copied effect brings its sound record (FXSE). Clips on the source fighter's bank go into empty slots of
Bartz's bank (35 empty in 012), so nothing in the bank moves. The moves' own sound events (8 slots) take the
original Bartz's clips the same way; their 012 counterparts are re-encoded and never match byte for byte.

Result for all 12 abilities: 122 of 128 effects, the archive at 2,191,452 bytes (the game's object block holds
2,506,752). Checked in game: Soul Eater (OG) shows Cecil's purple Soul Eater effects (2026-11-21).

## In the port (21 November 2026)

`tools/original_moves.py build --og-iso OG.iso MODDIR` writes a mod folder: mod.json and `package/`. The
package holds Bartz's archive with the 12 original commands added (1100-1111), his four models with the 26
original animations (2.68 MB each, over the game's block, so the port's room for bigger models is used), and
`main.bin` with abilities 0920-092B. It also holds the names "X (OG)" and help texts.

Three things the port does for it:
- **Package files** (`d012_package_mods.cpp`) over the Japanese voices' own index and package
  (`index_over` / `package_over`: only one provider can serve a file).
- **Hitbox groups.** The game makes a group's bit with a 64-bit shift (main 0x08837D18 calls
  0x089985B0), so ids of 64 and up act as id - 32. 012 Bartz uses 0..55 and no fighter uses 63, so the
  original's 30 groups of their own are parked on 63. `d012_shape_ids.cpp` overrides battle 0x08AB7F40, which
  marks the combo entries of the equipped abilities at scratch + 0x24. It finds the fighter
  (*(0x089D6A10) + 0x38/0x3C, + 0x390), works out which animations the enabled commands play, and puts their
  parked groups on free ids, then on ids only commands that are not enabled use.
  - The game asks for more than the loadout (an HP-only query, Lightning's forms), so groups go in for every
    command enabled so far.
  - Checked: Soul Eater (OG) hits with it and does not without it.
- **Abilities in saves** (`d012_mod_abilities.cpp`, mod.json `abilities`, docs/port/mod-folder.md §9).
  - Given by level through the record accessor main 0x088CB164 (record: +0x01 level, +0x3EE learned list).
  - Kept out of the save file around the write, in a side file.
  - Set slots: rows of 30 bytes, one per form (main 0x088CBDFC); only the rows a character uses are slots.
    Bartz's +0x5F of each set is another field.
  - Checked: the (OG) abilities show in Customize with their CP, AP and help. Soul Eater (OG) equipped there
    plays in battle. A save written with Ascendent Lance (OG) equipped loads with it back, and loads clean with the
    mod off.

Not done: making 64 MiB of guest RAM conditional (rollback copies all of it), online (both players need the
same mod), the setup step that builds the mod from the player's original disc, the voice lines only the original
bank has, two equip bonuses (sets 636, 646) and checking Holy (OG) in battle.

## Without the original disc (checked 21 November 2026)

What the 12 abilities take from the original disc, and whether 012's disc has it:

| Piece | On the 012 disc? |
| --- | --- |
| 26 animations on Bartz's skeleton | 8, byte for byte, in 012 Bartz's own models (moves both games share). The other 18 (`_600A`, `_603A`-`_605A`, `_607A`, `_611A`, `_612A`, `_620A`, `_621A`, `_630A`, `_634A`, `_641A`, `_700A`-`_704A`, `_703B`) are in none of its 779 models, assist, world-map and cutscene models included |
| Attack, hit and projectile records, event scripts, hitboxes of the 13 original-only attacks (1932, 1938, 1940, 1941, 1943, 1944, 1948, 1951-1955, 2217) | No: not in 012 Bartz's archive nor in his assist's |
| The original tuning of the 4 moves both games share | No; 012 has its own tuning |
| 28 of 32 effects | Yes, in the owners' packages (Cecil, Terra, ...) |
| 4 trail recolours | Close ones (Zidane's, Tidus's, Warrior of Light's), not the same |
| 8 move sounds | Re-encoded equivalents in the owners' banks, never byte for byte |
| Names, help, CP, AP, levels | Names of the owners' moves (Soul Eater, Paladin Force, Flood ...) yes; Bartz-only names (Ascendent Lance, Climbarrel, Storm Shot, Slidehazzard) no |

So dropping the original disc means rebuilding 13 animations and 13 attacks from 012's owners of those moves:
- Retarget the owners' animations onto Bartz's skeleton, re-timed.
- Copy their move data the way dissidia-fan's copy recipe does.
- Write our own names and numbers.

Of the 18 animations above, 5 (`_603A`-`_605A`, `_620A`, `_641A`) differ from 012 Bartz's only in their bytes: the poses are the same at every frame (checked later the same day, see the plan below). So 13 need rebuilding, not 18.

Because the result would be built from Square Enix's data, it could not be shipped as files. It would have to be a script that runs on the player's own 012 disc at install time. The moves would then be 012's owners' versions on Bartz, not the original Bartz's.

Shipping the converted original data instead is not an option: it is the game's data, and the project ships none (NOTICE.md).

## Plan: the 012 disc only (chosen 21 November 2026)

The owner's choice: the (OG) abilities are built from the player's 012 disc alone. The original disc stays a
development reference: we check our results against it, and players never need it.

### Where every animation comes from

The original Bartz's moves are the owners' moves, re-done on his skeleton. Each original-only animation was
compared with every animation of its owner by the direction of each limb in world space, which does not depend
on how the two skeletons hold their bones at rest. Bones were paired by name and sampled every 3 frames, and a
time offset, a speed and a left/right mirror were searched. An unrelated pair scores 40-55°; each of these has
one clear match:

| Ability (OG) | Original animation | Owner's 012 animation | How | Original vs owner |
| --- | --- | --- | --- | --- |
| Ascendent Lance | `_600A` | Warrior of Light `_600A` | as is | 8° |
| | `_621A` | Firion `_652A` | as is | 11° |
| Climbarrel | `_607A` | Squall `_606A` | mirrored | 13° |
| Soul Eater | `_701A`, `_702A` | Cecil `_700A`, `_701A` | mirrored | 21°, 18° |
| Flood | `_704A` | Terra `_700A` | 1.25 times as fast, from 4 frames in | 17° |
| Storm Shot | `_630A` | Zidane `_620A` | as is | 12° |
| | `_634A` | Tidus `_642A` | mirrored | 10° |
| Slidehazzard | `_611A`, `_612A` | Cloud `_611A` (first 39 of 44 frames), `_612A` | as is | 9°, 7° |
| Wind Shear | `_700A` | Onion Knight `_708A` | as is | 12° |
| Paladin Force | `_703A`, `_703B` | Cecil `_705A`, `_705B` | as is | 12°, 12° |

- All 13 owner animations are on the 012 disc with the original's poses: 9 byte for byte, 4 re-encoded with
  every frame the same (0.00°).
- The other 13 animations the abilities use are in 012 Bartz's own models (8 byte for byte, 5 with the same
  poses).
- "Mirrored": Bartz does the move with the other hand.
- The 7-21° left between the original Bartz and the owner is Square Enix's own retouching for his body and
  weapons. A retarget of the owner's animation lands about that far from the original too. That is the
  difference a player could see.

### What the moves' data would be

The original Bartz's event tracks mostly follow the owner's timing (13 to 36 of each track's events line up
within 3 frames). His attack and hit records are his own tuning:
- Attack records differ from the owner's 012 ones in selection ranges and heights (`range_max` in 8 of 13),
  the banner, the AI bytes and `field_0x20`. Paladin Force's second part also has its own approach movement.
- Hits differ in spark kind, `field_0x0C`, hit-stop, power and knock-back.
- His tracks add what the owner's lack:
  - `prop_on` events, which show the borrowed weapon among his parts;
  - face changes;
  - his own sounds;
  - his own effect spawns.

How far the owners' 012 moves are from the original's:
- The shared moves alone differ in 19 attack words, 76 hit words and 192 events.
- The original-only moves differ from their owners' 012 moves in 35 attack words and 85 hit words.

### The owner's decisions (21 November 2026)

- **Eight abilities.** Ascendent Lance, Climbarrel, Storm Shot, Slidehazzard, Soul Eater, Flood, Wind Shear and
  Paladin Force. Solid Ascension, Reel Impulse, Holy and Flare are left out: they share their animations and
  most of their data with his 012 abilities in the same slots.
- **As exact as possible.** The original's move data may ship as script, adjusted for 012: attack, hit and
  projectile records, event tracks, hitbox spheres, CP, AP and learn levels. Square Enix's assets stay out:
  - animations and models;
  - effects and sounds;
  - help texts.
- **CP: 30, and 15 once mastered**, for all eight, as 012's own Bartz abilities cost (the original's were 35 and
  45). `ABILITY_CP` in `tools/original_moves.py`; seen in Customize. The mastered cost is written to the same
  field 012 uses (+0x10 of the ability record) and has not been seen in the game.
- **Move Studio** (dissidia-fan) is where the moves are looked at and changed, not raw values. Our merged
  archive opens in their tools: `charmod_objx.unpack` turns it into their JSON (moves, actions, hits,
  timelines, projectiles) and `pack` gives back the same bytes.
  - The parked hitbox groups show as group numbers such as 23359 (63 + 256 × 91). The Studio keeps a group as a
    32-bit number, so its viewer and dummy still find their spheres.

So a player's build takes:
- the moves' data from our recipe, which comes from the Move Studio folder;
- the animations from the owners' 012 models, through our retarget;
- the effects and sounds from the owners' 012 packages. The 4 Bartz-only trail recolours are replaced by
  012's own trails, and the 7 move sounds by the owners' 012 clips;
- names: the owners' move names from the 012 disc. The recipe writes Ascendent Lance, Climbarrel, Storm Shot and
  Slidehazzard itself, and our own help texts.

Move Studio and Blender stay development tools. Their code has no licence, so the port cannot carry them. What
they save holds the game's own animations and effects, so it cannot be shared either.

### First results (21 November 2026)

- `tools/gmo_motion.py` reads and writes skeletons and motions; `tools/move_retarget.py` is the retarget above.
- All 13 retargets against the original Bartz's animations, on the bones Bartz shares with the owner:
  7.6-19.2°. Each is as close as the owner's animation itself, and some are a little closer. Against the owner:
  3-7°.
- Bartz's own bones. Each borrowed weapon sits on a weapon bone of its own (`r_wep3` Cecil's sword, `l_wep4` and
  `r_wep5` Zidane's daggers, `r_wep4` Cloud's sword ...). The recipe says, per animation:
  - **follow**: his weapon bone turns as the owner's weapon bone does (found by comparing the two against the
    original; 8-35° apart);
  - **held**: a bone the original keeps still is given that fixed pose (how the weapon sits in the hand). A
    weapon that moves like none of the owner's is held at its average grip.
  - Weapon bones against the original, in full rotation: 10-46°, from 46-95° with his idle pose.
  - The body's own bones, in full rotation with twist: 5-20° (hands about 20°).
  - The bind pose is the reference that works; the bones' rest transforms are 11-74° away from it.
- Built from the original disc with the eight abilities, the models are 2.44-2.47 MB. With the retargets they
  are 2.46-2.48 MB. Both are under the game's 2,506,752-byte model block, so these abilities may not need the
  port's 64 MiB at all (not checked in the game yet).
- **Built from the 012 disc alone** (later the same day):
  - `tools/original_moves.py recipe --og-iso OG.iso --iso 012.iso` (development) writes
    `port/features/bartz-original-moves.json`, 129 KB: 16 attacks, 29 hits, 10 projectiles, 61 effect records,
    the event tracks and hitbox spheres of the eight abilities, and where each animation, effect and sound comes
    from on the 012 disc.
  - `tools/original_moves.py build-012 --iso 012.iso OUT` builds the same files from that recipe and the 012
    disc. Against the original-disc build, the ability and name files are byte for byte the same. The moveset
    differs only where 012 stands in: effects, the sound bank and 54 bytes of sound ids.
  - Stand-ins found on the 012 disc:
    - the 4 trails are Zidane's, Tidus's and Warrior of Light's own;
    - 2 sounds are byte for byte in Cecil's bank;
    - 4 are in 012 Bartz's bank at the same slots, re-encoded;
    - Flood's water sound (slot 78) has no copy, so Terra's clip 93 stands in. It has not been listened to.
  - In the game (data `build/port/data-bartz-012`): Customize lists the (OG) abilities with our help texts.
    Each of the eight was equipped through Customize and used in a Quick Battle against an idle second player
    (`D012_LOCAL_VERSUS=1`; script `build/bartz-test/battle_tests.py`, one ability per run from a clean test
    save):

    | Ability (OG) | Seen |
    | --- | --- |
    | Ascendent Lance | hits for 7 Bravery, foe launched |
    | Climbarrel | hits (Bravery 95 to 100), foe launched, chase prompt |
    | Storm Shot | in the air: hits (95 to 100), dagger trails, chase prompt |
    | Slidehazzard | in the air: hits (95 to 100) |
    | Soul Eater | banner, hits (95 to 107), HP hit (foe 1242 to 1135) |
    | Flood | banner, HP hit (foe 1242 to 1147), foe thrown up |
    | Wind Shear | banner, foe drawn in with repeated hits (95 to 102), HP hit (1242 to 1140) |
    | Paladin Force | banner, hits (95 to 108), HP hit with wall rush (1242 to 1106) |

    Each run placed its hitbox groups (`[shape-ids] ... placed on id 56 57 58 59`) and ended normally.
    Not judged from stills: how the animations look in motion, and the sounds.
  - Models with the eight abilities: 2.36-2.39 MB, under the game's model block.
  - Still loaded through the mod-folder path. The owner wants it built into the port: the builder has to move
    into the port's C++ (step 3).
- **After the owner's first play** (21 November 2026; his recordings: `build/port/bartz-og/rec1`, 12.5 frames a
  second, and `rec2`, every second frame of a 60 fps ShareX video). He reported weapons showing late, trails
  and wind effects missing, and far too many EX orbs. Found and changed:
  - **Trails and weapon glows.** A `bone_effect` event (ranged 0x2A, also 0x5F) takes the first effect record of
    its id (9999 trails, 8888 glows) whose key mask at +0x44 has the event's bit. 012 Bartz's own records use
    the same bits and come first, on weapon bones that are not showing, so nothing was drawn. The original's
    records now get key bits his leave free, and the events are pointed at them. An event whose bit matches no
    original record is left alone (Squall's trail, bit 0, then finds 012's own, on the right bone).
  - **Weapons showing late.** Event times are in ticks, two to an animation frame. For the animations both
    games have, 012 moved `show_part` starts of tick 20 to 6 or 4 and brought the ends in by 12 to 16; hit
    windows did not move. So 012's engine draws a part later than the original's. The (OG) tracks now take
    012's own windows for the shared animations, and the same correction elsewhere (start 20 becomes 6, end 14
    earlier).
  - **Firion's spear** grows in and out by a scale track (0, then 1.2 from tick 2 to 68). It is carried in the
    recipe (`scales`); no other bone of the 13 animations is scaled.
  - **EX orbs.** Hit record +0x0D. The original's values (50 on finishing hits, 6, 2) are on a bigger scale than
    012's: over the hits both games share, 50 became 20 or 0, hit by hit. Each (OG) hit now takes the amount of
    the 012 hit at the same moment of the same move (the owner's, or 012 Bartz's own), else 50 → 20, 6 → 2,
    2 → 1. The largest is now 20; most are 0 to 4.
  - No effect record the engine triggers by itself (+0x34 non-zero) is left behind: the original has none bound
    to these animations.
  - Seen after the change (data `build/port/data-bartz-012b`): Zidane's dagger trails in Storm Shot, a few
    orbs per hit, not a cloud. Not confirmed: every trail and glow, and whether the weapon timing now feels
    right in play.
- **After the owner's second play** (21 November 2026; his recording: `build/port/bartz-og/rec3`). He reported
  every effect of Soul Eater, Flood and Paladin Force missing, no wind in Wind Shear, effects "appearing for one
  frame", a very short weapon flash, and no trail on Firion's spear or Tidus's sword. Found and changed:
  - **Effects lasting one frame.** One cause for all of it: the effect record conversion wrote the original's
    last field to +0x100 as well as +0xF8. 012 keeps +0x100 at 0 in every record; with a 1 there an effect is
    ended after its first frame. The rule in the table above is corrected, and `og_fepa_row` with it.
  - **How an effect starts.** A record with an animation name at +0xC8 starts with that animation, at the frame
    in +0x74; an `effect` event (instant 0x19) starts a record by id at any time. So the flash a weapon appears
    with needs no event in the animation its record is bound to (the original's `_600A` has only the event for
    the flash it leaves with), and effects such as Soul Eater's have no event at all.
  - **The weapon flash as 012 has it.** The owner asked for the flash to match 012 Bartz's own attacks. The
    records of `WEP_APPEAR_*` now take their offset, turn and size from 012 Bartz's record of the same effect on
    the same weapon bone, and the size alone when he has none on that bone (Firion's spear). `PLACED_AS_012`.
  - **Paladin Force's lance trail.** The original's `_703B` asks for trail record 9999 with key 7, and no trail
    record has that key; the lance's trail record, bound to `_703A` and used by nothing else, has key 6. Cecil's
    own Paladin Force draws its trail over the same ticks. The event now asks for key 6 (`KEY_FIXES`). This is
    the one place the build departs from the original's data on purpose. The owner chose to keep the trail.
  - Instant event 0x22 is a footstep (its first argument is 8 or 13 in every walk and run animation), not an
    effect group: nothing to renumber.
  - Seen after the change, in test battles filmed every fourth frame (`build/port/bartz-og/t_*`): Soul Eater's
    darkness, Flood's hand glows and water, Paladin Force's light and lance trail, Wind Shear's whirlwind, the
    spear's trail in Ascendent Lance, Tidus's trail in Slidehazzard, the daggers' in Storm Shot, and the weapon
    flash at both ends of every move, each lasting many frames. Climbarrel's second stage was not looked at
    closely. Not judged: how the flash compares with 012's by eye, and sounds.
  - The owner's test folder had `audio.mute=1`, copied from the automatic tests' settings: his first two plays
    were silent. It is 0 now. `build/bartz-test/battle_tests.py` takes `DATA` (the data folder, default
    `data-bartz-012t`, so a test never touches the folder he plays in), `STEP` (frames between shots) and
    `EXTRA` (later presses of a combo).
  - **The owner's third play** (21 November 2026): all eight moves work and he found nothing more to change.
    The Python prototype is accepted as the reference for the builder in the port.
- Two Move Studio folders for comparing, in `build/move-studio/mods/` (git-ignored, development only):
  - `bartz-og`: the eight abilities with the original's animations;
  - `bartz-012`: the same, with the 13 animations rebuilt from the owners' 012 ones.
  - To open them: open `build/external/dissidia-fan/studio/index.html` in Chrome or Edge. Under Locations, set the
    mods folder to `build/move-studio/mods` and the game ISO to the 012 disc. Then pick "Bartz (Modded)".

### Building it

The retarget, for each of the 13 animations, in world space like `build/scratch/model/retarget.py` (the Noctis
work) but simpler, because 012's skeletons share bone names (38 to 60 of Bartz's 83 bones per owner, every core
bone included):
- how far each owner bone turns from its rest pose, applied to Bartz's bone of the same name from his rest pose,
  then turned back into local rotations;
- mirrored moves are reflected across the body's side-to-side plane with left and right swapped first;
- the body's path (`root`) is scaled by one number per animation, 0.86 to 1.27. These are Square Enix's own
  scales, fitted against the original during development; they track the hip-height ratio except for the
  smallest owners. The gameplay root (`trans_include`) keeps the owner's distances (scale 1.0, except
  Ascendent Lance's 0.94);
- camera bones (`camera_include`, `camera2_include`) copied;
- Bartz-only bones (his weapon set, `pad`, `star`, the mantle) held as in one of his own 012 animations with the
  same weapon. The mantle is a swing chain the game moves anyway;
- Flood resampled at 1.25 times; the rest frame for frame (both games run their animations at 30 frames a
  second).

Hitbox groups move with their bones by name. Sphere offsets are turned from the owner's bone frame into Bartz's,
and weapon spheres go on his bone for that weapon.

Where it runs: the port, not `setup.exe`, so that Windows, Linux, the Steam Deck and Android all build it the
same way. As built on 21 November 2026 (step 3 below) it is a part of the port and no mod: the first time the game
opens its package, the port builds the eight files from the player's 012 disc and the recipe, in a quarter of a
second, and keeps them in `<data dir>/cache/original-moves/`, keyed by the recipe and the disc's index. No
progress panel is needed. The release carries only the recipe (the moves' data, the retarget settings, our text)
and our code. It carries no animation, model, effect or sound bytes. See
[docs/port/original-moves.md](../port/original-moves.md).

### Steps

1. **Python prototype.** `tools/original_moves.py build-012 --iso 012.iso MODDIR`.
   - Bartz's own bones in the retarget.
   - The recipe: written from the Move Studio folder by a development command, so changes made in the Studio
     carry over.
   - `bring()` taking its records from the recipe instead of the original disc.
   - Done when the eight abilities build from the 012 disc alone and match `bartz-012` byte for byte.
   - A development-only report compares every built animation with the original's by the measure above, using
     the owner's original disc. Each should land near the "original vs owner" figure in the table.
2. **In Move Studio and the game.** The owner compares `bartz-og` and `bartz-012` in the Studio. Then each
   ability goes through the route tool, with short clips for the owner. Done when each shows the right weapon in
   the right hand, its hits connect, its effects show, and Story and Arcade fights finish.
3. **C++ builder in the port.** Readers and writers for ARC, MPK and GMO, the effect copier, the move copy,
   the retarget and the `system.bin` and message-table edits: about 2,500 lines of Python today.
   - First-run build, progress panel, `d012_tests`.
   - **Same bytes on every platform.** Online play needs every player's built files to match, and animations
     move hitboxes. The retarget may only use arithmetic that is exact everywhere: add, subtract, multiply,
     divide and square root in doubles, with fused multiply-add off (the Android desync of November was that).
     Today's Python calls the maths library's `acos` and `sin` in the spherical interpolation and the key
     reduction; those become our own fixed series, in the prototype first so that both can be compared.
   - Done when Windows, Linux and Android write the same files as the Python prototype, byte for byte, from our
     disc.
   - **Done on Windows, 21 November 2026.** `port/host/d012_original_moves.cpp` (formats, the merge, the
     abilities), `d012_gmo_motion.cpp` (the retarget), `d012_effect_package.cpp` (the effect copier) and
     `d012_original_moves_feature.cpp` (the disc, the cache, the checks, the online item). GCC 14 and clang builds
     write all eight files byte for byte as the Python prototype does, in 0.2 s. The recipe now also names the
     fighter every borrowed effect comes from (`effect_from`), the new command ids (`command_ids`), and a `check`
     block: the size and CRC-32 of each of the 25 disc files a build reads and of the 8 files it must come to. The
     port leaves the moves out when the disc's files differ (a disc mod) or its own build differs. The USA and
     European discs have the 25 files the same. Linux and Android builds have not been run.
   - The owner asked for a part of the port and not a mod, so nothing is turned on and there is no progress
     panel; the old mod folder (`build-012 OUT`) is the development reference only. A mod folder that replaces
     one of the eight files makes the port leave its own build out.
4. **Finish.** Already listed in the port section above:
   - check whether the eight abilities still need 64 MiB of guest RAM; if they do, make it conditional;
   - online: both players need the same mod and the same recipe hash;
   - the two equip bonuses;
   - docs (`docs/port/mod-folder.md`, README);
   - commits.
   - The original-disc converter stays as the development reference.

### Risks

- **Retarget quality on the smaller owners** (Onion Knight, Zidane, Tidus): hands meeting weapons, and feet
  sliding.
- **Owner-only state.** Cecil's moves switch his form. Other owners' moves may set stances or flags that mean
  nothing on Bartz. Each copied track needs a pass for these.
- **Weapons.** Which of the 18 parts of Bartz's model is each owner's weapon has to be read from his 012 moves
  that borrow the same weapon. A weapon no 012 Bartz move uses may be missing from his 012 model.
- **Voices.** The original-only voice lines stay out: only his 012 bank is available.

## Moving one move at a time

dissidia-fan's Move Studio does this between 012 characters ("Copy a move from…", `studio/copy.js`). Its recipe,
which a "(OG) moves next to the 012 ones" build would follow once the original archive is in 012 form:

1. From the command, collect its presses' attack records and the follow-ups their `chain_followup` events offer.
2. Through the motion table, collect their animations and event tracks.
3. From the events, collect hits (and those of projectiles), hitbox groups (`hit_window`, `guard_counter_hit`,
   `hit_window_fire`, `weapon_volume`), projectiles with their `link_*` children, effects (FEPA ids) and sounds.
4. Copy effects into the target package (at most 128) with new FEPA ids, and projectile looks with new group bits.
5. Append hits with new ids (an identical record is shared), hitbox groups with new numbers (bones by name),
   projectiles with new ids and links, pool values merged, and sound clips to the bank.
6. Append the animations to every costume model of the moveset; their indices are the old motion count + k.
7. Give attacks and scripts new ids, rewrite plya +0x00 and +0x1C, copy the HP banner into the `text` chunk,
   add the motion table rows, rewrite every id in the copied tracks.
8. Add a command, an ability record and a CPU (`brat`) row.

Their Studio does step 8 only for new roster characters (a mod of a retail character keeps its retail abilities),
and new roster characters need their own runtime, not ours. In our port, "(OG)" abilities on the real Bartz would
also need new `ablt`, `chrl` and name-text rows in `system.bin` (the stock loader holds 808 records, retail has
790), and they would only show up once learned: the save keeps 150 learned abilities per character.

dissidia-fan's repository has no licence (checked at 3107b3b, 2026-11-21). Their tools can be used locally to
look at the result, but any converter shipped with the port has to be our own code.

## Abilities line up one to one

Both games give Bartz 14 combo entries, in the same order of slots. 012 renumbered the commands
(original 408-419, 604, 607; 012 162-175). Six slots are the same moves in both games and eight differ. So an
original-moves archive whose `comb` uses 012's command numbers works with 012's ability system as it is:
equipping a 012 ability enables the original move in that slot.

| Slot | Original command: attacks | 012 command: attacks |
| --- | --- | --- |
| 1 | 408: FF01_NEAR, FF02_LONG_COMBO | 162: FF13_BR_L_SMASH, FF12_BR_B_KATAN x2 |
| 2 | 409: FF08_NEAR chain (same) | 163: same |
| 3 | 410: FF07_LAND_MIDDL, FF08_NEAR_COMBO | 164: FF07_LAND_MIDDL, FF07_BR_L_RUSH1-4 |
| 4 | 411: FF02_LONG, FF09_AIR_UPPER_ (same) | 165: same |
| 5 | 412: FF04_LAND_FINIS x2 | 166: FF10_HP_L_HELL1 |
| 6 | 413: FF06_MIDDLE_ABI | 167: FF04_LAND_FINIS |
| 7 | 607: FF05_AIR_FINISH (same) | 168: same |
| 8 | 414: FF09_AIR_UPPER, FF10_AIR_MIDDLE | 169: FF04_BR_A_THRUS, FF13_BR_A_IMPAC |
| 9 | 415: FF10_AIR_MIDDLE x2, FF07_LAND_MIDDL x2 | 170: FF10_AIR_MIDDLE x2, FF04_BR_A_SHOOT |
| 10 | 416: FF06_AIR_LONG (same) | 171: same |
| 11 | 604: FF06_AIR_LONG + follow-up (same) | 172: same |
| 12 | 417: FF03_AIR_NEAR_F | 173: FF08_HP_A_BLADE |
| 13 | 418: FF04_AIR_FINISH x2 | 174: FF04_HP_A_HOLY1 |
| 14 | 419: FF05_AIR_FINISH (same) | 175: same |

Battle banners (`text` chunk): the original has Brave Blade, Wind Shear, Soul Eater, Paladin Force, Flood,
Goblin Punch, Flare; 012 has Master Mime, Brave Blade, Goblin Punch, Flare, Dark Flame, Luminous Shard,
Ragnarok Blade, Hellfire.

## How a player could get it

Every 012 Bartz costume row in `objentry.bin` names its battle archive separately (data name at +0x34, all
`p_fiv100` today), and the costume's `.cosx` names its voice bank suffix. So "Bartz (OG)" can be an extra
Bartz costume whose row points at its own archive, `obj/p_fiv1og.objx` for example, and its own model: a 012
Bartz costume with the original motions in place. The port already loads fan DLC costumes from
`<data>/dlc/<folder>/`; the setup could write this one there from the original disc.

- Works wherever a costume is chosen, and Bartz can fight Bartz (OG).
- The Customize menu shows 012's ability names, because abilities belong to the character; the moves played
  and the battle banners are the original ones. A port hook could show both names.
- Online needs both players to have built it.

Alternatives:
- A switch in Options that swaps Bartz everywhere. Story is included, but both versions never meet in one battle.
- The original moves as extra "(OG)" abilities on the real Bartz (the move-by-move recipe above, plus `system.bin`
  rows).
- A separate roster cell. This is the largest job: the port has no roster or save work for it yet, and Story's
  fixed parties would not include it.

## Not settled

- Whether the loader really takes the archive from a DLC costume row's data name. `docs/research/character-assets.md`
  says `0x08845230` builds `obj/<data name>.objx`; not tried with a new name.
- The few records where the rules above disagree with 012, such as the plya flag 0x1000 cases and FEPA +0xF4.
  Reading the original executable would settle them.
- How the original's balance plays under 012's rules (power, EX Force, hit-stop). This is a matter of judgment, not
  format.
- His original EX Mode. The original disc has no `.exex` files for him; EX parameters may sit in its
  `system.bin`.
- Whether Story mode lets a costume be chosen.
- The original bank on the US disc is under `voice/JP/`. Its clips match 012's in size but not byte for byte; the
  language is unchecked.

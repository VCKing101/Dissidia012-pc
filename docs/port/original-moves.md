# Bartz's moves from the first Dissidia

> 21 November 2026. `port/host/d012_original_moves.cpp` (the builder), `d012_original_moves_feature.cpp` (its place
> in the game), `d012_gmo_motion.cpp`, `d012_effect_package.cpp`, `port/features/bartz-original-moves.json` (the
> recipe). Windows, clang build, USA disc. Not released. Research and the Python reference:
> [original-bartz.md](../research/original-bartz.md).

In the first Dissidia (2008), Bartz had twelve attacks. Dissidia 012 kept four and gave him new ones for the rest.
The port gives the other eight back as abilities of their own, beside his 012 ones, in his Customize lists:

| Ability | List | Learned at level |
| --- | --- | --- |
| Ascendent Lance (OG) | Bravery, ground | 1 |
| Climbarrel (OG) | Bravery, ground | 3 |
| Storm Shot (OG) | Bravery, air | 1 |
| Slidehazzard (OG) | Bravery, air | 28 |
| Soul Eater (OG) | HP, ground | 1 |
| Flood (OG) | HP, ground | 19 |
| Wind Shear (OG) | HP, air | 1 |
| Paladin Force (OG) | HP, air | 38 |

Each costs 30 CP, 15 once mastered, and takes as much AP to master as 012 Bartz's own abilities: 90 for a Bravery
attack, 130 for an HP attack (the first game's were 120 to 300). The levels are the first game's. A save made before this already has those
levels behind it, so the port gives each ability when Bartz's record is at its level or above
(`d012_mod_abilities.cpp`).

It is a part of the port, not a mod: there is no folder to add and nothing to turn on. The player needs only the
Dissidia 012 disc they already play from; the first game's disc is not needed.

## Nothing of either game is in the port

The port holds a recipe, compiled into the program. It has the moves' records (attacks, hits, projectiles, event
tracks, hitbox spheres, effect placement, ability rows, all in 012's layouts) and, for everything else, where it is
on the player's own 012 disc:

- **Animations.** Three are 012 Bartz's own. The other thirteen are the same poses as an animation of the fighter
  Bartz mimics (Cecil's Soul Eater, Terra's Flood and so on), which 012 still has. The port rebuilds each on Bartz's
  skeleton from the owner's animation on the disc (`d012_gmo_motion.cpp`).
- **Effects** are copied from those fighters' effect packages on the disc into his (`d012_effect_package.cpp`), with
  their sounds.
- **Sounds** of the moves are his own bank's, or a clip of Cecil's or Terra's bank copied into an empty slot of his.
- **Help texts** are the port's own words, laid out as 012's: lines of about 30 characters (the box does not wrap),
  and the game's colour codes (unit 0x0010, two units that pick the colour, the text, unit 0x001B) for the equip
  bonus and the move's effects. Each ability keeps the first game's equip bonus set; every one of the eight is a set
  012 Bartz's own abilities also have (Hazard Raid's, Slide Shooter's and so on), so the bonus line says what that
  set gives.

The result is eight files, served to the game in place of the package's own (`d012_package_mods.cpp`); the disc is
never written:

| File | What changes |
| --- | --- |
| `obj/p_fiv100.objx` | his moveset: 16 attacks, 29 hits, 10 projectiles, 16 event tracks, effects, sounds, banners |
| `obj/P_FIV100_1P.gmo`, `_2P`, `_3P`, `_ZAKO` | his models: 16 animations added to each |
| `general_archive/main/main.bin` | the ability table, and what each level gives |
| `general_archive/main/EN/main_lang.bin` | the abilities' names |
| `text/EN/mess_pk_customize_ability_0.bin` | their help texts |

## When it is built

The first time the game opens its package, which is when the disc can be read. The port reads 25 files of the
package (about 45 MB), checks each against the size and CRC-32 the recipe has for it, builds, and checks the eight
results against the sizes and CRCs the recipe says they must have. On the PC this was tested on, that takes a
quarter of a second. The files are then kept in `<data dir>/cache/original-moves/`, and later starts read them from
there (checked again by size and CRC); the folder can be deleted at any time.

The USA and European discs (ULUS10566, ULES01505) have these 25 files byte for byte the same, so one recipe serves
both.

## Room for his sounds and effects

The moves bring 24 sound clips into Bartz's sound bank (the moves' own, and those of the effects copied from other
fighters), so it is 543 KB instead of 365 KB; no fighter on the disc has a bank over 368 KB. His effect package grows
the same way, from 1.03 MB to 1.42 MB; the biggest on the disc is 1.17 MB. A battle copies both fighters' banks
into one sound heap of 0x237000 bytes (main `0x088A0C10`), and then the battle's shared bank (534 KB: the chase
prompt, EX Core, assists and the rest). It copies their effect packages into one effect heap of 0x3DD000 bytes
(main `0x088A5054`), and then the stage's and the battle's shared effects, the sparks of a rail among them, with
their sounds. Each goes in at its own size, so whatever is loaded last is what finds no room.

The owner heard both on 22 November 2026: first the chase prompt, EX Core and assist sounds, then the sound of
grinding along a rail. So when the moves are on, the port makes the sound heap 1 MiB bigger and the effect heap
2 MiB bigger, from the guest memory above the game's 32 MiB that also gives bigger models their room
(`model_memory::fit_battle_heaps`, `d012_model_memory.cpp`). That covers his bank and package on both sides and in
two assists.

`D012_TRACE_HEAPS=1` logs every allocation of the game's handle heaps (`0x088673CC`) and block heaps
(`0x0885C6CC`) that finds no room; `=2` logs every allocation, so a heap's use can be added up. One block heap
allocation (40,112 bytes from `0x08866554`) finds no room in every battle, with the moves on or off: that one is the
game's own.

| Bartz against Bartz, Old Chaos Shrine | Sound heap | Effect heap | Found no room |
| --- | --- | --- | --- |
| Moves off | 2,354,712 of 2,322,432 bytes asked over the battle (some given back) | 3,175,375 of 4,050,944 | nothing |
| Moves on, the game's heaps (`D012_NO_BATTLE_ROOM=1`) | 2,176,232 | 3,996,207 | the shared sound bank |
| Moves on, the bigger heaps | | | nothing |

The effect heap was 99% full with the moves on, on a stage without rails; a stage with more effects of its own, or
assists, filled it.

Every stage of Quick Battle, Bartz against Bartz with the moves and the bigger heaps (`build/bartz-test/stage_sweep.sh`,
22 November 2026), by what the effect heap was asked for over the start of the battle against the game's 4,050,944
bytes: Pandaemonium 4,057,407 and Kefka's Tower 4,059,823 (more than the game's heap holds), Ultimecia's Castle
4,050,415 and Orphan's Cradle 4,044,143 (within a few KB of it), the other ten 3.85 to 3.98 MB. With the bigger
heaps nothing found no room on any of them. A VRAM heap (`0x04110000`) misses 127,532 bytes on Pandaemonium, Lunar
Subterrane and Sky Fortress Bahamut with the moves off too: the game's own.

## When the moves are left out

Each of these leaves the game as it is on the disc and writes one line to the log, starting
`[original-moves] left out:`.

- **A disc mod is on** (Emendatus, for one; [disc-mods.md](disc-mods.md)). The mod plays its own files, and the
  memory above the game's 32 MiB that the moves' bigger heaps use is the mod's: a mod that asks for the larger
  memory takes its own blocks from it. The port then reserves none of it (`d012_hle.cpp`).
- **The disc's files are not the recipe's.** A disc mod that changes Bartz, or one of the fighters his moves borrow
  from, or the ability table. The moves were not made for those files.
- **A mod's package replaces one of the eight files.** The mod's file is used. A character mod made with
  dissidia-fan's tools carries its own `main.bin`, so any such mod leaves the moves out for now.
- **The build does not come to the recipe's bytes.** This would be a fault in the port; it asks to be reported.

| Variable | Effect |
| --- | --- |
| `D012_NO_ORIGINAL_MOVES=1` | The moves are left out |
| `D012_NO_PACKAGE_MODS=1` | No package file is replaced: the moves are left out too |
| `D012_TEST_ORIGINAL_MOVES=1` | All eight are given from level 1 (testing) |
| `D012_NO_BATTLE_ROOM=1` | The sound and effect heaps keep the game's sizes (testing; battle sounds go missing) |
| `D012_TRACE_HEAPS=1` | Logs every allocation of the game's heaps that finds no room (`=2`: every allocation) |

## Online

Both players' files must be the same, or their games fall out of step. So the build must come to the same bytes on
every machine, and it is checked: a copy uses the moves only when its eight files have the recipe's CRCs. The
battle link then compares "Bartz's original moves" between the two copies (`net::battle::must_match`): on with the
same recipe, or off on both. A copy that left the moves out cannot fight one that has them; the refusal names the
item.

The animation rebuild is floating-point arithmetic, which is where machines could differ. It uses only `+ - * /`
and square roots on doubles, in a fixed order, with its own arc cosine and sine; no fused multiply-add (the two
files are built with `-ffp-contract=off`, and `d012_gmo_motion.cpp` refuses to build with fast maths). GCC and clang
builds on Windows come to the same bytes as the Python reference. The whole port has not been run on Linux or
Android yet; if a build differed there, its check would fail and it would leave the moves out rather than use
other bytes.

## For developers

The recipe is written from the first game's disc by the Python reference, and the C++ builder is a port of that
script's build, step for step.

```
python tools/original_moves.py recipe --og-iso OG.iso --iso 012.iso      # writes port/features/bartz-original-moves.json
python tools/original_moves.py build-012 --iso 012.iso OUT               # the Python build, from the recipe alone
python tools/original_moves_refs.py --iso 012.iso                        # reference files into build/original-moves-dev
```

After a change to the recipe or to either builder, build the checks and compare (they read the game's files from
`build/original-moves-dev`, so they are no part of the test run):

```
cmake --build <build dir> --target check_original_moves check_gmo_motion check_effect_package
<build dir>/bin/check_original_moves build/original-moves-dev port/features/bartz-original-moves.json
```

`check_original_moves` builds with the C++ builder and compares each of the eight files with the Python build's and
with the recipe's check block. `check_gmo_motion` compares the thirteen rebuilt animations one by one, and
`check_effect_package` the effect copies. The port's own tests (`d012_tests`) cover what needs no game files: the
recipe is read whole, its check block is as the builder expects, and a disc that lacks a file stops the build with
a reason.

The effect copier has a known fault, in both the Python and the C++ (which writes the same bytes): copying an
effect into a package whose table for one kind of resource is empty (Warrior of Light's and Cloud's have no `VTL`)
writes a package neither can read back. Bartz's package has one of each, so his build is not affected; it matters
only if another fighter is ever given moves this way. Fixing it changes bytes, so both builders must change
together.

Raise `kBuilderVersion` in `d012_original_moves_feature.cpp` when the builder changes what it writes, so that
caches made by the older one are not used. The recipe is part of the port's source hash, which online play
compares.

## Verified (21 November 2026, Windows, clang build, USA disc)

- The C++ build is byte for byte the Python build for all eight files, with GCC 14 and with clang (llvm-mingw).
- In the game, with no mods folder: built from the disc in 0.25 s, the eight abilities given to a level 1 Bartz with
  `D012_TEST_ORIGINAL_MOVES=1`, and each move used in a battle against a second player's fighter.
- A second start reads the files from the cache (0.06 s; 1.4 s once on a slow disk).
- Bartz against Bartz in a local versus battle: both of the bigger models load, each fighter's hitbox groups are
  placed, and Soul Eater (OG) hits.
- The animation rebuild alone also comes to Python's bytes under MSVC 14.44, GCC 13 on Linux (WSL) and the Android
  NDK's clang built for x86-64; its 64-bit ARM object has no fused multiply-add instruction.
- The effect copier alone also comes to Python's bytes under MSVC 2022, and GCC 13 and clang 19 on Linux (WSL): the
  32 copies of this build, and about 15,000 more steps of made-up and damaged packages, with both sides refusing
  the same ones.

## Checked after the merge into main (22 November 2026, Windows, clang build, USA disc)

- **A real save** (a copy of the owner's Steam Deck save, Bartz Lv 18; the clean copy is kept read-only): the
  five abilities his level has reached are given (Ascendent Lance, Climbarrel, Storm Shot, Soul Eater, Wind Shear)
  and the three above it are not. Soul Eater (OG) equipped and saved: the save file holds no (OG) entry, the side
  file holds 5 learned and 1 equipped, a reload brings all of them back, and Soul Eater (OG) hits in a battle.
- **The same save in a port without the moves** (`D012_NO_ORIGINAL_MOVES=1`, and the port as it was before the
  merge): it loads, Bartz's slot that held Soul Eater (OG) is empty, and a battle runs.
- **The moves left out by a mod** (a package replacing `obj/p_fiv100.objx`): the abilities are taken back, the side
  file puts nothing back, and a save leaves the side file as it was; the next start with the moves brings the five
  learned ones back (the equipped one not, as the save changed in between).
- **Emendatus** (a disc mod): with the mod on, the port reserves no memory above 32 MiB (the mod takes its own
  blocks there) and leaves the moves out; the mod boots, loads its starter save and fights with Bartz.
- **The Japanese voices**: both serve package files; the moves' plan goes past the voices' 512 MiB room, and the
  game reads the same Japanese files with the moves on as off.
- **Every Quick Battle stage**, Bartz against Bartz: no sound or effect finds no room.

## Not done

- Linux, Steam Deck and Android builds have not been run with it.
- Other languages: the USA and European packages hold the ability names and help texts under `EN` only, and that
  is what the build changes. How the game shows these abilities with another language chosen is not checked.
- With a character mod on, the moves are left out (see above); merging into a mod's files is not attempted.
- Bartz with his alternate costumes, and Bartz against the fighters with the biggest models, have not been played:
  his models are 280 KB bigger than the disc's and the game's model pool has little room to spare.
- The moves' sounds have not been checked by ear; Flood's water sound is Terra's, as a stand-in.
- **Rollback's cost of the larger memory.** The port's guest memory is 64 MiB for these moves' heaps (32 MiB before),
  and online rollback saves all of it every frame: on the PC this was measured on, a save takes 4.3 ms instead of
  2.0 ms, and the 11 saves rollback keeps through the public server take about 750 MB instead of about 400 MB. This
  is so for every player online, whether or not anyone picks Bartz. Saving only the parts of the upper 32 MiB the
  port hands out (about 9 MiB) would bring it near the old cost; that is a PortableKit change, not made.

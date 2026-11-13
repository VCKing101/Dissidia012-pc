# Cheats

> 6 November 2026. PortableKit patch 0085 and `port/host/d012_cheats.cpp`. Windows, clang build, USA disc.

The owner asked for cheats that give the same results as PPSSPP's. The port's menu (Esc, or L3 + R3) has a
**Cheats** page, between Network and System:

- **Use cheats** turns every cheat on or off at once (`cheats.enabled`).
- **The port's own cheats**, under Money and points, Items, Battle and Opponent. A cheat with several amounts is one
  row with Off and each amount. They are saved in `settings.ini` as `cheats.builtin=key=choice,...`.
- **PPSSPP cheat file** opens a page of the cheats in the player's own PPSSPP cheat file, which turns them on and off
  and writes the file back, as PPSSPP's cheat screen does.

## How cheats run

PortableKit's engine (`host/cheats/`, patch 0085) reads CWCheat codes with PPSSPP's meaning and runs them as PPSSPP
does: every cheat that is on, from its first line, every 77 ms of the game's time. The port's own run first, then the
file's. An address in a code is an offset from `0x08800000`.

The file is PPSSPP's: `PSP/Cheats/ULUS10566.ini` in the memory stick folder (`data/ms0` in an install;
`ULES01505.ini` with the European disc). PPSSPP's own file for the game can be copied in as it is. Action Replay
lines (`_M`) are read but not run.

A recompiled game does not change when its code is written, so a write into the game's code (the executable's code,
or an overlay's) is left out. The page marks such a cheat **Changes code**; its other writes are made. Running those
needs the interpreter (a later step).

## Compared with PPSSPP

PPSSPP 1.20.4 was run beside the port with the same save (its own memory stick in `tools/local`, nothing of the
player's), and both were read at the PP Catalog through PPSSPP's debugger. The catalog's copy of PP lies at
`0x096B2DC0` in both, with the same table around it, so the public list's data codes land where they do in PPSSPP. The
catalog copies PP in only when it opens, in PPSSPP too: the list's 20000 PP shows after leaving the catalog and opening
it again. The port's own PP cheat writes the save's PP instead (next sections).

One difference: the 3.9 MB block the game asks the system for (its descriptor at `0x09B4D420`) starts at `0x09B85700`
in the port and at `0x09B95A00` in PPSSPP, `0x10300` lower, with the same contents after the shift. No code in the
public list touches it.

Saves the port writes are 8 bytes longer than PPSSPP's (257,248 against 257,240), and PPSSPP cannot load them; cut
to PPSSPP's size, it can. A separate fix.

## The port's own cheats

The codes are those of the public CWCheat list for ULUS-10566 (addresses and values: facts about the game's memory);
the names, the grouping and the choices are the port's. The CWCheat database (cheat.db) has no licence and is not
used or shipped.

| Row | Choices | Writes | Seen in the game |
| --- | --- | --- | --- |
| Gil | 1,000,000 / 99,999,999 | `0x08BBFA00` | both, in Customization and on the world map |
| KP | 250 / 1,000,000 / 99,999,999 | `0x08BEE31C` | all three, on the world map |
| PP | 20,000 / 9,999,999 | `0x08BCCB38` | both, in the PP Catalog the first time it opens |
| All equipment ×99 | on | a count of 99 every 4 bytes from `0x08BBE41A`, 677 of them | the Equip list, every item ×99 |
| All accessories ×99 | on | the same from `0x08BBEEC6`, 589 | the accessory list, every item ×99 |
| Mala Bestia accessories ×99 | on | 9 of those counts | Growth Egg, Valor Resin, Chocobo Down, Wing and Feather, Supershine, Mog's Amulet, Qu'Bia Tail ×99 |
| All summons | on | 61 summonstones from `0x08BBF8BD`: 10 uses and a 3 | every summonstone, 10 uses left |
| Infinite HP, Max HP 999,999, Bravery 9,999, Infinite EX, Infinite Assist, Level 100 | on | 32 bits at the word at `0x08B633B0` minus `0xE58`, `0xE54`, `0xE4C`, `0xE48`, `0xE44`, `0xE60` | HP stayed at 1242 through a 755-damage HP attack (0 without the cheat); HP 999999; Bravery 9999; Enter EX Mode offered; the Assist gauge full with Use Assist offered; Lv100 |
| Opponent: Bravery 0, Max HP 100, No EX, No Assist | on | the same minus `0x2C`, `0x34`, `0x28`, `0x24` | Bravery 0 and a Break; max HP 100; against a Very High CPU, its EX and Assist gauges stayed empty (without the cheats it filled both and entered EX Mode) |
| Opponent: Defeat with Select | on | `0x38` = 999,999 while Select is held (a joker line) | the opponent falls at once |

**Level 100 is a real level-up.** It sets the level of the fighter in battle, and the game makes that the character's level: after the cheat is turned off, Customization still shows Lv100 with 1,969,359 EXP and HP 6,999, and the game keeps it when it saves.

The list's codes were changed in five places:

- Its **1,000,000 Gil** writes `0x08FF93F4` and `0x08FFB2D0`; the gil shown stayed at 788, so the port writes its
  Max Gil address, `0x08BBFA00`, for both choices.
- Its **1,000,000 KP** also writes `0x09692D14`, in the game's pool, which other data uses in a battle. The other two
  KP choices write only `0x08BEE31C` and show on the world map, so 1,000,000 does the same.
- Its **PP** codes write `0x096B2DC0`, the PP Catalog's copy, at all times. The port's write the save's PP,
  `0x08BCCB38` (next section).
- Its longer **accessories** code (672 counts) reaches the summonstones' bytes, so the port uses the shorter one (589).
- Each **battle** code is followed in the list by a write of 0 to `0x0880052C`, below the game, which the port leaves
  out.

Left out of the list: **CP Doesn't Matter** and **Remove Bloom** (they write the game's code; the port has its own
Bloom switch), the **widescreen** and **frame rate** codes (the port's aspect ratio and frame rate settings do those,
and the codes would fight them), the **Chaos** codes (a fan mod written as cheats; the owner chose to leave it out),
and the **AP after battle** codes (`0x09AEA568`): with 255 chosen, a Quick Battle still gave 1 AP.

## PP and the game's pool (issues 16 and 20)

> 12 November 2026. Windows, clang build, USA disc, a save with 420 PP.

The list's PP code writes `0x096B2DC0` every pass. That word is PP only while the PP Catalog is open: the catalog
copies the save's PP there when it opens (the word read 420, with `00000000 00000001 | 00000004 00000000` around it)
and back when it closes. The rest of the time the game's pool holds other things there, and the cheat overwrote them:

| When | What `0x096B2DC0` held (seen with the cheat off) |
| --- | --- |
| Loading the save, from the title | the save data being read (`0xDD399908`) |
| Loading Yuna vs Garland, Quick Battle | Yuna's `P_TEN110` motion `_651B` (file `+0xD37E4`), its 140th Animate command's FCurve reference, `0x000C001D` (`0000004B 00000000` before it, the next command's `000080B3 00000018` after it) |
| Later in that battle | other battle data, changing |

- **Issue 20** (v0.7.1, Yuna vs Garland, FIELD mode 7): main `0x089B72F4` (a GMO motion's setup) looks up each
  Animate's FCurve with `0x089B49F4(motion, 12, ref & 0xFFF)`. With 9,999,999 (`0x0098967F`) written there, the index
  was `0x67F` in a motion of 81 FCurves: the lookup returned 0 (its count left at `0x67F - 81 = 0x62E`, the crash
  log's `a2`) and the next read at address 0 stopped the game. On the disc no Animate in any of the 1,168 object GMOs
  names a missing FCurve. With the list's code in a cheat file, Yuna vs Garland in Quick Battle showed `0x000C001D`
  overwritten four frames after it loaded; that battle went on, as its setup had already read it.
- **Issue 16** (v0.6.0, Arcade): the Brave display's free ring, slot 12, lay at `0x096B2DC0` and held `0x4E20`,
  the 20,000 PP choice (`d012_brave_pool_guard.cpp`).

The save's PP is the word at `0x08BCCB38` (save root `+0x1DE00`, the last u32 of section 1 in
[roster-and-save.md](../research/roster-and-save.md) 1.6; 420 was found there and nowhere else in the save), in the
main RAM block that also holds Gil and KP. The port's PP cheat writes it: the catalog shows the amount the first time
it opens, and Yuna vs Garland loaded and fought with the word at `0x096B2DC0` left to the game.

Buying with it (13 November 2026): with 20,000 chosen, Preset - Lv 50 (900 PP) left 19,100 in the catalog. Leaving the
catalog, the game autosaved; opened again, it showed 20,000 with the preset bought. That save, loaded with no cheats
(`D012_NO_CHEATS=1`), opened the catalog at 20,000 PP with the preset bought.

## Switches

| Variable | Effect |
| --- | --- |
| `D012_NO_CHEATS=1` | No cheats at all; the page says so |
| `D012_CHEATS=gil=2,kp=1,...` | The port's own that are on, for one run (the rows are then greyed) |
| `D012_TRACE_CHEATS=1` | Each pass's cheats, with what they did |
| `D012_CHEAT_INTERVAL_MS=<ms>` | Another interval than 77 ms |

## Verified (6 November 2026, Windows, clang build, USA disc)

- `portablekit_cwcheat_tests`: 30 checks of the parser and every code type.
- A file cheat (20000 PP) gave the same result as in PPSSPP, at the same address.
- The port's own, as in the table: on the game's screens, through input scripts and window captures.
- The page: its headings and rows, the cheat file page and back, and a change made with the pad saved to
  `settings.ini`.

Not verified: a cheat that writes code, the European disc, Linux and the Steam Deck. The battle cheats were tried
in Quick Battle only, against Lightning.

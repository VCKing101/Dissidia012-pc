# The Manikin look

> 23 November 2026. `port/host/d012_manikin_look.cpp` (its place in the game), `d012_manikin_rules.cpp` (the rules,
> checked by `port/tests/d012_tests.cpp`). Windows, clang build, USA disc. Not released.

Every character the game has a manikin of gets one more look, **Manikin**, after its others: Square on Player Select
(Quick Battle, Party, Customization's Character Select), Triangle in Customization. It is the game's own manikin,
the crystal copy fought in Story Mode and the Labyrinth: its model, its assist and world map versions and its voice.
The thirty characters with a manikin are every fighter but Chaos and Feral Chaos.

It is a part of the port, not a mod: there is nothing to add or turn on, and the disc is never written.

## What the game already has

A manikin is costume 0 of its character. `objentry.bin` has a fighter row `P_<code>_ZAKO` for each (and `_ZAKO_A`,
`_ZAKO_W`), with its own model, `.cosx`, `.exex` and voice bank, and the character's moveset
([roster-and-save.md](../research/roster-and-save.md), 1.2). A battle given costume 0 loads it as it loads any other
look: the object loader takes the row `(character, costume, not assist)`. Only the menus never offer it:

- The looks a character has are a mask (main `0x088D5E0C`): bit 1 Normal, bit 2 Alt 1 and bit 3 Alt 2 when unlocked,
  bits 4 to 12 the DLC looks there are. The menus' character lists (main `0x088D5AC8`) carry it.
- Square or Triangle asks for the next look (main `0x088D608C`): the next bit of the mask above the current look,
  from 13 back to 1. Bit 0 is never reached.
- The look's name is string `table[costume]` of a 26-string label table ("Normal", "Alt 1", "Alt 2", "DLC 1" to
  "DLC 9", twice, then two dashes) in the screen's text pack. The three tables of string numbers, in the menu overlay,
  name costume 0 Normal: `0x08A85B92` (`0, 0, 1, 4, 5..13`), `0x08AE45B4` and `0x08B0159C` (`2, 2, 3, 14, 15..23`).

Chaos's costume 0 is his first form, and Feral Chaos's `_ZAKO` row is his own model again; the game's text names
the other thirty manikins (`text/EN/mess_pk_chara_select_0.bin`, table 0, strings 66 to 95, "False Hero" to
"Fallacious Giant").

## What the port changes

- **The looks.** The mask and the lists' entries gain bit 0 for a character with a manikin, when its costume 0
  fighter row is there (main `0x088470DC`).
- **The next look** goes on from the last look to the manikin, then to Normal: Normal, Alt 1, Alt 2, the DLC looks,
  Manikin. With bit 0 clear it is the game's own step, for every mask and look (checked against a transcription of
  the original's instructions).
- **The name.** "Manikin" is added as string 26 of the label table in the six text packs that have one; they are
  served in place of the disc's (`package_mods::add_late_source`, [mod-folder.md](mod-folder.md), section 9):

  | Text pack | Table |
  | --- | --- |
  | `text/EN/mess_pk_chara_select_0.bin` | 6 |
  | `text/EN/mess_pk_party_setting_0.bin` | 4 |
  | `text/EN/mess_pk_customize_resident_0.bin` | 3 |
  | `text/EN/mess_pk_creation_room_0.bin` | 6 |
  | `text/EN/mess_pk_story_mode_0.bin` | 9 |
  | entry `AE729F6B` (a customization pack whose name the game makes) | 9 |

  The table is found by its shape, not its words. The USA and European discs have only these English menu texts.
  The menu overlay is read from the disc each time it loads, which puts its tables back, so each call of the three
  functions above sets costume 0 of each table to 26 again; a table that is not the disc's is left alone.
- **The battle portrait.** The battle screen shows `menu/JP/battle/chara_image/<row name>.gim` (battle `0x08B21588`);
  the disc has one for every look but the manikins'. A manikin gets its character's Normal portrait
  (`P_<code>_1P.gim`, and `_1P_2` for Cecil's second form), as Player Select and the loading screen show: the
  portrait's name passes through main `0x08867984` while the byte at `0x089D5F1C` is set (the battle loading its
  portraits), and the port changes it there. The game's own manikin icons (`p_zako_n.gim` and the like) are generic
  enemy badges, not likenesses.

The overrides are of main functions called from other generated units (the menu and battle overlays, main
`0x0892905C`, main `0x08862C40`/`0x08862854`), so no `dispatch-calls.txt` row and no new game code are needed. The
list's own calls of the mask are inside its unit; the list override adds bit 0 to its entries itself.

## When it is left out

The log says `[manikin] the Manikin look is left out:` and why. The menus are then the game's.

- **A disc mod is on** ([disc-mods.md](disc-mods.md)): it plays its own files.
- **A mod's package replaces one of the six text packs**: the mod's file is used, and its labels have no Manikin.
- **A text pack is not laid out as the release's.**

| Variable | Effect |
| --- | --- |
| `D012_NO_MANIKIN_LOOK=1` | The look is left out |
| `D012_NO_PACKAGE_MODS=1` | No package file is replaced: the look is left out too |

A save with a manikin chosen still plays it when the look is left out, or on PPSSPP or a PSP: the battle loads
costume 0 as it is. The menus call it Normal there, and Square goes on to the next look as the game does.

## Online

Nothing to match. A look is a costume number the other player's game loads from its own disc, so a manikin shows as
one in an older port too; only that player's menus would call it Normal.

## The log

```text
[manikin] the Manikin look is on: 6 menu texts name it
[mods] package: 6 file(s) from the Manikin look, 772512 bytes
[manikin] the menu's look labels name costume 0 Manikin (3 of 3 tables)
[manikin] the battle portrait of a manikin is its Normal look's (menu/JP/battle/chara_image/P_GST130_1P.gim)
```

## Checked

Windows, clang build (`tools/local/port/build-d012-manikin`), USA disc, a test save, `D012_TEST_UNLOCK_FIGHTERS=1`:

- Quick Battle (1-on-1) Player Select: Square goes Normal, Manikin, Normal for Lightning (no Alt look unlocked in
  that save), and Vaan stays Normal beside her.
- Customization from Player Select (Triangle) shows "LOOK (Manikin)"; Customization's Character Select and Quick
  Battle (Party)'s Player Select show it too. A look chosen in Customization is the one Player Select opens with.
- Battles as Lightning's and Cecil's manikins: the crystal models, the manikins' own `.cosx` and `.exex`
  (`D012_TRACE_OBJECT_NAMES=1`), the battle to its end and its results, both of Cecil's forms with their portraits.
- All three label tables are the disc's when the menu loads (`3 of 3 tables`). The six packs on the European discs
  (`ULES01505`) have the same tables (read from the disc, not run).
- `D012_NO_MANIKIN_LOOK=1`: Square stays on Normal, as in the game.
- `d012_tests`: the rules (who has one, the next look for every mask, the label added to a made-up pack, the
  portrait names).

Not checked: Linux, Android, the European disc in the game, online, a character with Alt and DLC looks unlocked (the
order is checked by the tests), Story Mode's party screen (the third label table's screen was not reached by the
scripted runs; its table is set), the manikin's voice (audio was muted), EX Bursts as a manikin.

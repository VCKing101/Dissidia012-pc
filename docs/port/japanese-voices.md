# Japanese voices

Looked into on 3 November 2026 for issue 8 ("Japanese audio support"). Inputs: the owner's USA, Europe and Japan
ISOs (the same images as [regions.md](regions.md)).

## Summary

- The USA and European discs have English voices only. Neither has a Japanese (or French, German...) voice folder;
  their other languages are text only.
- The Japanese disc (NPJH50377) has Japanese voices only, under the same names in a `JP` folder.
- The port can play the Japanese disc's voices in the USA or European game, with its English text, from the
  player's own Japanese disc image: `port/host/d012_voices.cpp`. Nothing of the Japanese disc is copied or shipped.

## Where the voices are

Every voice file is in `PACKAGE.BIN`, in a language folder: `voice/EN/battle/voice_P_GST130_00.scd` (Lightning's
battle bank) on the USA disc, `voice/JP/battle/voice_P_GST130_00.scd` on the Japanese one. The game finds a file by
a hash of its lower-case name alone ([character-assets.md](../research/character-assets.md), section 1), so neither
index holds names.

Compared by index (hash, size, the first four bytes of the file):

| | USA | Japan |
| --- | --- | --- |
| Files in `PACKAGE.BIN` | 11,043 | 11,193 |
| In both (same hash) | 4,489 | 4,489 |
| Sound files of one disc only (`RIFF` ATRAC3, `SSCF` banks, `VOLD` clip lists) | 6,266 | 6,354 |

The European disc's index is the USA one.

The battle voices match one for one. For each of the 32 characters with a clip list
(`voice/<LANG>/voice_load_P_<code>.bin`), every clip id of the English list is in the Japanese list under the same
name (the Japanese lists have up to two more rows). All 2,179 streamed clips (`voice/<LANG>/battle/<name>.at3`) and
every battle bank (`_00`, `_zako`, `_a_00`) are on both discs.

## Pairing the files without names

The hash is CRC-32/BZIP2, which is linear. Two names of the same length that differ only in `jp` against `en`
have hashes that differ by a value that depends only on how many characters follow the language code, whatever the
name is. So for each Japanese-only file, `hash XOR delta(n)` for n = 0..63 gives the hash of the English name of
every possible length; when exactly one of those is an English-only file, the two are the same file in the two
languages.

- 6,222 of the 6,354 Japanese-only sound files pair with exactly one English-only sound file: 6,063 `RIFF`, 127
  `SSCF`, 32 `VOLD`. No hash pairs with two, no English file is claimed twice, and every pair has the same type.
- 44 English sound files have no Japanese twin (English-only lines, or another name); they stay English.
- The same pairing also finds the language folders' text and pictures (`MIG.`, `menu`, `mess`); those are left
  out, so the text stays English.

## How the port plays them

The game reads `PACKAGE_INFO.BIN` into a fixed buffer of `0x20F8C` bytes (main `0x08871938`), which leaves room for
210 more records: the Japanese files cannot be added under their own names. Instead each paired English record is
moved, in the index the game reads, to the Japanese file:

1. At the first open of `PACKAGE_INFO.BIN` the port reads both indexes, pairs the files and keeps the sound ones
   (it reads the first four bytes of each Japanese candidate from the Japanese image).
2. The game's `PACKAGE.BIN` gets the paired Japanese files after its own end, one per whole sector (about 200 MB,
   served from the Japanese image; nothing is copied). The paired records point there, with data-install category 0
   so a data install's English copy is never used. The index's sector field has 20 bits (2 GB); the files end
   near 1.2 GB.
3. Both are PortableKit file filters (`add_file_provider`): the game's own files are never changed.

Movies: 29 of the USA disc's 34 movies are the same size on the Japanese disc. In the three compared stream by
stream (`DES00_001`, `DES01_001`, `LOP`) the video is byte-identical and only the audio differs, so the port plays
the Japanese file whole. The other five (`DES01_010`, `END`, `UP_END`, `UP_OPN`, `UP_PRI`) have other video as well
(sizes differ) and stay English. The Japanese disc has two movies more (`DES01_013`, `DES10_010`), which the USA
game never asks for.

The game also has a voice-language table of its own, apart from the text one: `0x089D5EF4` maps the language
setting to a voice folder (`EN JP EN EN EN EN EN EN EN`: every language but Japanese hears English), used by the
name rewrite `0x08867984` when its second argument is 1 (only the list loader at `0x088EAEC8` passes 1). The port
does not need it: the game keeps asking for `voice/EN/...` and gets the Japanese file.

## In the game's Options

Two rows after Voice in the main menu's Options, drawn by the game like its own (`port/host/d012_voices_menu.cpp`):

- **Japanese Voice Disc**, always shown: the image's file name, or "Not set". Confirm (✕ on the USA disc) opens the
  port's file browser over the game (the setup's, which works with a gamepad); an image dropped on the window counts
  too. A file that is not the Japanese release is refused with the reason, and the browser stays open.
- **Voice Language**, English or Japanese, shown once a disc is chosen. The game applies it when the player leaves
  Options, like its other options; the files it loads from then on are in that language (no restart).

How (menu overlay addresses unless marked main):

- **The rows are records of the game's option table**, `optp` in `general_archive/main/main.bin`: 27 records of 0x1C
  bytes `{ordinal, option id, default (low nibble), value kind (0xFF: a list), shown for Japanese / other languages /
  another mode, -, title, eight values, help}`, the texts being string numbers of `text/EN/menu/option.bin`. Main
  `0x088C5044` copies the chunk (1,212 bytes) into a 2,048-byte buffer (`*0x089D76D8`; records at `*0x089D76DC`, the
  section after them at `*0x089D76E0`). The port puts two records after Voice's (option ids 0x70 and 0x71, unused in
  the game's 0x78), moves the rest down, renumbers the ordinals and fixes the count, the size and `*0x089D76E0`. The
  Options screen (`0x08AB9AEC`) lists every record shown for the language (main `0x088F4554`).
- **Their text** is five strings, numbers 200 to 204, added to `option.bin` by the port's copy of it (served after
  PACKAGE.BIN like the voices; the game's 174 strings stay where they are). The values reuse the game's "English" and
  "Japanese" (145, 146, Subtitle Language's). The disc's name is a string of 40 characters kept in the screen's loaded
  copy (screen `+0x8F8`, its text in the clear), so a disc chosen while the screen shows renames the row at once.
- **Values stay out of the save.** Options are bytes of the save at `*0x089D712C + 0x1DE04 + id`; the screen copies
  them into its cache (screen `+0xAB4`) on opening (main `0x088F472C`) and back on leaving (main `0x088F475C`, after
  main `0x088F4790` compares them to ask whether to save). The port wraps those three: its two values come from
  `settings.ini` (`voices.japanese_disc`, `voices.language`), take effect on leaving, never reach the save, and do
  not count as a change of the game's options.
- **Confirm on the disc row**: the screen does nothing for confirm on a row of values. Main `0x088D4B74` places a list
  screen's hand every frame; with the Options screen's `+0x8D8` it tells the port the screen is up and where its
  cursor is (`+0x8B0`, rows at `+0xD60`). The pad wrapper then takes the press from the game and opens the browser
  (PortableKit's `ui::open_panel_over_game`, patch 0079). After a disc is chosen the screen lists its rows again
  (`0x08AB9AEC`), its cursor's limit (`+0x8B8`) follows, and `0x08AB856C(object, 1)` draws them all.

`D012_JAPANESE_VOICES=<image>` sets the disc and Japanese voices for one run without saving them (`0` keeps the voices
English); `D012_NO_VOICE_OPTIONS=1` leaves Options as the game has it. The log says:

    [voices] Japanese disc <image>: ready; voices Japanese
    [voices] 6222 sound files of the Japanese disc paired with the English ones (6448 files paired by name, 6222 of them sound), 190 MB, in 36 ms
    [voices] Options has the Japanese voice rows after Voice
    [voices] the game has read 1 Japanese sound file (the last 283968 bytes)

and `[voices] disc0:/... plays the Japanese disc's` for each movie it swaps. A movie is swapped when the game's file
has a last byte where the Japanese file of the same name has its last, and none after it (the same size).

## Checked

3 November 2026, recompiled build on Windows, USA disc, `tools/port/gfx_tour.py` on the tour's data copy
(`--variants nointerp`), output in `build/port/jpvoice/`:

- Pairing: 6,222 files moved in 35 ms at start, the same count as the offline check in Python.
- Quick Battle (`qb_chaos_shrine`, Lightning against Lightning): the game read Lightning's Japanese battle bank
  (283,968 bytes, the Japanese file's size; the English one is 291,504), her Japanese clip list (1,672 bytes; English
  1,648) and a third Japanese file. The battle ran to its end; text, subtitles and the HUD are English and the
  captures look as without the switch.
- Story Mode (`story_gateway`): world map, a gateway and the moogle board, nothing out of place.
- Options (a scripted run on `build/port/data-jpvoice`, a copy of the tour's data with no disc set): the disc row
  after Voice reads "Not set" with its help; ✕ opened the browser over the game; the Japanese image dropped on the
  window was taken (pairing 3.9 s from a cold drive), the row showed its name and Voice Language appeared under it,
  English; right made it Japanese; leaving Options asked nothing and logged "the voices are now Japanese"; both keys
  were in `settings.ini`. The next start (a Quick Battle) came up with Japanese voices and read the Japanese files.
- In the game, 4 November 2026: the owner chose the disc from Options on the same data copy, confirmed it works, and
  found both settings kept after closing and starting the game again.

## Not checked

- A movie: no route played one, so the movie swap has not run.
- Story scenes with voiced dialogue, and the streamed battle clips (`li_battle_NN.at3`): the 90-second battle read
  only the banks.
- Lip-sync and subtitle timing against Japanese lines of other lengths.
- The European disc (its index is the USA one, so the pairing should be the same), the Steam Deck, a data install.
- Online: voices are local only and nothing is sent, but a battle between a player with them and one without was not
  tried.

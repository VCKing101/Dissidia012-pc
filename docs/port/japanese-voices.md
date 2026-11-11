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

Movies: every story movie plays with the Japanese disc's sound and the game's own picture (from 11 November;
"Story scenes" below has how). 29 of the USA disc's 34 movies are the same size on the Japanese disc, and in the
three compared stream by stream (`DES00_001`, `DES01_001`, `LOP`) the video is byte-identical. Six have other video:
`DES01_010`, `END`, `UP_END`, `UP_OPN`, `UP_PRI` (other sizes) and `UP_GIL` (the same size). Before 11 November the
port played a same-size movie's Japanese file whole, so `UP_GIL` showed the Japanese picture, and the other five
stayed English: `UP_END` is the 012 Chapter 8 ending of issue 18. The Japanese disc has two movies more
(`DES01_013`, `DES10_010`), which the USA game never asks for.

The game also has a voice-language table of its own, apart from the text one: `0x089D5EF4` maps the language
setting to a voice folder (`EN JP EN EN EN EN EN EN EN`: every language but Japanese hears English), used by the
name rewrite `0x08867984` when its second argument is 1 (only the list loader at `0x088EAEC8` passes 1). The port
does not need it: the game keeps asking for `voice/EN/...` and gets the Japanese file.

## Story scenes (issue 18)

11 November 2026, for issue 18: with Japanese voices on, the Vaan and Terra scene of 012 Chapter 2 played the wrong
voice, 013's narrations mixed English and Japanese lines after the Prologue, and the 012 Chapter 8 ending movie
stayed English. Found from the two discs and fixed in `port/host/d012_voice_scenes.cpp` with `d012_voices.cpp`.

### What was wrong

A story scene is a `talkevent/*.evex` file: an ARC archive (`"ARC\x01"`, a u32 chunk count, then from byte 16
`{u32 kind, 4-byte tag, u32 offset, u32 size}` per chunk). The first chunk is the script (tag `R_UP`, `R_ST`, `R_FF`,
`M_FF`...), then sometimes `text` (the scene's own lines, a language container), `masw` and others. In the script:

- 30 track offsets at `0x408`. Each track is a u32 count and that many 32-byte commands `{u32 op, u16 1, u16 step,
  u32 args[6]}`; track 0 runs the scene, the others its characters.
- At `0x480` a word, the offset of the voice table and the count of the 0x40-byte paths at `0x48C` (sequence files
  and the text file, `text/M_/event/...`). The voice table runs from its offset to the end of the chunk, 0x40 bytes a
  path, `voice/jp/event/<folder>/<name>.at3` (the game rewrites the language).
- Track 0's commands that matter here: `0x320` plays the next voice of the table; `1 [n]` waits n frames (60 a
  second); `0xC8 [n]` and `0xC9` show and hide speech line n, `0xD6 [n, ...]` and `0xD7` a narration line, `0xD0` and
  `0xD1` another box.

Each line waits a fixed time, tuned to its own release's recordings, and the English release rewrote scenes: 302
voiced scenes have different scripts on the two discs (55 with other voice lists, 247 with other waits only). The
Japanese voices played on the English scripts:

- **013's narrations**: the English ones have more lines. Squall's says lines 91, 92, 94 and 95 over four
  recordings; the Japanese one says 91 and 94 over two, its line 94 holding what English lines 94 and 95 say. So
  Japanese recording 02 played under English line 92, and the English recordings 03 and 04 followed: 37 narrator
  recordings (`na_rt/*na.at3`, in 23 narration scenes) exist only in English.
- **Vaan and Terra** (`up02_103`, scene `8441282B`): the English script plays a line of Vaan's first
  (`up02_103_000va`) that the Japanese script does not. The Japanese disc's file of that name is 42,124 bytes, the
  size of Terra's next line, and it played there, under Terra's own line a second later, with Vaan's reply 2.5
  seconds into it.
- For 109 lines the Japanese scripts play re-recordings (`voice/jp/event/kaigaiban_add/`) where the English ones play
  `ffst`, `ff11` or `ff12` files of the same name, whose old Japanese recordings the pairing found.

The text lines are numbered alike: English line n translates Japanese line n. The English text has no words for 30
lines the Japanese scripts show (short sounds the English release did not voice, and two lines it cut), and in the
narrations a Japanese line holds what the English release split into two or three lines. Every other part of the 302
scenes is the same on both discs, apart from `masw` in a few. The English game knows every command of the Japanese
scripts except `0x157` and `0x162` (one use each, in scenes `9DD69363` and `13600915`, which play).

### What the port does

With Japanese voices on, the game reads each of the 302 scenes as the Japanese disc has it, with the game's own text
in it (`voice_scenes::merge_scene`): the Japanese archive's chunks in order, from 16-byte boundaries, the `text` chunk
the game's. Nothing of the Japanese disc is kept: the scenes are made at start from both discs (18 MB, read in one
pass from each disc in about 0.1 s) and served after `PACKAGE.BIN` like the voices.

- **Voices.** Each line of a Japanese script is the Japanese file of its name, whatever the pairing found. 125 lines
  have no English file name at all (the re-recordings, and 16 lines the English release cut). The game's index of
  `PACKAGE.BIN` cannot take new names (it is searched by halving, and the data install keys files by position), so
  each is served under an English name no scene names with Japanese voices on: a re-recording under its English
  folder's name, the others under the 37 English-only narrator lines and other unused English lines. The scene's voice
  table says that name. `tools/port/jp_story_scenes.py` chose them from the two discs
  (`port/host/d012_voice_scenes_data.inc`: the 302 scenes and the 125 names, hashes and names only).
- **Text.** Eleven text files (`text/M_/event/MOVIE/M_000.bin`, the narrations, and ten `text/M_/event/RT/R_UPnn.bin`)
  and seven scenes' own text are served fixed, in every language's table. The 30 lines the English text has no words
  for get the port's (`Hmph.`, `Ngh―`, `Let them get away`...), and 30 narration lines show the English lines they
  hold, a row each (up to five rows; Squall's line 94 shows lines 94 and 95). The list is in `d012_voice_scenes.cpp`.
- **Movies** (`voice_scenes::MovieSplice`): the game's movie file with the Japanese sound. A movie is an MPEG program
  stream of 2,048-byte packs from the offset at byte 8 of its header, each a pack header and whole PES packets; the
  sound is private stream 1 (0xBD), 4 bytes (the stream number, where the first frame starts) and then ATRAC3plus
  frames of 752 bytes, on both discs from the first sound byte on. So the Japanese sound's byte n goes where the
  game's sound byte n is, and each packet's 4 bytes stay right. Both files are scanned as the game reads them;
  nothing is read ahead (`END` is 161 MB). The sound is the same length on both discs except in `DES01_010`, whose
  Japanese sound is one frame (46 ms) longer: its last frame is left out.

`D012_JAPANESE_SCENES=0` keeps the game's own scenes and text with the Japanese voices, as before. Test only:
`D012_TEST_SCENE=<scene>=<scene>[,...]` plays the second scene (hash in hex) where the game asks for the first. Each
scene loads its own stage and characters, so the Prologue's two scenes (`3DE35414` and `3843E88D`, reached from
Story Mode by leaving the story in progress and playing the Prologue) can play any other. `D012_TEST_MOVIE=<name>`
plays that movie wherever the game opens one (the title screen's `LOP.PMF`, after about 80 seconds there).
`D012_TRACE_VOICES=1` logs every Japanese sound file the game reads, by the hash of the name it asked for.

### Checked (11 November)

Windows, clang build, USA disc, scripted runs on a copy of the test data whose save is early in Story Mode
(`build/port/jpscenes/`). The sound was recorded with `D012_AUDIO_DUMP` and every voice clip of a scene found in the
recording by correlation with the Japanese file (most at 0.9 and over):

- The Prologue: both scenes play to the end with the English text and every Japanese line in order, none into the
  next (`3843E88D`, 35 lines). With `D012_JAPANESE_SCENES=0` the narration ended on its English-only line.
- With `D012_TEST_SCENE`, 18 more scenes. Among them: Vaan and Terra (21 lines; its one overlap, 0.36 s, is the
  Japanese script's own), Squall's narration (lines 94 and 95 in three rows), the narrations that join into four and
  five rows (`4C5C78BD`, `B8340999`, `C70C6228`, `C2EC3F51`), the scenes with the port's lines (`CB011B41`,
  `4D32ACBE`: "Let them get away", "Ngh―", a two-row laugh), the two with commands the English game never used
  (`9DD69363`, `13600915`), a 2D scene of 18 re-recordings (`5CF34C62`) and the scenes whose lines are served under
  other names (`B21132DB`, `9F692553`).
- With `D012_TRACE_VOICES`, all 87 lines of four scenes were read from the intended Japanese files, the renamed ones
  too. An earlier run had one wrong: a scene whose table names a file it never plays took back the name another line
  was served under. Fixed: renamed lines win, and no name in any scene's table is chosen.
- `UP_END` through `D012_TEST_MOVIE`, recorded: with Japanese voices its sound matched the Japanese disc's (decoded
  apart) at 0.96 to 0.99 all through, and the English at 0.5 to 0.8; with English voices the other way round. The
  title screen's `LOP` played through the splice.
- Start-up: 0.1 to 0.2 s for the pairing and the scenes together, from a warm disc cache.
- `d012_tests`: a language container read and written back, the joins and fills, a scene merged, and a movie spliced
  from two synthetic movies cut differently.

Not checked: the scenes in their own places in the story past the Prologue (only through the test switch), the
other 284 scenes, the
movies other than `UP_END` and `LOP` (`END` and `DES01_010` are spliced the same way, one cut differently and the
other a frame longer), the European disc's text (its lines should be numbered alike), the Steam Deck, a cold start
from a slow drive, lip movement.

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
    [voices] 302 story scenes as the Japanese disc has them (18302 KB), 126 voice lines set by them, 11 text files fixed for them
    [voices] 6543 files of the Japanese disc in place of the English ones (6448 sound files paired by name, 6222 of them sound), 209 MB, in 101 ms
    [voices] Options has the Japanese voice rows after Voice
    [voices] the game has read 1 Japanese sound file (the last 283968 bytes)
    [voices] the game has read the story scene 3DE35414 of the Japanese disc

and `[voices] disc0:/... plays with the Japanese disc's sound` for each movie.

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

- The streamed battle clips (`li_battle_NN.at3`): the 90-second battle read only the banks (story scenes:
  above).
- Lip-sync and subtitle timing against Japanese lines of other lengths.
- The European disc (its index is the USA one, so the pairing should be the same), the Steam Deck, a data install.
- Online: voices are local only and nothing is sent, but a battle between a player with them and one without was not
  tried.

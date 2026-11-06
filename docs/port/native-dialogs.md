# Native dialogs

The game asks the PSP for three dialogs: messages (`sceUtilityMsgDialog`, the autosave notice and the questions
about saving), the save-data list (`sceUtilitySavedata` LISTLOAD, LISTSAVE and LISTDELETE) and the keyboard
(`sceUtilityOsk`, the player name). A PSP draws them in its own system look, and PortableKit draws them in its
interface's look. Since 6 November 2026 the port draws them like the game's own Data Config page instead:

- the page title in a silver serif, a dark band with an instruction line, and the choices on the game's silver
  plates, the selected one on its blue plate with the pointing hand;
- the save list with each save's own icon (`ICON0.PNG`), its title and the game's detail on the right;
- the keyboard's keys on the game's bevelled cells, the selected key blue;
- the hints in the game's own button symbols (the cross and circle follow the menu's **Confirm button** setting);
- the hand's poke: as on the game's pages, every half second it pokes 2, 3, 4, 3, 2 PSP pixels toward its choice
  over five frames (measured frame by frame on Data Config);
- the game's own menu sounds: the cursor sound when the selection moves, the choice sound for an answer, a slot or a
  key typed, the back sound for backing out, and the "not allowed" sound for a key that cannot be typed.

The code is `port/host/d012_dialogs.cpp`. PortableKit keeps everything the dialogs do: the saves, the answers the
game gets, the typing, and the gamepad and keyboard controls. Only the drawing is the port's, through PortableKit's
dialog skin (`host/ui/dialog_skin.hpp`, patch 0083). The mock-ups the look was agreed from are local
(`build/port/save-ui-research`).

## Where the pieces come from

There is no save screen in the game's files to reuse: on a PSP that screen is the system's. The pieces are the
game's menu pictures, read from the player's disc when the first dialog opens. Each is a GIM (8-bit indexed, an
RGBA8888 palette, not swizzled) in a "menu pak" entry of `PACKAGE.BIN`, found through `PACKAGE_INFO.BIN` by its
name hash:

| Picture | Size | Entry (name hash) | What is used |
| --- | --- | --- | --- |
| Cursor, plates, arrows | 128x128 | `09478AE5` (54,512 bytes) | The pointing hand, the silver and blue plate halves, the scroll arrows |
| Button symbols | 128x128 | `09478AE5` | Cross, circle, triangle, square, START |
| Key cells | 128x128 | `4AA1BF07` (583,744 bytes) | The silver cell; the selected key is that cell in the blue plate's colours, as the game's blue cell has a dark stripe down its face that a key's width stretches into a block |
| Feather | 256x256 | `9B9D2137` (133,744 bytes) | The backdrop at the page's right |

Each menu screen's entry carries its own copy of the pictures it uses, so every picture is in many entries (56
copies of these four); these three entries are the smallest set. The port decodes every GIM in an entry and takes the
one whose decoded pixels have the expected fingerprint (FNV-1a 64 of the RGBA), so a disc whose pictures differ keeps
PortableKit's look rather than drawing the wrong pieces. Nothing of the game's is in the port: the hashes, sizes
and where each piece sits in its picture. The EU disc's `PACKAGE.BIN` is the USA one's but for its header
([regions.md](regions.md)), so the same entries serve both; only the USA disc was tried.

They were found by dumping the menus' textures (**Dump textures**, `D012_TEXTURE_DUMP`) on the way to Data Config,
then scanning `PACKAGE.BIN` for GIMs and comparing each decoded GIM with the dumped pictures.

## The sounds

PortableKit's skin reports what a dialog did (`DialogSkin::sound`: moved, chose, backed out, refused) from the
interface's frame. The port plays the game's sound for it with the game's own menu-sound function (main
`0x088A24D4(0, sound, 1, 0)`, as the BGM selector does), so the game's sound bank and its sound volume apply. The
sounds are the menu table's (`0x089CBE60`): 10000 the cursor (0), 10001 a choice (1), 10002 back (2; the game's
Data Config plays it when backing out), 10003 not allowed (4). The game's code has to run on the game's thread, so the
interface queues the sounds and the game's thread plays them the next time it polls the dialog: the port wraps the six
calls the game makes every frame while one is up (`sceUtilityMsgDialog`, `sceUtilitySavedata` and `sceUtilityOsk`,
Update and GetStatus) and runs the function from there (`decomp::call_game`). The function does nothing until the
game's sound is set up (its flag byte `0x089D6C98`), and the port checks that flag first; at the boot notice it is
already set.

## The title font

The game's titles are pre-drawn pictures, and it has none for "Load", "Save" or the keyboard's title, so the port
draws them in Goudy Bookletter 1911 (Barry Schwartz, SIL Open Font License 1.1), compiled into the program from
`port/fonts/`. Both setups ship its licence. It has one weight, a little lighter than the game's titles, so it is
drawn 0.3 PSP pixels heavier to each side.

## Switches

| Variable | Effect |
| --- | --- |
| `D012_NO_NATIVE_DIALOGS=1` | PortableKit's look, as before (silent) |
| `D012_NO_DIALOG_SOUNDS=1` | The game's look without its sounds |
| `D012_TRACE_DIALOG_SOUNDS=1` | Each sound played, with what the game's function answered |
| `D012_TITLE_FONT=<file>` | Another font file for the titles |
| `D012_TITLE_HEAVIER=<pixels>` | How much heavier the titles are drawn (default 0.3) |

## Verified (6 November 2026, Windows, clang build, USA disc)

Driven by input scripts with window captures (`tools/port/gfx_tour.py --script`):

- The autosave notice at start, with OK.
- LOAD GAME: the list with the save's icon and detail, "Load this data?" over the list, No back to the list and Back
  to the title screen.
- New Game: the player-name keyboard (moving over the keys, typing), then the save list with the save and the empty
  slots, "Save the data?" for an empty slot and "Overwrite this data?" (opening on No) for the save.
- `D012_NO_NATIVE_DIALOGS=1` gives PortableKit's dialogs back.
- The sounds, traced (`D012_TRACE_DIALOG_SOUNDS=1`): the notice's OK, a save chosen, a move on its question, back
  from the question and from the list, moves on the keyboard, a key typed, delete and Shift; the game's function
  returned a voice for each. The game's own Data Config backs out with menu sound 2 (`D012_TRACE_VERSUS_MENU=1`).
  Not listened to.
- The hand's poke, in frame captures of the load list: 8 to 16 window pixels at 4x (2 to 4 PSP pixels).

Not verified: clicking with the mouse, a disc whose pictures could not be read (the fallback), deleting from the list
(LISTDELETE), the symbols page and Shift on the keyboard, the European disc, and the Steam Deck.

## The blank menu (fixed by patch 0084)

As first merged (main `3cfbb32`, not released), the dialogs left the port's menu blank: after the load screen at
start, every menu page but Video showed only specks of text, with no panels. Each dialog picture and save icon is a
texture of the interface, and PortableKit's interface had room for eight. The dialogs used them up, so when the font
atlas grew for the text of another page it had none, and everything drawn from it (the panels too) sampled nothing.
`D012_NO_NATIVE_DIALOGS=1` made the pages draw again, which is how it was found. Patch 0084 gives the interface room
for 256; with it every page draws with the dialogs on (Windows, 6 November 2026). The run's own `shot` captures do
not show the menu, so the menu was checked in captures of the window.

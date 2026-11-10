# Button prompts for the controller in use

> 10 November 2026. `port/host/d012_button_prompts.cpp`, PortableKit patch 0100. Windows, clang build, USA disc.

The game's prompts show the PSP's buttons: cross, circle, triangle, square, L, R, SELECT and START. The menu's
Controls page now has **Button prompts**: Automatic, PSP, Xbox, PlayStation or Nintendo. Automatic shows the
buttons of the gamepad in use, and the PSP's with no gamepad. The game's own prompts change, the ones inside its
hints too ("Press RB while holding LB to Change Paradigms!"), and so do the port's message, save and keyboard
screens.

| PSP | Xbox | PlayStation | Nintendo |
| --- | --- | --- | --- |
| cross | A | cross | B |
| circle | B | circle | A |
| square | X | square | Y |
| triangle | Y | triangle | X |
| L / R | LB / RB (L1 / R1 on a Steam Deck) | L1 / R1 | L / R |
| START | Menu | OPTIONS | + |
| SELECT | View | SHARE | − |

The table is for the default **Confirm button** (Bottom). The face buttons follow where the pad's buttons are, so
a prompt shows the button that sends what the game asks for: with Confirm on the right, the game's cross is the
right button (B on an Xbox pad, circle on a PlayStation pad) and its circle the bottom one.

## Where the symbols are

The game draws its prompts from textures in its menu and battle files, not from its font: a Quick Battle route
with `D012_TRACE_FONT=1` asked the font for no button symbol, and every prompt on the menus, the customization
screens, battle setup and a battle came from these nine (found with **Dump textures**; the keys are as it names
the files):

| Key | Size | Symbols in it |
| --- | --- | --- |
| `00000000c1a4601a0f4147e8` | 128x128 | The icon sheet: SELECT, START, L, R, triangle, circle, square, cross. Menu bars, hints, the port's dialogs |
| `00000000077b753ce322c2b0` | 128x128 | A large cross, in battle |
| `00000000292b5c3bdf76a538` | 128x128 | Triangle beside LOOK, customization |
| `00000000f78fbcba7c0a4ec6` | 64x32 | Small cross, triangle, square, circle: EX defense |
| `000000004da4c8c7207c3a18` | 128x32 | Circle and cross beside NEXT |
| `000000002923c51e34372acf` | 64x64 | Small cross, triangle, circle, square, and an R key |
| `00000000eecbc58d528a24c2` | 64x32 | MANUAL START |
| `00000000f5efd5f2c2280980` | 128x128 | Blue and white L and R |
| `000000009e70b4097ca59bf2` | 128x128 | Opponent Info's L R page switch |

The community pack [D012FFRT](https://github.com/Niosa/D012FFRT) also replaces two textures this route never
loaded, its ability customization icons (`00000000e51b77084959dd20`) and rules icons (`0000000036786f58dfede80a`);
story mode, the world map, Labyrinth and the results screens were not looked through. A sheet not listed keeps the
PSP's symbols.

## How they are drawn

PortableKit hands the port each listed texture as it loads (`GameProfile::edited_textures`, patch 0100), and the
port draws it again at four times its size:

1. The game's pixels, scaled up as the Texture filter samples them: smooth, or the nearest pixel with Sharp.
2. Each PSP symbol painted over. A round button keeps the game's rim and glow; its dark face is painted again and
   the new symbol drawn on it. The silver L/R keys and the dark SELECT/START pills are drawn again in the game's
   colours. A symbol drawn on its own gets a dark border.
3. Letters are strokes, not a font, so they look the same on every system. Xbox letters are in Xbox's colours,
   PlayStation shapes in the PS4's, Nintendo letters light grey.

The bigger picture is drawn with the same texture coordinates, as a texture pack's image is. A texture pack's own
image for one of these textures is drawn instead. When the setting, the gamepad in use, the Confirm button or the
Texture filter changes, the renderer drops its cached textures and they are drawn again. The port's dialogs draw
their hints from the same sheet (`00000000c1a4601a0f4147e8` is the dialogs' "button symbols" picture, fingerprint
`0x1D66DFA8877D95E2`), drawn by the same code.

Nothing of the game's is in the port but where its symbols sit.

## Switches

- `D012_BUTTON_PROMPTS=auto|psp|xbox|playstation|nintendo`: the setting for this run (`input.button_prompts` in
  `settings.ini`).
- `D012_NO_TEXTURE_EDITS=1`: the game's own symbols whatever the setting.
- Each change of style is logged: `[render] button prompts: xbox`.

## Tested

On Windows with the clang build and the USA disc, a scripted Quick Battle (main menu, Player Select, the Assist
screen, battle setup, a battle with Lightning's paradigm hint), with each setting:

- Xbox, PlayStation and Nintendo drew the menu bars, the hint, the MANUAL badge and the autosave notice's hint
  as in the mock-ups the owner approved (SHARE instead of CREATE).
- Xbox with Confirm on the right showed B for the game's cross and A for its circle, and B on the notice's OK.
- Automatic started with the PSP's symbols and changed to Xbox's when the script's virtual gamepad was plugged in.
- PSP, and Xbox with `D012_NO_TEXTURE_EDITS=1`, drew the same prompts as the build before.
- All nine sheets in every style, Confirm on the right, and Sharp, drawn by the port's code from the dumped
  textures outside the game and looked at.
- PortableKit's input tests (the mapping for each pad and setting) and the port's tests pass.

The owner then played it on Windows with a DualSense and DS4Windows, whose virtual pads SDL names "PS4
Controller" and "Xbox 360 Controller". Automatic showed PlayStation's buttons for the DualSense and the virtual
DS4 and Xbox's for the virtual Xbox 360 pad. Every change of the setting in the menu showed at once, and so did
switching DS4Windows' output from one virtual pad to the other.

The prompts follow player 1's gamepad. A gamepad connected while player 1 still has one is player 2's (with Local
versus) or unused, and when player 1's is unplugged a pad held as player 2's stays player 2's: until it is
connected again, player 1 has no gamepad and Automatic shows the PSP's buttons. DS4Windows connects its new
virtual pad before it removes the old one, so a switch of its output does this.

Not tested: a real Xbox, Nintendo or Steam Deck controller, the Linux build, the European disc, a texture pack
that replaces one of the sheets, and the screens the route does not reach. Steam Input can present a controller
as an Xbox pad, so under Steam, Automatic may show Xbox's buttons for another pad; choosing the style by name
avoids it.

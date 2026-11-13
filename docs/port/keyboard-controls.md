# Keyboard and mouse: battle moves, the mouse wheel, key prompts

> 13 November 2026. PortableKit patch 0103 (`host/input/bindings.*`, `host/input/button_prompts.*`, the renderer's
> keyboard and wheel, the Controls page, settings), the port's `d012_profile.cpp`, `d012_button_prompts.cpp` and
> `d012_dialogs.cpp`. Windows, clang build, USA disc.

Players on the keyboard asked on Discord for one key per battle move that takes two buttons, the mouse wheel in the
menus, and prompts that show their keys. All three are in; clicking the game's menu items is not (see the end).

## One key for a two-button move

The Controls page's **Keyboard and mouse** section lists six more rows after the PSP's controls. Each key presses
both of the move's buttons at once:

| Row | Buttons | Key |
| --- | --- | --- |
| Dodge | R + × | C |
| Dash | R + △ | R |
| EX Mode | R + □ (EX Revenge when hit) | G |
| Summon | R + ○ (not checked: the test save has no summon equipped, and R + ○ was a Brave attack) | V |
| Brave assist | L + ○ | Z |
| HP assist | L + □ | X |

The game takes these moves only when both buttons go down within two frames of each other: with R down three or
more frames before × or △, Lightning blocks and the dodge or dash never comes, which is what made them hard to play
on two keys. A held L still makes the assists, and a held R still enters EX Mode.

They are rebound like any other row (`input.bind.dodge`, `input.bind.dash`, `input.bind.ex_mode`,
`input.bind.summon`, `input.bind.brave_assist`, `input.bind.hp_assist` in `settings.ini`), take a mouse button or the
wheel, and reach player 2 when player 2 plays on the keyboard. Holding R or L and pressing the face button still
works as before. **Use the classic keyboard layout** keeps them, apart from a key the classic layout takes.

The rows are the profile's (`GameProfile::combos`): PortableKit reads the key and adds the buttons to the pad in the
same read, so the game sees R and × go down together, as when both are pressed at once on a pad.

## The mouse wheel

**Wheel Up** and **Wheel Down** can be bound to any control. Each notch is one short press: the control is held for
two of the game's pad reads and let go for two, so a menu moves one row per notch however fast the wheel turns. At
most four notches wait; turning the other way drops them. The wheel counts while the window has the pointer (the
**Mouse** row), as the mouse's buttons do.

By default the wheel is D-pad up and down, which moves through the game's lists. A `settings.ini` written before this
version held every binding, so the wheel is added to D-pad up and down there once, where those rows have room and the
wheel is bound nowhere else; the file then says `input.bindings_version=2`, and the wheel is never added again.

In a fight the D-pad moves the camera (up raises it and draws back, down lowers it and comes in close, left and
right swing round; it stays where it is left), and the fighter does not move. So the wheel scrolls the menus and
tilts the battle camera, with the same two bindings.

## Key prompts

**Button prompts** has a sixth choice, **Keyboard**, and Automatic shows it whenever player 1 played last on the
keyboard or the mouse's buttons, or has no controller. Each of the PSP's buttons is drawn as the key or mouse button
bound to it first, on the game's own silver key (the one its L and R already have): `SPACE`, `E`, `Q`, `SHIFT`,
`ENTER`, `BKSP`, arrows, and a mouse with the bound button filled in. The game's hints change with them ("SHIFT + E :
Enter EX Mode", "Press SHIFT while holding Q to Change Paradigms!"), and so does the MANUAL badge. Touching the
controller switches back to its buttons; the textures are drawn again at each switch.
[button-prompts.md](button-prompts.md) has the details.

The port's own message, save and keyboard screens answer Enter and Esc on the keyboard, so in the keyboard style
their hints read "Enter: OK" and "Esc: Back".

## Clicking the game's menus

Not built. The game moves through most of its menus with one shared list cursor (set up by main `0x088D46E4`, moved
by `0x088D688C`, 126 call sites in the menu overlay), so the port could find out which list is live and walk its
cursor to a row with D-pad presses through the input record. But nothing tells where a row is on screen: every
screen has its own table of parts and attachment points. Each screen would need its own row positions, from about an
hour each for the main menu, Battle Mode and Options to days for Customize, and Player Select has a cursor of its own.
The pointer is also captured (hidden) while the game has input, and the left button is △ by default. The wheel
covers moving through lists; a click-to-select for a few screens remains possible if wanted.

## Switches

- `D012_WHEEL_HOLD=N`, `D012_WHEEL_GAP=N`: the game's pad reads a wheel notch is held and let go (2 each).
- `D012_BUTTON_PROMPTS=keyboard`: the keyboard style for this run.
- `D012_TRACE_PAD=1`: logs the bindings (the combos' too) and every change of the buttons the game reads.

## Tested

On Windows with the clang build and the USA disc, keyboard only (no controller connected), scripted:

- The combos, on the build before this change with a virtual pad pressing both buttons in one read and with R or L
  held first, in a Quick Battle forced into local versus so the opponent stood still (`D012_TRACE_VERSUS` for the
  fighter's state): R + × dodged (and only within two frames of R), R + △ dashed (a burst towards the opponent, not
  △'s own dash), R + □ entered EX Mode and, when hit, EX Revenge, L + ○ and L + □ called Vaan's Brave and HP
  attacks once his gauge allowed. L alone turns lock-on off and on; L + R is Lightning's Paradigm Shift.
- With this build, keys only: C, R, Z, X, G and V each sent their two buttons in one read (`D012_TRACE_PAD`:
  `0x4200`, `0x1200`, `0x2100`, `0x8100`, `0x8200`, `0x2200`); C dodged and G entered EX Mode
  (`D012_TEST_EX_FULL`).
- The wheel on the main menu: one notch down from Story Mode was Battle Mode, another Communications Mode, one up
  Battle Mode again, three down Collection. The earlier build moved one row for presses of one, two and three reads,
  on the main menu and in Battle Mode, unthrottled and at normal speed. In a fight, notches up and down raised and
  lowered the camera.
- `settings.ini` from before (every binding saved, no wheel): D-pad up and down read "Up / Wheel Up" and
  "Down / Wheel Down"; the Controls page listed the six rows with their keys.
- Prompts: the main menu's "Customization" and "Mognet" showed the mouse with its left button and `E`, the MANUAL
  badge `ENTER`, the battle hints `SHIFT + E` and `SHIFT` / `Q`, the autosave notice "Enter: OK". All nine sheets
  drawn outside the game with the default keys, arrows, the wheel, the keypad, F10, PGDN, and nothing bound.
- PortableKit's input, settings and profile tests and the port's tests pass.

Not tested: a summon (none equipped), other fighters than Lightning, player 2 on the keyboard, online and replays
(the combos are plain buttons in the input record, the wheel plain D-pad presses), a real mouse wheel and a
touchpad's fractional scrolling, the Linux build, the Steam Deck, the European disc, the screens the route does not
reach.

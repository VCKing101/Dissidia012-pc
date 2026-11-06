# Closing the game from the title screen

> 6 November 2026. `port/host/d012_title_quit.cpp`, PortableKit patch 0087. Windows, clang build, USA disc.

On a PSP, circle does nothing on the title screen's menu (New Game, Load Game, Extras). In the port it asks
**Close the game?**, with No selected. Yes closes the game as the menu's Quit does; No, or circle again, leaves the
title as it was. Only the title screen's menu asks: in the main menu, circle still brings up the game's own "Save
and return to the title screen?".

## How the title screen is told apart

The title screen is a step of the boot sequence in the menu overlay, after the autosave notice and around the Load
list. Its handler is `menu:0x089DF14C`. While its menu takes input, it asks main `0x08872088` for player 1's pad
object once a frame from `menu:0x089DF330` (and `0x088729A0` from three more places). Those calls stop when a row is
chosen and the title fades out, and the main menu asks for the pad from elsewhere (`menu:0x089E2B5C`,
`0x089E2BC4`, `0x089E4678`, `0x089E4720`). Found by logging every caller of the two pad functions from the overlay
on the title, the Load list and the main menu.

Nothing in the menu overlay can be overridden, so the port overrides main `0x08872088`. It returns what the
original returns and, only when the caller is `menu:0x089DF330`, checks player 1's circle button
(`first_pad_sample_now()`). A press since that call's last frame opens the question. A circle already held when the
title comes back (backing out of the Load list with it) does not count: the press must follow a frame of the title
less than 200 ms before. Nothing opens while a PortableKit screen is over the game (a dialog, the Load list, the
keyboard, the menu, a panel).

## The question

It is PortableKit's message dialog (`ui::open_game_message`), so it has the same controls as the game's own
questions (left and right, confirm, circle for No; Enter and Esc on the keyboard; the mouse), and the native
dialogs draw it in the game's look. The game reads a neutral pad while it is up. The dialog's sounds are queued
for the game's thread; the game polls none of its own dialogs meanwhile, so the override plays them
(`dialogs::play_queued_sounds`) from the title's pad call, which goes on once a frame behind the dialog. Yes calls
`ui::request_quit()` (patch 0087), the menu's Quit.

| Variable | Effect |
| --- | --- |
| `D012_NO_TITLE_QUIT=1` | Circle does nothing on the title screen, as on a PSP |

## Verified (6 November 2026, Windows, clang build, USA disc)

Through an input script, with window captures:

- Circle on the title: the question, drawn in the game's look, No selected. Cross on No: the title, its cursor where
  it was. Circle on the question: the same.
- Load Game, then circle out of the Load list: no question.
- Circle, left to Yes, cross: the game closed ("quit from the menu").
- Circle in the main menu: the game's own "Save and return to the title screen?", no question of the port's.

Not verified: the sounds by ear, a keyboard or mouse player, the update notice over the title (a button that
closes it can also ask), the European disc, Linux and the Steam Deck.

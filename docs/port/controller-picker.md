# Choosing each player's controller

> 10 November 2026. PortableKit patch 0101 (`host/input/players.*`, the renderer's pads, the Controls page), the
> port's `d012_versus.cpp` and `d012_camera.cpp`. Windows, clang build, USA disc.

The menu's Controls page has **Player 1** and **Player 2**, each **Automatic**, **Keyboard and mouse**, or a
controller by name. Before, player 1 always had the keyboard and the first controller connected, player 2 the next
one, and nothing could change that. With DS4Windows that meant the game could take the real controller as player 1
and DS4Windows' virtual one as player 2.

## What each choice does

- **Automatic** (the default): player 1 has the keyboard and mouse and the first controller free, player 2 the next.
  Each player keeps the controller it has while it stays connected. The row shows what Automatic picked:
  "Automatic: Xbox 360 Controller".
- **Keyboard and mouse**: that player plays on the keyboard and mouse alone. For player 2, player 1 then has no
  keyboard and plays on a controller, and the mouse turns player 2's camera. Local versus works with one controller
  and the keyboard.
- **A controller**: that player plays on it. A controller is named by its USB vendor and product ids, so it is the
  same over USB and Bluetooth; two alike are one choice, and each player who takes it gets one. While none is
  connected, that player is on Automatic and the row says so: "Xbox 360 Controller (not connected), using
  DualSense Wireless Controller". A player left with no controller shows "Automatic: none".

Player 2's choice counts only while the game wants a second player: on the start menu and in a versus battle. At
other times player 2's keyboard is player 1's again, and a controller chosen for player 2 is kept for player 2,
unread.

A controller held for player 2 now moves to player 1 when player 1's goes, as long as player 2 is not in a versus
battle. That is the swap DS4Windows makes: its new virtual pad connects before the old one goes, so for a moment it
was player 2's, and before, player 1 was left with none until it was connected again. In a versus battle player 2's
controller stays player 2's, and the battle pauses for player 1's as before.

Button prompts and the menu's own button names follow player 1's controller. With player 1 on the keyboard,
Automatic shows the PSP's buttons.

## Settings and switches

- `input.player1`, `input.player2` in `settings.ini`: `auto`, `keyboard`, or `pad:<vendor><product>:<name>`, for
  example `pad:054c0ce6:DualSense Wireless Controller`. `D012_PLAYER1` and `D012_PLAYER2` set them for one run.
- `D012_PLAYER2_PAD_STAYS=1`: player 2's controller never moves to player 1, as before.
- The log says each change: `[pad] player 2: the keyboard and mouse`, `[pad] player 1 has no gamepad`.

The input script's two virtual pads still take over from real ones on Automatic, so scripted runs go as before
with a controller plugged in, and they can be chosen by name like any other (the first is `00000000`, the second
`50540002`).

## Tested

On Windows with the clang build and the USA disc, scripted runs with the owner's DualSense plugged in beside the
script's virtual pads:

- Automatic on the start menu: player 1 on the script's pad, player 2 on the DualSense, then on the script's second
  pad once it connected. With player 1's pad unplugged, the DualSense (free) became player 1's.
- The Controls page cycling Player 1 through Keyboard and mouse, the DualSense and the script's pad, and Player 2 to
  Keyboard and mouse: each change took effect at once, the button prompts followed player 1's controller, and both
  rows were saved.
- A versus Quick Battle with player 2 on the keyboard (`D012_PLAYER2=keyboard`, `D012_LOCAL_VERSUS=1`): the
  keyboard picked and confirmed the opponent, held D and A moved player 2's fighter (raw pad 1's stick 255 and 0)
  while the script's pad played player 1, and the mouse turned player 2's camera and not player 1's.
- A Quick Battle without versus played as before.
- The owner, with a DualSense, cycled Player 1 between Keyboard and mouse and the DualSense: each change took the
  controller at once, the button prompts with it, and the last choice was saved.
- PortableKit's input tests: the choices read and written, and who gets which pad in each case (Automatic, chosen,
  not connected, two alike, the swap with player 2 playing or not, the keyboard for either player, the script's
  pads).

Not tested: the swap with real controllers (a free controller went to player 1 first in every scripted run, so the
move from player 2 is covered by the unit tests only), DS4Windows itself, two identical controllers, Bluetooth,
Linux, the Steam Deck, online play with player 2 on the keyboard.

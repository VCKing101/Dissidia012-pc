# The menu

Esc on the keyboard, or L3+R3 on a gamepad, opens the menu over the game. Since PortableKit patch 0124 it is two
screens instead of one panel, drawn for a large display rather than enlarged from a small one. `D012_CLASSIC_MENU=1`
brings back the panel of before, with the new colours and faces.

## Why it changed

The panel sized everything from its text, and its text was 3.4 % of the window's height: 37 px on a 1080-line
screen, 72 px at 2160. A larger screen showed the same eight rows, only larger. The new menu's text is about 21 px
from a Steam Deck's 800 lines to 1080 lines, then grows with the screen at about 2 % of its height (27 px at 1440, 40
px at 2160); a small window gets 16 px or more. A phone keeps the old size, which fingers need.
`D012_MENU_TEXT_SIZE=24` sets it by hand. The other screens (the setup, the game's dialogs, the keyboard opened by
the game) keep their own size.

## The pause screen

What the menu opens on. The game stays in view on the right; the left darkens under:

- the title, from the profile (`Dissidia 012`, with `Duodecim Final Fantasy` under it), and Paused or Running;
- **Resume**, **Settings** (the Video page), **Controls** and **Cheats** (with how many are on), each opening the
  settings screen at its page;
- **Quick settings**: Resolution and Frame rate, changed in place with left and right;
- **Quit game**, which asks first, as before;
- the focused item's description, the button hints, and in the bottom right corner Online, Disc and Build, with
  the profile's credit under them (`GameProfile::pause_credit`): a PC recompilation by VCKing101, not affiliated
  with Square Enix, and the game's copyright line as its title screen gives it.

Back (or Esc) resumes the game. Start resumes it from either screen.

## The settings screen

The six pages behind tabs, as before (L1/R1 or Q/W): Video, Audio, Controls, Network, Cheats, System. The rows are
on the left half; Back returns to the pause screen. On the right half, a side panel shows the focused row's name,
what it does, and for some settings a preview of what it does:

| Setting | The preview | From |
|---|---|---|
| Stick dead zone | The left stick's range, the dead zone, where the stick is now with a short trail, and the value the game reads (0 to 255 on each axis) | the pad, live |
| Full diagonals | A full push the way the stick points, with the setting off and on, and what the game reads for each | the pad, live |
| Trigger point | Each trigger's travel, the point where it presses, and what it presses (L2 and R2 in this game; L and R with Swap shoulders and triggers) | the pad, live |
| Right stick | Camera: how far the stick is pushed and which way. D-pad: which directions it presses | the pad, live |
| Right stick D-pad point | The line on each axis past which the stick presses that direction | the pad, live |
| Mouse sensitivity | How far 100 counts of mouse motion turn the camera, and the counts for a full turn | the setting |
| Resolution | Close-ups of one place on the screen at ×1, ×2, the step below the paused one, and as paused (see below) | the paused frame |
| Scaling filter | Close-ups of the screen with Sharp and with Smooth, at the size the game is drawn and shown now; when the two are the same (Auto draws at about the screen's size), also at Resolution ×2 | the paused frame |
| Aspect ratio | The paused frame in the window's shape with Original and Stretch, and the size each shows the picture at | the paused frame |
| Font, Weight | The profile's sample line drawn as the game draws its text, and which of the game's text the font changes | the font |
| Sharp text | A block of the game's own text, the last it drew: its PSP-sized letters enlarged by the Texture filter (Off), and the same glyphs drawn again from the font at the resolution's scale (On) | the glyphs Sharp text keeps (`fonts::text_sample`) |
| Bloom | The world map at ×4, the same moment with Bloom off and on (`port/menu/bloom-*.png`) | pictures the profile gives (`GameProfile::setting_pictures`) |
| Button prompts | Confirm, back and the top button in each style, the one in use marked | the setting |
| Confirm button | Which face button confirms and which goes back, on the pad in use | the setting |

"The pad" is the controller in the player's hands: the one last pressed or pushed, whichever player has it, since
the menu answers every connected controller (`Layer::menu_gamepad()`; the button hints follow it too). When the game
does not read that controller (one chosen for player 2 outside a mode for two), the preview says so and names the row
that chooses player 1's.

The paused frame is read back once, the first time a preview needs it (`VulkanRenderer::read_frame`).

Resolution's lower steps are worked out from the paused frame, each pixel a tent-weighted average of the pixels it
covers. Checked against the game drawn at ×1 and ×4 on the same screen, that came within about 1 % of the real ×1, on
its text (which a plain average matches better) and its pictures (which the centre pixel alone matches better). The
close-ups go where the lower resolutions change the picture most, nearer the middle counting more (where the camera
keeps the player's character), and leave out the game's 2D interface: the renderer notes where the last whole frame
drew it (`VulkanRenderer::interface_rects()`), since its pictures do not change with the resolution and its text
changes with Sharp text instead. A higher step than the paused one cannot be worked out, and over a menu's pictures
the steps look alike, as they do in the game.

Texture filter and Texture pack have no preview: the paused frame shows only the setting in use, and showing the
other would need the game to draw a frame again. Bloom has the same problem, and the profile's two pictures instead.

The pages' cards stay at the panel's bottom while the focus moves from row to row, and move down only below a
preview that reaches them.

The pages' long lists of information move to the side panel as cards, so they stay in view: the Network page's
Status, the System page's About, and on the Audio and Cheats pages the volume and the cheats that are on. A window
less than 50 text heights wide has no side panel; the description is then under the rows, as before.

## The look

PortableKit's interface takes its colours, faces and title from `GameProfile::theme` (`InterfaceTheme`,
`host/profile.hpp`); a game without one keeps PortableKit's brown and gold. Dissidia's (`d012_profile.cpp`) is the
night blue and silver of the game's menus, a crystal-blue bar on the focused row with a diamond on its left edge, and
the title's "012" and the open tab's line in crimson. The theme applies to every screen of the port's own (the setup,
the file browser, the save screens), not only the menu.

The faces are compiled into the program (`port/CMakeLists.txt`, from `port/fonts/`), under the SIL Open Font
License 1.1, whose texts the setups, the AppImage and the app ship with their licences:

- Cinzel SemiBold, by Natanael Gama: titles and section names (`Cinzel-OFL.txt`).
- Barlow Medium and Barlow Semi Condensed SemiBold, by Jeremy Tribby: the text and the small capitals
  (`Barlow-OFL.txt`).

Characters they lack (Cyrillic, Japanese, some symbols) come from the system's faces, as before.

## What the menu says about this game

PortableKit came from the Monster Hunter port, and some of its menu text was Monster Hunter's. The profile now
supplies what is the game's own (`GameProfile::right_stick_camera`, `mouse_camera`, `extra_triggers`,
`font_preview_text`, `font_note`), and the rest says nothing about a particular game: Reconnect now and Pause during
multiplayer speak of a battle, Export save of saves.

## Bloom's pictures

`port/menu/bloom-off.png` and `bloom-on.png` are the port's own captures of the game: the world map at ×4, 290 by 340
pixels around the moogle, from the issue 13 triage (`build/port/gfx-triage/fix13_worldmap`, frame 4340, Bloom off
and Bloom on with smooth sampling). They are Square Enix's game shown, like the pictures in `docs/images`, and are
compiled into the program by `port/CMakeLists.txt`.

## Verified

26 November 2026, Windows 11, the clang (llvm-mingw) build, USA disc, test data (`build/port/data-escmenu`: its
own settings and the port's own New Game save, none of a player's), driven by `D012_INPUT_SCRIPT` with its virtual
gamepad, windows of 1920×1088 (×4) and 1440×816 (×3, about a Steam Deck's 800 lines), each capture looked at:

- The pause screen: title, items, quick settings, description, hints, the corner's lines; on 816 lines the items
  close up so Quit game stays in view.
- Every page of the settings screen, L1/R1 between them, Back to the pause screen, Start and Back to the game.
- The previews: Stick dead zone (the readout matches `input::stick::to_nub`: 0.41, −0.22 at 15 % reads 178, 101),
  Full diagonals, Trigger point, Right stick (Camera), Mouse sensitivity, Scaling filter (the same-size case at
  1920×1088 with ×2 below it, and the shrunk case at 1440×816), Aspect ratio, Font, Button prompts, Confirm button.
- The left stick held on the Stick dead zone row moves the preview and neither the focus nor the slider.
- A controller that is not player 1's (the DualSense chosen for Player 2, Player 1 on Automatic with no other pad):
  L3+R3 on a second virtual pad opens the menu, and the Stick dead zone and Right stick previews follow that pad and
  say the game does not read it.
- The cards: Network status, Audio, Cheats on now, About.
- The Quit confirmation, and Cancel.
- `D012_CLASSIC_MENU=1`: the panel of before, in the new colours.
- `d012_tests` passes (`tools/port/pk_ctest.cmd`).
- Later the same day: the controller in hand drives the previews (the DualSense chosen for Player 2, a second
  virtual pad); the Network page's Status card stays at the bottom from row to row, and the Address row names
  itself in the panel; the Resolution close-ups on the story menu (text left out) and on the world map (Lightning);
  the Sharp text preview with Sharp text on; the ×1 estimate measured against real ×1 and ×2 captures.

## Not verified

- A real gamepad, the mouse and the keyboard (only the script's virtual pad was used); touch.
- Linux, the Steam Deck itself, Android; the setups, the AppImage and the app with the new licence files.
- Changing Resolution or Frame rate from the pause screen (shown, not changed); the font list, the texture pack and
  save screens and the on-screen keyboard opened from the new screens; the menu over the running game (Pause the game
  when the menu opens off), and during online play.
- Windows wider than 16:9, 4K, and a window narrow enough to drop the side panel.
- PortableKit's framework build (`portablekit_host_check`) was not built.

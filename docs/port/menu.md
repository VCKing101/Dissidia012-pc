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
- the focused item's description, the button hints, and in the bottom right corner Online, Disc and Build.

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
| Scaling filter | Close-ups of the screen with Sharp and with Smooth, at the size the game is drawn and shown now; when the two are the same (Auto draws at about the screen's size), also at Resolution ×2 | the paused frame |
| Aspect ratio | The paused frame in the window's shape with Original and Stretch, and the size each shows the picture at | the paused frame |
| Font, Weight | The profile's sample line drawn as the game draws its text, and which of the game's text the font changes | the font |
| Button prompts | Confirm, back and the top button in each style, the one in use marked | the setting |
| Confirm button | Which face button confirms and which goes back, on the pad in use | the setting |

The paused frame is read back once, the first time a preview needs it (`VulkanRenderer::read_frame`). The settings
that change how the game draws itself (Texture filter, Resolution, Sharp text, Texture pack) have no preview: the
paused frame shows only the setting in use, and showing another would need the game to draw a frame again.

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
- The cards: Network status, Audio, Cheats on now, About.
- The Quit confirmation, and Cancel.
- `D012_CLASSIC_MENU=1`: the panel of before, in the new colours.
- `d012_tests` passes (`tools/port/pk_ctest.cmd`).

## Not verified

- A real gamepad, the mouse and the keyboard (only the script's virtual pad was used); touch.
- Linux, the Steam Deck itself, Android; the setups, the AppImage and the app with the new licence files.
- Changing Resolution or Frame rate from the pause screen (shown, not changed); the font list, the texture pack and
  save screens and the on-screen keyboard opened from the new screens; the menu over the running game (Pause the game
  when the menu opens off), and during online play.
- Windows wider than 16:9, 4K, and a window narrow enough to drop the side panel.
- PortableKit's framework build (`portablekit_host_check`) was not built.

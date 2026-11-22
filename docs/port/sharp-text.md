# Sharp text

22 November 2026. At a higher internal resolution the game's sentence text was enlarged from the PSP's small
bitmaps and looked soft. The port now draws that text again at the internal resolution (PortableKit patch 0120).

## The game's two kinds of text

| Text | Where it comes from | At 4x |
| --- | --- | --- |
| Sentences and labels: descriptions, dialogs ("Play as this character?"), menu rows ("Create New Data", "Normal", "Set A"), the help line's labels | The PSP font library (`sceLibFont`). The disc has no system font, so the port draws each letter from a TrueType font on the computer (MS Gothic by default, or the one chosen under Video, Text, Font) | Sharp since patch 0120 |
| Titles and names in the game's serif style ("Player Select", character names, "LOOK", "SET", "Lv", "YES"/"NO"), and the battle tips ("Press RB while holding LB…") | Pictures in the game's files (GIM textures; names from `menu/JP/common/chara_name/p_<id>_name.gim`; the tips from an image font) | As before: only a texture pack can redraw them |

Traced with `D012_TRACE_FONT=1`: the game hands the font library one buffer per block of text (4 bits per pixel,
sized to the text: 96x26, 448x104, 320x130 and so on, 26 rows per line) and the position of each letter in 1/64
pixels, and the port draws each letter into it at about 20 pixels high (main `0x0887F294`, calls at `0x0888161C` and
`0x08881838`). The game then draws the whole buffer as a texture at about 0.6 of its size (main `0x0887BC2C`), which
on a PSP makes the text look clean. At 4x the same 20-pixel, 16-shade letters are enlarged about 2.4 times, which
is why they looked soft. Changing the font changed their shape, not their sharpness.

## How it works

PortableKit `host/fonts/sharp_text.hpp`, nothing in the profile:

1. Each letter the port draws for the game is kept against its buffer: character, position, clip rectangle. So is a
   copy of the buffer as the letter left it. A buffer the game cleared starts a new text; one it wrote itself is
   marked and never drawn sharp.
2. When the renderer loads a 4- or 8-bit texture lying in such a buffer, and the buffer holds exactly what the
   letters left, the letters are drawn again as many times larger as the internal resolution (4x: 4 times), at the
   same positions, and coloured through the game's own palette. Outside the buffer the texture is the game's. The
   larger picture is drawn with the same texture coordinates, as a texture pack's image is.
3. Anything else (the font changed since, the buffer was changed after its letters) draws the game's texture as
   before.

The game's memory is only read, so the game runs exactly as without it: online, rollback and cross-play see no
difference.

## Settings and switches

| | |
| --- | --- |
| Video, Text, **Sharp text** (`text.sharp` in settings.ini) | On by default. Off draws the game's text as before |
| `D012_NO_SHARP_TEXT=1` | Off whatever the setting says, for a comparison |
| `D012_SHARP_TEXT_MEMORY=<MB>` | GPU memory sharp text may use, 256 MB by default. Past it, sharp text not drawn in the last two frames goes first; when that is not enough, new text is drawn from the game's texture |
| `D012_TRACE_SHARP_TEXT=1` | Each texture looked up, and why one is not drawn sharp |

## Cost

- A sharp text texture is the game's size times the scale squared: a 512x64 block is 2 MB at 4x.
- Battle at the Old Chaos Shrine at 4x (`gfx_tour.py`'s route, Windows): 60 fps, render 2.4 to 2.8 ms a frame with
  sharp text and without; 334 text textures drawn sharp on the way, none fell back.
- With a 16 MB budget the menus route drew 44 textures sharp and the rest as the game's, and ran at its usual
  speed (before the two-frame rule it redrew the same text every frame).

## Verified

Windows, the clang build, USA disc, test data only (a fresh folder, and the graphics tour's copy of the
development save): the first-run Data Config screen, the menus to Quick Battle Player Select with "Play as this
character?", the Battle Setup page and a Quick Battle at the Old Chaos Shrine, at 4x, with sharp text and
`D012_NO_SHARP_TEXT=1`; at 2x; with Arial chosen as the font; with `text.sharp=0`; with a 16 MB budget.
`d012_tests` pass.

Not verified: the Sharp text row in the menu (switching while playing), Story Mode text that appears letter by
letter, Japanese text, the European disc, Linux, the Steam Deck, Android, a texture pack together with it.

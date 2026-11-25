# Sharp text

22 November 2026. At a higher internal resolution the game's sentence text was enlarged from the PSP's small
bitmaps and looked soft. The port now draws that text again at the internal resolution (PortableKit patch 0120).

26 November 2026: the text is now drawn straight from the font, glyph by glyph, in place of the game's text
quads (**native text**, PortableKit patch 0123, below). Patch 0120 made a large texture for every new text, and a
list scrolled by holding a direction makes 20 to 40 new texts at once, which stalled the frame on each step.

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

## Native text (patch 0123)

The game's text system is mapped in `docs/decomp/text-system.md` on the decomp branch
(`claude/game-decompile-speedup-bbc65d`): a text object per string, made by main `0x0887D868`, its buffer drawn
by `0x0887BC2C`. What matters here:

- The game draws a text as one quad per *run* (a line, or a part of a line in one colour or size): a
  `TRIANGLE_FAN` of four through-mode vertices sampling the run's part of the buffer as a CLUT4 texture whose
  palette is white with an alpha ramp (`0x089D68D0`), modulated by the vertex colour (doubled), alpha test "greater
  than 0". A shadow is a second text sharing the buffer, drawn first, one pixel down and right, in a dark colour.
- Every step of a held list in the shop or the accessory list makes the stat comparison panel (18 numbers and 6
  arrows) and the item's detail panel (about 5 lines) again: about 24 new buffers (the map's section 5).

PortableKit `host/gpu/vulkan_renderer.cpp`, `Impl::native_text`, nothing in the profile:

1. A quad of a glyph buffer (Sharp text's registry: the letters the port drew into it, unchanged since) is taken
   when it is upright with its texels upright too, its colours and depths equal, the texture modulated by the
   vertex colour with that alpha test, and the palette one colour with an alpha ramp. Anything else is drawn as
   before (from Sharp text's picture).
2. Each letter of the buffer that lies in the quad's texels is placed through the quad's mapping (buffer
   position to screen pixel), drawn from the font at that size (to 1/32 of a pixel per texel, and to half a pixel
   across) into a glyph atlas the first time, and drawn as a quad of its own, cut at the run's edges, with the
   quad's colour and depth, in the quad's place in the frame: the frame's blend, alpha test, depth test and depth
   writes, scissor and draw order are the game's.
3. A quad that can draw nothing is skipped: alpha 0 (a faded panel's text, about 50 a frame on the shop screen),
   wholly outside the scissor (the 14 texts the shop keeps above the screen), or of no area.
4. The atlas has two pages, 2048 pixels square up to 4x and 4096 above. New letters go into the current page; when
   it is full, the other page, once no frame has drawn from it for six frames (frames in flight and the two that
   frame interpolation draws again), is cleared and becomes the current one. Until then new text is drawn from
   Sharp text's pictures.

The inline icons and button pictures of a text are the game's own draws and stay as they were. The battle banners
and tips use another font (a picture in the game's files) and stay as they were.

What it changes on screen: the letters are the font's own coverage (256 shades, where Sharp text's pictures kept
the game's 16), so their edges are a little smoother; where two letters' pictures overlap, their edges blend twice.

## Settings and switches

| | |
| --- | --- |
| Video, Text, **Sharp text** (`text.sharp` in settings.ini) | On by default. Off draws the game's text as before |
| `D012_NO_SHARP_TEXT=1` | Off whatever the setting says, for a comparison |
| `D012_NO_NATIVE_TEXT=1` | Every text drawn from Sharp text's pictures, as patch 0120 did |
| `D012_TRACE_NATIVE_TEXT=1` | Every 300 frames: the quads drawn as letters, the letters, those added to the atlas, the quads skipped, those drawn from pictures and why, and the atlas page's fill |
| `D012_SHARP_TEXT_MEMORY=<MB>` | GPU memory sharp text may use, 256 MB by default. Past it, sharp text not drawn in the last two frames goes first; when that is not enough, new text is drawn from the game's texture |
| `D012_TRACE_SHARP_TEXT=1` | Each texture looked up, and why one is not drawn sharp |

## Cost

Native text (patch 0123), Windows, RX 9070 XT, test data, the shop route (Customization, Lightning, Shop, Buy;
the d-pad held down for 7 seconds, up for 5, down again in the next category), frames of 60 fps:

| | Frames over 25 ms (hold down / up / down) | Longest frame | Render time, most | Textures made |
| --- | --- | --- | --- | --- |
| 4x, patch 0120 | 8 / 7 / 3 | 67.5 ms | 69.9 ms | 3304 |
| 4x, Sharp text off | 0 / 0 / 0 | 20.4 ms | 18.5 ms | 3278 |
| 4x, native text | 0 / 0 / 0 | 17.6 ms | 0.8 ms | 0 |
| 8x, patch 0120 | 20 / 23 / 10 | 72.3 ms | 129.0 ms | 3297 |
| 8x, native text | 0 / 0 / 0 | 17.3 ms | 0.9 ms | 0 |

The whole route added 650 letters to the atlas at 4x (a fifth of a 2048 page) and 819 at 8x (a fifth of a 4096
page). On the graphics tour's menus, Story Mode, Old Chaos Shrine and results routes every text quad was drawn
as letters (35,678 to 110,353 quads a route, none drawn from a texture, no page switch, at most 338 letters in the
atlas), and the quads that draw nothing (about 55,000 to 109,000 a route) were skipped.

Patch 0120's pictures, which native text keeps for any other draw of a glyph buffer:

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

Native text (26 November 2026), Windows, the clang build, USA disc, test data only: the shop route above at 4x
and 8x; the accessory list (Customization, Accessories, Equip, a slot, the list held down and up: frames over
25 ms 9 before, 0 after); the graphics tour's menus, Story Mode gateway, Old Chaos Shrine battle and battle
results routes at 4x with frame interpolation and its replay check (0.02% of pixels, as before), whose captures
differ from patch 0120's at the letters' edges only; the shop under Fill in a 2384x781 window, the text where
patch 0120 puts it; `D012_NO_NATIVE_TEXT=1` and `text.sharp=0`, each drawing as before. `d012_tests` pass.

Not verified for native text: Japanese text, text whose size animates (each new size adds letters to the atlas),
changing the font while playing, a quad of a glyph buffer drawn some other way (none seen: every one on the
routes above was drawn as letters), Linux, the Steam Deck, Android, the European disc.

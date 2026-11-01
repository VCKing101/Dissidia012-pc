# How the game loads textures

Research for prefetching texture pack images (1 November 2026). PortableKit decodes a pack's PNG on a loader thread
the first time a texture is drawn and draws the game's own texture until the image is on the GPU, so a replaced
texture can appear a few frames late (the same pop-in PPSSPP has). The port can do better than an emulator: if it
knows a texture's key when the game *loads* it, the PNG can be decoded during the loading screen. This page maps
where textures come from, so we know where such a hook would go.

**Status: parked (1 November 2026).** The load-time prefetch is the chosen fix for the pop-in, but it is not being
built yet. To pick it up: start with the hook on the return of `FUN_088DE0E8` for title and menu textures and a
`data_hash` index in PortableKit's `TexturePack` (section 7); model textures need `FUN_089BCC84`'s texture entry
mapped first. The player-facing guide is [texture-packs.md](texture-packs.md).

Marks: **V** verified here (traced in the recompiled build or read in the disassembly), **D** from dissidia-fan's
repository ([dissidia-fan/dissidia](https://github.com/dissidia-fan/dissidia) at `619189a`, facts only, not
re-verified), **I** inferred. EU and USA code are identical, so their addresses are ours.

## Summary

- **Four sources of textures**, each needing its own hook (sections 2 to 5):
  1. GIM files in `PACKAGE.BIN` entries, used where they were loaded: the title screen, menus, portraits, the HUD.
  2. The same image library loaded directly by game code (GIM, TIM2, BMP).
  3. Textures of models (GMO): stages and, by the look of the VRAM textures in a battle, fighters. The model
     library copies them into VRAM while it builds the model.
  4. Textures the game makes while it runs: lines of text (512x32 4-bit, two thirds of all distinct textures in a
     battle), render targets, the screen copies. These cannot be prefetched, and packs rarely replace them.
- **Textures are loaded long before they are drawn.** The title screen's 512x512 atlas is written during the boot
  loading, before the logos, and first drawn at the title menu; a Quick Battle's stage texture is copied into VRAM
  during the loading screen; menu textures are drawn up to hundreds of I/O operations after they load.
  There is time to decode a PNG in all the cases traced.
- **Pixels never change after loading** for the textures traced (title atlas, two menu textures, a stage texture):
  each was written once, and only overwritten when the memory was reused for something else.
- **The key can be predicted only in part at load time** (section 7). Its pixel half (`data_hash`) needs only the
  pixels' address, format, size and row length, all in the loaded file. Its other half mixes in the CLUT format word
  and how much palette the CLUT load reads, which only the draw commands say. A prefetch that looks images up by the
  pixel half alone avoids predicting the palette.

## 1. Reading files

- **V** `PACKAGE_INFO.BIN` (132,532 bytes) is read whole at boot into `0x09B26BC0`. **D** It is a sorted index of
  11,043 entries `{crc, sector, flags<<28 | size}`, the CRC being BZip2's CRC32 of the lower-case path; the index
  functions are `FUN_08871938` (load), `FUN_08871674` (count), `FUN_08871684` (entry by index) and `FUN_088713E4`
  (find by name). Files in `PACKAGE.BIN` are stored raw: no compression, no encryption.
- **V** At boot and in menus `PACKAGE.BIN` entries are read with `sceIoRead` into a staging buffer (`0x0973D880`
  during boot, `0x092B4F80` in the menus); in battle loading they are read with `sceIoReadAsync`
  (staging at `0x095DE880`, `0x097BDA00` and others).
- **V** `func_08862854(entry, destination, size, flag)` is the general "load this package entry into this buffer"
  call: 56 callers in all four modules. It resolves the entry (`FUN_08867984`), then either loads it at once
  (`FUN_0885CBD8`, when the byte at `*0x089D5EA0` is 1) or queues a request (`FUN_088714F4`) and polls
  (`FUN_08863148`). The bytes reach their destination in byte and word stores on the loader thread `ffst_thread`
  (uid 258), copied from the staging buffer.
- **D** `FUN_088D0810` is a generic GIM/file load request (the path in `a3`, a slot code in `a2`); models are staged
  by `FUN_08843014` in a FILE heap from `FUN_0888204C(0x275800)` and built into a pool from
  `FUN_0888204C(0x780000)`.

## 2. GIMs in package entries (menus, title, HUD)

Traced with the title screen's atlas, a 512x512 8-bit (CLUT8) texture at `0x08C63E30`, and two menu
textures from the Quick Battle route (`0x08E42030` 256x256 and `0x08FA25B0` 128x128, both CLUT8).

| Step | Function | What it does (V unless marked) |
| --- | --- | --- |
| Entry point | `FUN_088D00FC` (200 B), `FUN_088D1028` (84 B) | Callers of the package loader: game code (`0x0888C4C8`, `0x0891E34C`...) and about 20 menu-overlay screens |
| Package loader | `FUN_088D2980` (1,264 B) | Loads a package entry with `func_08862854`, then walks its table of sub-files (count at `+0x0A`, 8-byte entries `{offset, size}` from `+0x10`) |
| Language | `FUN_08870B10` (196 B) | If a sub-file starts with `mlng`, returns the part for the current language (`FUN_08867888` gives its index); no copy |
| By magic | in `FUN_088D2980` | First byte `M` (`MIG.`, a GIM): takes a texture object from a free list at `*(0x089D7830) + 0xEBD4 + 0x1D88` and calls `FUN_088DE0E8`; first byte `F`: `FUN_0887D5C8` (calls `FUN_08935524`) |
| GIM | `FUN_088DE0E8(object, gim, size, ...)` (176 B) | `object[0] = 0x0899B9F0(0)` (create an image), `0x0899BB68(object[0], gim, size, 0)` (load it), `0x0899BB1C` (destroy) if that fails; stores three parameters at `+6`, `+8`, `+0x0A` and 1 at `+4`. Also called by `FUN_088D01E4` and `FUN_088DAE0C` |

- **V** The GIM is used where it was loaded: its pixels are not copied anywhere. The image library loads it in place:
  it overwrites the image block's pixel offset (`0x40`, at `0x08C63E20`) with the pixels' address (`0x08C63E30`)
  and sets a word in the block's header (`0x08C63DF2`). The image header before the pixels reads format 5 (8-bit
  indexed), order 0 (not swizzled), 512x512, which is what the GE draws.
- **V** The title atlas was written once, during the boot loading, and never again; it was first drawn at the title
  menu. The two menu textures were written when a menu screen loaded, on the way to the battle, and drawn later
  (one only at the battle's loading screen); in battle their memory was reused for model data (`0x089B5910`,
  `0x089B7BD4`, `0x0886E108`, `0x089AEE10`).
- **V** The same texture loads at different addresses in different screens (one 256x256 menu texture at
  `0x08CFA230` and at `0x08C332F0`, same key): the reason packs ignore the address.
- **D** GIM layout: a 16-byte header `MIG.00.1PSP`, then blocks `{u16 type, u16 0, u32 size, u32 next, u32
  data offset}`: 0x02 root, 0x03 picture, 0x04 image, 0x05 palette, 0xFF file info. The image and palette headers
  hold format, order, width, height, bits per pixel, pitch alignment and the pixel offset (`+0x1C`, 0x40 in
  practice). Menu GIMs are 8-bit indexed with a 256-entry RGBA8888 palette, not swizzled. Menu GIM packs
  (`menu pak`, `*_gim_N.bin`) hold many GIMs, one entity each.

## 3. The image library

- **V** `0x0899B9F0` (create), `0x0899BB68` (load) and `0x0899BB1C` (destroy) are one image library, just below
  libGu in main, called from `FUN_088DE0E8` and directly from dozens of functions in game code (`0x0884C4DC`,
  `0x08851C78`, `0x08852378`...).
- **V** `0x0899BB68(image, data, size, 0)` checks the data's magic: `MIG.` (GIM) goes to `FUN_0899D348`, `TIM2` to
  `FUN_0899E4B0`, `BM` (BMP) to `FUN_0899E998` and `FUN_0899E9EC`, anything else to `FUN_0899DEF4`. The three
  parsers share helpers (`0x0899BEE0`, `0x0899C1D4`, `0x0899C24C`, `0x0899EAEC`, `0x0899EE30`, `0x089C6040`).
- **I** Hooking the return of `0x0899BB68` covers every image the library loads, whatever called it. The image
  object's layout (where it keeps the pixels' address, format, size and palette) is not mapped yet; reading the
  loaded GIM's blocks instead works for GIMs.

## 4. Model textures (stages, fighters)

Traced with a Chaos Shrine stage texture, 128x128 4-bit indexed at `0x04150780` in VRAM.

| Step | Function | What it does (V unless marked) |
| --- | --- | --- |
| Game code | `FUN_0886A6A4` (528 B) | Calls the model library to build a model |
| Build | `FUN_089B0720` (148 B) | Calls `0x089AB7A0`, `0x089ABCA0`, `0x089ABD28` (D: the model pool's build), `0x089B02F0` and `FUN_089B04E0` |
| Texture setup | `FUN_089B04E0(model, flags, ...)` (576 B) | Walks the model's texture table (count at `model+0x26`, 16-byte entries from `model+0x18`) and calls `FUN_089BCC84(entry, flags)` for each entry whose `+4` is set |
| Per texture | `FUN_089BCC84` (148 B) | Puts one texture in place: the stage texture's texels were written into VRAM here, in word stores, on `ffst_thread` |

- **V** The stage texture was written into VRAM right after an async `PACKAGE.BIN` read during the battle's loading,
  and first drawn when the battle started. The stage's textures sit one after another from `0x04110080`.
- **D** The game has a VRAM heap at `0x04110000`. A GMO holds its textures as GIMs (texture block `0x000A` with a
  file name `0x8012` and the GIM in `0x8013`); materials point at them. Fighter textures are 8-bit indexed with a
  256-entry RGBA8888 palette, not swizzled, 128x128 to 256x256 in retail (512x512 at most).
- **I** The VRAM textures seen in a battle (128x128 4-bit for the stage, 128x64 and 128x128 8-bit) are the models'.
  Which fighter textures go to VRAM and which stay in RAM, and the layout of a texture entry (where the source GIM,
  the destination and the format are), are not mapped yet. A hook on the return of `FUN_089BCC84` would see each
  model texture once it is in place.
- **D** GMO texture animation moves UVs only (`FUN_0886E5D0`); texels stay as loaded.

## 5. Textures made at run time

- **V/D** Lines of text are drawn by the game into 512x32 4-bit textures in RAM (331 distinct ones in one Quick
  Battle, out of 484 lookups) and change with the text. Their keys change too, so a pack cannot usefully list them,
  and nothing can be prefetched.
- **V** Render targets (`0x04000000`, `0x04044000`, `0x040CC000`...) and the screen copies are drawn, not loaded
  (see [graphics-census.md](graphics-census.md)). PortableKit already keeps them out of packs.
- **D** Effects (FEP) carry their own `TEX` blocks (8-bit indexed, swizzled, palettes tinted at run time) in effect
  memory (`EFFECT_ALLOCATOR`, `0x0A010000`-`0x0A3ED000`); the effect code binds them through `FUN_08972C14`. Not
  traced here.

## 6. Where textures live

| Where | What | Source |
| --- | --- | --- |
| Main RAM heap, `0x08C0xxxx`-`0x090xxxxx` | Package GIMs used in place (title, menus, HUD), reused between screens | V |
| VRAM from `0x04110000` | Model textures copied in by the model library | V (traced), D (the heap) |
| Model pool (`FUN_0888204C(0x780000)`, heap object `0x08C15F80`) | Built fighter data | D |
| `0x0A010000`-`0x0A3ED000` | Effects and their `TEX` blocks | D |
| RAM, rewritten | Text lines, screen copies | V |

## 7. What a prefetch needs

PortableKit's key for a texture (`compute_texture_pack_key`, PPSSPP's format) has two halves:

| Part | Inputs | Known at load time? |
| --- | --- | --- |
| `data_hash` (last 8 digits) | The pixels' address, format, width, height and row length; for a 512-tall texture drawn in through mode, only the rows drawn so far | Yes, from the loaded GIM or the model texture, except a 512-tall texture's first key (its height is the first draw's lowest row, at least 272) |
| `cache_key` (first 16 digits) | Width and height as powers of two; for indexed formats, the palette bytes the last CLUT load read, the CLUT format word (format, shift, mask, start) | Size yes; the palette half depends on the draw commands |

So the prefetch can work without predicting the palette:

1. **Index the pack by `data_hash`** as well as by full key (a pack has few images per pixel hash).
2. **At load time**, a hook computes the `data_hash` of each texture just loaded, the same way the renderer will, and
   asks the pack to decode every image with that hash. It does not upload anything; the renderer still matches by
   full key at the first draw, and finds the image already decoded.
3. **512-tall textures**: hash the full height (the key they get once drawn further down); their first key, over
   fewer rows, usually names the same image in a pack.

Hooks, in order of value:

| Hook | Covers | Gives | Status |
| --- | --- | --- | --- |
| Return of `FUN_088DE0E8` (or of `0x0899BB68`) | Menu, title, HUD GIMs; with `0x0899BB68`, every image the library loads | The loaded GIM: pixels, format, size, pitch | Ready to try: the GIM layout is known |
| Return of `FUN_089BCC84` | Model textures (stages, fighters) | The texture in its final place | Needs the texture entry's layout |
| None | Text lines, render targets | | Not prefetchable |

What PortableKit would need: a call such as `TexturePack::prefetch(memory, address, format, width, height,
buffer_width)` that hashes the pixels and queues the matching images, safe to call from the game's loader thread
while the renderer draws.

## 8. How long the pop-in is

Measured on 1 November 2026 at normal speed on a Windows PC (i7-12700K, two pack loader threads), with timing
added to the pack trace for the measurement only: from the first draw's request, through the PNG decode, to the
upload that the next draw uses.

| Case | Images | Decode | First draw to replaced draw |
| --- | --- | --- | --- |
| Title screen | 256x64, 256x128 (4x) | 0.3 ms | 35-38 ms |
| Title screen | 2048x2048 (4x of the 512x512 atlas) | 45-49 ms | 68-81 ms |
| Quick Battle, Chaos Shrine (46 images first drawn at normal speed) | 4x of everything but text lines: 64x64 to 2048x1024 | median 2.0 ms, at most 23 ms (1024x1024) | median 35 ms; 7 at 184-194 ms; 2 at 3.1 and 6.8 s |

- **V** The floor is one game frame (33 ms at the game's 30 Hz; two frames on a 60 Hz display): the renderer asks for
  the image at the first draw and draws the original in that frame however fast the decode is, and the upload waits
  for the texture's next draw. A 1024x1024 image decodes within that frame; a 2048x2048 one takes two or three.
- **I** The slow cases are not long pop-ins: their decodes took under 25 ms, so the image was waiting for the
  texture to be drawn again (a camera cut in the battle intro, a texture drawn on one frame and then seconds later).
  The trace cannot tell the two apart.
- **V** The pack did not slow the battle measurably: the seconds below full speed (75-81%, frames of about 500 ms)
  are the same without a pack and come from the route's screenshots. One second of the intro ran at 76% with the
  pack and 86% without; one run each, so not conclusive.
- **I** So a load-time prefetch removes one game frame of the original for nearly every texture. Waiting a few
  milliseconds at the first draw for the decode would remove it for small images only (median decode 2 ms), at the
  cost of a short stall.

## 9. Not verified

- The pop-in on the Steam Deck (a slower CPU; expect decodes about twice as long).
- The layouts of the image library's object and of a model's texture entry.
- Fighter textures: no fighter texture was traced; the two RAM textures picked in the battle run turned out to be
  menu textures.
- Effects (`TEX` in FEP), the world map, and the story mode's loading.
- Whether a texture's palette is ever changed after loading in a way that changes its key without changing its
  pixels (it would not matter to a `data_hash` prefetch).

## How it was traced

The port's clang build with `D012_TRACE_IO=1` (each read and its destination), `D012_TRACE_TEXTURE_PACK=1` with a
pack loaded (each texture's address, size, format and key on first use) and `PSPRECOMP_WATCH_WRITE=<address>`
with `PSPRECOMP_WATCH_WRITE_SIZE` (every guest store to the range, with the thread and the last dispatched address),
run through `tools/port/gfx_tour.py` (the title screen and the `qb_chaos_shrine` route). The dispatched address is
where the recompiled code last entered through the dispatcher, so a store is in that function or in one it called
directly. Functions were looked up in `config/inventory/main.json` and read with `tools/disassemble_elf.py`.

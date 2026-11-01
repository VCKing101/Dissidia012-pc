# HD texture packs

The port draws texture packs in the format PPSSPP defined: a folder holding a `textures.ini` and the images it
names, each image replacing one of the game's textures wherever the game draws it. This page covers using a pack,
what the port supports, and making one with **Dump textures**. The code is PortableKit's (`host/gpu/texture_pack*`,
`replacement_textures*`, `host/ui/texture_pack_screen.cpp`); [texture-loading.md](texture-loading.md) has the
research on where the game's textures come from and how late a replacement appears.

## 1. Installing a pack

Put the pack's files in a folder named **`ULUS10566`** (the USA game's id) in the `textures` folder of the data
folder, so that `textures.ini` is directly inside it:

| Installed with | Put the pack in |
| --- | --- |
| The Windows setup | `data\textures\ULUS10566\` in the install folder |
| `setup.ps1` (from source) | `build\port\data\textures\ULUS10566\` |
| The Steam Deck package | `data/textures/ULUS10566/` in its folder |

Or import it from the menu: open it (Esc, or L3 + R3 on a controller), go to **Video**, and choose **Import texture
pack…**. Pick a folder and the port finds the pack in it, in any of these layouts:

- the pack folder itself (it holds `textures.ini`);
- a port's data folder (`textures/ULUS10566/textures.ini`);
- a `TEXTURES` folder (`ULUS10566/textures.ini`) or PPSSPP's memory stick (`PSP/TEXTURES/ULUS10566/textures.ini`);
- a pack named for another release whose `textures.ini` lists `ULUS10566` under `[games]`.

It checks the pack (keys, images, missing files, size, free space) before anything is copied, then offers:

- **Copy into the data folder** (or **Copy and replace**): copies the pack to `textures\ULUS10566`. A pack already
  there moves to `textures\.backup\<date>_<time>`; nothing is deleted.
- **Use it where it is**: reads the pack from its own folder and copies nothing. The folder must stay where it is.
  The **Pack used from** row shows it, and **Stop using the pack folder** goes back to the installed pack.

The **Texture pack** row turns packs on and off (on by default) and says what is loaded: how many textures the pack
has and how many images are on the GPU, or where the port looked when there is none. **Open the textures folder**
shows the folder in Explorer.

## 2. What is supported

- **`textures.ini`**, in the sections PPSSPP defined:
  - `[options]`: `hash` (`xxh64` or `xxh32`; required), `ignoreAddress`, `reduceHash`, `ignoreMipmap`,
    `skipLastDXT1Blocks128x64`, `skipLastDXT1Blocks128x128`.
  - `[hashes]`: `key = image`, the image's path relative to the pack. An empty image keeps the game's texture.
  - `[hashranges]`, `[reducehashranges]`, `[filtering]` (`nearest`, `linear` or `auto` per key).
  - `[games]`: `ULUS10566 = textures.ini` (or `true`). The value names the ini file to use for that game, not the
    game's title; another file named there is read on top of `textures.ini`.
- **Images named by their key** (`0000000034bb2152a65d22ce.png`) in the pack's top folder count without a line in
  `textures.ini`; a line for the same key wins. A folder made by **Dump textures** is a pack this way.
- **PNG images**, up to 8192 pixels a side. Names are matched without regard to case when the exact name is missing,
  so packs made on Windows work on Linux.

Not supported yet:

- `.dds`, `.ktx2` and `.zim` images (some newer PPSSPP packs use them): the ini loads, those images fail, and the
  game's own textures are drawn.
- Zipped packs (`textures.zip`): unpack them first. The import says so.
- `hash = quick`, PPSSPP's first hash: the pack does not load.

## 3. How replacement behaves

- **A replaced texture appears one game frame late** the first time it is drawn (33 ms; two frames on a 60 Hz
  display), and two or three frames late for 2048x2048 images: the image is decoded on a background thread, and
  the game's own texture is drawn until it is on the GPU. After that it stays, and later draws are replaced at once.
  Decoding images while the game loads them would remove this; that is planned (see
  [texture-loading.md](texture-loading.md)).
- **GPU memory:** replacement images may use 1 GB of GPU memory. Above that, the images not drawn for a few frames
  are dropped and decoded again when next drawn. `D012_TEXTURE_PACK_MEMORY=<MB>` sets another limit (64 at least).
- **Texture filter:** the **Texture filter** setting (smooth or sharp) applies to replacement images too, unless
  the pack's `[filtering]` names a filter for that texture. Replacement images get mipmaps, so large images stay
  smooth when drawn small.
- **What cannot be replaced usefully:** lines of text are drawn by the game into textures while it runs (512x32,
  most of the distinct textures in a battle), so their keys change with the text. Render targets and screen copies
  (glow, the EX Burst background) are never looked up.
- **Same texture, different places:** the game often loads one texture at different addresses in different scenes.
  Packs with `ignoreAddress = true` (what the dump writes) match it everywhere.

## 4. Making a pack

1. Open the menu, go to **Video**, and turn on **Dump textures**. It is off again each time the game starts, so a
   dump left on cannot fill the disk unnoticed.
2. Play the scenes you want to change. Each texture the game loads is written once, as a PNG named by its key, into
   `textures\dump\ULUS10566\` in the data folder. Turning the option on also writes the textures already on screen.
   The row's note counts the images written.
3. Copy the images you want to change to a new pack folder, with the dump's `textures.ini`, and edit or upscale
   them, keeping their names. Keep the texture's shape: a 256x128 texture's image should be 2:1, ideally a whole
   multiple of its size (4x gives 1024x512).
4. Put the pack folder in `textures\ULUS10566` (or import it, or use it where it is) and look at the scene. After
   changing images, turn **Texture pack** off and on again to reload them.

To start from a dump without the menu, start the game with `D012_TEXTURE_DUMP=<folder>` set; it dumps into that
folder from the first frame.

**Keys.** A key is 24 hex digits: 16 for the texture's size (and its palette, for the game's indexed textures)
and 8 for the hash of its pixels. Two things catch pack makers out:

- **A 512-pixel-tall texture can have two or more keys.** When the game draws only its top part, the key covers the
  rows drawn so far; when it later draws further down, the same texture gets a new key, and the dump writes a second
  file. The title screen's 512x512 picture has two (`…a65d22ce` before the menu appears, `…979fe307` after). A pack
  needs an image for each; point both keys at one image in `textures.ini` rather than shipping two copies:

  ```ini
  [options]
  version = 1
  hash = xxh64
  ignoreAddress = true

  [games]
  ULUS10566 = textures.ini

  [hashes]
  0000000034bb2152a65d22ce = title/title.png
  0000000034bb2152979fe307 = title/title.png
  ```

- **The same picture with another palette is another texture.** Indexed textures mix their palette into the key,
  so a recoloured variant (a costume, a team colour) needs its own image.

## 5. Checking a pack

Start the game with `D012_TRACE_TEXTURE_PACK=1` set. The log then lists each texture the game uses, the first time
it is drawn, with its address, size, format, key and whether the pack has an image for it:

```text
[texpack] 1 textures from ...\textures\ULUS10566
[texpack] 0x8c63e30 512x512 fmt=5 key 0000000034bb2152a65d22ce -> title/title.png
[texpack] 0x8c63e30 512x512 fmt=5 key 0000000034bb2152979fe307 (not in the pack)
[texpack] decoded title/title.png 2048x2048
[texpack] frame 1028: uploaded title/title.png (21888 KiB with mips)
```

`(not in the pack)` on a texture you meant to replace usually means a second key (section 4) or another palette.
`cannot use <image>` means the image is missing or not a PNG. Every 60 frames the log also says how many draws
were replaced and how much GPU memory the images use.

Setting a variable:

- **Windows install:** open a Command Prompt in the install folder and start the game from it:

  ```bat
  set D012_TRACE_TEXTURE_PACK=1
  D012Native.exe
  ```

  The log is `data\logs\D012Native.log`.
- **Steam Deck** (not tried): in the game's Steam properties, set the launch options to
  `D012_TRACE_TEXTURE_PACK=1 %command%`.
- **From source:** set it in PowerShell (`$env:D012_TRACE_TEXTURE_PACK = "1"`) before `tools\port\play_d012.ps1`,
  or pass `--env NAME=VALUE` to `tools/port/run_d012.py`.

## 6. Tested

On Windows, with the clang build (1 November 2026): a pack made from a dump of the title screen, with its images
four times the size, replaced the whole title screen; a pack of four-times images of everything a Quick Battle at the
Old Chaos Shrine loads (135 images, text lines left out) was drawn through the battle without slowing it; the menu's
**Dump textures** wrote both of the title picture's keys and turned off cleanly. Not tried: a community pack, story
mode, the world map, and the Steam Deck.

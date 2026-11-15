"""Draws the Android app's launcher icons into port/android/res.

The icon is the port's own, drawn here like the Linux setup's
(tools/port/linux_setup/dissidia012-setup.svg): a gold ring and "012" on
dark blue. Nothing of the game's. The PNGs are committed, so a build does not
need this script or Pillow; run it again after changing the drawing.

    python tools/port/android_icons.py

Writes, at every density, the adaptive icon's foreground and monochrome
layers (108 dp, the drawing inside the 72 dp the launcher keeps), the legacy
square and round icons (48 dp), and values/ic_launcher_background.xml.

MIT licence; see port/README.md.
"""

from __future__ import annotations

import sys
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

BACKGROUND = (0x1B, 0x2A, 0x4A)
GOLD = (0xD8, 0xB2, 0x5A)
WHITE = (0xF2, 0xF2, 0xF2)
DENSITIES = {"mdpi": 1.0, "hdpi": 1.5, "xhdpi": 2.0, "xxhdpi": 3.0, "xxxhdpi": 4.0}
FONTS = [
    "C:/Windows/Fonts/segoeuib.ttf",
    "C:/Windows/Fonts/arialbd.ttf",
    "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
]
SIZE = 1024  # drawn large, scaled down for each density


def font(size: int) -> ImageFont.FreeTypeFont:
    for path in FONTS:
        if Path(path).is_file():
            return ImageFont.truetype(path, size)
    sys.exit("no bold sans-serif font found; add one to FONTS")


def emblem(with_background: bool) -> Image.Image:
    """The ring and the number on a transparent square, or on the rounded
    dark blue square of the legacy icon."""
    image = Image.new("RGBA", (SIZE, SIZE), (0, 0, 0, 0))
    draw = ImageDraw.Draw(image)
    if with_background:
        draw.rounded_rectangle((32, 32, SIZE - 32, SIZE - 32), radius=160, fill=BACKGROUND + (255,))
    centre = SIZE / 2
    ring = SIZE * 0.34
    draw.ellipse((centre - ring, centre - ring, centre + ring, centre + ring), outline=GOLD + (255,),
                 width=int(SIZE * 0.04))
    text_font = font(int(SIZE * 0.28))
    draw.text((centre, centre), "012", font=text_font, fill=WHITE + (255,), anchor="mm")
    return image


def main() -> None:
    res = Path(__file__).resolve().parents[2] / "port" / "android" / "res"
    foreground = emblem(False)
    legacy = emblem(True)
    for density, scale in DENSITIES.items():
        folder = res / f"mipmap-{density}"
        folder.mkdir(parents=True, exist_ok=True)
        layer = round(108 * scale)
        inner = round(72 * scale)
        icon = round(48 * scale)
        # Adaptive foreground: the drawing inside the middle 72 dp of 108.
        layer_image = Image.new("RGBA", (layer, layer), (0, 0, 0, 0))
        offset = (layer - inner) // 2
        layer_image.alpha_composite(foreground.resize((inner, inner), Image.LANCZOS), (offset, offset))
        layer_image.save(folder / "ic_launcher_foreground.png", optimize=True)
        # Monochrome, for themed icons: the same shape in white.
        alpha = layer_image.getchannel("A")
        mono = Image.new("RGBA", (layer, layer), (255, 255, 255, 0))
        mono.putalpha(alpha)
        mono.save(folder / "ic_launcher_monochrome.png", optimize=True)
        # Legacy icons, for launchers without adaptive icons.
        small = legacy.resize((icon, icon), Image.LANCZOS)
        small.save(folder / "ic_launcher.png", optimize=True)
        small.save(folder / "ic_launcher_round.png", optimize=True)
    values = res / "values"
    values.mkdir(parents=True, exist_ok=True)
    (values / "ic_launcher_background.xml").write_text(
        '<?xml version="1.0" encoding="utf-8"?>\n<resources>\n'
        f'    <color name="ic_launcher_background">#{BACKGROUND[0]:02x}{BACKGROUND[1]:02x}{BACKGROUND[2]:02x}</color>\n'
        "</resources>\n", encoding="utf-8", newline="\n")
    print(f"icons written to {res}")


if __name__ == "__main__":
    main()

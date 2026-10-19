"""Convert the port's BMP captures to PNG, optionally as one contact sheet.

    python tools/port/shots_to_png.py DIR [--sheet OUT.png] [--width W] [--match TEXT]

Writes DIR/png/NAME.png for every DIR/NAME.bmp (skipping ones already done).
With --sheet, also writes a grid of the matching captures, each scaled to W
pixels wide (default 320) and labelled with its name, for a quick look.
"""

import argparse
import os
import re
import sys

from PIL import Image, ImageDraw


def natural(name):
    return [int(part) if part.isdigit() else part for part in re.split(r"(\d+)", name)]


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("dir")
    parser.add_argument("--sheet")
    parser.add_argument("--width", type=int, default=320)
    parser.add_argument("--match", default="")
    parser.add_argument("--columns", type=int, default=4)
    options = parser.parse_args()

    out_dir = os.path.join(options.dir, "png")
    os.makedirs(out_dir, exist_ok=True)
    names = sorted((n for n in os.listdir(options.dir) if n.endswith(".bmp") and options.match in n), key=natural)
    images = []
    for name in names:
        source = os.path.join(options.dir, name)
        target = os.path.join(out_dir, name[:-4] + ".png")
        image = Image.open(source).convert("RGB")
        if not os.path.exists(target) or os.path.getmtime(target) < os.path.getmtime(source):
            image.save(target)
        images.append((name[:-4], image))
    print(f"{len(images)} captures in {out_dir}")

    if options.sheet and images:
        width = options.width
        height = max(round(img.height * width / img.width) for _, img in images)
        columns = min(options.columns, len(images))
        rows = (len(images) + columns - 1) // columns
        sheet = Image.new("RGB", (columns * width, rows * (height + 14)), (40, 40, 40))
        draw = ImageDraw.Draw(sheet)
        for i, (label, img) in enumerate(images):
            x, y = (i % columns) * width, (i // columns) * (height + 14)
            sheet.paste(img.resize((width, round(img.height * width / img.width))), (x, y + 14))
            draw.text((x + 2, y + 1), label, fill=(255, 255, 0))
        sheet.save(options.sheet)
        print(f"sheet {options.sheet}")
    return 0


if __name__ == "__main__":
    sys.exit(main())

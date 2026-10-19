"""The Battle BGM Maker: set up a mod's mod.json, above all its loop points, in the browser.

    python port/tools/bgm_maker/bgm_maker.py [mods folder] [--ffmpeg PATH] [--port N] [--no-browser]

Serves a page on 127.0.0.1 only (a free port unless --port), opens it in the
default browser, and works on the mods folder given (default
build/port/data/mods in this repository). Audio is decoded with the port's own
FFmpeg (found in the port's build, or --ffmpeg), so a loop sample in the tool
is the same sample in the game. Needs only Python 3.10+ and its standard
library; no network. Stop it with Ctrl+C. Format: docs/port/mod-folder.md.
MIT licence; see port/README.md.
"""

from __future__ import annotations

import argparse
import os
import sys
import webbrowser

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from bgmmaker.audio import REPO_ROOT, Decoder, FFmpegMissing, find_ffmpeg  # noqa: E402
from bgmmaker.server import App, serve  # noqa: E402


def default_mods_dir() -> str:
    in_repo = os.path.join(REPO_ROOT, "build", "port", "data")
    if os.path.isdir(os.path.join(REPO_ROOT, "port", "host")):
        return os.path.join(in_repo, "mods")
    # Outside the repository: the port's own data folder.
    if os.name == "nt":
        base = os.environ.get("APPDATA", os.path.expanduser("~"))
        return os.path.join(base, "Decomp-Dissidia", "D012Native", "mods")
    if sys.platform == "darwin":
        return os.path.expanduser("~/Library/Application Support/Decomp-Dissidia/D012Native/mods")
    return os.path.expanduser("~/.local/share/Decomp-Dissidia/D012Native/mods")


def main(argv=None) -> int:
    if sys.version_info < (3, 10):
        print("The Battle BGM Maker needs Python 3.10 or newer.", file=sys.stderr)
        return 2
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("mods_dir", nargs="?", default=None, help="the mods folder (default build/port/data/mods)")
    parser.add_argument("--ffmpeg", help="ffmpeg executable (or its folder) to decode with; ffprobe must be beside it")
    parser.add_argument("--port", type=int, default=0, help="port on 127.0.0.1 (default: a free one)")
    parser.add_argument("--no-browser", action="store_true", help="print the address instead of opening it")
    options = parser.parse_args(argv)

    mods_dir = os.path.abspath(options.mods_dir or default_mods_dir())
    if not os.path.isdir(mods_dir):
        parent = os.path.dirname(mods_dir)
        if not os.path.isdir(parent):
            print(f"No such folder: {parent}", file=sys.stderr)
            return 2
        os.makedirs(mods_dir)
        print(f"Made the mods folder {mods_dir}")
    try:
        ff = find_ffmpeg(options.ffmpeg)
    except FFmpegMissing as error:
        print(f"error: {error}", file=sys.stderr)
        return 2
    app = App(mods_dir, Decoder(ff))
    server = serve(app, options.port)
    url = f"http://127.0.0.1:{app.port}/#token={app.token}"
    print(f"Mods folder: {mods_dir}", flush=True)
    print(f"FFmpeg:      {ff.ffmpeg} ({ff.version}; from {ff.source})")
    print(f"Battle BGM Maker:   {url}", flush=True)
    print("Stop with Ctrl+C.", flush=True)
    if not options.no_browser:
        webbrowser.open(url)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        server.server_close()
    return 0


if __name__ == "__main__":
    sys.exit(main())

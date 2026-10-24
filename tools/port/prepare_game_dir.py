"""Set up the port's game directory, build/port/game, from the read-only inputs.

    python tools/port/prepare_game_dir.py [--eboot FILE] [--iso FILE] [--copy-iso] [--force]

PortableKit runs a game from a directory holding EBOOT.ELF, disc.iso and ms0/
(<PREFIX>_GAME_DIR, see port/README.md). This makes that directory without
touching the originals:

- EBOOT.ELF: a copy of the decrypted executable (--eboot, default
  build/ULUS10566/EBOOT.ELF), checked against the hash the profile declares.
- disc.iso: a hard link to the retail ISO (--iso, default the USA ISO at the
  repository root; PortableKit opens it read-only), or a copy with --copy-iso or
  when the ISO is on another drive. The ISO's EBOOT.BIN is not checked here; the
  profile's hash of it is checked by the installer only (setup.ps1 checks the
  whole ISO).
- ms0/: an empty memory stick. No saves are copied in.

It also makes build/port/data, the directory to pass as D012_DATA_DIR.
Re-running it keeps files that are already right; --force replaces them.
"""

import argparse
import hashlib
import os
import shutil
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
ISO_NAME = "Dissidia 012 - Duodecim Final Fantasy (USA) (En,Fr,De,Es,It).iso"
EBOOT_SHA256 = "0bc8e779aad2814a3d1bd77346f69ca5e16665845ed053cd0ad44a9cbfaefe6d"


def sha256(path):
    digest = hashlib.sha256()
    with open(path, "rb") as f:
        for block in iter(lambda: f.read(1 << 20), b""):
            digest.update(block)
    return digest.hexdigest()


def same_file(a, b):
    try:
        return os.path.samefile(a, b)
    except OSError:
        return False


def prepare(eboot, iso, game, data, copy_iso=False, force=False):
    """Make game/ (EBOOT.ELF, disc.iso, ms0/) and data/; returns lines describing what was done."""
    eboot, iso, game, data = Path(eboot), Path(iso), Path(game), Path(data)
    for path in (eboot, iso):
        if not path.is_file():
            raise SystemExit(f"missing input: {path}")
    if sha256(eboot) != EBOOT_SHA256:
        raise SystemExit(f"{eboot} is not the decrypted ULUS10566 v1.00 executable")

    (game / "ms0" / "PSP" / "SAVEDATA").mkdir(parents=True, exist_ok=True)
    data.mkdir(parents=True, exist_ok=True)
    done = []

    target = game / "EBOOT.ELF"
    if same_file(eboot, target) or (not force and target.is_file() and sha256(target) == EBOOT_SHA256):
        done.append(f"kept     {target}")
    else:
        if target.exists():
            target.unlink()
        shutil.copyfile(eboot, target)
        done.append(f"copied   {target}")

    target = game / "disc.iso"
    if same_file(iso, target):
        done.append(f"kept     {target} (hard link)")
    elif (not force and target.is_file() and target.stat().st_size == iso.stat().st_size
          and int(target.stat().st_mtime) == int(iso.stat().st_mtime)):
        done.append(f"kept     {target} (copy)")
    else:
        if target.exists():
            target.unlink()  # removes the link or the copy, never the original
        linked = False
        if not copy_iso:
            try:
                os.link(iso, target)
                linked = True
                done.append(f"linked   {target}")
            except OSError as error:
                done.append(f"cannot hard-link the ISO ({error.strerror}); copying it (about 1.7 GB)")
        if not linked:
            shutil.copy2(iso, target)
            done.append(f"copied   {target}")
    done.append(f"data dir {data}")
    return done


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("--eboot", default=str(ROOT / "build" / "ULUS10566" / "EBOOT.ELF"),
                        help="decrypted executable (default build/ULUS10566/EBOOT.ELF)")
    parser.add_argument("--iso", default=str(ROOT / ISO_NAME), help="the USA ISO (default: at the repository root)")
    parser.add_argument("--copy-iso", action="store_true", help="copy the ISO instead of hard-linking it")
    parser.add_argument("--force", action="store_true", help="replace EBOOT.ELF and disc.iso if present")
    options = parser.parse_args()
    for line in prepare(options.eboot, options.iso, ROOT / "build" / "port" / "game", ROOT / "build" / "port" / "data",
                        options.copy_iso, options.force):
        print(line)
    return 0


if __name__ == "__main__":
    sys.exit(main())

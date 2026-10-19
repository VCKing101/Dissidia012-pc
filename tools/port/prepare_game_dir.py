"""Set up the port's game directory, build/port/game, from the read-only inputs.

    python tools/port/prepare_game_dir.py [--copy-iso] [--force]

PortableKit runs a game from a directory holding EBOOT.ELF, disc.iso and ms0/
(<PREFIX>_GAME_DIR, see port/README.md). This makes that directory without
touching the originals:

- EBOOT.ELF: a copy of build/ULUS10566/EBOOT.ELF, checked against the hash the
  profile declares.
- disc.iso: a hard link to the retail ISO at the repository root (PortableKit
  opens it read-only), or a copy with --copy-iso. The ISO's EBOOT.BIN is not
  checked here; the profile's hash of it is checked by the installer only.
- ms0/: an empty memory stick. No saves are copied in.

It also makes build/port/data, the directory to pass as D012_DATA_DIR.
"""

import argparse
import hashlib
import os
import shutil
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
ISO_NAME = "Dissidia 012 - Duodecim Final Fantasy (USA) (En,Fr,De,Es,It).iso"
EBOOT_SHA256 = "0bc8e779aad2814a3d1bd77346f69ca5e16665845ed053cd0ad44a9cbfaefe6d"


def sha256(path):
    digest = hashlib.sha256()
    with open(path, "rb") as f:
        for block in iter(lambda: f.read(1 << 20), b""):
            digest.update(block)
    return digest.hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("--copy-iso", action="store_true", help="copy the ISO instead of hard-linking it")
    parser.add_argument("--force", action="store_true", help="replace EBOOT.ELF and disc.iso if present")
    options = parser.parse_args()

    eboot = os.path.join(ROOT, "build", "ULUS10566", "EBOOT.ELF")
    iso = os.path.join(ROOT, ISO_NAME)
    game = os.path.join(ROOT, "build", "port", "game")
    data = os.path.join(ROOT, "build", "port", "data")
    for path in (eboot, iso):
        if not os.path.isfile(path):
            sys.exit(f"missing input: {path}")
    if sha256(eboot) != EBOOT_SHA256:
        sys.exit(f"{eboot} is not the decrypted ULUS10566 v1.00 executable")

    os.makedirs(os.path.join(game, "ms0", "PSP", "SAVEDATA"), exist_ok=True)
    os.makedirs(data, exist_ok=True)

    target = os.path.join(game, "EBOOT.ELF")
    if options.force or not os.path.exists(target):
        if os.path.exists(target):
            os.remove(target)
        shutil.copyfile(eboot, target)
        print(f"copied   {target}")
    else:
        print(f"kept     {target}")

    target = os.path.join(game, "disc.iso")
    if options.force or not os.path.exists(target):
        if os.path.exists(target):
            os.remove(target)  # removes the link or the copy, never the original
        if options.copy_iso:
            shutil.copyfile(iso, target)
            print(f"copied   {target}")
        else:
            os.link(iso, target)
            print(f"linked   {target}")
    else:
        print(f"kept     {target}")
    print(f"data dir {data}")
    return 0


if __name__ == "__main__":
    sys.exit(main())

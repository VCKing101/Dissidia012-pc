"""Install checksum-pinned local PSP compiler artifacts; default is SNC + wibo."""

import argparse
import hashlib
import json
import os
import shutil
import subprocess
import tarfile
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def digest(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("names", nargs="*", help="Artifact names; default installs the build's SNC compiler and wibo")
    args = parser.parse_args()
    manifest = json.loads((ROOT / "config/compiler-artifacts.json").read_text())
    names = args.names or ["pspsnc_1.2.7503.0", "wibo"]
    if set(names) - set(manifest):
        parser.error("Unknown compiler artifact")
    for name in names:
        artifact = manifest[name]
        suffix = ".tar.gz" if artifact["format"] == "tar.gz" else "-x86_64"
        archive = ROOT / "tools/downloads" / (name + suffix)
        archive.parent.mkdir(parents=True, exist_ok=True)
        if not archive.exists() or digest(archive) != artifact["sha256"]:
            temporary = archive.with_suffix(".part")
            request = urllib.request.Request(artifact["url"], headers={"User-Agent": "Dissidia-compiler-setup"})
            with urllib.request.urlopen(request, timeout=60) as response, temporary.open("wb") as output:
                shutil.copyfileobj(response, output)
            if digest(temporary) != artifact["sha256"]:
                raise ValueError(f"Download hash mismatch: {name}")
            temporary.replace(archive)
        destination = (ROOT / artifact["destination"]).resolve()
        destination.relative_to(ROOT)
        if artifact["format"] == "tar.gz":
            destination.mkdir(parents=True, exist_ok=True)
            with tarfile.open(archive) as packed:
                packed.extractall(destination, filter="data")
        else:
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(archive, destination)
            if os.name == "nt":
                linux_path = "/mnt/" + destination.drive[0].lower() + destination.as_posix()[2:]
                subprocess.run(["wsl", "-d", "Ubuntu-24.04", "--", "chmod", "+x", linux_path], check=True)
            else:
                destination.chmod(0o755)
        print(f"Installed {name}; verified SHA-256 {artifact['sha256']}")


if __name__ == "__main__":
    main()

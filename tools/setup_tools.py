"""Download pinned portable analysis tools and verify official SHA-256 digests."""

import argparse
import hashlib
import json
import urllib.request
import zipfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def digest(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def download(item):
    name, spec = item
    archive = ROOT / "tools/downloads" / (name + ".zip")
    archive.parent.mkdir(parents=True, exist_ok=True)
    if not archive.exists() or digest(archive) != spec["sha256"]:
        print(f"Downloading {name} {spec['version']}", flush=True)
        request = urllib.request.Request(spec["url"], headers={"User-Agent": "Dissidia-decomp-setup"})
        temporary = archive.with_suffix(".part")
        with urllib.request.urlopen(request, timeout=60) as response, temporary.open("wb") as output:
            while chunk := response.read(1024 * 1024):
                output.write(chunk)
        if digest(temporary) != spec["sha256"]:
            raise ValueError(f"SHA-256 mismatch for {name}; archive not installed")
        temporary.replace(archive)
    print(f"Verified {name}: {spec['sha256']}", flush=True)
    return name, spec, archive


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("names", nargs="*", help="Selected tools; default is all tools")
    args = parser.parse_args()
    manifest = json.loads((ROOT / "config/tools.json").read_text(encoding="utf-8"))
    selected = args.names or list(manifest)
    if set(selected) - set(manifest):
        parser.error("Unknown tool name")
    with ThreadPoolExecutor(max_workers=4) as pool:
        downloaded = list(pool.map(download, [(name, manifest[name]) for name in selected]))
    for name, spec, archive in downloaded:
        destination = (ROOT / spec["destination"]).resolve()
        destination.relative_to(ROOT)
        destination.mkdir(parents=True, exist_ok=True)
        marker = destination / ("." + name + "-installed.json")
        if marker.exists() and json.loads(marker.read_text())["sha256"] == spec["sha256"]:
            print(f"Already installed: {name}", flush=True)
            continue
        with zipfile.ZipFile(archive) as zipped:
            for member in zipped.infolist():
                (destination / member.filename).resolve().relative_to(destination)
            zipped.extractall(destination)
        marker.write_text(json.dumps(spec, indent=2) + "\n", encoding="utf-8")
        print(f"Installed {name} in {destination}", flush=True)


if __name__ == "__main__":
    main()

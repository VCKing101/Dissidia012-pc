"""Verify the USA reference ISO and extract selected files to ignored orig/."""

import argparse
import hashlib
import json
from pathlib import Path

from inspect_iso import Iso

ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("iso", type=Path)
    args = parser.parse_args()
    config = json.loads((ROOT / "config/ulus10566.json").read_text(encoding="utf-8"))
    with args.iso.open("rb") as stream:
        digest = hashlib.file_digest(stream, "sha256").hexdigest()
    if digest != config["iso_sha256"]:
        raise ValueError("ISO does not match the verified ULUS10566 reference")
    destination = ROOT / "orig" / config["version"]
    iso = Iso(args.iso)
    extracted = []
    try:
        entries = {entry["path"]: entry for entry in iso.files()}
        for path in config["files"]:
            relative = Path(path.lstrip("/"))
            output = (destination / relative).resolve()
            output.relative_to(destination.resolve())
            output.parent.mkdir(parents=True, exist_ok=True)
            data = iso.content(entries[path])
            if output.exists() and output.read_bytes() != data:
                raise ValueError(f"Existing extracted input differs; preserving {output}")
            if not output.exists():
                output.write_bytes(data)
            with output.open("rb") as stream:
                file_hash = hashlib.file_digest(stream, "sha256").hexdigest()
            extracted.append({"iso_path": path, "local_path": output.relative_to(ROOT).as_posix(),
                              "size": output.stat().st_size, "sha256": file_hash})
            print(f"Extracted {path}: {output.stat().st_size} bytes")
    finally:
        iso.stream.close()
    (destination / "extraction.json").write_text(json.dumps({"iso_sha256": digest, "files": extracted}, indent=2) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()

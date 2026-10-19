"""Make local decrypted test ISOs without modifying the pinned retail image.

Only single-extent ISO9660 is supported. All file LBAs and volume descriptors
remain unchanged. A smaller replacement shrinks both endian directory lengths;
its unused allocation tail is zeroed. This is not retail encryption/signing.
"""

import argparse
import hashlib
import json
import os
import struct
import tempfile
from datetime import datetime, timezone
from pathlib import Path

from inspect_iso import Iso

ROOT = Path(__file__).resolve().parents[1]
SOURCE_NAME = "Dissidia 012 - Duodecim Final Fantasy (USA) (En,Fr,De,Es,It).iso"
TARGETS = {
    "main": "/PSP_GAME/SYSDIR/EBOOT.BIN",
    "extra": "/PSP_GAME/USRDIR/DATA/OVL_EXTRA_APP.ELF",
    "menu": "/PSP_GAME/USRDIR/DATA/OVL_MENU_APP.ELF",
    "battle": "/PSP_GAME/USRDIR/DATA/OVL_BATTLE_APP.ELF",
}
OUTPUTS = {"main": "EBOOT.ELF", "extra": "OVL_EXTRA_APP.ELF",
           "menu": "OVL_MENU_APP.ELF", "battle": "OVL_BATTLE_APP.ELF"}


def digest(path):
    result = hashlib.sha256()
    with Path(path).open("rb") as stream:
        while chunk := stream.read(8 * 1024 * 1024):
            result.update(chunk)
    return result.hexdigest()


def round_block(size, block):
    return (size + block - 1) // block * block


def directory_entries(iso):
    """Return records with absolute directory offsets; reject ambiguous geometry."""
    found, directories, visited = [], [], set()

    def walk(directory, prefix):
        key = (directory["extent_lba"], directory["size"])
        if key in visited:
            raise ValueError("Directory alias/cycle unsupported")
        visited.add(key)
        start = directory["extent_lba"] * iso.block
        directories.append((start, start + round_block(directory["size"], iso.block)))
        data = iso.read(start, directory["size"])
        position = 0
        while position < len(data):
            length = data[position]
            if not length:
                position = round_block(position + 1, iso.block)
                continue
            if position % iso.block + length > iso.block:
                raise ValueError("Directory record crosses logical block")
            raw = data[position:position + length]
            record = iso.record(raw)
            if (struct.unpack_from(">I", raw, 6)[0] != record["extent_lba"]
                    or struct.unpack_from(">I", raw, 14)[0] != record["size"]):
                raise ValueError("Directory endian fields disagree")
            if raw[1] or raw[26] or raw[27] or record["flags"] & 0x80:
                raise ValueError("Extended/interleaved/multi-extent records unsupported")
            position += length
            if record["name"] in (b"\0", b"\1"):
                continue
            path = prefix + "/" + record["name"].decode("ascii").split(";")[0]
            if record["flags"] & 2:
                walk(record, path)
            else:
                found.append({"path": path, "extent_lba": record["extent_lba"],
                              "size": record["size"], "record_offset": start + position - length})

    walk(iso.root, "")
    if len({e["path"] for e in found}) != len(found):
        raise ValueError("Duplicate ISO paths unsupported")
    allocations = directories + [(e["extent_lba"] * iso.block,
                                  e["extent_lba"] * iso.block + round_block(e["size"], iso.block))
                                 for e in found if e["size"]]
    allocations.sort()
    if any(a < 0 or b > iso.size for a, b in allocations):
        raise ValueError("Allocation outside ISO")
    if any(end > following for (_, end), (following, _) in zip(allocations, allocations[1:])):
        raise ValueError("File/directory allocations overlap")
    return found


def replacement_patches(iso, entries, replacements):
    by_path = {e["path"]: e for e in entries}
    patches, manifest = [], []
    for path, data in replacements.items():
        entry = by_path.get(path)
        if entry is None:
            raise ValueError(f"ISO entry missing: {path}")
        if not data or len(data) > entry["size"]:
            raise ValueError(f"Replacement does not fit original file: {path}")
        start = entry["extent_lba"] * iso.block
        allocation = round_block(entry["size"], iso.block)
        old = iso.read(start, allocation)
        if any(old[entry["size"]:]):
            raise ValueError(f"Original allocation padding is nonzero: {path}")
        patches.append((start, data + bytes(allocation - len(data))))
        if len(data) != entry["size"]:
            patches.append((entry["record_offset"] + 10,
                            struct.pack("<I", len(data)) + struct.pack(">I", len(data))))
        manifest.append({"iso_path": path, "extent_lba": entry["extent_lba"],
                         "original_file_bytes": entry["size"], "replacement_file_bytes": len(data),
                         "allocation_bytes": allocation, "sha256": hashlib.sha256(data).hexdigest(),
                         "original_extent_sha256": hashlib.sha256(old[:entry["size"]]).hexdigest(),
                         "directory_record_offset": entry["record_offset"]})
    patches.sort(key=lambda p: p[0])
    if any(a + len(data) > b for (a, data), (b, _) in zip(patches, patches[1:])):
        raise ValueError("Replacement patches overlap")
    return patches, manifest


def copy_patched(source, output, patches):
    source, output = Path(source), Path(output)
    if source.resolve() == output.resolve() or (output.exists() and source.samefile(output)):
        raise ValueError("Cannot overwrite source ISO")
    # Replace the directory entry, never truncate an existing output in place:
    # it could be a hard link to another user-owned image.
    temporary = None
    try:
        with tempfile.NamedTemporaryFile(mode="w+b", dir=output.parent,
                                         prefix=f".{output.name}.", suffix=".tmp", delete=False) as result:
            temporary = Path(result.name)
            with source.open("rb") as original:
                while chunk := original.read(8 * 1024 * 1024):
                    result.write(chunk)
            for offset, data in patches:
                result.seek(offset)
                result.write(data)
            result.flush()
            os.fsync(result.fileno())
        os.replace(temporary, output)
    finally:
        if temporary is not None and temporary.exists():
            temporary.unlink()


def verify_patched(source, output, patches):
    """Every output byte must equal source or an explicitly listed replacement."""
    if Path(source).stat().st_size != Path(output).stat().st_size:
        raise ValueError("Patched image size changed")
    sha = hashlib.sha256()
    with Path(source).open("rb") as original, Path(output).open("rb") as result:
        offset = 0
        while chunk := original.read(8 * 1024 * 1024):
            expected = bytearray(chunk)
            for begin, replacement in patches:
                left, right = max(offset, begin), min(offset + len(chunk), begin + len(replacement))
                if left < right:
                    expected[left - offset:right - offset] = replacement[left - begin:right - begin]
            actual = result.read(len(chunk))
            if actual != expected:
                raise ValueError(f"Unexpected package bytes in block starting 0x{offset:X}")
            sha.update(actual)
            offset += len(chunk)
    return sha.hexdigest()


def validate_builds():
    # collect rechecks all four current source fingerprints and full-file proofs.
    from progress import collect
    report = collect()
    if len(report["modules"]) != 4 or not all(m["module_rebuild_verified"] for m in report["modules"]):
        raise ValueError("All four current full-file build proofs are required")
    return {module: json.loads((ROOT / f"build/ULUS10566/{module}.verify.json").read_text())["build_inputs"]
            for module in TARGETS}


def prepare(source, output_dir):
    source, output_dir = Path(source).resolve(), Path(output_dir).resolve()
    permitted = (ROOT / "build/ULUS10566/runtime/packages").resolve()
    if output_dir != permitted and permitted not in output_dir.parents:
        raise ValueError("Packages must stay under build/ULUS10566/runtime/packages")
    spec = json.loads((ROOT / "config/ulus10566.json").read_text())
    if digest(source) != spec["iso_sha256"]:
        raise ValueError("Source ISO does not match pinned USA SHA256")
    builds = validate_builds()
    scope = json.loads((ROOT / "config/progress.json").read_text())
    original_paths = {m["id"]: ROOT / m["path"] for m in scope["modules"]}
    original = {key: path.read_bytes() for key, path in original_paths.items()}
    rebuilt_paths = {key: ROOT / "build/ULUS10566" / filename for key, filename in OUTPUTS.items()}
    rebuilt = {key: path.read_bytes() for key, path in rebuilt_paths.items()}
    if original != rebuilt:
        raise ValueError("Reference/rebuilt bytes diverged after build verification")
    output_dir.mkdir(parents=True, exist_ok=True)
    iso = Iso(source)
    try:
        if iso.block != 2048 or iso.volume_blocks * iso.block != iso.size:
            raise ValueError("Unsupported block size or truncated volume")
        entries = directory_entries(iso)
        packages = []
        for label, contents, paths in (("original-decrypted", original, original_paths),
                                       ("rebuilt", rebuilt, rebuilt_paths)):
            patches, files = replacement_patches(iso, entries,
                                                  {TARGETS[key]: data for key, data in contents.items()})
            output = output_dir / f"ULUS10566-{label}.iso"
            copy_patched(source, output, patches)
            output_hash = verify_patched(source, output, patches)
            output_iso = Iso(output)
            try:
                updated = directory_entries(output_iso)
                by_path = {e["path"]: e for e in updated}
                for file in files:
                    entry = by_path[file["iso_path"]]
                    if entry["size"] != file["replacement_file_bytes"] or entry["extent_lba"] != file["extent_lba"]:
                        raise ValueError("Patched directory geometry differs")
                    if hashlib.sha256(output_iso.content(entry)).hexdigest() != file["sha256"]:
                        raise ValueError("Patched file payload differs")
            finally:
                output_iso.stream.close()
            packages.append({"label": label, "path": output.relative_to(ROOT).as_posix(),
                             "sha256": output_hash, "file_bytes": output.stat().st_size,
                             "files": files,
                             "inputs": {key: {"path": path.relative_to(ROOT).as_posix(),
                                              "sha256": hashlib.sha256(contents[key]).hexdigest()}
                                        for key, path in paths.items()}})
    finally:
        iso.stream.close()
    if digest(source) != spec["iso_sha256"]:
        raise ValueError("Source ISO changed during package preparation")
    if validate_builds() != builds:
        raise ValueError("Build provenance changed during package preparation")
    manifest = {"schema_version": 1, "generated_at_utc": datetime.now(timezone.utc).isoformat(),
                "version": "ULUS10566", "package_kind": "local-decrypted-test-ISO",
                "source": {"path": str(source), "sha256": spec["iso_sha256"],
                           "file_bytes": source.stat().st_size, "unchanged": True},
                "packages": packages, "build_inputs": builds,
                "packages_byte_identical": packages[0]["sha256"] == packages[1]["sha256"],
                "verification": {"all_unpatched_bytes_unchanged": True, "file_lbas_preserved": True,
                                 "directory_endian_sizes_checked": True, "allocation_overlap_checked": True},
                "runtime_verified": False}
    if not manifest["packages_byte_identical"]:
        raise ValueError("Equal scoped binaries produced unequal test packages")
    target = output_dir / "manifest.json"
    target.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    return manifest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, default=ROOT / SOURCE_NAME)
    parser.add_argument("--output-dir", type=Path, default=ROOT / "build/ULUS10566/runtime/packages")
    args = parser.parse_args()
    manifest = prepare(args.source, args.output_dir)
    print(json.dumps({"packages": [{k: p[k] for k in ("label", "path", "sha256")}
                                   for p in manifest["packages"]],
                      "packages_byte_identical": manifest["packages_byte_identical"]}, indent=2))


if __name__ == "__main__":
    main()

"""Check a reconstructed extra overlay against the pinned retail file hash."""

import argparse
import hashlib
import json
from pathlib import Path
from probe_compilers import functions_from_object
from mips_relocations import configured_addresses, resolve_function

ROOT = Path(__file__).resolve().parents[1]


def verify(candidate):
    config = json.loads((ROOT / "config/ulus10566.json").read_text(encoding="utf-8"))
    reference = ROOT / "orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_EXTRA_APP.ELF"
    original = reference.read_bytes()
    if hashlib.sha256(original).hexdigest() != config["extra_sha256"]:
        raise ValueError("Reference overlay differs from the pinned retail input")
    rebuilt = Path(candidate).read_bytes()
    if rebuilt != original:
        first = next((i for i, (a, b) in enumerate(zip(original, rebuilt)) if a != b), min(len(original), len(rebuilt)))
        raise ValueError(f"Overlay mismatch at file offset 0x{first:X}; expected {len(original)} bytes, got {len(rebuilt)}")
    matches = json.loads((ROOT / "config/extra.matches.json").read_text())
    specifications = [function for unit in matches["units"] for function in unit["functions"]]
    raw_functions = {}
    for unit in matches["units"]:
        raw_functions.update(functions_from_object(ROOT / f"build/ULUS10566/{unit['name']}.raw.o"))
    linked_functions = functions_from_object(ROOT / "build/ULUS10566/extra.linked.elf")
    for specification in specifications:
        address = int(specification["address"], 16)
        offset = address - 0x089DD4C0 + 0x54
        expected = original[offset:offset + specification["size"]]
        raw = raw_functions[specification["name"]]
        linked = linked_functions[specification["name"]]
        if resolve_function(raw, configured_addresses(matches)) != expected:
            raise ValueError(f"Compiled C does not match: {specification['name']}")
        if linked["address"] != address or linked["bytes"] != expected:
            raise ValueError(f"Linked C address/bytes do not match: {specification['name']}")
    report = {"version": config["version"], "overlay": "OVL_EXTRA_APP.ELF",
              "file_bytes": len(rebuilt), "reconstructed_payload_bytes": 0x5F4C - 0x54,
              "remaining_assembly_and_data_bytes": (0x5F4C - 0x54) - sum(function["size"] for function in specifications),
              "preserved_header_and_metadata_bytes": len(rebuilt) - (0x5F4C - 0x54),
              "sha256": hashlib.sha256(rebuilt).hexdigest(), "byte_identical": True,
              "recovered_c_functions": len(specifications),
              "recovered_c_bytes": sum(function["size"] for function in specifications),
              "c_functions": specifications,
              "method": "matching C plus remaining assembly/data; preserved ELF header and metadata"}
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("candidate", nargs="?", type=Path, default=ROOT / "build/ULUS10566/OVL_EXTRA_APP.ELF")
    args = parser.parse_args()
    report = verify(args.candidate)
    print(f"PASS: {report['file_bytes']} bytes match retail SHA-256 {report['sha256']}")


if __name__ == "__main__":
    main()

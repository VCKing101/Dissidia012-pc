"""Verify menu packaging, fixed-address C and reconstruction accounting."""

import argparse
import hashlib
import json
from pathlib import Path

from mips_relocations import configured_addresses, resolve_function
from probe_compilers import functions_from_object
from compiler_inline_sections import verify_inline_units, validate_inline_manifest
from menu_build_provenance import layout_digest

ROOT = Path(__file__).resolve().parents[1]


def verify(candidate):
    spec = json.loads((ROOT / "config/menu.reference.json").read_text())
    reference = (ROOT / spec["path"]).read_bytes()
    if hashlib.sha256(reference).hexdigest() != spec["sha256"]:
        raise ValueError("Menu reference hash mismatch")
    rebuilt = Path(candidate).read_bytes()
    if rebuilt != reference:
        first = next((i for i, (a, b) in enumerate(zip(reference, rebuilt)) if a != b), min(len(reference), len(rebuilt)))
        raise ValueError(f"Menu overlay mismatch at file offset 0x{first:X}; expected {len(reference)} bytes, got {len(rebuilt)}")
    base, offset, size = int(spec["payload_address"], 16), spec["payload_file_offset"], spec["payload_bytes"]
    matches = json.loads((ROOT / "config/menu.matches.json").read_text())
    validate_inline_manifest(matches)
    addresses = configured_addresses(matches)
    linked = functions_from_object(ROOT / "build/ULUS10566/menu.linked.elf")
    functions, identities, ranges = [], set(), []
    for unit in matches["units"]:
        raw = functions_from_object(ROOT / f"build/ULUS10566/menu_{unit['name']}.raw.o")
        for function in unit["functions"]:
            address = int(function["address"], 16)
            end = address+function["size"]
            if function["name"] in identities or function["size"] <= 0 or not base <= address < end <= base+size:
                raise ValueError("Menu C manifest has duplicate identities or invalid ranges")
            identities.add(function["name"])
            ranges.append((address, end))
            expected = reference[offset+address-base:offset+end-base]
            original_object, linked_symbol = raw[function["name"]], linked[function["name"]]
            if resolve_function(original_object, addresses) != expected:
                raise ValueError(f"Menu raw C object does not match: {function['name']}")
            if linked_symbol["address"] != address or linked_symbol["bytes"] != expected:
                raise ValueError(f"Menu linked C address/bytes do not match: {function['name']}")
            functions.append(function)
    ordered = sorted(ranges)
    if any(z > a for (_, z), (a, _) in zip(ordered, ordered[1:])):
        raise ValueError("Menu C functions overlap")
    prepared = json.loads((ROOT / "build/ULUS10566/menu.prepare.json").read_text())
    inventory = json.loads((ROOT / "config/inventory/menu.json").read_text())
    if prepared["reference_sha256"] != spec["sha256"] or prepared["input_layout_sha256"] != layout_digest(inventory):
        raise ValueError("Menu preparation belongs to stale inventory ranges")
    c_bytes = sum(f["size"] for f in functions)
    instruction_ranges = [(int(r["start"], 16), int(r["end"], 16)) for r in inventory["instruction_ranges"]]
    instruction_bytes = sum(z-a for a, z in instruction_ranges)
    if prepared["inventory_instruction_bytes"] != instruction_bytes or prepared["opaque_original_bytes"] != size-instruction_bytes:
        raise ValueError("Menu preparation code/opaque accounting differs from its input layout")
    if prepared["matching_c_bytes"] != c_bytes or prepared["payload_bytes"] != size:
        raise ValueError("Menu preparation C/payload accounting differs")
    accounted = sum(prepared[k] for k in ("matching_c_bytes", "assembly_instruction_bytes", "raw_instruction_fallback_bytes", "opaque_original_bytes"))
    if accounted != size or prepared["raw_instruction_fallback_bytes"] != 4*len(prepared["raw_instruction_fallbacks"]):
        raise ValueError("Menu preparation does not account for the complete payload")
    cursor = base
    fallback_addresses = set()
    for fallback in prepared["raw_instruction_fallbacks"]:
        address = int(fallback["address"], 16)
        if (address % 4 or address in fallback_addresses or
                not any(a <= address < z for a, z in instruction_ranges) or
                any(a <= address < z for a, z in ranges)):
            raise ValueError("Menu raw fallback is not a distinct remaining instruction word")
        fallback_addresses.add(address)
        expected = reference[offset+address-base:offset+address-base+4]
        if int.from_bytes(expected, "little") != int(fallback["word"], 16):
            raise ValueError("Menu raw fallback differs from the original word")
    for part in prepared["parts"]:
        start, end = int(part["start"], 16), int(part["end"], 16)
        if start != cursor or end <= start:
            raise ValueError("Menu preparation regions have a gap or overlap")
        in_code = any(a <= start and end <= z for a, z in instruction_ranges)
        if (part["kind"] in ("inventory-instructions", "matching-c")) != in_code:
            raise ValueError("Menu preparation part differs from instruction layout")
        cursor = end
    if cursor != base+size:
        raise ValueError("Menu preparation leaves payload bytes out")
    expected_c_parts = sorted((int(unit["functions"][0]["address"], 16),
                               int(unit["functions"][0]["address"], 16)+sum(f["size"] for f in unit["functions"]),
                               unit["name"]) for unit in matches["units"])
    actual_c_parts = sorted((int(p["start"], 16), int(p["end"], 16), p.get("unit"))
                            for p in prepared["parts"] if p["kind"] == "matching-c")
    if expected_c_parts != actual_c_parts:
        raise ValueError("Menu matching-C parts differ from manifest unit ranges/identities")
    discarded_inline = verify_inline_units(ROOT, "menu", matches["units"], linked, prepared)
    return {"version": spec["version"], "overlay": "OVL_MENU_APP.ELF", "file_bytes": len(rebuilt),
            "sha256": hashlib.sha256(rebuilt).hexdigest(), "byte_identical": True,
            "reconstructed_payload_bytes": size, "preserved_header_and_metadata_bytes": len(reference)-size,
            "discarded_inline_definitions": discarded_inline,
            "recovered_c_functions": len(functions), "recovered_c_bytes": c_bytes, "c_functions": functions,
            "assembly_instruction_bytes": prepared["assembly_instruction_bytes"],
            "raw_instruction_fallback_bytes": prepared["raw_instruction_fallback_bytes"],
            "opaque_original_bytes": prepared["opaque_original_bytes"],
            "input_layout_sha256": prepared["input_layout_sha256"],
            "method": "Fixed-address matching C plus assembly; original opaque data/raw unsupported instruction words and ELF packaging preserved",
            "runtime_verified": False}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("candidate", nargs="?", type=Path, default=ROOT / "build/ULUS10566/OVL_MENU_APP.ELF")
    args = parser.parse_args()
    report = verify(args.candidate)
    print(f"PASS: menu {report['file_bytes']:,} bytes match retail; {report['recovered_c_functions']} exact C functions")


if __name__ == "__main__":
    main()

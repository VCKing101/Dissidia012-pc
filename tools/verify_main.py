"""Verify the full decrypted main ELF and its separately rebuilt load spans."""

import argparse
import hashlib
import io
import json
from pathlib import Path
from elftools.elf.elffile import ELFFile

from main_build_provenance import layout_digest
from mips_relocations import configured_addresses, resolve_function
from probe_compilers import functions_from_object
from compiler_inline_sections import verify_inline_units, validate_inline_manifest
from inspect_imports import inspect as inspect_imports

ROOT = Path(__file__).resolve().parents[1]


def geometry(reference):
    elf = ELFFile(io.BytesIO(reference))
    segments = [{"index": i, "address": f"0x{s['p_vaddr']:08X}", "file_offset": s["p_offset"],
                 "file_bytes": s["p_filesz"], "memory_bytes": s["p_memsz"]}
                for i, s in enumerate(elf.iter_segments()) if s["p_type"] == "PT_LOAD"]
    for s in segments:
        if s["file_bytes"] <= 0 or s["memory_bytes"] < s["file_bytes"] or s["file_offset"]+s["file_bytes"] > len(reference):
            raise ValueError("Main load geometry has invalid file/memory bounds")
    for key in ("file_offset", "address"):
        ranges = sorted((int(s[key], 16) if key == "address" else s[key], s["file_bytes"]) for s in segments)
        if any(a+size > b for (a, size), (b, _) in zip(ranges, ranges[1:])):
            raise ValueError("Main initialized load spans overlap")
    return segments


def reference_at(reference, segments, address, size):
    for s in segments:
        base = int(s["address"], 16)
        if size > 0 and base <= address and address+size <= base+s["file_bytes"]:
            offset = s["file_offset"]+address-base
            return reference[offset:offset+size]
    raise ValueError("Main address is not wholly initialized/file-backed")


def verify(candidate):
    spec = json.loads((ROOT / "config/main.reference.json").read_text())
    reference = (ROOT / spec["path"]).read_bytes()
    if hashlib.sha256(reference).hexdigest() != spec["sha256"] or len(reference) != spec["file_bytes"]:
        raise ValueError("Main reference identity differs")
    segments = geometry(reference)
    if segments != spec["load_segments"]:
        raise ValueError("Main load geometry differs from its pinned specification")
    rebuilt = Path(candidate).read_bytes()
    if rebuilt != reference:
        first = next((i for i, (a, b) in enumerate(zip(reference, rebuilt)) if a != b), min(len(reference), len(rebuilt)))
        raise ValueError(f"Main ELF mismatch at file offset 0x{first:X}; expected {len(reference)} bytes, got {len(rebuilt)}")
    prepared = json.loads((ROOT / "build/ULUS10566/main.prepare.json").read_text())
    inventory = json.loads((ROOT / "config/inventory/main.json").read_text())
    if prepared["reference_sha256"] != spec["sha256"] or prepared["input_layout_sha256"] != layout_digest(inventory, segments):
        raise ValueError("Main preparation belongs to stale load/instruction layout")
    code = [(int(r["start"], 16), int(r["end"], 16)) for r in inventory["instruction_ranges"]]
    loaded_bytes = sum(s["file_bytes"] for s in segments)
    instruction_bytes = sum(z-a for a, z in code)
    matches = json.loads((ROOT / "config/main.matches.json").read_text())
    validate_inline_manifest(matches)
    addresses = configured_addresses(matches)
    linked = functions_from_object(ROOT / "build/ULUS10566/main.linked.elf")
    elf = ELFFile(io.BytesIO(reference))
    executable = [(s["sh_addr"], s["sh_addr"]+s["sh_size"]) for s in elf.iter_sections()
                  if s["sh_type"] != "SHT_NOBITS" and s["sh_flags"] & 4]
    import_stubs = [(int(f["address"], 16), int(f["address"], 16)+8)
                    for library in inspect_imports(ROOT / spec["path"])["libraries"] for f in library["functions"]]
    functions, ranges, names = [], [], set()
    for unit in matches["units"]:
        raw = functions_from_object(ROOT / f"build/ULUS10566/main_{unit['name']}.raw.o")
        for f in unit["functions"]:
            a, z = int(f["address"], 16), int(f["address"], 16)+f["size"]
            if f["name"] in names or a % 4 or f["size"] <= 0 or f["size"] % 4 or not any(x <= a < z <= y for x, y in executable):
                raise ValueError("Main C identities/ranges are invalid")
            if any(max(a, x) < min(z, y) for x, y in import_stubs):
                raise ValueError("SDK import stubs cannot earn main C source credit")
            names.add(f["name"]); ranges.append((a, z))
            expected = reference_at(reference, segments, a, f["size"])
            if resolve_function(raw[f["name"]], addresses) != expected:
                raise ValueError(f"Main raw C object does not match: {f['name']}")
            if linked[f["name"]]["address"] != a or linked[f["name"]]["bytes"] != expected:
                raise ValueError(f"Main linked C address/bytes do not match: {f['name']}")
            functions.append(f)
    ordered = sorted(ranges)
    if any(z > a for (_, z), (a, _) in zip(ordered, ordered[1:])):
        raise ValueError("Main C ranges overlap")
    c_bytes = sum(f["size"] for f in functions)
    if prepared["matching_c_bytes"] != c_bytes or prepared["reconstructed_load_bytes"] != loaded_bytes or prepared["inventory_instruction_bytes"] != instruction_bytes:
        raise ValueError("Main preparation source/load accounting differs")
    if prepared["opaque_original_bytes"] != loaded_bytes-instruction_bytes or sum(prepared[k] for k in ("matching_c_bytes", "assembly_instruction_bytes", "raw_instruction_fallback_bytes", "opaque_original_bytes")) != loaded_bytes:
        raise ValueError("Main preparation does not account for initialized load bytes")
    if len(prepared["segments"]) != len(segments):
        raise ValueError("Main preparation dropped a load segment")
    all_parts = []
    for expected, actual in zip(segments, prepared["segments"]):
        if any(actual[k] != v for k, v in expected.items()):
            raise ValueError("Main prepared segment geometry differs")
        payload = (ROOT / f"build/ULUS10566/main.segment{expected['index']}.bin").read_bytes()
        if payload != reference[expected["file_offset"]:expected["file_offset"]+expected["file_bytes"]]:
            raise ValueError("Main linked load segment differs from the original")
        cursor = int(expected["address"], 16)
        for part in actual["parts"]:
            a, z = int(part["start"], 16), int(part["end"], 16)
            if a != cursor or z <= a or z > int(expected["address"], 16)+expected["file_bytes"]:
                raise ValueError("Main load parts overlap, have gaps or materialize memory-only bytes")
            if part["kind"] not in ("inventory-instructions", "matching-c", "opaque-original-bytes"):
                raise ValueError("Main preparation uses an unknown part kind")
            is_code = any(x <= a and z <= y for x, y in code)
            if (part["kind"] in ("inventory-instructions", "matching-c")) != is_code:
                raise ValueError("Main part differs from instruction layout")
            cursor = z
        if cursor != int(expected["address"], 16)+expected["file_bytes"]:
            raise ValueError("Main load part coverage is incomplete")
        all_parts.extend(actual["parts"])
    expected_c = sorted((int(u["functions"][0]["address"], 16), int(u["functions"][0]["address"], 16)+sum(f["size"] for f in u["functions"]), u["name"]) for u in matches["units"])
    actual_c = sorted((int(p["start"], 16), int(p["end"], 16), p.get("unit")) for p in all_parts if p["kind"] == "matching-c")
    if actual_c != expected_c:
        raise ValueError("Main matching-C parts differ from manifest units")
    fallback_addresses = set()
    for f in prepared["raw_instruction_fallbacks"]:
        a = int(f["address"], 16)
        if a % 4 or a in fallback_addresses or not any(x <= a < y for x, y in code) or any(x <= a < y for x, y in ranges):
            raise ValueError("Main raw fallback is not a distinct remaining instruction")
        fallback_addresses.add(a)
        if int.from_bytes(reference_at(reference, segments, a, 4), "little") != int(f["word"], 16):
            raise ValueError("Main raw fallback word differs")
    if prepared["raw_instruction_fallback_bytes"] != len(fallback_addresses)*4:
        raise ValueError("Main raw fallback accounting differs")
    discarded_inline = verify_inline_units(ROOT, "main", matches["units"], linked, prepared)
    return {"version": spec["version"], "executable": "EBOOT.ELF", "file_bytes": len(rebuilt),
            "sha256": spec["sha256"], "byte_identical": True, "reconstructed_load_bytes": loaded_bytes,
            "reconstructed_payload_bytes": loaded_bytes, "preserved_nonload_file_bytes": len(reference)-loaded_bytes,
            "memory_only_bytes": sum(s["memory_bytes"]-s["file_bytes"] for s in segments), "load_segments": segments,
            "discarded_inline_definitions": discarded_inline,
            "recovered_c_functions": len(functions), "recovered_c_bytes": c_bytes, "c_functions": functions,
            **{k: prepared[k] for k in ("assembly_instruction_bytes", "raw_instruction_fallback_bytes", "opaque_original_bytes", "input_layout_sha256")},
            "runtime_verified": False, "method": "Fixed-address C/assembly per original initialized PT_LOAD; opaque data/raw words and all nonload file bytes preserved, BSS unmaterialized"}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("candidate", nargs="?", type=Path, default=ROOT / "build/ULUS10566/EBOOT.ELF")
    report = verify(parser.parse_args().candidate)
    print(f"PASS: main {report['file_bytes']:,} bytes match; {report['recovered_c_functions']} C functions / {report['recovered_c_bytes']} bytes")


if __name__ == "__main__":
    main()

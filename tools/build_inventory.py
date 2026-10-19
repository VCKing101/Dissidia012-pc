"""Validate metadata-only Ghidra inventories against pinned ELF section ranges.

All automatic classifications remain provisional. Unknown regions are retained.
"""

import hashlib
import json
import struct
import re
from pathlib import Path

from elftools.elf.elffile import ELFFile

from inspect_imports import inspect
from verify_extra import verify
from build_provenance import require_current_build
from audit_layout import validate_extra_layout, validate_overlay_hooks
from audit_menu_data import validate_menu_data, elf_word_reader
from audit_overlay_dispatch import FileImage, inspect_dispatch, validate_dispatch_entries, validate_shutdown_bodies, trace_dispatch_calls
from audit_deferred_battle import inspect_deferred_battle
from audit_menu_member_table import inspect_empty_callback
from audit_presentation_callbacks import collect_callbacks
from verify_battle import verify as verify_battle
from battle_build_provenance import require_current_battle_build
from verify_menu import verify as verify_menu
from menu_build_provenance import require_current_menu_build
from verify_main import verify as verify_main
from main_build_provenance import require_current_main_build

ROOT = Path(__file__).resolve().parents[1]


def merged(ranges):
    result = []
    for start, end in sorted(ranges):
        if not 0 <= start < end <= 0x100000000:
            raise ValueError("Invalid PSP address range")
        if result and start <= result[-1][1]:
            result[-1] = (result[-1][0], max(result[-1][1], end))
        else:
            result.append((start, end))
    return result


def intersection(left, right):
    left, right = merged(left), merged(right)
    result = []
    i = j = 0
    while i < len(left) and j < len(right):
        start, end = max(left[i][0], right[j][0]), min(left[i][1], right[j][1])
        if start < end:
            result.append((start, end))
        if left[i][1] < right[j][1]:
            i += 1
        else:
            j += 1
    return merged(result)


def difference(whole, removed):
    removed = merged(removed)
    result = []
    for start, end in merged(whole):
        cursor = start
        for cut_start, cut_end in removed:
            if cut_end <= cursor:
                continue
            if cut_start >= end:
                break
            if cursor < cut_start:
                result.append((cursor, min(cut_start, end)))
            cursor = max(cursor, cut_end)
            if cursor >= end:
                break
        if cursor < end:
            result.append((cursor, end))
    return result


def byte_count(ranges):
    return sum(end - start for start, end in merged(ranges))


def decode(ranges):
    return merged((int(r["start"], 16), int(r["end"], 16)) for r in ranges)


def encode(ranges):
    return [{"start": f"0x{start:08X}", "end": f"0x{end:08X}"} for start, end in merged(ranges)]


def build_module(specification, matched_functions, override=None, main_starts=None, main_reference=None):
    module = specification["id"]
    reference = ROOT / specification["path"]
    digest = hashlib.sha256(reference.read_bytes()).hexdigest()
    if digest != specification["sha256"]:
        raise ValueError(f"Reference hash mismatch: {module}")
    raw_path = ROOT / f"analysis/ULUS10566/{module}/inventory.json"
    raw = json.loads(raw_path.read_text())
    override = override or {}
    if override and override.get("reference_sha256") != digest:
        raise ValueError("Inventory override belongs to another reference")
    if raw["module"] != module or raw["executable_sha256"].lower() != digest:
        raise ValueError(f"Ghidra project differs from the pinned reference: {module}")
    if raw.get("analysis_timed_out") is True:
        raise ValueError(f"Analysis timed out for {module}; inventory is incomplete")
    sections = []
    section_data = []
    with reference.open("rb") as stream:
        elf = ELFFile(stream)
        for index, section in enumerate(elf.iter_sections()):
            if section["sh_type"] == "SHT_NOBITS" or not section["sh_flags"] & 2 or not section["sh_size"]:
                continue
            start = section["sh_addr"]
            sections.append({"index": index, "name": section.name or f"section_{index}",
                             "start": f"0x{start:08X}", "end": f"0x{start + section['sh_size']:08X}",
                             "file_offset": section["sh_offset"], "bytes": section["sh_size"],
                             "executable": bool(section["sh_flags"] & 4)})
            section_data.append((start, section.data()))
    executable = decode([s for s in sections if s["executable"]])
    backed = decode(sections)
    instructions = intersection(decode(raw["instruction_ranges"]), executable)
    data = intersection(decode(raw["defined_data_ranges"]), executable)
    if intersection(instructions, data):
        raise ValueError(f"Instruction and data classifications overlap: {module}")
    import_libraries = {}
    import_specs = {}
    if module == "main":
        for library in inspect(reference)["libraries"]:
            for function in library["functions"]:
                import_libraries[int(function["address"], 16)] = library["library"]
                import_specs[int(function["address"], 16)] = function
    functions, quarantined, all_ranges = [], [], []
    ids = set()
    for original in raw["functions"]:
        function = dict(original)
        address = int(function["address"], 16)
        expected_id = f"{module}:0x{address:08X}"
        if function["id"] != expected_id or expected_id in ids:
            raise ValueError(f"Duplicate or incorrectly qualified function identity: {expected_id}")
        ids.add(expected_id)
        ranges = decode(function["ranges"])
        if byte_count(ranges) != function["body_bytes"] or difference(ranges, backed):
            raise ValueError(f"Function body is not fully backed by the reference: {expected_id}")
        if not any(start <= address < end for start, end in ranges):
            raise ValueError(f"Function entry is outside its body: {expected_id}")
        code_ranges = intersection(ranges, executable)
        if byte_count(code_ranges) != byte_count(ranges):
            function["quarantine_reason"] = "body outside executable-flagged sections"
            quarantined.append(function)
            continue
        function["primary_owner"] = "unclassified"
        function["matched_c"] = False
        if address in import_libraries:
            function["primary_owner"] = "platform_imports"
            function["import_library"] = import_libraries[address]
        if address in matched_functions:
            match = matched_functions[address]
            if ranges != [(address, address + match["size"])]:
                raise ValueError(f"Matched C differs from Ghidra's function ranges: {expected_id}")
            function["matched_c"] = True
            function["primary_owner"] = match.get("primary_owner", "unclassified")
            function["confidence"] = "exact-source-match-boundary"
        functions.append(function)
        all_ranges.extend(ranges)
    if raw["function_count"] != len(ids):
        raise ValueError(f"Ghidra function count is inconsistent: {module}")
    declared_imports_added = 0
    starts = {int(f["address"], 16) for f in functions}
    for address, specification_import in sorted(import_specs.items()):
        if address in starts:
            continue
        ranges = [(address, address + 8)]
        if difference(ranges, executable) or intersection(ranges, all_ranges + data):
            raise ValueError("Missing import entry conflicts with existing classification")
        stub = None
        for start, contents in section_data:
            if start <= address and address + 8 <= start + len(contents):
                stub = contents[address - start:address - start + 8]
                break
        # Standard unpatched PSP import stub: jr $ra; nop. Keep this explicit
        # guard; do not infer arbitrary extra functions from byte patterns.
        if stub != bytes.fromhex("0800e00300000000"):
            raise ValueError("ELF-declared import has an unexpected stub encoding")
        functions.append({"id": f"{module}:0x{address:08X}",
                          "name": f"import_{specification_import['nid']}", "address": f"0x{address:08X}",
                          "body_bytes": 8, "ranges": encode(ranges), "thunk": True,
                          "confidence": "elf-import-declaration-and-stub-bytes", "primary_owner": "platform_imports",
                          "import_library": import_libraries[address], "matched_c": False,
                          "direct_call_targets": [], "unresolved_call_sites": 0})
        all_ranges.extend(ranges)
        instructions = merged(instructions + ranges)
        declared_imports_added += 1
    reviewed_functions_added = 0
    for review in override.get("additional_functions", []):
        address, size = int(review["address"], 16), review["size"]
        ranges = [(address, address + size)]
        if size < 8 or size % 4 or difference(ranges, executable) or intersection(ranges, all_ranges + data):
            raise ValueError("Reviewed function conflicts with reference bounds/classification")
        contents = next(contents[address-start:address-start+size] for start, contents in section_data
                        if start <= address and address + size <= start + len(contents))
        if struct.unpack_from("<I", contents, size - 8)[0] != 0x03E00008:
            raise ValueError("Reviewed leaf function no longer ends in jr-ra and a delay slot")
        entry_review = None
        if review.get("entry_audit") == "menu-member-table-empty-callback":
            if module != "menu":
                raise ValueError("Menu member callback audit applied to another module")
            member_review = json.loads((ROOT / "config/menu-member-table-review.json").read_text())
            if member_review["reference_sha256"] != digest:
                raise ValueError("Menu member-table review belongs to another reference")
            entry_review = inspect_empty_callback(FileImage(reference, digest), functions, member_review)
            if int(entry_review["entry"], 16) != address or entry_review["bytes"] != size:
                raise ValueError("Reviewed member callback differs from additional function")
        elif review.get("entry_audit") == "presentation-callback":
            audited = next((row for row in collect_callbacks() if row["module"] == module), None)
            if audited is None or int(audited["address"], 16) != address or audited["size"] != size:
                raise ValueError("Reviewed presentation callback differs from additional function")
            entry_review = audited
        elif review.get("entry_audit"):
            raise ValueError("Unknown reviewed function entry audit")
        match = matched_functions.get(address)
        if match and match["size"] != size:
            raise ValueError("Matched C differs from reviewed leaf function extent")
        functions.append({"id": f"{module}:0x{address:08X}", "name": review["name"],
                          "address": f"0x{address:08X}", "body_bytes": size, "ranges": encode(ranges),
                          "thunk": False, "confidence": "exact-source-match-boundary" if match else "reviewed-leaf-boundary",
                          "primary_owner": match.get("primary_owner", "unclassified") if match else "unclassified", "matched_c": bool(match),
                          "direct_call_targets": [], "unresolved_call_sites": 0,
                          "review_evidence": review["evidence"],
                          **({"member_entry_review": entry_review} if entry_review else {})})
        all_ranges.extend(ranges)
        instructions = merged(instructions + ranges)
        reviewed_functions_added += 1
    non_functions = decode(override.get("non_function_ranges", []))
    if difference(non_functions, executable) or intersection(non_functions, all_ranges):
        raise ValueError("Non-function review overlaps a function or leaves executable sections")
    # Keep these bytes unknown: not being a function does not prove their role
    # as a data field, alignment padding or a loader instruction sequence.
    instructions = difference(instructions, non_functions)
    functions.sort(key=lambda f: int(f["address"], 16))
    present_matches = {int(f["address"], 16) for f in functions if f["matched_c"]}
    if present_matches != set(matched_functions):
        raise ValueError(f"Matched C is missing from the executable inventory: {module}")
    union = merged(all_ranges)
    hook_audit = None
    if module in ("menu", "battle"):
        if len(executable) != 1:
            raise ValueError("Expected one overlay payload")
        base, end = executable[0]
        contents = next(contents[base-start:end-start] for start, contents in section_data
                        if start <= base and end <= start + len(contents))
        hook_audit = validate_overlay_hooks(contents, base, module,
                                           {int(f["address"], 16) for f in functions}, main_starts or set())
        hook_data = decode(hook_audit["data_regions"])
        if intersection(hook_data, instructions + union):
            raise ValueError("Loader hook data intersects decoded instructions/functions")
        data = merged(data + hook_data)
        hook_entries = set(hook_audit["initializers"]) | {r["callback"] for r in hook_audit["cleanup"]}
        for function in functions:
            if function["id"] in hook_entries:
                function["entry_review"] = "loader descriptor/table reference"
    padding = []
    data_island_audit = None
    partial_review = override.get("data_island_audit")
    if partial_review:
        if module != "menu" or not main_reference:
            raise ValueError("Partial menu review needs the pinned main reference")
        base, end = executable[0]
        contents = next(contents[base-start:end-start] for start, contents in section_data
                        if start <= base and end <= start + len(contents))
        read_main_word = elf_word_reader(ROOT / main_reference["path"], main_reference["sha256"])
        reviewed = validate_menu_data(contents, base, partial_review, read_main_word, main_starts,
                                      {int(f["address"], 16) for f in functions})
        reviewed_ranges = reviewed["data"] + reviewed["padding"]
        if intersection(reviewed_ranges, instructions + union):
            raise ValueError("Reviewed menu data/padding intersects code")
        if intersection(reviewed["padding"], data):
            raise ValueError("Reviewed menu padding intersects existing defined data")
        data = merged(data + reviewed["data"])
        padding = merged(reviewed["padding"])
        data_island_audit = {k: v for k, v in reviewed.items() if k not in ("data", "padding")}
    audit = None
    layout = override.get("layout_audit")
    if layout:
        if module != "extra" or len(executable) != 1:
            raise ValueError("This reviewed layout validator currently applies only to extra")
        base, end = executable[0]
        contents = next(contents[base-start:end-start] for start, contents in section_data
                        if start <= base and end <= start + len(contents))
        assembly = (ROOT / "asm/ULUS10566/extra/ovl_extra_app.s").read_text()
        disassembly_sizes = {int(name.split("_")[1], 16): int(size, 16)
                             for name, size in re.findall(r"nonmatching (func_[0-9A-Fa-f]+), 0x([0-9A-Fa-f]+)", assembly)}
        for function in functions:
            address = int(function["address"], 16)
            expected = [(address, address + function["body_bytes"])]
            if disassembly_sizes.get(address) != function["body_bytes"] or decode(function["ranges"]) != expected:
                raise ValueError("Extra boundary audit disagrees with independent disassembly")
            function["boundary_review"] = "Ghidra/reviewed leaf and spimdisasm agreement"
        audit = validate_extra_layout(contents, base, union, {int(f["address"], 16) for f in functions}, layout)
        instructions, data, padding = audit["code"], audit["data"], audit["padding"]
        audit = {"scope": layout["scope"], "descriptor": audit["descriptor"],
                 "validated_data_regions": audit["validated_data_regions"],
                 "independent_disassembly_sha256": hashlib.sha256(assembly.encode()).hexdigest()}
    unknown = difference(executable, instructions + data + padding)
    overlap_bytes = sum(f["body_bytes"] for f in functions) - byte_count(union)
    return {"schema_version": 1, "module": module, "reference_sha256": digest,
            "inventory_source_sha256": hashlib.sha256(raw_path.read_bytes()).hexdigest(),
            "analysis_project": raw.get("analysis_project"),
            "language": raw["language"], "audited": audit is not None, "ranges_end_exclusive": True,
            "classification": "reviewed-boundaries-and-layout" if audit else "automatic-provisional", "sections": sections,
            "layout_audit": audit,
            "hook_audit": hook_audit,
            "data_island_audit": data_island_audit,
            "ghidra_function_count": raw["function_count"], "elf_declared_imports_added": declared_imports_added,
            "reviewed_functions_added": reviewed_functions_added,
            "non_function_reviews": override.get("non_function_ranges", []),
            "function_count": len(functions), "functions": functions,
            "quarantined_functions": quarantined,
            "instruction_ranges": encode(instructions), "defined_data_ranges": encode(data),
            "padding_or_zero_fill_ranges": encode(padding),
            "unclassified_executable_ranges": encode(unknown), "function_body_ranges": encode(union),
            "summary": {"executable_section_bytes": byte_count(executable),
                        "decoded_instruction_bytes": byte_count(instructions),
                        "defined_data_bytes_in_executable_sections": byte_count(data),
                        "padding_or_zero_fill_bytes": byte_count(padding),
                        "unclassified_executable_bytes": byte_count(unknown),
                        "function_body_bytes_union": byte_count(union),
                        "overlapping_function_body_bytes": overlap_bytes,
                        "function_body_bytes_not_decoded": byte_count(difference(union, instructions)),
                        "decoded_instruction_bytes_outside_functions": byte_count(difference(instructions, union)),
                        "platform_import_functions": sum(f["primary_owner"] == "platform_imports" for f in functions),
                        "matched_c_functions": len(present_matches)}}


def main():
    scope = json.loads((ROOT / "config/progress.json").read_text())
    saved = json.loads((ROOT / "build/ULUS10566/extra.verify.json").read_text())
    require_current_build(saved.get("build_inputs"))
    verified = verify(ROOT / "build/ULUS10566/OVL_EXTRA_APP.ELF")
    matched = {int(f["address"], 16): {**f, "primary_owner": "data_install"} for f in verified["c_functions"]}
    battle_matches = {}
    menu_matches = {}
    main_matches = {}
    battle_proof = ROOT / "build/ULUS10566/battle.verify.json"
    if battle_proof.exists():
        proof = json.loads(battle_proof.read_text())
        require_current_battle_build(proof.get("build_inputs"))
        verified_battle = verify_battle(ROOT / "build/ULUS10566/OVL_BATTLE_APP.ELF")
        battle_matches = {int(f["address"], 16): {**f, "primary_owner": "battle_helpers"} for f in verified_battle["c_functions"]}
    menu_proof = ROOT / "build/ULUS10566/menu.verify.json"
    if menu_proof.exists():
        proof = json.loads(menu_proof.read_text())
        require_current_menu_build(proof.get("build_inputs"))
        verified_menu = verify_menu(ROOT / "build/ULUS10566/OVL_MENU_APP.ELF")
        menu_matches = {int(f["address"], 16): {**f, "primary_owner": "menu_helpers"} for f in verified_menu["c_functions"]}
    main_proof = ROOT / "build/ULUS10566/main.verify.json"
    if main_proof.exists():
        proof = json.loads(main_proof.read_text())
        require_current_main_build(proof.get("build_inputs"))
        verified_main = verify_main(ROOT / "build/ULUS10566/EBOOT.ELF")
        main_matches = {int(f["address"], 16): {**f, "primary_owner": "main_helpers"} for f in verified_main["c_functions"]}
    output = ROOT / "config/inventory"
    output.mkdir(parents=True, exist_ok=True)
    reports = []
    overrides = json.loads((ROOT / "config/inventory-overrides.json").read_text())
    for specification in scope["modules"]:
        main_starts = {int(f["address"], 16) for r in reports if r["module"] == "main" for f in r["functions"]}
        module_matches = {"extra": matched, "battle": battle_matches, "menu": menu_matches, "main": main_matches}.get(specification["id"], {})
        report = build_module(specification, module_matches,
                              overrides["modules"].get(specification["id"]), main_starts,
                              next(m for m in scope["modules"] if m["id"] == "main"))
        reports.append(report)
    manifest = json.loads((ROOT / "config/overlay-dispatch.json").read_text())
    main_spec = next(m for m in scope["modules"] if m["id"] == "main")
    main_report = next(r for r in reports if r["module"] == "main")
    image = FileImage(ROOT / main_spec["path"], main_spec["sha256"])
    dispatch = inspect_dispatch(image, main_report["functions"], manifest["review"])
    deferred_review = json.loads((ROOT / "config/deferred-battle.json").read_text())
    if deferred_review["main_reference_sha256"] != main_spec["sha256"]:
        raise ValueError("Deferred review belongs to another main reference")
    deferred = inspect_deferred_battle(image, main_report["functions"], deferred_review, manifest["review"]["slots"])
    dispatch["roots"]["battle"] = sorted(dispatch["roots"]["battle"]+deferred["roots"],
                                        key=lambda r: (int(r["address"], 16), r.get("call_pc", "")))
    main_report["deferred_battle_audit"] = deferred
    for report in reports:
        module = report["module"]
        seeds = manifest["modules"][module]
        if seeds["reference_sha256"] != report["reference_sha256"] or seeds["entries"] != dispatch["roots"][module]:
            raise ValueError("Dispatch seed manifest differs from pinned references/calls; refresh it before seeding")
        if module != "main":
            slot = next(s for s in manifest["review"]["slots"] if s["module"] == module)
            if decode([s for s in report["sections"] if s["executable"]]) != [(int(slot["payload_start"], 16), int(slot["payload_end"], 16))]:
                raise ValueError("Dispatch module bounds differ from executable payload")
    dispatch = validate_dispatch_entries(dispatch, reports)
    dispatch["shutdown_bodies"] = validate_shutdown_bodies(image, main_report["functions"], manifest["review"])
    dispatch["direct_call_reachability"] = {}
    main_report["overlay_dispatch_audit"] = dispatch
    for report in reports:
        spec = next(s for s in scope["modules"] if s["id"] == report["module"])
        module_image = FileImage(ROOT / spec["path"], spec["sha256"])
        reached = trace_dispatch_calls(module_image, report["functions"], dispatch["roots"][report["module"]], report["module"])
        dispatch["direct_call_reachability"][report["module"]] = reached
        reached_by_id = {row["id"]: row for row in reached}
        roots = {}
        for row in dispatch["roots"][report["module"]]:
            roots.setdefault(row["id"], []).append({k: v for k, v in row.items() if k not in ("address", "id", "body_bytes")})
        for function in report["functions"]:
            if function["id"] in roots:
                function["dispatch_entry_review"] = roots[function["id"]]
            if function["id"] in reached_by_id:
                function["dispatch_reachability"] = reached_by_id[function["id"]]
    # Validate every module before replacing any maintained metadata.
    for report in reports:
        (output / f"{report['module']}.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
        s = report["summary"]
        print(f"{report['module']}: {report['function_count']} candidates; {s['function_body_bytes_union']:,} function bytes; "
              f"{s['unclassified_executable_bytes']:,} unknown executable-section bytes; "
              f"{s['overlapping_function_body_bytes']:,} overlapping body bytes")


if __name__ == "__main__":
    main()

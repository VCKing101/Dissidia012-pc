"""Generate verified local progress without treating assembly or assets as C."""

import hashlib
import json
from datetime import datetime, timezone
from pathlib import Path

from elftools.elf.elffile import ELFFile

from build_provenance import require_current_build
from verify_extra import verify
from verify_battle import verify as verify_battle
from battle_build_provenance import require_current_battle_build
from verify_menu import verify as verify_menu
from menu_build_provenance import require_current_menu_build
from verify_main import verify as verify_main
from main_build_provenance import require_current_main_build
from menu_controller_scope import collect_controller_scope, render_controller_scope
from audit_savedata_setup import collect_setup

ROOT = Path(__file__).resolve().parents[1]
RANKS = ["Unmapped", "Mapped in part", "Behavior explained in part",
         "Feature source recovered", "Feature integrated", "Runtime scenarios verified"]


def percent(numerator, denominator):
    return round(100 * numerator / denominator, 4) if denominator else None


def collect():
    scope = json.loads((ROOT / "config/progress.json").read_text())
    saved = json.loads((ROOT / "build/ULUS10566/extra.verify.json").read_text())
    require_current_build(saved.get("build_inputs"))
    verified = verify(ROOT / "build/ULUS10566/OVL_EXTRA_APP.ELF")
    verified_modules = {"extra": verified}
    manifests = {"extra": json.loads((ROOT / "config/extra.matches.json").read_text())}
    battle_proof = ROOT / "build/ULUS10566/battle.verify.json"
    if battle_proof.exists():
        proof = json.loads(battle_proof.read_text())
        require_current_battle_build(proof.get("build_inputs"))
        verified_modules["battle"] = verify_battle(ROOT / "build/ULUS10566/OVL_BATTLE_APP.ELF")
        manifests["battle"] = json.loads((ROOT / "config/battle.matches.json").read_text())
    menu_proof = ROOT / "build/ULUS10566/menu.verify.json"
    if menu_proof.exists():
        proof = json.loads(menu_proof.read_text())
        require_current_menu_build(proof.get("build_inputs"))
        verified_modules["menu"] = verify_menu(ROOT / "build/ULUS10566/OVL_MENU_APP.ELF")
        manifests["menu"] = json.loads((ROOT / "config/menu.matches.json").read_text())
    main_proof = ROOT / "build/ULUS10566/main.verify.json"
    if main_proof.exists():
        proof = json.loads(main_proof.read_text())
        require_current_main_build(proof.get("build_inputs"))
        verified_modules["main"] = verify_main(ROOT / "build/ULUS10566/EBOOT.ELF")
        manifests["main"] = json.loads((ROOT / "config/main.matches.json").read_text())
    feature_ids = {feature["id"] for feature in scope["features"]}
    if len(feature_ids) != len(scope["features"]) or len({m["id"] for m in scope["modules"]}) != len(scope["modules"]):
        raise ValueError("Progress scope contains duplicate feature or module identities")
    totals = {key: {"matched_functions": 0, "matched_c_bytes": 0} for key in feature_ids}
    for unit in [u for manifest in manifests.values() for u in manifest["units"]]:
        feature_id = scope["matched_unit_features"].get(unit["name"])
        if feature_id not in feature_ids:
            raise ValueError(f"C unit needs a valid feature assignment: {unit['name']}")
        totals[feature_id]["matched_functions"] += len(unit["functions"])
        totals[feature_id]["matched_c_bytes"] += sum(f["size"] for f in unit["functions"])
    modules = []
    inventoried_ids = set()
    for specification in scope["modules"]:
        path = ROOT / specification["path"]
        if hashlib.sha256(path.read_bytes()).hexdigest() != specification["sha256"]:
            raise ValueError(f"Reference hash mismatch: {specification['id']}")
        with path.open("rb") as stream:
            elf = ELFFile(stream)
            flagged_bytes = sum(section["sh_size"] for section in elf.iter_sections()
                                if section["sh_type"] != "SHT_NOBITS" and section["sh_flags"] & 4)
        is_extra = specification["id"] == "extra"
        module_proof = verified_modules.get(specification["id"])
        row = {"id": specification["id"], "name": specification["name"],
               "reference_sha256": specification["sha256"], "map_status": specification["map_status"],
               "matched_c_functions": module_proof["recovered_c_functions"] if module_proof else 0,
               "matched_c_bytes": module_proof["recovered_c_bytes"] if module_proof else 0,
               "executable_section_bytes": flagged_bytes,
               "audited_code_completion_percent": None,
               "module_rebuild_verified": module_proof["byte_identical"] if module_proof else False}
        if module_proof:
            row["payload_c_coverage_percent"] = percent(row["matched_c_bytes"], module_proof["reconstructed_payload_bytes"])
        if specification["id"] in ("battle", "menu", "main") and module_proof:
            row["reconstruction_breakdown"] = {k: module_proof[k] for k in ("assembly_instruction_bytes", "raw_instruction_fallback_bytes", "opaque_original_bytes")}
        if specification.get("function_inventory"):
            inventory_path = ROOT / specification["function_inventory"]
            inventory = json.loads(inventory_path.read_text())
            if inventory.get("module") != specification["id"] or inventory.get("reference_sha256") != specification["sha256"]:
                raise ValueError("Inventory identity/reference does not match its scoped module")
            functions = inventory["functions"]
            inventoried_ids.update(f["id"] for f in functions)
            by_address = {int(f["address"], 16): f for f in functions}
            if len(by_address) != len(functions) or inventory["function_count"] != len(functions):
                raise ValueError("Invalid provisional function inventory")
            for function in (module_proof["c_functions"] if module_proof else []):
                candidate = by_address.get(int(function["address"], 16))
                if candidate is None or candidate["body_bytes"] != function["size"]:
                    raise ValueError("Provisional inventory differs from a matched function boundary; audit it")
            total_bytes = inventory["summary"]["function_body_bytes_union"]
            row.update({"provisional_functions": len(functions), "provisional_function_body_bytes": total_bytes,
                        "provisional_function_count_percent": percent(row["matched_c_functions"], len(functions)),
                        "provisional_function_byte_percent": percent(row["matched_c_bytes"], total_bytes),
                        "inventory_sha256": hashlib.sha256(inventory_path.read_bytes()).hexdigest(),
                        "inventory_audited": inventory["audited"], "classification_summary": inventory["summary"]})
            if inventory["audited"]:
                row["audited_code_completion_percent"] = percent(row["matched_c_bytes"], total_bytes)
                row["audit_scope"] = inventory["layout_audit"]["scope"]
        modules.append(row)
    total_sections = sum(row["executable_section_bytes"] for row in modules)
    total_matched = sum(row["matched_c_bytes"] for row in modules)
    all_inventoried = all("provisional_function_body_bytes" in row for row in modules)
    provisional_bytes = sum(row.get("provisional_function_body_bytes", 0) for row in modules)
    provisional_functions = sum(row.get("provisional_functions", 0) for row in modules)
    features = []
    for specification in scope["features"]:
        rank = specification["evidence_rank"]
        if any(entry not in inventoried_ids for entry in specification.get("evidence_functions", [])):
            raise ValueError("Feature evidence names a function outside the maintained maps")
        if not 0 <= rank < len(RANKS):
            raise ValueError("Invalid evidence rank")
        if rank >= 3 and not specification["scope_complete"]:
            raise ValueError("Feature recovery/integration ranks require a complete ownership scope")
        features.append({**specification, **totals[specification["id"]],
                         "rank_label": RANKS[rank], "completion_percent": None})
    return {"schema_version": 1, "scope_version": scope["scope_version"], "version": scope["version"],
            "inventory_revision": scope.get("inventory_revision", 0),
            "generated_at_utc": datetime.now(timezone.utc).isoformat(),
            "whole_game_audited_code_percent": None,
            "matched_c_functions": sum(row["matched_c_functions"] for row in modules), "matched_c_bytes": total_matched,
            "executable_section_bytes": total_sections,
            "mixed_section_coverage_proxy_percent": percent(total_matched, total_sections),
            "provisional_total_functions": provisional_functions if all_inventoried else None,
            "provisional_function_body_bytes": provisional_bytes if all_inventoried else None,
            "provisional_function_byte_coverage_percent": percent(total_matched, provisional_bytes) if all_inventoried else None,
            "proxy_note": "Includes data embedded in executable overlay sections and import stubs; not audited game-code completion.",
            "source_languages": sorted({"C++" if Path(u["source"]).suffix == ".cpp" else "C" for m in manifests.values() for u in m["units"]}),
            "metric_note": "Legacy matched_c keys count exact original C/C++ function bodies. Discarded compiler inline copies, assembly, SDK stubs and opaque bytes receive no matching-source credit.",
            "local_source_scopes": [collect_controller_scope(verified_modules["menu"])] if "menu" in verified_modules else [],
            "static_savedata_setup": collect_setup() if "menu" in verified_modules and "main" in verified_modules else None,
            "modules": modules, "features": sorted(features, key=lambda f: -f["evidence_rank"])}


def render(report):
    lines = ["# Current project progress", "", "Generated by `tools/progress.py` from a verified build and pinned references.", "",
             f"Target: **{report['version']}**. Scope version: {report['scope_version']}.", "",
             f"**{report['matched_c_functions']} exact C/C++ functions, {report['matched_c_bytes']:,} code bytes.**",
             "Whole-game audited code completion: **not yet established**.", "",
             "| Module | Functions | Matched C/C++ functions | Matched C/C++ bytes | Code match | Map | Full module rebuild |",
             "| --- | ---: | ---: | ---: | ---: | --- | --- |"]
    for module in report["modules"]:
        count = module.get("provisional_functions")
        count_label = f"{count:,}" if count is not None else "Unknown"
        score = module["audited_code_completion_percent"]
        score_label = f"{score:.2f}%" if score is not None else "Pending audit"
        lines.append(f"| {module['name']} | {count_label} | {module['matched_c_functions']} | {module['matched_c_bytes']:,} | {score_label} | {module['map_status']} | {'Verified' if module['module_rebuild_verified'] else 'Not built'} |")
    if report["provisional_total_functions"] is not None:
        lines.extend(["", f"Inventory revision **{report['inventory_revision']}** records **{report['provisional_total_functions']:,} module-qualified candidate functions**, covering **{report['provisional_function_body_bytes']:,} function-body bytes**.",
                      f"C/C++ / provisional function-body bytes across the four modules: **{report['provisional_function_byte_coverage_percent']:.4f}%**. This is not the audited whole-game headline.", "",
                      "| Module | Instruction bytes | Data bytes | Padding/zero-fill bytes | Unknown bytes |",
                      "| --- | ---: | ---: | ---: | ---: |"])
        for module in report["modules"]:
            summary = module["classification_summary"]
            lines.append(f"| {module['name']} | {summary['decoded_instruction_bytes']:,} | {summary['defined_data_bytes_in_executable_sections']:,} | {summary.get('padding_or_zero_fill_bytes', 0):,} | {summary['unclassified_executable_bytes']:,} |")
        lines.extend(["", "The extra overlay's boundaries/layout are reviewed; the other three maps remain provisional. Overlay hooks/dispatch and selected menu data regions have partial audits. Unknown bytes stay in the scope. See [inventory findings](inventory-findings.md), [extra layout audit](extra-layout-audit.md), [overlay hook audit](overlay-hook-audit.md), [menu data review](menu-data-audit.md) and [overlay dispatch audit](overlay-dispatch-audit.md)."])
    for local in report.get("local_source_scopes", []):
        lines.extend(["", "## Reviewed menu-controller scope", "",
                      f"The twenty-entry member table has **{local['matching_handlers']} / {local['handler_count']} source-matching handlers ({local['handler_count_percent']:.2f}% by count)** and **{local['matching_handler_bytes']:,} / {local['reviewed_handler_bytes']:,} bytes ({local['handler_byte_percent']:.2f}% by code bytes)**.",
                      "This covers one controller's callbacks. Its dispatcher matches separately; called services, other menu controllers and gameplay modes are outside this denominator. The whole menu/game percentages remain unknown.",
                      "The report repeats direct target, contiguous extent, end/neighbor, bounded branch and overlap checks. See [controller coverage](menu-controller-coverage.md) for every handler and remaining work."])
    extra = next(module for module in report["modules"] if module["id"] == "extra")
    battle = next(module for module in report["modules"] if module["id"] == "battle")
    menu = next(module for module in report["modules"] if module["id"] == "menu")
    main = next(module for module in report["modules"] if module["id"] == "main")
    if main["module_rebuild_verified"]:
        breakdown = main["reconstruction_breakdown"]
        lines.extend(["", "## Complete scoped build foundation", "",
                      f"**All four scoped files are byte-identical to their pinned references.** Main contributes {main['matched_c_functions']} C/C++ functions / {main['matched_c_bytes']} bytes.",
                      f"Main's initialized load spans use {breakdown['assembly_instruction_bytes']:,} reassembled instruction bytes, {breakdown['raw_instruction_fallback_bytes']:,} raw fallback bytes and {breakdown['opaque_original_bytes']:,} preserved bytes. Memory-only BSS is not emitted.",
                      "All four builds remain exact after the [source expansion](source-recovery-milestone.md). Most source recovery and full layout audits remain unfinished. Earlier [post-tutorial runtime checks](hub-runtime-validation.md) cover selected scenarios; this source batch used static verification without rerunning the emulator."])
    if menu["module_rebuild_verified"]:
        breakdown = menu["reconstruction_breakdown"]
        lines.extend(["", "## Menu-overlay build", "",
                      f"**Byte-identical full file**, including **{menu['matched_c_functions']} C/C++ functions / {menu['matched_c_bytes']} bytes**.",
                      f"The remaining payload uses {breakdown['assembly_instruction_bytes']:,} reassembled instruction bytes, {breakdown['raw_instruction_fallback_bytes']} explicit raw fallback bytes and {breakdown['opaque_original_bytes']:,} preserved original bytes.",
                      "Setup/teardown and all descriptor initializers now have exact C/C++. Full layout and menu-flow ownership remain incomplete. See [source expansion](source-recovery-milestone.md) and the earlier [menu build milestone](menu-build-milestone.md)."])
    if battle["module_rebuild_verified"]:
        breakdown = battle["reconstruction_breakdown"]
        lines.extend(["", "## Battle-overlay build", "",
                      f"**Byte-identical full file**, including **{battle['matched_c_functions']} C/C++ functions / {battle['matched_c_bytes']} bytes**.",
                      f"The remaining payload uses {breakdown['assembly_instruction_bytes']:,} reassembled instruction bytes, {breakdown['raw_instruction_fallback_bytes']} explicit raw fallback bytes and {breakdown['opaque_original_bytes']:,} preserved original bytes.",
                      "Setup/update/cleanup dispatch and selected state transitions now have exact C/C++. Battle boundaries and gameplay ownership remain provisional. See [source expansion](source-recovery-milestone.md) and the earlier [battle build milestone](battle-build-milestone.md)."])
    lines.extend(["", "## Extra-overlay measures", "",
                  f"- C/C++ / entire payload: **{extra['payload_c_coverage_percent']:.2f}%** (includes data in the denominator).",
                  f"- C/C++ / reviewed code bytes: **{extra['provisional_function_byte_percent']:.2f}%** ({extra['matched_c_bytes']:,} / {extra['provisional_function_body_bytes']:,}).",
                  f"- Matched / reviewed function count: **{extra['matched_c_functions']} / {extra['provisional_functions']} = {extra['provisional_function_count_percent']:.2f}%**.",
                  "- Rebuilt file: **byte-identical**, including remaining assembly/data. This is build fidelity, not 100% C recovery.", "",
                  f"Across all four scoped modules, C/C++ / executable-flagged section bytes is **{report['mixed_section_coverage_proxy_percent']:.4f}%** ({report['matched_c_bytes']:,} / {report['executable_section_bytes']:,}).",
                  report["proxy_note"], "", "## Feature evidence ranking", "",
                  "Ranks are ordinal milestones, not a percentage or equal-sized chunks of work. All current feature denominators remain incomplete.", "",
                  "| Feature | Rank / 5 | Evidence | Matched functions | Feature completion |",
                  "| --- | ---: | --- | ---: | --- |"])
    for feature in report["features"]:
        lines.append(f"| {feature['name']} | {feature['evidence_rank']} | {feature['rank_label']} | {feature['matched_functions']} | Unknown |")
    lines.extend(["", "See [scope and roadmap](scope-and-roadmap.md) for definitions, acceptance criteria and next work.", ""])
    return "\n".join(lines)


def main():
    report = collect()
    (ROOT / "docs/decomp/progress.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    (ROOT / "docs/decomp/progress.md").write_text(render(report), encoding="utf-8")
    if report["static_savedata_setup"]:
        (ROOT / "docs/decomp/savedata-request-mapping.json").write_text(json.dumps(report["static_savedata_setup"], indent=2)+"\n", encoding="utf-8")
    for local in report["local_source_scopes"]:
        (ROOT / "docs/decomp/menu-controller-coverage.json").write_text(json.dumps(local, indent=2)+"\n", encoding="utf-8")
        (ROOT / "docs/decomp/menu-controller-coverage.md").write_text(render_controller_scope(local), encoding="utf-8")
    print(f"Verified {report['matched_c_functions']} C/C++ functions / {report['matched_c_bytes']:,} bytes; whole-game audited percent pending")


if __name__ == "__main__":
    main()

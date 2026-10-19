"""Fingerprint maintained menu build inputs without inventory-report cycles."""

import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def layout_digest(inventory):
    layout = {k: inventory[k] for k in ("module", "reference_sha256", "instruction_ranges")}
    return hashlib.sha256(json.dumps(layout, sort_keys=True, separators=(",", ":")).encode()).hexdigest()


def fingerprint_menu_build():
    matches = json.loads((ROOT / "config/menu.matches.json").read_text())
    paths = {"config/menu.reference.json", "config/menu.matches.json", "config/menu.link.ld",
             "tools/build_menu.py", "tools/build_menu.sh",
             "tools/prepare_menu.py", "tools/verify_menu.py", "tools/menu_build_provenance.py",
             "tools/compile_psp.py", "tools/mips_relocations.py", "tools/probe_compilers.py", "tools/compiler_inline_sections.py",
             "include/main/drawing_record.h", "include/main/callback_runtime.h", "include/main/resource_descriptor.h",
             "include/main/presentation_channel.h",
             "config/menu-member-table-review.json", "tools/audit_menu_member_table.py",
             "config/presentation-callback-review.json", "tools/audit_presentation_callbacks.py",
             "tools/audit_overlay_dispatch.py", "tools/build_inventory.py"}
    paths.update({matches["compiler"], "tools/compilers/psp-gcc-1.3.1/bin/psp-as", "tools/local/wibo"})
    paths.update(unit["source"] for unit in matches["units"])
    paths.update(path.relative_to(ROOT).as_posix() for path in (ROOT / "include/menu").glob("*.h"))
    result = {path: hashlib.sha256((ROOT / path).read_bytes()).hexdigest() for path in sorted(paths)}
    result["layout:instruction_ranges"] = layout_digest(json.loads((ROOT / "config/inventory/menu.json").read_text()))
    menu_overrides = json.loads((ROOT / "config/inventory-overrides.json").read_text())["modules"]["menu"]
    result["review:menu-overrides"] = hashlib.sha256(json.dumps(menu_overrides, sort_keys=True,
                                                               separators=(",", ":")).encode()).hexdigest()
    return result


def require_current_menu_build(saved):
    if not saved or saved != fingerprint_menu_build():
        raise ValueError("Menu build inputs changed or provenance is missing; run tools/build_menu.py before reporting progress")

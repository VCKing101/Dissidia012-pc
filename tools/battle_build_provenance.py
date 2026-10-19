"""Fingerprint maintained battle build inputs without inventory-report cycles."""

import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def layout_digest(inventory):
    layout = {k: inventory[k] for k in ("module", "reference_sha256", "instruction_ranges")}
    return hashlib.sha256(json.dumps(layout, sort_keys=True, separators=(",", ":")).encode()).hexdigest()


def fingerprint_battle_build():
    matches = json.loads((ROOT / "config/battle.matches.json").read_text())
    paths = {"config/battle.reference.json", "config/battle.matches.json", "config/battle.link.ld",
             "include/battle/state_prefix.h", "include/battle/runtime_helpers.h", "include/battle/drawing_step.h",
             "include/main/drawing_record.h", "tools/build_battle.py", "tools/build_battle.sh",
             "include/battle/oct13_source_views.h", "include/battle/oct13_activation_views.h",
             "tools/prepare_battle.py", "tools/verify_battle.py", "tools/battle_build_provenance.py",
             "tools/compile_psp.py", "tools/mips_relocations.py", "tools/probe_compilers.py", "tools/compiler_inline_sections.py"}
    paths.update({matches["compiler"], "tools/compilers/psp-gcc-1.3.1/bin/psp-as", "tools/local/wibo"})
    paths.update(unit["source"] for unit in matches["units"])
    result = {path: hashlib.sha256((ROOT / path).read_bytes()).hexdigest() for path in sorted(paths)}
    result["layout:instruction_ranges"] = layout_digest(json.loads((ROOT / "config/inventory/battle.json").read_text()))
    return result


def require_current_battle_build(saved):
    if not saved or saved != fingerprint_battle_build():
        raise ValueError("Battle build inputs changed or provenance is missing; run tools/build_battle.py before reporting progress")

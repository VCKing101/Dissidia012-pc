"""Fingerprint main source inputs and multi-segment layout independently."""

import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def layout_digest(inventory, segments):
    layout = {k: inventory[k] for k in ("module", "reference_sha256", "instruction_ranges")}
    layout["load_segments"] = segments
    return hashlib.sha256(json.dumps(layout, sort_keys=True, separators=(",", ":")).encode()).hexdigest()


def fingerprint_main_build():
    matches = json.loads((ROOT / "config/main.matches.json").read_text())
    spec = json.loads((ROOT / "config/main.reference.json").read_text())
    paths = {"config/main.reference.json", "config/main.matches.json", "config/main.link.ld",
             "tools/build_main.py", "tools/build_main.sh", "tools/prepare_main.py", "tools/verify_main.py",
             "tools/main_build_provenance.py", "tools/compile_psp.py", "tools/mips_relocations.py",
             "tools/probe_compilers.py", "tools/compiler_inline_sections.py", "tools/inspect_imports.py", matches["compiler"],
             "config/presentation-callback-review.json", "tools/audit_presentation_callbacks.py", "tools/build_inventory.py",
             "tools/compilers/psp-gcc-1.3.1/bin/psp-as", "tools/local/wibo", "tools/local/allegrex-binutils/allegrex-objcopy"}
    paths.update(u["source"] for u in matches["units"])
    paths.update(p.relative_to(ROOT).as_posix() for p in (ROOT / "include/main").glob("*.h"))
    result = {p: hashlib.sha256((ROOT / p).read_bytes()).hexdigest() for p in sorted(paths)}
    result["layout:instruction_ranges_and_load_segments"] = layout_digest(json.loads((ROOT / "config/inventory/main.json").read_text()), spec["load_segments"])
    overrides = json.loads((ROOT / "config/inventory-overrides.json").read_text())["modules"].get("main", {})
    result["review:main-overrides"] = hashlib.sha256(json.dumps(overrides, sort_keys=True, separators=(",", ":")).encode()).hexdigest()
    return result


def require_current_main_build(saved):
    if not saved or saved != fingerprint_main_build():
        raise ValueError("Main build inputs changed or provenance is missing; run tools/build_main.py before reporting progress")

"""Fingerprint the maintained inputs to the extra-overlay build."""

import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def fingerprint_extra_build():
    matches = json.loads((ROOT / "config/extra.matches.json").read_text())
    paths = {"config/extra.matches.json", "config/extra.link.ld", "config/ulus10566.json",
             "config/ulus10566.extra.yaml", "include/extra/data_install.h", "include/macro.inc",
             "tools/build_extra.py", "tools/build_extra.sh", "tools/prepare_extra.py",
             "tools/compile_psp.py", "tools/mips_relocations.py", "tools/probe_compilers.py",
             "tools/verify_extra.py", "tools/build_provenance.py"}
    paths.update(unit["source"] for unit in matches["units"])
    return {path: hashlib.sha256((ROOT / path).read_bytes()).hexdigest() for path in sorted(paths)}


def require_current_build(saved):
    if not saved or saved != fingerprint_extra_build():
        raise ValueError("Build inputs changed or provenance is missing; run tools/build_extra.py before reporting progress")

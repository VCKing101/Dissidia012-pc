"""Compile, assemble and byte-verify a third scoped PSP overlay."""

import json
import os
import subprocess
from pathlib import Path

from menu_build_provenance import fingerprint_menu_build
from prepare_menu import prepare
from verify_menu import verify

ROOT = Path(__file__).resolve().parents[1]


def main():
    inputs = fingerprint_menu_build()
    prepare()
    if os.name == "nt":
        linux_root = "/mnt/"+ROOT.drive[0].lower()+ROOT.as_posix()[2:]
        command = ["wsl", "-d", "Ubuntu-24.04", "--cd", linux_root, "--", "bash", "tools/build_menu.sh"]
    else:
        command = ["bash", "tools/build_menu.sh"]
    subprocess.run(command, cwd=ROOT, check=True)
    spec = json.loads((ROOT / "config/menu.reference.json").read_text())
    original = (ROOT / spec["path"]).read_bytes()
    build = ROOT / "build/ULUS10566"
    payload = (build / "menu.payload.bin").read_bytes()
    offset, size = spec["payload_file_offset"], spec["payload_bytes"]
    if len(payload) != size:
        raise ValueError("Menu linked payload changed size")
    candidate = build / "OVL_MENU_APP.ELF"
    candidate.write_bytes(original[:offset]+payload+original[offset+size:])
    report = verify(candidate)
    if fingerprint_menu_build() != inputs:
        raise ValueError("Menu build inputs changed during compilation")
    report["build_inputs"] = inputs
    (build / "menu.verify.json").write_text(json.dumps(report, indent=2)+"\n", encoding="utf-8")
    print(f"PASS: menu full file {report['file_bytes']:,} bytes / SHA-256 {report['sha256']}")
    print(f"Matching C: {report['recovered_c_functions']} functions / {report['recovered_c_bytes']} bytes")
    print(f"Assembly instructions: {report['assembly_instruction_bytes']:,}; raw instruction words: {report['raw_instruction_fallback_bytes']:,}; opaque data: {report['opaque_original_bytes']:,} bytes")


if __name__ == "__main__":
    main()

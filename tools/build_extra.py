"""Split, assemble, link, package and byte-verify the USA extra overlay."""

import json
import os
import subprocess
import sys
from pathlib import Path

from verify_extra import verify
from prepare_extra import prepare
from build_provenance import fingerprint_extra_build

ROOT = Path(__file__).resolve().parents[1]


def main():
    inputs = fingerprint_extra_build()
    subprocess.run([sys.executable, "-m", "splat", "split", "config/ulus10566.extra.yaml"], cwd=ROOT, check=True)
    prepare()
    if os.name == "nt":
        drive = ROOT.drive[0].lower()
        linux_root = "/mnt/" + drive + ROOT.as_posix()[2:]
        command = ["wsl", "-d", "Ubuntu-24.04", "--cd", linux_root, "--", "bash", "tools/build_extra.sh"]
    else:
        command = ["bash", "tools/build_extra.sh"]
    subprocess.run(command, cwd=ROOT, check=True)
    build = ROOT / "build/ULUS10566"
    payload = (build / "extra.payload.bin").read_bytes()
    if len(payload) != 0x5F4C - 0x54:
        raise ValueError("Assembler changed the payload size")
    assets = ROOT / "assets/ULUS10566/extra"
    candidate = build / "OVL_EXTRA_APP.ELF"
    candidate.write_bytes((assets / "header.bin").read_bytes() + payload + (assets / "elf_metadata.bin").read_bytes())
    report = verify(candidate)
    if fingerprint_extra_build() != inputs:
        raise ValueError("Build inputs changed during compilation; rebuild before reporting progress")
    report["build_inputs"] = inputs
    (build / "extra.verify.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: reconstructed {report['file_bytes']} bytes, SHA-256 {report['sha256']}")
    matches = json.loads((ROOT / "config/extra.matches.json").read_text())
    print(f"Retail overlay verified; integrated matching C functions: {sum(len(unit['functions']) for unit in matches['units'])}")


if __name__ == "__main__":
    main()

"""Build the decrypted main ELF without flattening its memory gaps or BSS."""

import json
import os
import subprocess
from pathlib import Path
from main_build_provenance import fingerprint_main_build
from prepare_main import prepare
from verify_main import verify, geometry

ROOT = Path(__file__).resolve().parents[1]


def main():
    inputs = fingerprint_main_build()
    prepare()
    if os.name == "nt":
        linux_root = "/mnt/"+ROOT.drive[0].lower()+ROOT.as_posix()[2:]
        command = ["wsl", "-d", "Ubuntu-24.04", "--cd", linux_root, "--", "bash", "tools/build_main.sh"]
    else:
        command = ["bash", "tools/build_main.sh"]
    subprocess.run(command, cwd=ROOT, check=True)
    spec = json.loads((ROOT / "config/main.reference.json").read_text())
    original = (ROOT / spec["path"]).read_bytes()
    if geometry(original) != spec["load_segments"]:
        raise ValueError("Main packaging geometry differs from the pinned reference")
    candidate = bytearray(original)
    build = ROOT / "build/ULUS10566"
    for s in spec["load_segments"]:
        payload = (build / f"main.segment{s['index']}.bin").read_bytes()
        if len(payload) != s["file_bytes"]:
            raise ValueError("Main linked segment changed initialized size")
        a = s["file_offset"]
        candidate[a:a+s["file_bytes"]] = payload
    output = build / "EBOOT.ELF"
    output.write_bytes(candidate)
    report = verify(output)
    if fingerprint_main_build() != inputs:
        raise ValueError("Main build inputs changed during compilation")
    report["build_inputs"] = inputs
    (build / "main.verify.json").write_text(json.dumps(report, indent=2)+"\n", encoding="utf-8")
    print(f"PASS: main full decrypted file {report['file_bytes']:,} bytes / SHA-256 {report['sha256']}")
    print(f"Matching C: {report['recovered_c_functions']} functions / {report['recovered_c_bytes']} bytes; initialized load bytes: {report['reconstructed_load_bytes']:,}")
    print(f"Preserved nonload file bytes: {report['preserved_nonload_file_bytes']:,}; memory-only bytes not emitted: {report['memory_only_bytes']:,}")


if __name__ == "__main__":
    main()

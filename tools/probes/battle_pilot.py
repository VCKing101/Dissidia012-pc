"""Compile the battle pilot C units and compare fully resolved bytes to retail."""

import hashlib
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from compile_psp import compile_object
from mips_relocations import configured_addresses, resolve_function
from probe_compilers import functions_from_object


def main():
    manifest = json.loads((ROOT / "config/battle.matches.json").read_text())
    reference = (ROOT / "orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_BATTLE_APP.ELF").read_bytes()
    expected_hash = "1f96cfb3be771ff6e94741ae3916fab340df74e36ede07ca39545d015f4c49dc"
    if hashlib.sha256(reference).hexdigest() != expected_hash:
        raise ValueError("Battle reference hash mismatch")
    output = ROOT / "analysis/battle-pilot-probes"
    output.mkdir(parents=True, exist_ok=True)
    rows = []
    for unit in manifest["units"]:
        path = output / (unit["name"] + ".o")
        process = compile_object(manifest["compiler"], manifest["flags"],
                                 unit["source"], path.relative_to(ROOT))
        path.with_suffix(".log").write_text(process.stdout + process.stderr, encoding="utf-8")
        if process.returncode:
            raise RuntimeError(f"Compiler failed for {unit['name']}")
        functions = functions_from_object(path)
        for specification in unit["functions"]:
            function = functions[specification["name"]]
            compiled = resolve_function(function, configured_addresses(manifest))
            offset = int(specification["address"], 16) - 0x089DD4C0 + 0x54
            expected = reference[offset:offset + specification["size"]]
            row = {**specification, "unit": unit["name"], "compiled_size": len(compiled),
                   "raw_object_sha256": hashlib.sha256(path.read_bytes()).hexdigest(),
                   "relocations": function["relocations"], "match": compiled == expected,
                   "different_words": sum(compiled[i:i+4] != expected[i:i+4]
                                          for i in range(0, max(len(compiled), len(expected)), 4))}
            rows.append(row)
            print(f"{specification['name']}: {'MATCH' if row['match'] else 'DIFF'} "
                  f"({len(compiled)} bytes; {row['different_words']} different words)", flush=True)
    (output / "results.json").write_text(json.dumps({"reference_sha256": expected_hash,
        "compiler": manifest["compiler"], "flags": manifest["flags"], "functions": rows}, indent=2) + "\n")
    if not all(row["match"] for row in rows):
        raise ValueError("Battle C pilot differs from retail")


if __name__ == "__main__":
    main()

"""Compare SNC source-debug scheduling profiles on every integrated C unit."""

import json
from pathlib import Path

from compile_psp import compile_object
from mips_relocations import configured_addresses, resolve_function
from probe_compilers import functions_from_object

ROOT = Path(__file__).resolve().parents[1]


def main():
    matches = json.loads((ROOT / "config/extra.matches.json").read_text())
    reference = (ROOT / "orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_EXTRA_APP.ELF").read_bytes()
    output = ROOT / "analysis/debug-profile-probes"
    output.mkdir(parents=True, exist_ok=True)
    profiles = {"optimized": ["-O2"], "optimized_debug": ["-O2", "-g"],
                "source_debug_disabled": ["-O2", "-g", "-Xnosourcedebug=1"]}
    rows = []
    for name, flags in profiles.items():
        row = {"profile": name, "flags": flags, "functions": []}
        for unit in matches["units"]:
            path = output / (name + "_" + unit["name"] + ".o")
            process = compile_object(matches["compiler"], flags, unit["source"], path.relative_to(ROOT))
            (path.with_suffix(".log")).write_text(process.stdout + process.stderr, encoding="utf-8")
            if process.returncode:
                raise RuntimeError(f"Compiler failed for {name}/{unit['name']}")
            functions = functions_from_object(path)
            for specification in unit["functions"]:
                compiled = resolve_function(functions[specification["name"]], configured_addresses(matches))
                offset = int(specification["address"], 16) - 0x089DD4C0 + 0x54
                expected = reference[offset:offset + specification["size"]]
                row["functions"].append({**specification, "compiled_size": len(compiled),
                                         "match": compiled == expected,
                                         "different_words": sum(compiled[i:i+4] != expected[i:i+4]
                                                                for i in range(0, max(len(compiled), len(expected)), 4))})
        print(f"{name}: {sum(f['match'] for f in row['functions'])}/{len(row['functions'])} exact functions", flush=True)
        rows.append(row)
    (output / "results.json").write_text(json.dumps(rows, indent=2) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()

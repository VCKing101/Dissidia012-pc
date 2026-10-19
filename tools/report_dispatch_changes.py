"""Compare preserved pre-dispatch exports with the validated current maps."""

import hashlib
import json
import argparse
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--baseline-name", default="dispatch-baseline-v4")
    parser.add_argument("--output", type=Path, default=ROOT / "docs/decomp/overlay-dispatch-changes.json")
    parser.add_argument("--allow-untraced", action="store_true", help="Retain automatic candidates separately without inventing call-path evidence")
    args = parser.parse_args()
    if not args.baseline_name or any(c not in "abcdefghijklmnopqrstuvwxyz0123456789-" for c in args.baseline_name):
        parser.error("Baseline name must use lowercase letters, digits and hyphens")
    scope = json.loads((ROOT / "config/progress.json").read_text())
    modules = []
    for module in ("main", "menu", "battle"):
        spec = next(s for s in scope["modules"] if s["id"] == module)
        baseline_path = ROOT / f"analysis/ULUS10566/{module}/{args.baseline_name}/inventory.json"
        current_path = ROOT / spec["function_inventory"]
        baseline, current = (json.loads(p.read_text()) for p in (baseline_path, current_path))
        if baseline["module"] != module or baseline["executable_sha256"].lower() != spec["sha256"] or current["reference_sha256"] != spec["sha256"]:
            raise ValueError("Dispatch comparison uses another reference")
        before = {f["id"]: f for f in baseline["functions"]}
        after = {f["id"]: f for f in current["functions"]}
        if before.keys()-after.keys():
            raise ValueError("Dispatch review unexpectedly removed a function")
        if any(before[k]["ranges"] != after[k]["ranges"] for k in before):
            raise ValueError("Existing function boundaries changed; review before reporting a simple addition")
        # Main's ELF-declared import additions predate this review. They are
        # outside the Ghidra baseline and must not appear as newly found code.
        added = [after[k] for k in sorted(after.keys()-before.keys())
                 if after[k]["confidence"] != "elf-import-declaration-and-stub-bytes"]
        untraced = [f for f in added if "dispatch_reachability" not in f]
        if untraced and not args.allow_untraced:
            raise ValueError("New candidate lacks a checked dispatch call path")
        if any(f["confidence"] != "automatic-provisional" for f in untraced):
            raise ValueError("Untraced candidates require explicit automatic/provisional confidence")
        modules.append({"module": module, "reference_sha256": spec["sha256"],
                        "baseline_export_sha256": hashlib.sha256(baseline_path.read_bytes()).hexdigest(),
                        "current_inventory_sha256": hashlib.sha256(current_path.read_bytes()).hexdigest(),
                        "functions_added": len(added), "function_bytes_added": sum(f["body_bytes"] for f in added),
                        "matched_c_functions_among_additions": sum(f["matched_c"] for f in added),
                        "matched_c_bytes_among_additions": sum(f["body_bytes"] for f in added if f["matched_c"]),
                        "added_entries": [{"id": f["id"], "body_bytes": f["body_bytes"],
                                           "reachability": f["dispatch_reachability"],
                                           "direct_dispatch_root": "dispatch_entry_review" in f} for f in added if "dispatch_reachability" in f],
                        "untraced_entries": [{"id": f["id"], "body_bytes": f["body_bytes"],
                                              "confidence": f["confidence"]} for f in untraced]})
    report = {"inventory_revision": scope["inventory_revision"], "baseline_name": args.baseline_name, "modules": modules,
              "functions_added": sum(m["functions_added"] for m in modules),
              "function_bytes_added": sum(m["function_bytes_added"] for m in modules),
              "untraced_functions_added": sum(len(m["untraced_entries"]) for m in modules),
              "matched_c_functions_among_additions": sum(m["matched_c_functions_among_additions"] for m in modules),
              "matched_c_bytes_among_additions": sum(m["matched_c_bytes_among_additions"] for m in modules)}
    if args.output.resolve() in {ROOT / s["path"] for s in scope["modules"]}:
        parser.error("Output must not overwrite a reference")
    args.output.write_text(json.dumps(report, indent=2)+"\n", encoding="utf-8")
    print(f"Dispatch comparison: {report['functions_added']} additions / {report['function_bytes_added']:,} bytes; {report['untraced_functions_added']} explicitly untraced")


if __name__ == "__main__":
    main()

"""Refresh metadata-only dispatch seeds from pinned references and main ranges."""

import json
from pathlib import Path
from audit_overlay_dispatch import FileImage, inspect_dispatch
from audit_deferred_battle import inspect_deferred_battle

ROOT = Path(__file__).resolve().parents[1]


def main():
    path = ROOT / "config/overlay-dispatch.json"
    manifest = json.loads(path.read_text())
    scope = json.loads((ROOT / "config/progress.json").read_text())
    specs = {m["id"]: m for m in scope["modules"]}
    inventory = json.loads((ROOT / specs["main"]["function_inventory"]).read_text())
    if inventory["module"] != "main" or inventory["reference_sha256"] != specs["main"]["sha256"]:
        raise ValueError("Main inventory belongs to another dispatch reference")
    image = FileImage(ROOT / specs["main"]["path"], specs["main"]["sha256"])
    report = inspect_dispatch(image, inventory["functions"], manifest["review"])
    deferred = json.loads((ROOT / "config/deferred-battle.json").read_text())
    if deferred["main_reference_sha256"] != specs["main"]["sha256"]:
        raise ValueError("Deferred review belongs to another main reference")
    deferred_report = inspect_deferred_battle(image, inventory["functions"], deferred, manifest["review"]["slots"])
    report["roots"]["battle"] = sorted(report["roots"]["battle"]+deferred_report["roots"],
                                        key=lambda r: (int(r["address"], 16), r.get("call_pc", "")))
    modules = {}
    for module, rows in report["roots"].items():
        spec = specs[module]
        reference = FileImage(ROOT / spec["path"], spec["sha256"])
        if module != "main":
            slot = next(s for s in manifest["review"]["slots"] if s["module"] == module)
            payload = [b for b in reference.blocks if b[2]]
            if len(payload) != 1 or payload[0][0] != int(slot["payload_start"], 16) or payload[0][0]+len(payload[0][1]) != int(slot["payload_end"], 16):
                raise ValueError("Reviewed dispatch payload bounds differ from the reference")
        for row in rows:
            reference.read(int(row["address"], 16), 8, executable=True)
        modules[module] = {"reference_sha256": spec["sha256"], "entries": rows}
    manifest["modules"] = modules
    path.write_text(json.dumps(manifest, indent=2)+"\n", encoding="utf-8")
    for module, data in modules.items():
        print(f"{module}: {len({r['address'] for r in data['entries']})} distinct dispatch roots")


if __name__ == "__main__":
    main()

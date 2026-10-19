"""Repeat the twenty-entry menu controller's local source coverage audit."""
import json
import struct
from pathlib import Path
from audit_overlay_dispatch import FileImage
from audit_menu_member_table import inspect_empty_callback

ROOT = Path(__file__).resolve().parents[1]


def inspect_controller_scope(image, functions, review, scope, verified):
    if (scope["reference_sha256"] != review["reference_sha256"] or
            scope["reference_sha256"] != verified["sha256"] or
            not verified["byte_identical"]):
        raise ValueError("Controller scope requires the pinned exact menu build")
    for key in ("dispatcher", "table", "record_count", "record_bytes"):
        if scope[key] != review[key]:
            raise ValueError("Controller scope differs from the copied table review")
    inspect_empty_callback(image, functions, review)
    candidates = {int(f["address"], 16): f for f in functions}
    matched = {int(f["address"], 16): f for f in verified["c_functions"]}
    if len(candidates) != len(functions) or len(matched) != len(verified["c_functions"]):
        raise ValueError("Controller inputs have duplicate function entries")
    entries = scope["entries"]
    if (len(entries) != 20 or [e["record_index"] for e in entries] != list(range(20)) or
            len({e["address"] for e in entries}) != 20):
        raise ValueError("Controller scope must list each of the twenty targets exactly once")
    table = int(scope["table"], 16)
    rows, ranges = [], []
    for entry in entries:
        index = entry["record_index"]
        start, size = int(entry["address"], 16), entry["body_bytes"]
        end = start + size
        record = table + index * 8
        if struct.unpack("<hhI", image.read(record, 8)) != (0, 0, start):
            raise ValueError("Controller record is not its reviewed direct callback")
        candidate = candidates.get(start)
        if (size < 8 or size % 4 or start % 4 or candidate is None or
                candidate["body_bytes"] != size or
                candidate["ranges"] != [{"start": entry["address"], "end": entry["next_function"]}] or
                int(entry["next_function"], 16) != end or end not in candidates):
            raise ValueError("Controller callback boundary differs from reviewed contiguous extent")
        body = image.read(start, size, True)
        if struct.unpack_from("<I", body, size - 8)[0] != 0x03E00008:
            raise ValueError("Controller callback does not end with jr-ra and its delay slot")
        for offset in range(0, size, 4):
            word = struct.unpack_from("<I", body, offset)[0]
            opcode = word >> 26
            branch = opcode in (1, 4, 5, 6, 7, 20, 21, 22, 23)
            branch |= opcode in (17, 18) and ((word >> 21) & 31) == 8
            if branch:
                immediate = word & 0xFFFF
                if immediate >= 0x8000: immediate -= 0x10000
                target = start + offset + 4 + immediate * 4
                if not start <= target < end:
                    raise ValueError("Controller callback has a branch outside its reviewed body")
        recovered = matched.get(start)
        if recovered and recovered["size"] != size:
            raise ValueError("Controller source proof disagrees with reviewed callback extent")
        rows.append({**entry, "member_record": f"0x{record:08X}", "matching_source": bool(recovered)})
        ranges.append((start, end))
    ordered = sorted(ranges)
    if any(z > a for (_, z), (a, _) in zip(ordered, ordered[1:])):
        raise ValueError("Controller callback extents overlap")
    dispatcher = matched.get(int(scope["dispatcher"], 16))
    count = sum(r["matching_source"] for r in rows)
    total = sum(r["body_bytes"] for r in rows)
    recovered_bytes = sum(r["body_bytes"] for r in rows if r["matching_source"])
    return {"scope_id": scope["scope_id"], "scope": scope["scope"],
            "reference_sha256": scope["reference_sha256"], "table": scope["table"],
            "dispatcher": scope["dispatcher"], "dispatcher_matching_source": bool(dispatcher),
            "handler_count": 20, "matching_handlers": count,
            "handler_count_percent": round(100 * count / 20, 4),
            "reviewed_handler_bytes": total, "matching_handler_bytes": recovered_bytes,
            "handler_byte_percent": round(100 * recovered_bytes / total, 4),
            "extent_checks": "Pinned direct table targets; contiguous inventory extents; terminal jr-ra/delay slot; next known entries; bounded relative branches; no overlaps.",
            "dependencies_included": False, "runtime_verified": False, "entries": rows}


def collect_controller_scope(verified):
    scope = json.loads((ROOT / "config/menu-controller-scope.json").read_text())
    review = json.loads((ROOT / "config/menu-member-table-review.json").read_text())
    inventory = json.loads((ROOT / "config/inventory/menu.json").read_text())
    image = FileImage(ROOT / "orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_MENU_APP.ELF", scope["reference_sha256"])
    report = inspect_controller_scope(image, inventory["functions"], review, scope, verified)
    manifest = json.loads((ROOT / "config/menu.matches.json").read_text())
    sources = {f["address"]: u["source"] for u in manifest["units"] for f in u["functions"]}
    for entry in report["entries"]:
        entry["source"] = sources.get(entry["address"]) if entry["matching_source"] else None
    return report


def render_controller_scope(report):
    lines = ["# Menu controller source coverage", "", "Generated by `tools/progress.py` after current raw, linked and full-file verification.", "",
             f"**{report['matching_handlers']} / 20 handlers match ({report['handler_count_percent']:.2f}% by count)**; **{report['matching_handler_bytes']:,} / {report['reviewed_handler_bytes']:,} bytes ({report['handler_byte_percent']:.2f}% by bytes)**.", "",
             report["scope"], "The dispatcher matches separately. Dependencies are excluded from these totals; this is not complete source recovery for the menu overlay, UI feature, or game.", "",
             "The pinned table has twenty distinct direct targets, all with zero adjustment and flags. Each reviewed extent is contiguous, ends in `jr $ra` and a delay slot, abuts the next known entry, contains its relative branch targets, and does not overlap another handler. The complete menu inventory remains provisional.", "",
             "| Record | Callback | Code bytes | Exact source |", "| ---: | --- | ---: | --- |"]
    for entry in report["entries"]:
        source = f"[{entry['source']}](../../{entry['source']})" if entry["source"] else "Pending"
        lines.append(f"| {entry['record_index']} | `{entry['address']}` | {entry['body_bytes']:,} | {source} |")
    lines.extend(["", f"The {report['handler_count'] - report['matching_handlers']} unmatched handlers carry most of the remaining bytes. Savedata status polling, larger setup/dialog handlers and later transitions remain pending; draft switch/loop candidates receive no credit. Count and byte percentages must be read together.", "",
                  "The [review metadata](../../config/menu-controller-scope.json) pins this denominator. [The audit](../../tools/menu_controller_scope.py) repeats the checks; tests reject changed target/extent/source proof/build identities. [Machine-readable coverage](menu-controller-coverage.json) records every member and source path.", "",
                  "No new runtime claim is made. Emulator and data-install validation remain deferred.", ""])
    return "\n".join(lines)

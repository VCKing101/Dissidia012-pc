"""Repeat pinned SDK import, request-mode and save-slot extent observations."""
import json
from pathlib import Path
from audit_overlay_dispatch import FileImage
from inspect_imports import inspect as inspect_imports

ROOT = Path(__file__).resolve().parents[1]
BINDINGS = {"sceUtilitySavedataInitStart": ("sceUtility", "0x50C4CD57", "0x089C5858"),
            "sceRtcGetTick": ("sceRtc", "0x6FF40ACC", "0x089C5D78")}
CALLS = {("0x089DD8F0", "0x089C5858"), ("0x089DDB10", "0x089C5858"),
         ("0x089DDA4C", "0x089C5D78"), ("0x089DE430", "0x0885C6CC")}
CHECKPOINTS = {"twenty_name_loop_limit": "0x089DD7C8", "request_clear_600": "0x089DD7E4",
               "list_mode_11": "0x089DD808", "autoload_mode_0": "0x089DDB00",
               "twenty_slot_loop_limit": "0x089DDAEC", "slot_stride_48": "0x089DDAF4",
               "name_list_clear_1a4": "0x089DE4B4", "slot_list_clear_c": "0x089DE4CC",
               "slot_entries_clear_5a0": "0x089DE4E4"}


def inspect_setup(image, imports, review):
    actual = {f["address"]: (library["library"], f["nid"])
              for library in imports["libraries"] for f in library["functions"]}
    if (len(review["imports"]) != len(BINDINGS) or
            {r["api"] for r in review["imports"]} != set(BINDINGS)):
        raise ValueError("Savedata setup must review both SDK bindings")
    for row in review["imports"]:
        expected = BINDINGS.get(row["api"])
        if (expected != (row["library"], row["nid"], row["address"]) or
                actual.get(row["address"]) != expected[:2]):
            raise ValueError("Savedata setup SDK import binding differs")
    if len(review["calls"]) != 4 or {(r["pc"], r["target"]) for r in review["calls"]} != CALLS:
        raise ValueError("Savedata setup call review is incomplete or redirected")
    for row in review["calls"]:
        pc = int(row["pc"], 16)
        word = image.word(pc, True)
        target = ((pc + 4) & 0xF0000000) | ((word & 0x03FFFFFF) << 2)
        if word >> 26 != 3 or target != int(row["target"], 16):
            raise ValueError("Savedata setup call target differs")
    constants = {}
    for row in review["materialized_constants"]:
        high = image.word(int(row["high_pc"], 16), True)
        low = image.word(int(row["low_pc"], 16), True)
        reg = row["register"]
        immediate = low & 0xFFFF
        if immediate >= 0x8000: immediate -= 0x10000
        value = ((high & 0xFFFF) << 16) + immediate
        if (high >> 16 != (0x3C00 | reg) or low >> 16 != (0x2400 | (reg << 5) | reg) or
                value != row["value"]):
            raise ValueError("Savedata setup buffer/allocation constant differs")
        constants[row["name"]] = value
    if constants != {"savedata_capacity": 0x3ECC8, "execution_allocation": 0x3FAB0}:
        raise ValueError("Savedata setup extent review differs")
    if 0xDD0 + constants["savedata_capacity"] + 16 > constants["execution_allocation"]:
        raise ValueError("Savedata buffer and cached timestamp exceed the observed allocation")
    if (len(review["instruction_checks"]) != len(CHECKPOINTS) or
            {r["meaning"]: r["pc"] for r in review["instruction_checks"]} != CHECKPOINTS):
        raise ValueError("Savedata setup mode/loop/clear review is incomplete")
    for row in review["instruction_checks"]:
        if image.word(int(row["pc"], 16), True) != int(row["word"], 16):
            raise ValueError("Savedata setup mode/loop/clear instruction differs")
    for row in review["strings"]:
        expected = row["value"].encode("ascii") + b"\0"
        if image.read(int(row["address"], 16), len(expected)) != expected:
            raise ValueError("Savedata setup name/filename string differs")
    return {"slot_capacity": 20, "name_list_records": 21, "slot_record_bytes": 0x48,
            "request_bytes": 0x600, "payload_offset": 0xDD0,
            "payload_capacity": constants["savedata_capacity"],
            "cached_timestamp_offset": 0x3FA98,
            "observed_allocation_bytes": constants["execution_allocation"],
            "sdk_bindings": review["imports"], "source_matching_not_assessed": True,
            "runtime_verified": False, "scope": review["scope"]}


def collect_setup():
    review = json.loads((ROOT / "config/menu-savedata-setup-review.json").read_text())
    image = FileImage(ROOT / "orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_MENU_APP.ELF", review["menu_reference_sha256"])
    main_path = ROOT / "analysis/ULUS10566/main/EBOOT.ELF"
    FileImage(main_path, review["main_reference_sha256"])
    return inspect_setup(image, inspect_imports(main_path), review)


def main():
    print(json.dumps(collect_setup(), indent=2))


if __name__ == "__main__": main()

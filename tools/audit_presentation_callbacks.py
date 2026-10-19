"""Pin two callback entries discovered through native presentation dispatch."""
import json
import struct
from pathlib import Path
from audit_overlay_dispatch import FileImage
from inspect_imports import inspect as inspect_imports

ROOT = Path(__file__).resolve().parents[1]
IDENTITIES = {"main": ("0x08873F78", 8, "0x08873F54", 36, "0x08873F80", 348),
              "menu": ("0x089DE9D4", 24, "0x089DE990", 68, "0x089DE9EC", 40)}
BODIES = {"main": [0x03E00008, 0],
          "menu": [0x3C04089E, 0x8C8409C0, 0x34050001, 0x8C8401DC, 0x03E00008, 0xA0850048]}
EVIDENCE = {
    "main": {0x088739D0: 0xA7A00002, 0x088739D4: 0x3C050887,
             0x088739D8: 0xA7A00000, 0x088739DC: 0x24A53F78,
             0x088739E0: 0xAFA50004, 0x08873A50: 0x00A0F809},
    "menu": {0x089DE13C: 0x3C05089E, 0x089DE148: 0x00003025,
             0x089DE154: 0x0E21CF4D, 0x089DE158: 0x24A5E9D4},
}
INVOCATION = {0x08874060: 0x8E240014, 0x0887406C: 0x00802825,
              0x08874070: 0x00A0F809, 0x08874074: 0x8E240018}


def inspect_callbacks(images, functions, review, imports):
    callbacks = review["callbacks"]
    if len(callbacks) != 2 or {c["module"] for c in callbacks} != set(IDENTITIES):
        raise ValueError("Presentation callback review must cover both entries")
    imported = {int(f["address"], 16) for lib in imports["libraries"] for f in lib["functions"]}
    rows = []
    for callback in callbacks:
        module = callback["module"]
        identity = (callback["address"], callback["size"], callback["previous"],
                    callback["previous_bytes"], callback["next"], callback["next_bytes"])
        if identity != IDENTITIES[module]:
            raise ValueError("Presentation callback identity/boundaries differ")
        address, size = int(callback["address"], 16), callback["size"]
        image = images[module]
        if image.read(address, size, True) != struct.pack("<" + "I" * len(BODIES[module]), *BODIES[module]):
            raise ValueError("Presentation callback body differs")
        known = {f["address"]: f for f in functions[module]}
        previous, following = known[callback["previous"]], known[callback["next"]]
        if (previous["body_bytes"] != callback["previous_bytes"] or
                following["body_bytes"] != callback["next_bytes"] or
                previous["ranges"] != [{"start": callback["previous"], "end": callback["address"]}] or
                address + size != int(callback["next"], 16)):
            raise ValueError("Presentation callback neighbor extent differs")
        if image.word(address - 8, True) != 0x03E00008:
            raise ValueError("Presentation callback predecessor does not end independently")
        for function in functions[module]:
            if function["address"] == callback["address"]:
                if function["body_bytes"] != size:
                    raise ValueError("Presentation callback maintained extent differs")
                continue
            for span in function["ranges"]:
                if int(span["start"], 16) < address + size and address < int(span["end"], 16):
                    raise ValueError("Presentation callback overlaps another function")
        if module == "main" and any(address < stub + 8 and stub < address + size for stub in imported):
            raise ValueError("Presentation callback overlaps an SDK import stub")
        checks = callback["instruction_checks"]
        expected = {f"0x{pc:08X}": f"0x{word:08X}" for pc, word in EVIDENCE[module].items()}
        if checks != expected:
            raise ValueError("Presentation callback address/call evidence redirected")
        for pc, word in EVIDENCE[module].items():
            if image.word(pc, True) != word:
                raise ValueError("Presentation callback address/call instruction differs")
        rows.append({"module": module, "address": callback["address"], "size": size,
                     "next": callback["next"], "previous": callback["previous"],
                     "entry_evidence": "local-member-record" if module == "main" else "queued-function-pointer",
                     "callback_interface": "unused receiver" if module == "main" else "void(int)",
                     "runtime_verified": False})
    invocation = {f"0x{pc:08X}": f"0x{word:08X}" for pc, word in INVOCATION.items()}
    if review["event_invocation"] != invocation:
        raise ValueError("Presentation event invocation/argument evidence differs")
    for pc, word in INVOCATION.items():
        if images["main"].word(pc, True) != word:
            raise ValueError("Presentation event pointer/argument instruction differs")
    return rows


def collect_callbacks(functions=None):
    review = json.loads((ROOT / "config/presentation-callback-review.json").read_text())
    scope = json.loads((ROOT / "config/progress.json").read_text())
    specs = {m["id"]: m for m in scope["modules"]}
    images = {}
    for module in IDENTITIES:
        if review["references"][module] != specs[module]["sha256"]:
            raise ValueError("Presentation callback reference identity differs")
        images[module] = FileImage(ROOT / specs[module]["path"], review["references"][module])
    if functions is None:
        functions = {module: json.loads((ROOT / specs[module]["function_inventory"]).read_text())["functions"]
                     for module in IDENTITIES}
    imports = inspect_imports(ROOT / specs["main"]["path"])
    return inspect_callbacks(images, functions, review, imports)


if __name__ == "__main__": print(json.dumps(collect_callbacks(), indent=2))

"""Reproduce callback signature/qualifier probes with and without SNC -g."""

import json
from itertools import product
from pathlib import Path

from compile_psp import compile_object
from probe_compilers import functions_from_object

ROOT = Path(__file__).resolve().parents[1]


def main():
    output = ROOT / "analysis/callback-probes"
    output.mkdir(parents=True, exist_ok=True)
    callbacks = (ROOT / "src/extra/data_install.c").read_text().split("void func_089DD648", 1)[0]
    notifications = (ROOT / "src/extra/data_install_notifications.c").read_text()
    volatile_source = callbacks + "\n" + notifications
    declaration = "    volatile DataInstallState *state = (volatile DataInstallState *)userdata;\n"
    variants = {
        "typed_pointer": volatile_source.replace("void *userdata)", "DataInstallState *state)").replace(declaration, ""),
        "userdata_cast": volatile_source.replace("volatile DataInstallState *", "DataInstallState *"),
        "volatile_userdata_cast": volatile_source,
    }
    specifications = [specification for specification in json.loads((ROOT / "config/compiler-probes.json").read_text())["functions"]
                      if specification["name"] not in {"func_089DD648", "func_089DD65C"}]
    reference = (ROOT / "orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_EXTRA_APP.ELF").read_bytes()
    results = []
    for (name, source_text), debug in product(variants.items(), [False, True]):
        tag = name + ("_debug" if debug else "")
        flags = ["-O2", "-g"] if debug else ["-O2"]
        source = output / (tag + ".c")
        object_path = output / (tag + ".o")
        source.write_text(source_text, encoding="utf-8")
        process = compile_object("tools/compilers/pspsnc_1.2.7503.0/pspsnc.exe", flags,
                                 source.relative_to(ROOT), object_path.relative_to(ROOT))
        (output / (tag + ".log")).write_text(process.stdout + process.stderr, encoding="utf-8")
        if process.returncode:
            raise RuntimeError(f"Callback probe failed: {name}")
        functions = functions_from_object(object_path)
        row = {"source_variant": name, "flags": flags, "functions": []}
        for specification in specifications:
            function = functions[specification["name"]]
            offset = int(specification["address"], 16) - 0x089DD4C0 + 0x54
            match = function["bytes"] == reference[offset:offset + specification["size"]] and not function["has_relocations"]
            row["functions"].append({**specification, "compiled_size": len(function["bytes"]), "match": match})
        print(tag + ": " + ", ".join(function["name"] for function in row["functions"] if function["match"]), flush=True)
        results.append(row)
    (output / "results.json").write_text(json.dumps(results, indent=2) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()

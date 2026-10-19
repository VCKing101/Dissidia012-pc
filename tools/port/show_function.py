"""Print the disassembly of the inventoried function containing an address, with
import stubs named. Local analysis aid; output goes to the terminal only.

    show_function.py main 0x08933330 [--max 400]
"""

import argparse
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402


def stub_names():
    path = common.OUT / "imports.json"
    if not path.exists():
        return {}
    data = json.loads(path.read_text(encoding="utf-8"))
    return {int(r["stub"], 16): r["name"] or r["nid"] for r in data["imports"] if r["binary"] == "main"}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("module", choices=list(common.MODULES))
    parser.add_argument("address", type=lambda t: int(t, 0))
    parser.add_argument("--max", type=int, default=400)
    args = parser.parse_args()
    module = common.Module(args.module)
    names = stub_names()
    owner = module.owner_of(args.address)
    if owner is None:
        print("address is not inside an inventoried function")
        return
    func = next(f for f in module.functions if f["address"] == owner)
    print(f"{args.module}:{owner} {func['body_bytes']} bytes")
    shown = 0
    for r in func["ranges"]:
        start, end = int(r["start"], 16), int(r["end"], 16)
        for a in range(start, end, 4):
            w = module.word(a)
            text = common.instr(w, a).disassemble()
            if common.is_jal(w) or common.is_j(w):
                t = common.jal_target(w, a)
                if t in names:
                    text += f"    <{names[t]}>"
            print(f"{a:08X}: {text}")
            shown += 1
            if shown >= args.max:
                return


if __name__ == "__main__":
    main()

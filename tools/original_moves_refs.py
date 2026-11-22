"""Reference files for checking the port's C++ builder of Bartz's original moves against this
Python one (tools/original_moves.py).

    python tools/original_moves_refs.py --iso 012.iso [OUT]

Writes into OUT (default build/original-moves-dev, git-ignored; it holds the game's data and is
never shared):

    disc/<package path>     every file of the 012 disc the build reads
    expect/package/...      the files the Python build writes
    ref/anim/<name>.bin     each rebuilt animation: the GMO motion chunk
    ref/fep/base.fep        012 Bartz's effect package before the build
    ref/fep/<fighter>.fep   the effect packages effects are copied from
    ref/fep/steps.json      the copies in order: [{"source", "source_name", "name", "index"}]
    ref/fep/after.fep       his effect package after them
"""

import argparse
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import effect_package  # noqa: E402
import original_moves as om  # noqa: E402


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("--iso", default=str(om.ISO_012))
    parser.add_argument("--recipe", default=str(om.RECIPE))
    parser.add_argument("out", nargs="?", default=str(ROOT / "build" / "original-moves-dev"))
    args = parser.parse_args()
    out = Path(args.out)
    data = json.loads(Path(args.recipe).read_text(encoding="utf-8"))
    discs = om.read_base(args.iso)
    read = discs["read"]

    def put(path, content):
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(content)

    _, effect_owners = om.recipe_needs(data)
    paths = om.recipe_inputs(discs, data)
    for path in paths:
        put(out / "disc" / path, read(path))
    print(f"disc: {len(paths)} files")

    recipe = om.Recipe(data)
    recipe.attach(discs["models"]["1P"], read)
    for anim, info in recipe.anims.items():
        if "owner" in info:
            put(out / "ref" / "anim" / f"{info['name']}.bin", recipe.anim_chunk(anim, info["name"] + "_OG").raw)
    print(f"ref/anim: {sum(1 for a in recipe.anims.values() if 'owner' in a)} animations")

    base = om.Fighter(discs["base"])
    put(out / "ref" / "fep" / "base.fep", base.fep.to_bytes())
    labels = {id(s.fep): s.label for s in discs["sources"]}
    for source in discs["sources"]:
        if source.label in effect_owners:
            put(out / "ref" / "fep" / f"{source.label}.fep", source.fep.to_bytes())
    steps, copy_effect = [], effect_package.copy_effect

    def logged(target, source, name, new_name=None):
        index, other = copy_effect(target, source, name, new_name)
        steps.append({"source": labels[id(source)], "source_name": name, "name": new_name, "index": index})
        return index, other
    effect_package.copy_effect = logged
    commands = {old: new for old, new in om.shipped_commands().items()}
    om.bring(base, recipe, om.model_counts(discs)["1P"], commands, om.Report(), sources=discs["sources"])
    effect_package.copy_effect = copy_effect
    put(out / "ref" / "fep" / "steps.json", json.dumps(steps, indent=1).encode())
    put(out / "ref" / "fep" / "after.fep", base.fep.to_bytes())
    print(f"ref/fep: {len(steps)} copies")


if __name__ == "__main__":
    main()

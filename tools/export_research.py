"""Export research pseudocode and references from an existing Ghidra project.

Runs the analyzed program read-only (no re-analysis) and writes
analysis/ULUS10566/<module>/research/{pseudocode.c,refs.tsv}. These ignored
files support reverse engineering; they earn no matching credit.
"""

import argparse
import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
GHIDRA = ROOT / "tools/local/ghidra_12.1.3_PUBLIC"
JAVA = ROOT / "tools/local/jdk-21.0.12.1+1/bin/java.exe"


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("module", choices=("main", "menu", "battle", "extra"))
    args = parser.parse_args()
    scope = json.loads((ROOT / "config/progress.json").read_text())
    entry = next(m for m in scope["modules"] if m["id"] == args.module)
    inventory = json.loads((ROOT / f"config/inventory/{args.module}.json").read_text())
    project = inventory["analysis_project"]
    if not (ROOT / f"analysis/ghidra-projects/{project}.gpr").exists():
        sys.exit(f"Analyzed Ghidra project is missing: {project}")
    # A private settings home lets several modules export concurrently.
    home = ROOT / f"analysis/ghidra-research-home-{args.module}"
    output = ROOT / f"analysis/ULUS10566/{args.module}/research"
    home.mkdir(parents=True, exist_ok=True)
    output.mkdir(parents=True, exist_ok=True)
    command = [str(JAVA), "-Xmx3G", "-Djava.system.class.loader=ghidra.GhidraClassLoader", "-Dfile.encoding=UTF8",
               f"-Duser.home={home}", f"-Dapplication.settingsdir={home}/settings",
               f"-Dapplication.cachedir={home}/cache", f"-Djava.util.prefs.userRoot={home}/prefs",
               "-cp", str(GHIDRA / "Ghidra/Framework/Utility/lib/Utility.jar"),
               "ghidra.Ghidra", "ghidra.app.util.headless.AnalyzeHeadless",
               str(ROOT / "analysis/ghidra-projects"), project,
               "-process", Path(entry["path"]).name, "-noanalysis", "-readOnly",
               "-scriptPath", str(ROOT / "tools/ghidra"), "-postScript", "ExportResearch.java", str(output),
               "-log", str(output / "headless.log")]
    subprocess.run(command, cwd=ROOT, check=True)
    for name in ("pseudocode.c", "refs.tsv"):
        if not (output / name).stat().st_size:
            sys.exit(f"Ghidra did not write {name}; inspect {output / 'headless.log'}")
    print(f"Research export written to {output.relative_to(ROOT)}")


if __name__ == "__main__":
    main()

"""Guided setup of the Dissidia 012 PC port: steps 2 onwards of setup.ps1 (which checks the
prerequisites and makes .venv first). Run setup.ps1, not this file; docs/port/setup.md has the details.

    .venv\\Scripts\\python.exe tools/port/setup/d012_setup.py [--update] [--deck] [--non-interactive]
        [--iso FILE] [--eboot FILE] [--no-shortcuts] [--shortcut-dir DIR]
        [--portablekit-source PATH] [--deps-cache DIR] [--skip-iso-hash]

Every step checks what is already done and skips it, so running it again only redoes what changed.
State (the ISO path, an EBOOT path if one was given, what was last applied and generated) is kept in
build/port/setup/state.json.
"""

import argparse
import os
import subprocess
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import ui  # noqa: E402
from ui import ROOT, SetupError, info, step  # noqa: E402

SHORTCUT_NAME = "Dissidia 012 (PC port)"


def parse():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("--update", action="store_true", help="re-apply the patches and rebuild; no questions")
    parser.add_argument("--deck", action="store_true", help="also build the Steam Deck package")
    parser.add_argument("--non-interactive", action="store_true", help="never ask; fail instead")
    parser.add_argument("--iso", help="the USA ULUS10566 ISO")
    parser.add_argument("--eboot", help="a decrypted EBOOT.ELF to use instead of decrypting the ISO's")
    parser.add_argument("--no-shortcuts", action="store_true", help="make no desktop or Start Menu shortcut")
    parser.add_argument("--shortcut-dir", help="make the two shortcuts under this folder instead (for tests)")
    parser.add_argument("--portablekit-source", help="clone PortableKit from this path or URL (it must have 33800c4)")
    parser.add_argument("--deps-cache", help="folder with the dependency zips, used before downloading")
    parser.add_argument("--skip-iso-hash", action="store_true", help="accept an ISO whose SHA-1 differs")
    return parser.parse_args()


def check_root_path():
    text = str(ROOT)
    bad = [c for c in "!%^&" if c in text]
    if bad:
        raise SetupError(f"the folder path {text} contains {' '.join(bad)}, which the Windows build scripts "
                         "cannot handle. Move the folder (for example to C:\\Games\\Dissidia012-pc) and run "
                         "setup again.")
    if len(text) > 60:
        ui.warn(f"the folder path is long ({len(text)} characters). If the build fails with 'path too long' "
                "errors, move the folder closer to the drive root (for example C:\\Games\\Dissidia012-pc).")


def make_shortcuts(icon, folder=None):
    script = ROOT / "tools" / "port" / "setup" / "shortcuts.ps1"
    args = ["powershell.exe", "-NoProfile", "-ExecutionPolicy", "Bypass", "-File", str(script),
            "-Root", str(ROOT), "-Name", SHORTCUT_NAME]
    if icon.exists():
        args += ["-Icon", str(icon)]
    if folder:
        args += ["-Folder", str(Path(folder).resolve())]
    result = subprocess.run(args, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            text=True)
    if result.returncode != 0:
        raise SetupError(f"could not make the shortcuts:\n      {result.stdout.strip()}")
    for line in result.stdout.splitlines():
        if line.strip():
            info(f"shortcut: {line.strip()}")


def main():
    options = parse()
    ui.INTERACTIVE = not (options.non_interactive or options.update)
    state = ui.load_state()
    first_run = "built" not in state
    if options.update and not state.get("iso") and not options.iso:
        raise SetupError("-Update needs a finished setup to update; run setup.ps1 without -Update first")
    check_root_path()
    total = 7 if options.deck else 6
    started = time.monotonic()

    if first_run and ui.INTERACTIVE:
        print()
        info("Setup will now download about 100 MB (PortableKit and its build dependencies), check your")
        info("game files, and compile the port. Compiling takes 5 to 30 minutes, depending on the PC.")
        if not ui.confirm("Continue?"):
            raise SetupError("stopped at your request; nothing was changed")

    import deps
    import portablekit
    import game
    import build

    step(2, total, "Build dependencies (SDL3, glslang, Vulkan headers)")
    deps.ensure(options.deps_cache)
    ui.save_state(state)

    step(3, total, "PortableKit (the framework) with this port's patches")
    portablekit.ensure(state, options.portablekit_source, force=options.update)
    ui.save_state(state)

    step(4, total, "Your game files")
    iso_path, eboot_path = game.ensure(state, options.iso, options.eboot, ui.INTERACTIVE, options.skip_iso_hash)
    ui.save_state(state)
    if eboot_path is None:
        info("EBOOT: the port decrypts the game's executable from your ISO")
        program = build.bootstrap()
        try:
            game.prepare(iso_path, game.decrypt(program, iso_path))
        finally:
            game.cleanup()
    else:
        game.prepare(iso_path, eboot_path)
    ui.save_state(state)

    step(5, total, "Building the port")
    exe = build.ensure(state)
    state["built"] = True
    ui.save_state(state)

    number = 6
    if options.deck:
        step(number, total, "Steam Deck package")
        build.ensure_release(state)
        ui.save_state(state)
        number += 1

    step(number, total, "Shortcuts")
    if options.no_shortcuts:
        info("skipped (-NoShortcuts)")
    else:
        make_shortcuts(game.ICON, options.shortcut_dir)

    data = ROOT / "build" / "port" / "data"
    print()
    print(f"Done in {ui.duration(time.monotonic() - started)}. The port is ready.")
    print()
    print("  Play:      the \"Dissidia 012 (PC port)\" shortcut on the desktop or in the Start menu,")
    print("             or run  .\\tools\\port\\play_d012.ps1")
    print("  In game:   Esc (keyboard) or L3+R3 (controller) opens the port's menu: video, controls, audio.")
    print(f"  Saves:     {data / 'ms0' / 'PSP' / 'SAVEDATA'}  (the game makes them; back this folder up)")
    print(f"  Settings:  {data / 'settings.ini'}")
    print(f"  Mods:      {data / 'mods'}  (docs/port/mod-folder.md)")
    print(f"  Program:   {exe}")
    print("  Update:    git pull, then run setup.ps1 again (or setup.ps1 -Update): it rebuilds only what changed.")
    if options.deck:
        print("  Deck:      build/port/deck-package/Dissidia012; see docs/port/steam-deck.md to send it.")
    return 0


def entry():
    code = 1
    try:
        code = main()
    except SetupError as error:
        print()
        print(f"Setup stopped: {error}")
        print("Fix that, then run setup.ps1 again; it continues where it stopped. "
              "Help: docs/port/setup.md (Troubleshooting).")
    except KeyboardInterrupt:
        print()
        print("Setup interrupted. Run setup.ps1 again to continue.")
        code = 130
    sys.stdout.flush()
    sys.stderr.flush()
    # os._exit: pycdlib trips an internal error in some Python versions at interpreter shutdown.
    os._exit(code)


if __name__ == "__main__":
    entry()

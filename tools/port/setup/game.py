"""The game: the user's ISO checked, the overlays, the disc PRX and the icon taken from the ISO,
the decrypted EBOOT made from the ISO, and build/port/game prepared (tools/port/prepare_game_dir.py).

Nothing here decrypts anything itself. The port does: `D012Native --install <iso> --in-place`, with
the profile's key for the executable's tag (port/host/d012_profile.cpp), writes EBOOT.ELF into its
data directory, here a temporary folder (decrypt()). A decrypted EBOOT the user already has can be
given instead (-Eboot). Either way it is accepted only if its SHA-256 is that of the known ULUS10566
v1.00 executable.
"""

import hashlib
import io
import os
import shutil
import struct
import subprocess
import sys
import time
from pathlib import Path

from ui import ROOT, SetupError, ask, info, warn

sys.path.insert(0, str(ROOT / "tools" / "port"))
import prepare_game_dir  # noqa: E402

DISC_ID = "ULUS10566"
DISC_VERSION = "1.00"
# docs/decomp/usa-comparison.md and docs/decomp/iso-inventory-usa.json: Redump's USA v1.00 entry.
ISO_SIZE = 1754529792
ISO_SHA1 = "ec23f5beac89312a2a25654e202ae1c997999c9e"
EBOOT_SHA256 = prepare_game_dir.EBOOT_SHA256
EBOOT_SIZE = 4640948
DECRYPT_TIMEOUT = 60  # seconds; decrypting takes a second or two
DISC_FILES = {  # path on the disc: (folder under build/port, SHA-256)
    "/PSP_GAME/USRDIR/DATA/OVL_MENU_APP.ELF":
        ("ovl", "f021f96fa05da5907563a28c232966aa8a344526d6b8bdee728053ec75224d0a"),
    "/PSP_GAME/USRDIR/DATA/OVL_BATTLE_APP.ELF":
        ("ovl", "1f96cfb3be771ff6e94741ae3916fab340df74e36ede07ca39545d015f4c49dc"),
    "/PSP_GAME/USRDIR/DATA/OVL_EXTRA_APP.ELF":
        ("ovl", "f6b7be5e15c28774985417f1ec2c64f6655e79cc3c9dce7600fa7f6bacabca01"),
    "/PSP_GAME/USRDIR/DATA/MODULE/LIBSUPPREACC.PRX":
        ("prx", "aed4d30f80b1e83561bdfdff4ddfc47dead5b29694be314e253bee69f61f8778"),
}
PORT = ROOT / "build" / "port"
LAUNCHER = PORT / "launcher"
ICON = LAUNCHER / "dissidia012.ico"
SETUP_DIR = PORT / "setup"
DECRYPT_DIR = SETUP_DIR / "decrypt"  # D012Native --install's data directory, removed afterwards
LOG_DIR = PORT / "logs"

def sha256_file(path, progress=None):
    digest = hashlib.sha256()
    with open(path, "rb") as stream:
        for block in iter(lambda: stream.read(1 << 20), b""):
            digest.update(block)
    return digest.hexdigest()


def sha1_with_progress(path, size):
    digest = hashlib.sha1()
    done = 0
    shown = -1
    started = time.monotonic()
    with open(path, "rb") as stream:
        for block in iter(lambda: stream.read(4 << 20), b""):
            digest.update(block)
            done += len(block)
            tenth = done * 10 // max(size, 1)
            if tenth != shown and sys.stdout.isatty():
                shown = tenth
                print(f"\r    checking the ISO's SHA-1 (once): {tenth * 10}%", end="", flush=True)
    if not sys.stdout.isatty():
        print("    checking the ISO's SHA-1 (once): 100%", end="")
    print(f" ({time.monotonic() - started:.0f} s)")
    return digest.hexdigest()


def parse_sfo(data):
    """PARAM.SFO's keys and values (strings and integers)."""
    if data[:4] != b"\0PSF":
        raise ValueError("not a PARAM.SFO")
    key_table, data_table, count = struct.unpack_from("<III", data, 8)
    values = {}
    for index in range(count):
        key_offset, fmt, length, _max_length, data_offset = struct.unpack_from("<HHIII", data, 20 + index * 16)
        end = data.index(b"\0", key_table + key_offset)
        key = data[key_table + key_offset:end].decode("ascii", errors="replace")
        raw = data[data_table + data_offset:data_table + data_offset + length]
        if fmt == 0x0404:
            values[key] = struct.unpack("<I", raw[:4])[0]
        else:
            values[key] = raw.split(b"\0", 1)[0].decode("utf-8", errors="replace")
    return values


class Disc:
    """The ISO opened with pycdlib (read-only)."""

    def __init__(self, path):
        try:
            import pycdlib
        except ImportError:
            raise SetupError("pycdlib is missing from .venv; run setup.ps1 (it installs requirements-port.txt)")
        self.iso = pycdlib.PyCdlib()
        try:
            self.iso.open(str(path))
        except Exception as error:  # pycdlib raises its own exception types
            raise SetupError(f"{path} is not a readable ISO image ({error})")

    def read(self, iso_path):
        out = io.BytesIO()
        try:
            self.iso.get_file_from_iso_fp(out, iso_path=iso_path)
        except Exception:
            raise SetupError(f"the ISO has no {iso_path}: it is not a Dissidia 012 disc image")
        return out.getvalue()

    def close(self):
        self.iso.close()


def check_iso(path, state, skip_hash=False):
    """Raises SetupError with the reason when path is not the USA v1.00 disc; returns the Disc."""
    path = Path(path)
    if not path.is_file():
        raise SetupError(f"no such file: {path}")
    suffix = path.suffix.lower()
    if suffix in (".cso", ".chd", ".zso", ".7z", ".zip", ".rar", ".pbp"):
        raise SetupError(f"{path.name} is a {suffix} file; the port needs the plain .iso image "
                         "(decompress or extract it first)")
    disc = Disc(path)
    try:
        sfo = parse_sfo(disc.read("/PSP_GAME/PARAM.SFO"))
    except ValueError:
        disc.close()
        raise SetupError(f"{path.name}: its PARAM.SFO cannot be read")
    disc_id, version = sfo.get("DISC_ID"), sfo.get("DISC_VERSION")
    if disc_id != DISC_ID:
        disc.close()
        hint = " (the European release; the port supports the USA release only)" if disc_id == "ULES01505" else ""
        raise SetupError(f"{path.name} is disc {disc_id} \"{sfo.get('TITLE', '?')}\"{hint}; "
                         f"the port needs {DISC_ID} (Dissidia 012 Duodecim Final Fantasy, USA)")
    if version != DISC_VERSION:
        disc.close()
        raise SetupError(f"{path.name} is {DISC_ID} version {version}; the port needs version {DISC_VERSION}")
    size = path.stat().st_size
    if size != ISO_SIZE:
        disc.close()
        raise SetupError(f"{path.name} is {size:,} bytes; a clean dump of {DISC_ID} v{DISC_VERSION} is "
                         f"{ISO_SIZE:,} bytes (Redump). The image is trimmed, padded or modified.")
    for iso_path, (_folder, expected) in DISC_FILES.items():
        if hashlib.sha256(disc.read(iso_path)).hexdigest() != expected:
            disc.close()
            raise SetupError(f"{path.name}: {iso_path} differs from the retail disc's; the image is modified "
                             "or damaged. Use a clean dump of your disc.")
    key = [str(path.resolve()), size, path.stat().st_mtime_ns]
    if skip_hash:
        warn("the ISO's SHA-1 check was skipped (-SkipIsoHash)")
    elif state.get("iso_checked") != key:
        sha1 = sha1_with_progress(path, size)
        if sha1 != ISO_SHA1:
            disc.close()
            raise SetupError(f"{path.name}: SHA-1 {sha1}, but a clean dump of {DISC_ID} v{DISC_VERSION} has "
                             f"{ISO_SHA1} (Redump). Its game files look right, so it may still work: "
                             "run setup again with -SkipIsoHash to use it anyway.")
        state["iso_checked"] = key
    info(f"ISO: {DISC_ID} v{DISC_VERSION}, {size:,} bytes, matches the known-good dump")
    return disc


def check_eboot(path):
    path = Path(path)
    if not path.is_file():
        raise SetupError(f"no such file: {path}")
    with open(path, "rb") as stream:
        magic = stream.read(4)
    size = path.stat().st_size
    if magic == b"~PSP":
        raise SetupError(f"{path.name} is the encrypted EBOOT.BIN (it starts with ~PSP). The port needs the "
                         "decrypted executable, which starts with an ELF header. Leave -Eboot out and setup makes it "
                         "from your ISO.")
    if magic != b"\x7fELF":
        raise SetupError(f"{path.name} is not a PSP executable (no ELF header)")
    digest = sha256_file(path)
    if digest != EBOOT_SHA256:
        raise SetupError(
            f"{path.name} is an ELF file ({size:,} bytes), but not the decrypted executable the port knows.\n"
            f"    SHA-256 {digest}\n    expected {EBOOT_SHA256} ({EBOOT_SIZE:,} bytes)\n"
            "    Possible reasons: it comes from another release or version of the game (only USA v1.00, "
            "ULUS10566, is supported), it was patched, or it was written in another form. Leave -Eboot out "
            "and setup makes the right file from your ISO.")
    info(f"EBOOT: the decrypted ULUS10566 v1.00 executable ({size:,} bytes, SHA-256 matches)")


def extract_disc_files(disc):
    for iso_path, (folder, expected) in DISC_FILES.items():
        target = PORT / folder / iso_path.rsplit("/", 1)[1]
        if target.is_file() and sha256_file(target) == expected:
            continue
        data = disc.read(iso_path)
        if hashlib.sha256(data).hexdigest() != expected:
            raise SetupError(f"{iso_path}: differs from the retail disc's")
        target.parent.mkdir(parents=True, exist_ok=True)
        temporary = target.with_suffix(target.suffix + ".tmp")
        temporary.write_bytes(data)
        os.replace(temporary, target)
        info(f"extracted {iso_path} -> {target.relative_to(ROOT)}")
    info("overlays and LIBSUPPREACC.PRX: in build/port/ovl and build/port/prx")


def make_icon(disc):
    """build/port/launcher/dissidia012.ico from the disc's ICON0.PNG (kept if it already exists)."""
    if ICON.exists():
        return "kept"
    try:
        from PIL import Image
    except ImportError:
        warn("Pillow is missing, so the shortcut gets no icon")
        return "skipped"
    image = Image.open(io.BytesIO(disc.read("/PSP_GAME/ICON0.PNG"))).convert("RGBA")
    side = max(image.size)
    square = Image.new("RGBA", (side, side), (0, 0, 0, 0))
    square.paste(image, ((side - image.width) // 2, (side - image.height) // 2))
    LAUNCHER.mkdir(parents=True, exist_ok=True)
    sizes = [(s, s) for s in (16, 24, 32, 48, 64, 128) if s <= side]
    square.save(ICON, format="ICO", sizes=sizes)
    return "made from the disc's ICON0.PNG"


def ask_path(question, default, checker):
    while True:
        answer = ask(question, default)
        if not answer:
            continue
        try:
            return Path(answer).expanduser().resolve(), checker(Path(answer).expanduser())
        except SetupError as error:
            print()
            print(f"    That file cannot be used: {error}")
            print()


def ensure(state, iso=None, eboot=None, interactive=True, skip_iso_hash=False):
    """Checks the ISO and takes the overlays, the PRX and the icon from it. Returns (ISO path, decrypted
    EBOOT path or None); None means setup must make it from the ISO (decrypt(), then prepare())."""
    # The ISO
    disc = None
    if not iso and state.get("iso"):
        try:
            disc = check_iso(state["iso"], state, skip_iso_hash)
            iso_path = Path(state["iso"]).resolve()
        except SetupError as error:
            if not interactive:
                raise SetupError(f"the ISO used before cannot be used now: {error}. Pass -Iso <path>.")
            warn(f"the ISO used before cannot be used now: {error}")
    if disc:
        pass
    elif iso:
        disc = check_iso(iso, state, skip_iso_hash)
        iso_path = Path(iso).resolve()
    elif interactive:
        candidates = sorted(ROOT.glob("*.iso")) + sorted(ROOT.glob("*.ISO"))
        default = next((str(p) for p in candidates if p.stat().st_size == ISO_SIZE), "")
        info("Your Dissidia 012 Duodecim Final Fantasy disc image (USA, ULUS10566), as an .iso file.")
        info("Type or paste its full path, or drag the file into this window, then press Enter.")
        iso_path, disc = ask_path("ISO file", default, lambda p: check_iso(p, state, skip_iso_hash))
    else:
        raise SetupError("no ISO given: pass -Iso <path to your ULUS10566 .iso>")
    state["iso"] = str(iso_path)
    try:
        extract_disc_files(disc)
        info(f"icon: {make_icon(disc)}")
    finally:
        disc.close()

    # The decrypted EBOOT: -Eboot, else the game directory's if it is right, else one given before.
    target = PORT / "game" / "EBOOT.ELF"
    if eboot:
        check_eboot(eboot)
        state["eboot"] = str(Path(eboot).resolve())
        return iso_path, Path(eboot).resolve()
    if target.is_file() and sha256_file(target) == EBOOT_SHA256:
        info("EBOOT: build/port/game/EBOOT.ELF is the decrypted ULUS10566 v1.00 executable (kept)")
        return iso_path, target.resolve()
    remembered = state.get("eboot")
    if remembered and Path(remembered).is_file():
        try:
            check_eboot(remembered)
            return iso_path, Path(remembered).resolve()
        except SetupError as error:
            warn(f"the EBOOT given before cannot be used now ({error}); setup makes it from the ISO instead")
    state.pop("eboot", None)
    return iso_path, None


def decrypt(program, iso_path):
    """Runs `program --install <iso> --in-place` with its data directory in a temporary folder, and
    returns the EBOOT.ELF it wrote there, checked. The caller removes the folder (cleanup())."""
    cleanup()
    DECRYPT_DIR.mkdir(parents=True)
    env = dict(os.environ)
    env["D012_DATA_DIR"] = str(DECRYPT_DIR)
    env["D012_GAME_DIR"] = str(DECRYPT_DIR)
    LOG_DIR.mkdir(parents=True, exist_ok=True)
    log = LOG_DIR / "setup-decrypt.log"
    started = time.monotonic()
    try:
        result = subprocess.run([str(program), "--install", str(iso_path), "--in-place"], cwd=str(DECRYPT_DIR),
                                env=env, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT, timeout=DECRYPT_TIMEOUT)
        output = result.stdout.decode("utf-8", errors="replace")
        code = result.returncode
    except subprocess.TimeoutExpired as error:  # subprocess.run stops the program it started
        output = (error.stdout or b"").decode("utf-8", errors="replace")
        log.write_text(output, encoding="utf-8")
        raise SetupError(f"decrypting the EBOOT did not finish in {DECRYPT_TIMEOUT} s (output: {log})")
    log.write_text(output, encoding="utf-8")
    eboot = DECRYPT_DIR / "EBOOT.ELF"
    if code != 0 or not eboot.is_file():
        print()
        for line in output.strip().splitlines()[-20:]:
            print(f"      | {line}")
        raise SetupError(f"{Path(program).name} --install could not decrypt the EBOOT from the ISO (exit code "
                         f"{code}; output: {log}). If you have the decrypted EBOOT, pass -Eboot <path>.")
    digest = sha256_file(eboot)
    if digest != EBOOT_SHA256:
        raise SetupError(f"the decrypted EBOOT has SHA-256 {digest}, expected {EBOOT_SHA256}")
    info(f"EBOOT: decrypted from the ISO in {time.monotonic() - started:.1f} s "
         f"({eboot.stat().st_size:,} bytes, SHA-256 matches)")
    return eboot


def cleanup():
    """Removes the temporary folder of decrypt()."""
    if DECRYPT_DIR.exists():
        shutil.rmtree(DECRYPT_DIR)


def prepare(iso_path, eboot_path):
    """build/port/game: EBOOT.ELF, disc.iso and ms0/."""
    for line in prepare_game_dir.prepare(eboot_path, iso_path, PORT / "game", PORT / "data"):
        info(line)

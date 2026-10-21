"""Deploy the packaged port to the Steam Deck over the home network, sending only what changed.

    python tools/port/deploy_deck.py [--host deck@steamdeck.local] [--dest /home/deck/Games/Dissidia012]
                                     [--identity KEY] [--dry-run] [--local-target DIR] [--no-build] [--rehash]

Defaults for --host, --dest and --identity come from tools/local/deck.json when it exists
(git-ignored, so the owner's host and key path stay out of the repository):

    {"host": "deck@...", "identity": "<path to your private key>", "dest": "/home/deck/Games/Dissidia012"}

Command-line options win over the file.

First builds the release target and refreshes build/port/deck-package/Dissidia012 with
package_deck.py (--no-build uses the package as it is). Then it compares the package's
SHA-256 manifest with the .deploy-manifest.json the last deploy left in the destination
and:

  - sends only new and changed files (the 1.7 GB disc image and EBOOT.ELF only when they
    changed), through one ssh connection as a tar stream, into a staging folder first;
  - moves them into place, removes the files the last deploy sent that the package no
    longer has, and writes the new manifest last, so an interrupted deploy is redone;
  - never changes, removes or reads anything in the destination's data/ folder, where
    the Deck's saves, settings and mods live. The one exception: when data/settings.ini
    does not exist yet (the first deploy), the package's Deck defaults are put there.

With no manifest in the destination (a first deploy, or one that was interrupted), the
destination's files are hashed instead, so files already there are not sent again;
--rehash does that even with a manifest. Files no manifest lists are never removed.

--local-target DIR deploys to a local folder instead, with the same logic (for tests, or
a USB drive). --dry-run says what would be sent and removed, and changes nothing there.

ssh: Windows' own OpenSSH (ssh.exe), key-based only: every call has BatchMode=yes (it never
asks for a password) and ConnectTimeout=10. If it cannot connect, it prints the one-time
setup (docs/port/steam-deck.md). The destination folder is made if it is missing.
"""

import argparse
import io
import json
import os
import shlex
import shutil
import subprocess
import sys
import tarfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import package_deck  # noqa: E402

ROOT = package_deck.ROOT
PACKAGE = package_deck.OUT_DIR / package_deck.PACKAGE_NAME
HASH_CACHE = package_deck.OUT_DIR / ".hash-cache.json"
MANIFEST = ".deploy-manifest.json"
STAGING = ".deploy-staging"
SEED = "data/settings.ini"
CONFIG = ROOT / "tools/local/deck.json"
DEFAULT_HOST = "deck@steamdeck.local"  # SteamOS's own default user and network name
DEFAULT_DEST = "/home/deck/Games/Dissidia012"

SETUP = """
Cannot reach {host} with a key (ssh said: {error}).

One-time setup (docs/port/steam-deck.md has the details):

  On the Deck, in Desktop Mode, open Konsole and run:
      passwd                              (only if you have never set a password)
      sudo systemctl enable --now sshd

  On this PC, in PowerShell (KEY is your private key, KEY.pub its public half):
      ssh-keygen -t ed25519 -f KEY        (only if you have no key yet)
      type KEY.pub | ssh {host} "mkdir -p ~/.ssh && chmod 700 ~/.ssh && cat >> ~/.ssh/authorized_keys && chmod 600 ~/.ssh/authorized_keys"
      ssh -i KEY -o BatchMode=yes {host} true     (must return without asking for anything)
  Then name the key in tools/local/deck.json ("identity") or pass --identity KEY.

  If steamdeck.local is not found, use the Deck's IP address instead
  (Settings > Internet on the Deck, or `ip -4 addr` in Konsole): --host deck@192.168.x.y
"""


def is_data(relative):
    return relative == "data" or relative.startswith("data/")


def log(message):
    print(f"deploy: {message}", flush=True)


def human(size):
    for unit in ("B", "KiB", "MiB", "GiB"):
        if size < 1024 or unit == "GiB":
            return f"{size:.0f} {unit}" if unit == "B" else f"{size:.1f} {unit}"
        size /= 1024


# ---------------------------------------------------------------- the package's manifest

def package_manifest(package):
    """{relative path: {"sha256", "size"}} for every file but data/ and dot files."""
    try:
        cache = json.loads(HASH_CACHE.read_text(encoding="utf-8"))
    except (OSError, ValueError):
        cache = {}
    files, new_cache = {}, {}
    for path in sorted(package.rglob("*")):
        if not path.is_file():
            continue
        relative = path.relative_to(package).as_posix()
        if is_data(relative) or any(part.startswith(".") for part in relative.split("/")):
            continue
        stat = path.stat()
        key = [stat.st_size, stat.st_mtime_ns]
        cached = cache.get(relative)
        if cached and cached[:2] == key:
            digest = cached[2]
        else:
            if stat.st_size > 64 << 20:
                log(f"hashing {relative} ({human(stat.st_size)})")
            digest = package_deck.sha256_file(path)
        new_cache[relative] = key + [digest]
        files[relative] = {"sha256": digest, "size": stat.st_size}
    HASH_CACHE.parent.mkdir(parents=True, exist_ok=True)
    HASH_CACHE.write_text(json.dumps(new_cache, indent=1), encoding="utf-8")
    return files


# ---------------------------------------------------------------- destinations

class LocalTarget:
    def __init__(self, folder):
        self.folder = Path(folder).resolve()
        self.name = str(self.folder)

    def check(self):
        pass

    def read_manifest(self):
        try:
            return json.loads((self.folder / MANIFEST).read_text(encoding="utf-8"))
        except FileNotFoundError:
            return None

    def hash_files(self):
        files = {}
        if not self.folder.is_dir():
            return files
        for path in sorted(self.folder.rglob("*")):
            relative = path.relative_to(self.folder).as_posix()
            if not path.is_file() or is_data(relative) or relative == MANIFEST or relative.startswith(STAGING):
                continue
            files[relative] = {"sha256": package_deck.sha256_file(path), "size": path.stat().st_size}
        return files

    def has_seed(self):
        return (self.folder / SEED).exists()

    def apply(self, package, uploads, removals, seed, manifest_text):
        self.folder.mkdir(parents=True, exist_ok=True)
        staging = self.folder / STAGING
        if staging.exists():
            shutil.rmtree(staging)
        staging.mkdir()
        for relative in uploads + ([SEED] if seed else []):
            target = staging / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(package / relative, target)  # a real copy, never a link
            shutil.copystat(package / relative, target)
        for relative in uploads:
            target = self.folder / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            os.replace(staging / relative, target)
        if seed and not (self.folder / SEED).exists():
            (self.folder / "data").mkdir(parents=True, exist_ok=True)
            os.replace(staging / SEED, self.folder / SEED)
        for relative in removals:
            target = self.folder / relative
            if target.is_file():
                target.unlink()
            prune_empty(target.parent, self.folder)
        partial = staging / MANIFEST
        partial.write_text(manifest_text, encoding="utf-8", newline="\n")
        os.replace(partial, self.folder / MANIFEST)
        shutil.rmtree(staging)


def prune_empty(directory, top):
    while directory != top and top in directory.parents and directory.name != "data":
        try:
            directory.rmdir()
        except OSError:
            return
        directory = directory.parent


class SshTarget:
    def __init__(self, host, dest, identity=None):
        self.host = host
        self.dest = dest.rstrip("/")
        self.identity = identity
        self.name = f"{host}:{self.dest}"
        system = os.environ.get("SystemRoot", r"C:\Windows")
        self.ssh = self.find(Path(system) / "System32/OpenSSH/ssh.exe", "ssh")
        self.scp = self.find(Path(system) / "System32/OpenSSH/scp.exe", "scp")  # checked; tar over ssh is used

    def ssh_args(self, command):
        args = [self.ssh, "-o", "BatchMode=yes", "-o", "ConnectTimeout=10", "-o", "ServerAliveInterval=15"]
        if self.identity:
            args += ["-i", str(self.identity), "-o", "IdentitiesOnly=yes"]
        return args + [self.host, command]

    @staticmethod
    def find(path, name):
        if path.exists():
            return str(path)
        found = shutil.which(name)
        if not found:
            raise SystemExit(f"deploy: {name}.exe not found. Windows' OpenSSH Client is an optional feature "
                             "(Settings > System > Optional features); this script installs nothing.")
        return found

    def run(self, command, stdin=None, capture=True):
        return subprocess.run(self.ssh_args(command), input=stdin, capture_output=capture)

    def check(self):
        result = self.run("true")
        if result.returncode != 0:
            error = result.stderr.decode(errors="replace").strip().splitlines()
            print(SETUP.format(host=self.host, error=error[-1] if error else f"exit {result.returncode}"))
            raise SystemExit(2)

    def remote(self, command, stdin=None):
        result = self.run(command, stdin=stdin)
        if result.returncode != 0:
            raise SystemExit(f"deploy: remote command failed ({result.returncode}): "
                             f"{result.stderr.decode(errors='replace').strip()}")
        return result.stdout.decode(errors="replace")

    def read_manifest(self):
        path = shlex.quote(f"{self.dest}/{MANIFEST}")
        text = self.remote(f"if [ -f {path} ]; then cat {path}; fi")
        return json.loads(text) if text.strip() else None

    def hash_files(self):
        dest = shlex.quote(self.dest)
        text = self.remote(f"cd {dest} 2>/dev/null || exit 0; find . -type f ! -path './data/*' "
                           f"! -path './{STAGING}/*' ! -name {MANIFEST} -print0 | xargs -0 -r sha256sum")
        sizes_text = self.remote(f"cd {dest} 2>/dev/null || exit 0; find . -type f ! -path './data/*' "
                                 f"! -path './{STAGING}/*' ! -name {MANIFEST} -printf '%s %p\\n'")
        sizes = {}
        for line in sizes_text.splitlines():
            size, _, path = line.partition(" ")
            sizes[path[2:]] = int(size)
        files = {}
        for line in text.splitlines():
            digest, _, path = line.partition("  ")
            relative = path[2:] if path.startswith("./") else path
            files[relative] = {"sha256": digest, "size": sizes.get(relative, -1)}
        return files

    def has_seed(self):
        path = shlex.quote(f"{self.dest}/{SEED}")
        return self.remote(f"if [ -e {path} ]; then echo yes; fi").strip() == "yes"

    def apply(self, package, uploads, removals, seed, manifest_text):
        dest = self.dest
        staging = f"{dest}/{STAGING}"
        q = shlex.quote
        # 1. One connection: everything new, the manifest included, into the staging folder.
        command = (f"mkdir -p {q(dest)} && rm -rf {q(staging)} && mkdir -p {q(staging)} && "
                   f"tar -xf - -C {q(staging)}")
        process = subprocess.Popen(self.ssh_args(command), stdin=subprocess.PIPE)
        try:
            with tarfile.open(fileobj=process.stdin, mode="w|", format=tarfile.PAX_FORMAT) as tar:
                for relative in uploads + ([SEED] if seed else []):
                    path = package / relative
                    if path.stat().st_size > 64 << 20:
                        log(f"sending {relative} ({human(path.stat().st_size)})")
                    info = tar.gettarinfo(str(path), arcname=relative)
                    info.uid = info.gid = 0
                    info.uname = info.gname = ""
                    info.mode = 0o755 if relative.endswith(".exe") else 0o644
                    with open(path, "rb") as stream:
                        tar.addfile(info, stream)
                data = manifest_text.encode("utf-8")
                info = tarfile.TarInfo(MANIFEST)
                info.size = len(data)
                info.mode = 0o644
                tar.addfile(info, io.BytesIO(data))
            process.stdin.close()
        except BrokenPipeError:
            pass
        if process.wait() != 0:
            raise SystemExit(f"deploy: sending failed ({process.returncode}); nothing was moved into place")
        # 2. Move into place, remove what went away, the manifest last.
        lines = ["set -e"]
        for relative in uploads:
            target = f"{dest}/{relative}"
            lines.append(f"mkdir -p {q(target.rsplit('/', 1)[0])}")
            lines.append(f"mv -f {q(f'{staging}/{relative}')} {q(target)}")
        if seed:
            lines.append(f"mkdir -p {q(dest + '/data')}")
            lines.append(f"[ -e {q(dest + '/' + SEED)} ] || mv {q(staging + '/' + SEED)} {q(dest + '/' + SEED)}")
        for relative in removals:
            target = f"{dest}/{relative}"
            lines.append(f"rm -f {q(target)}")
            parent = target.rsplit("/", 1)[0]
            if parent != dest and not is_data(parent[len(dest) + 1:]):
                lines.append(f"rmdir -p --ignore-fail-on-non-empty {q(parent)} 2>/dev/null || true")
        lines.append(f"mv -f {q(staging + '/' + MANIFEST)} {q(dest + '/' + MANIFEST)}")
        lines.append(f"rm -rf {q(staging)}")
        self.remote("sh -s", stdin=("\n".join(lines) + "\n").encode("utf-8"))


# ---------------------------------------------------------------- the deploy

def plan(local, remote):
    uploads = [relative for relative, entry in local.items()
               if relative not in remote or remote[relative]["sha256"] != entry["sha256"]]
    return uploads


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    config = json.loads(CONFIG.read_text(encoding="utf-8")) if CONFIG.exists() else {}
    parser.add_argument("--host", default=config.get("host", DEFAULT_HOST))
    parser.add_argument("--dest", default=config.get("dest", DEFAULT_DEST))
    parser.add_argument("--identity", default=config.get("identity"), help="private key for ssh -i")
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--local-target", type=Path, help="deploy to this local folder instead of over ssh")
    parser.add_argument("--no-build", action="store_true", help="use the package as it is")
    parser.add_argument("--rehash", action="store_true", help="hash the destination's files, not its manifest")
    parser.add_argument("--package", type=Path, default=PACKAGE, help=argparse.SUPPRESS)
    options = parser.parse_args()

    if options.identity and not options.local_target and not Path(options.identity).is_file():
        raise SystemExit(f"deploy: the key {options.identity} does not exist")
    target = (LocalTarget(options.local_target) if options.local_target
              else SshTarget(options.host, options.dest, options.identity))
    target.check()  # before the build, so a missing key is found at once

    package = options.package.resolve()
    if not options.no_build:
        package_deck.check_release_build(package_deck.BUILD_DIR)
        package_deck.build_release(package_deck.BUILD_DIR)
        package = package_deck.assemble(package_deck.BUILD_DIR, package_deck.OUT_DIR, package_deck.GAME_DIR, False)
    if not (package / package_deck.EXE_NAME).exists():
        raise SystemExit(f"deploy: no package at {package} (run tools/port/package_deck.py)")

    local = package_manifest(package)
    previous = target.read_manifest()
    previous_files = (previous or {}).get("files", {})
    if previous is None or options.rehash:
        log(f"hashing the files already in {target.name}" + ("" if previous else " (no manifest there)"))
        present = target.hash_files()
    else:
        present = previous_files
    uploads = plan(local, present)
    # Only files a deploy sent are ever removed; data/ never.
    removals = sorted(r for r in previous_files if r not in local and not is_data(r) and r != MANIFEST)
    seed = (package / SEED).exists() and not target.has_seed()
    for relative in uploads + removals:
        assert not is_data(relative), relative

    manifest = {"version": 1, "package": package_deck.PACKAGE_NAME, "files": local}
    manifest_text = json.dumps(manifest, indent=1, sort_keys=True) + "\n"
    up_to_date = not uploads and not removals and not seed and previous is not None and previous_files == local

    size = sum(local[r]["size"] for r in uploads)
    log(f"target {target.name}")
    log(f"{len(local)} files in the package; send {len(uploads)} ({human(size)}), remove {len(removals)}"
        + (", put the Deck defaults in data/settings.ini (none there yet)" if seed else ""))
    for relative in uploads:
        log(f"  send   {relative} ({human(local[relative]['size'])})")
    for relative in removals:
        log(f"  remove {relative}")
    if up_to_date:
        log("up to date; nothing sent")
        return 0
    if options.dry_run:
        log("dry run; nothing changed")
        return 0
    target.apply(package, uploads, removals, seed, manifest_text)
    log("done; restart the game on the Deck to use the new build")
    return 0


if __name__ == "__main__":
    sys.exit(main())

"""PortableKit: a clone in tools/local/port/PortableKit on the local branch "dissidia", which is
PortableKit 33800c4 plus this repository's patches (port/portablekit-patches/*.patch, applied with git am).

The patches are applied in a temporary worktree with a throwaway committer identity (the patches keep
their authors, and the committer date is the author date, so the result is the same commit on every
machine); "dissidia" is then moved to that commit. A checkout only rewrites the files that differ, so a
re-run after `git pull` rebuilds only what the changed patches touch. If "dissidia" already has the same
files (the same tree), it is left as it is.
"""

import hashlib
import shutil
import subprocess
from pathlib import Path

from ui import ROOT, SetupError, info

PK_DIR = ROOT / "tools" / "local" / "port" / "PortableKit"
APPLY_DIR = ROOT / "tools" / "local" / "port" / ".portablekit-apply"
PATCH_DIR = ROOT / "port" / "portablekit-patches"
UPSTREAM = "https://github.com/TeamGDB/PortableKit.git"
BASE = "33800c4060c53f3233e08f8ab2cc4590a0872063"
BRANCH = "dissidia"
IDENTITY = ["-c", "user.name=Dissidia 012 port setup", "-c", "user.email=setup@dissidia012.invalid"]


def git(*args, cwd=PK_DIR, check=True, capture=True):
    result = subprocess.run(["git", *args], cwd=str(cwd), stdin=subprocess.DEVNULL,
                            stdout=subprocess.PIPE if capture else None,
                            stderr=subprocess.PIPE if capture else None, text=True)
    if check and result.returncode != 0:
        detail = (result.stderr or result.stdout or "").strip() if capture else ""
        raise SetupError(f"git {' '.join(args[:3])} failed in {cwd}:\n      {detail}")
    return result.stdout.strip() if capture else ""


def patches():
    found = sorted(PATCH_DIR.glob("*.patch"))
    if not found:
        raise SetupError(f"no patches in {PATCH_DIR}")
    return found


def patch_digest(found):
    digest = hashlib.sha256(BASE.encode())
    for path in found:
        digest.update(path.name.encode() + b"\0" + path.read_bytes() + b"\0")
    return digest.hexdigest()


def rev(name):
    result = subprocess.run(["git", "rev-parse", "--verify", "--quiet", name], cwd=str(PK_DIR),
                            stdout=subprocess.PIPE, stderr=subprocess.DEVNULL, text=True)
    return result.stdout.strip() if result.returncode == 0 else None


def clone(source):
    if PK_DIR.exists() and any(PK_DIR.iterdir()):
        raise SetupError(f"{PK_DIR} exists but is not a git clone; move it away and run setup again")
    PK_DIR.parent.mkdir(parents=True, exist_ok=True)
    info(f"cloning PortableKit from {source}")
    git("clone", "--quiet", str(source), str(PK_DIR), cwd=ROOT)
    if str(source) != UPSTREAM:
        git("remote", "set-url", "origin", UPSTREAM)


def remove_apply_dir():
    if APPLY_DIR.exists():
        git("worktree", "remove", "--force", str(APPLY_DIR), check=False)
        shutil.rmtree(APPLY_DIR, ignore_errors=True)
    git("worktree", "prune", check=False)


def apply_patches(found):
    """The commit that BASE plus the patches makes, built in a temporary worktree."""
    remove_apply_dir()
    git("-c", "core.autocrlf=false", "worktree", "add", "--detach", "--quiet", str(APPLY_DIR), BASE)
    try:
        result = subprocess.run(["git", "-c", "core.autocrlf=false", *IDENTITY, "am", "--quiet",
                                 "--committer-date-is-author-date", *[str(p) for p in found]],
                                cwd=str(APPLY_DIR), stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT, text=True)
        if result.returncode != 0:
            subprocess.run(["git", "am", "--abort"], cwd=str(APPLY_DIR), stdin=subprocess.DEVNULL,
                           stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            raise SetupError("the PortableKit patches do not apply to 33800c4:\n      "
                             + "\n      ".join(result.stdout.strip().splitlines()[-12:]))
        return git("rev-parse", "HEAD", cwd=APPLY_DIR)
    finally:
        remove_apply_dir()


def ensure(state, source=None, force=False):
    found = patches()
    digest = patch_digest(found)
    fresh = not (PK_DIR / ".git").exists()
    if fresh:
        clone(source or UPSTREAM)
    if subprocess.run(["git", "cat-file", "-e", f"{BASE}^{{commit}}"], cwd=str(PK_DIR),
                      stderr=subprocess.DEVNULL).returncode != 0:
        info("fetching PortableKit's history for 33800c4")
        git("fetch", "--quiet", "origin")
        if rev(f"{BASE}^{{commit}}") is None:
            raise SetupError(f"PortableKit commit {BASE[:7]} is not in {PK_DIR} or its origin")
    dirty = git("status", "--porcelain", "--untracked-files=no")
    if dirty:
        raise SetupError(f"{PK_DIR} has uncommitted changes:\n      {dirty.splitlines()[0]} ...\n"
                         "    Commit or stash them there (or delete the folder to start over), then run setup again.")
    head_ref = git("symbolic-ref", "--quiet", "HEAD", check=False)
    current = rev(f"refs/heads/{BRANCH}")
    if (not force and state.get("portablekit_patches") == digest and current
            and current == state.get("portablekit_commit") and head_ref == f"refs/heads/{BRANCH}"):
        info(f"branch {BRANCH} is up to date ({len(found)} patches on 33800c4, {current[:10]})")
        return False

    info(f"applying {len(found)} patches to 33800c4 (git am, in a temporary worktree)")
    new = apply_patches(found)
    changed = True
    if current and rev(f"{current}^{{tree}}") == rev(f"{new}^{{tree}}"):
        info(f"branch {BRANCH} already has exactly these files ({current[:10]}); left as it is")
        new, changed = current, False
        if head_ref != f"refs/heads/{BRANCH}":
            git("checkout", "--quiet", BRANCH)
    else:
        if current:
            info(f"moving branch {BRANCH} from {current[:10]} to {new[:10]} (the patches changed)")
        git("checkout", "--quiet", "-B", BRANCH, new)
    state["portablekit_patches"] = digest
    state["portablekit_commit"] = new
    info(f"PortableKit {BRANCH} = 33800c4 + {len(found)} patches ({new[:10]})")
    return changed

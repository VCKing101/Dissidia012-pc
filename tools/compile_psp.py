"""Run the pinned historical PSP compiler through wibo in Ubuntu WSL.

Compilers driven by wibo run in batches from a Linux-local mirror
(tools/snc_batch_guest.py): the sources and headers travel in one tar, so a
batch costs one WSL start instead of seconds of slow-mount I/O per file.
Set D012_SLOW_COMPILE=1 to use the original one-process-per-file path.

Sony psp-gcc (tools/compilers/psp-gcc-*/bin/psp-gcc) runs natively in the
same batches: -S, then that toolchain's psp-as. Build profiles pass -G0
themselves; a GCC call without -G0 is a compiler probe
(tools/probe_compilers.py) and keeps the original probe script.
"""

import hashlib
import io
import json
import os
import re
import subprocess
import tarfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
_PREFETCHED = {}


def _linux_root():
    return "/mnt/" + ROOT.drive[0].lower() + ROOT.as_posix()[2:]


def compiler_command(executable, arguments):
    guest_command = ["tools/local/wibo", Path(executable).as_posix(), *arguments]
    if os.name == "nt":
        return ["wsl", "-d", "Ubuntu-24.04", "--cd", _linux_root(), "--", *guest_command]
    return guest_command


def _is_gcc(executable):
    return "psp-gcc-" in Path(executable).as_posix()


# Driver-internal directory (cc1, specs) and assembler of each Sony GCC.
_GCC = {"psp-gcc-1.3.1": ("bin/psp-gcc", "lib/gcc-lib/psp/3.3.3+allegrex-2.2.1-psp-1.3.1", "bin/psp-as")}


def gcc_toolchain(executable):
    """(root, driver, internal dir, assembler) of a Sony psp-gcc, all relative to ROOT."""
    path = Path(executable).as_posix()
    root = path[:path.index("/bin/")] if "/bin/" in path else path
    name = Path(root).name
    if name not in _GCC or path != f"{root}/{_GCC[name][0]}":
        raise ValueError(f"Unsupported GCC build compiler: {executable}")
    driver, internal, assembler = _GCC[name]
    return root, driver, internal, assembler


def _includes(executable):
    if _is_gcc(executable):
        return ["-nostdinc", "-Iinclude"]
    return ["-Iinclude"] if Path(executable).name.lower() == "pspsnc.exe" else ["-nostdinc", "-cwd", "include", "-i", "include"]


def _compile_slow(executable, flags, source, output):
    args = [*flags, *_includes(executable), "-c", Path(source).as_posix(), "-o", Path(output).as_posix()]
    return subprocess.run(compiler_command(executable, args), cwd=ROOT, capture_output=True, text=True, timeout=45)


def _stamp(directory):
    digest = hashlib.sha256()
    for path in sorted(p for p in (ROOT / directory).rglob("*") if p.is_file()):
        stat = path.stat()
        digest.update(f"{path.relative_to(ROOT).as_posix()}\0{stat.st_size}\0{stat.st_mtime_ns}\n".encode())
    wibo = (ROOT / "tools/local/wibo").stat()
    digest.update(f"wibo\0{wibo.st_size}\0{wibo.st_mtime_ns}".encode())
    return digest.hexdigest()


def _relative(path):
    path = Path(path)
    if path.is_absolute():
        path = Path(os.path.abspath(path)).relative_to(ROOT)
    return path.as_posix()


_HEADER_SUFFIXES = (".h", ".hpp", ".hh", ".inc", ".inl")
_INCLUDE = re.compile(r'^\s*#\s*include\s*"([^"]+)"', re.M)


def _source_tree(sources):
    """Every header under include/ and src/, each job, and files the jobs #include by path."""
    files = {}
    for root in (ROOT / "include", ROOT / "src"):
        if root.exists():
            for path in root.rglob("*"):
                if path.suffix.lower() in _HEADER_SUFFIXES and path.is_file():
                    files[path.relative_to(ROOT).as_posix()] = path
    pending = [ROOT / source for source in sources]
    for directory in {path.parent for path in pending}:
        for path in directory.iterdir():
            if path.suffix.lower() in _HEADER_SUFFIXES and path.is_file():
                files[Path(os.path.abspath(path)).relative_to(ROOT).as_posix()] = path
    seen = set()
    while pending:
        path = pending.pop()
        key = Path(os.path.abspath(path)).relative_to(ROOT).as_posix()
        if key in seen:
            continue
        seen.add(key)
        files[key] = path
        try:
            text = path.read_text(encoding="utf-8", errors="replace")
        except OSError:
            continue
        for name in _INCLUDE.findall(text):
            for base in (path.parent, ROOT / "include"):
                candidate = Path(os.path.normpath(base / name))
                if candidate.is_file() and candidate.is_relative_to(ROOT):
                    pending.append(candidate)
                    break
    return files


def compile_many(executable, flags, jobs):
    """Compile [(source, output)] in one WSL call; return CompletedProcess-like results."""
    jobs = [(_relative(source), _relative(output)) for source, output in jobs]
    if not jobs:
        return []
    executable_path = Path(executable)
    manifest = {"root": _linux_root() if os.name == "nt" else ROOT.as_posix(),
                "compiler_dir": executable_path.parent.as_posix(),
                "executable": executable_path.name,
                "flags": list(flags), "includes": _includes(executable),
                "jobs": [{"source": source} for source, _ in jobs]}
    if _is_gcc(executable):
        root, driver, internal, assembler = gcc_toolchain(executable)
        manifest.update(kind="gcc", compiler_dir=root, executable=driver, internal=internal,
                        assembler=assembler, assembler_flags=["-EL", "-G0"])
    manifest["stamp"] = _stamp(manifest["compiler_dir"])
    buffer = io.BytesIO()
    with tarfile.open(fileobj=buffer, mode="w") as archive:
        data = json.dumps(manifest).encode()
        info = tarfile.TarInfo("__manifest__.json")
        info.size = len(data)
        archive.addfile(info, io.BytesIO(data))
        for name, path in sorted(_source_tree([s for s, _ in jobs]).items()):
            archive.add(path, arcname=name, recursive=False)
    guest = ["python3", "tools/snc_batch_guest.py"]
    command = ["wsl", "-d", "Ubuntu-24.04", "--cd", _linux_root(), "--", *guest] if os.name == "nt" else guest
    process = subprocess.run(command, cwd=ROOT, input=buffer.getvalue(), capture_output=True, timeout=600)
    if process.returncode:
        raise RuntimeError("Batch compiler failed: " + process.stderr.decode(errors="replace")[-2000:])
    results = []
    with tarfile.open(fileobj=io.BytesIO(process.stdout)) as archive:
        records = json.load(archive.extractfile("__results__.json"))
        for (source, output), record in zip(jobs, records):
            if record["object"]:
                target = ROOT / output
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_bytes(archive.extractfile(record["object"]).read())
            results.append(subprocess.CompletedProcess(command, record["returncode"], record["stdout"], record["stderr"]))
    return results


def _probe_gcc(executable, flags):
    return _is_gcc(executable) and "-G0" not in flags


def _fast(executable, flags=()):
    return not _probe_gcc(executable, flags) and not os.environ.get("D012_SLOW_COMPILE")


def _key(executable, flags, source, output):
    data = (ROOT / source).read_bytes()
    return (Path(executable).as_posix(), tuple(flags), _relative(source), _relative(output), hashlib.sha256(data).hexdigest())


def prefetch(executable, flags, jobs):
    """Batch-compile jobs now; later compile_object calls for them return at once."""
    if not _fast(executable, flags):
        return
    jobs = [(_relative(s), _relative(o)) for s, o in jobs]
    if len({output for _, output in jobs}) != len(jobs):
        return  # shared outputs would overwrite each other; let callers reject or compile one by one
    for (source, output), result in zip(jobs, compile_many(executable, flags, jobs)):
        _PREFETCHED[_key(executable, flags, source, output)] = result


def compile_object(executable, flags, source, output):
    if _probe_gcc(executable, flags):
        compiler_name = Path(executable).parts[2]
        guest_command = ["bash", "tools/probe_legacy_gcc.sh", compiler_name, *flags,
                         Path(source).as_posix(), Path(output).as_posix()]
        if os.name == "nt":
            command = ["wsl", "-d", "Ubuntu-24.04", "--cd", _linux_root(), "--", *guest_command]
        else:
            command = guest_command
        return subprocess.run(command, cwd=ROOT, capture_output=True, text=True, timeout=45)
    if not _fast(executable, flags):
        if _is_gcc(executable):
            raise RuntimeError("D012_SLOW_COMPILE does not support GCC build profiles")
        return _compile_slow(executable, flags, source, output)
    key = _key(executable, flags, source, output)
    if key in _PREFETCHED and (ROOT / key[3]).exists():
        return _PREFETCHED.pop(key)
    return compile_many(executable, flags, [(source, output)])[0]


def unit_compiler(matches, unit):
    """(compiler, flags) for a unit: its named build profile, else the module's pinned one."""
    name = unit.get("profile")
    if name is None:
        return matches["compiler"], list(matches["flags"])
    profile = matches.get("profiles", {}).get(name)
    if not profile:
        raise ValueError(f"C unit {unit.get('name')} names an unknown build profile: {name}")
    return profile["compiler"], list(profile["flags"])


def prefetch_units(matches, units, output):
    """Batch-compile every unit, one batch per build profile; `output(unit)` names its object."""
    groups = {}
    for unit in units:
        compiler, flags = unit_compiler(matches, unit)
        groups.setdefault((compiler, tuple(flags)), []).append((unit["source"], output(unit)))
    for (compiler, flags), jobs in groups.items():
        prefetch(compiler, list(flags), jobs)


def compiler_files(executable):
    """Files whose bytes decide a compiler's output, for build provenance."""
    if not _is_gcc(executable):
        return [Path(executable).as_posix()]
    root, driver, internal, assembler = gcc_toolchain(executable)
    return [f"{root}/{driver}", f"{root}/{internal}/cc1", f"{root}/{internal}/specs", f"{root}/{assembler}"]


def repair_local_symbol_count(data):
    """Fix the old GNU as off-by-one in .symtab's sh_info (first global index).

    psp-gcc 1.3.1's psp-as places the FILE symbol last among the locals but
    counts it as global, which modern ld rejects ("local symbol at index N
    (>= sh_info of N)"). Only that header field changes, and only when every
    local symbol already precedes every global one. Returns the bytes.
    """
    import struct
    data = bytearray(data)
    shoff, = struct.unpack_from("<I", data, 32)
    shentsize, shnum = struct.unpack_from("<HH", data, 46)
    for index in range(shnum):
        header = shoff + index * shentsize
        kind, = struct.unpack_from("<I", data, header + 4)
        if kind != 2:  # SHT_SYMTAB
            continue
        offset, size, _, info, _, entsize = struct.unpack_from("<IIIIII", data, header + 16)
        binds = [data[offset + i * entsize + 12] >> 4 for i in range(size // entsize)]
        locals_end = next((i for i, bind in enumerate(binds) if bind != 0), len(binds))
        if any(bind == 0 for bind in binds[locals_end:]):
            raise ValueError("Object symbol table mixes local and global symbols")
        if info != locals_end:
            struct.pack_into("<I", data, header + 28, locals_end)
    return bytes(data)

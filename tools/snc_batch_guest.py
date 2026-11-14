"""Compile a batch of sources inside WSL from Linux-local copies.

Runs in the WSL guest (or on Linux). Reads a tar on stdin holding
__manifest__.json plus the source tree, compiles every job with a mirrored
compiler and wibo under /tmp, and writes a tar of objects and results to
stdout. Running the compiler from the Linux filesystem avoids the slow
Windows-drive mount, which costs seconds per invocation.

Two kinds of compiler are supported. "wibo" (the default) runs a Windows
compiler such as SNC under wibo with -c. "gcc" runs Sony's native 32-bit
psp-gcc: the driver writes assembly with -S and the same toolchain's psp-as
assembles it, which is how the SDK libraries in main were built.
"""

import fcntl
import io
import json
import os
import shutil
import subprocess
import sys
import tarfile
import tempfile
from concurrent.futures import ThreadPoolExecutor

MIRROR = "/tmp/d012-compilers"


def ensure_mirror(root, compiler_dir, stamp):
    os.makedirs(MIRROR, exist_ok=True)
    name = os.path.basename(compiler_dir.rstrip("/"))
    target = os.path.join(MIRROR, name)
    with open(os.path.join(MIRROR, ".lock"), "w") as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        stamp_path = os.path.join(target, ".stamp")
        current = open(stamp_path).read() if os.path.exists(stamp_path) else None
        if current != stamp:
            staging = target + ".new"
            shutil.rmtree(staging, ignore_errors=True)
            shutil.copytree(os.path.join(root, compiler_dir), os.path.join(staging, "compiler"))
            shutil.copy2(os.path.join(root, "tools/local/wibo"), os.path.join(staging, "wibo"))
            os.chmod(os.path.join(staging, "wibo"), 0o755)
            # The Windows drive mount may hide execute bits; the native GCC
            # driver, cc1 and psp-as all need them.
            for directory, _, names in os.walk(os.path.join(staging, "compiler")):
                for name in names:
                    path = os.path.join(directory, name)
                    os.chmod(path, os.stat(path).st_mode | 0o755)
            with open(os.path.join(staging, ".stamp"), "w") as stream:
                stream.write(stamp)
            shutil.rmtree(target, ignore_errors=True)
            os.rename(staging, target)
    return target


def run_step(command, work, env):
    """Run one compiler step; a hang counts as a failure so the serial retry gets it."""
    try:
        return subprocess.run(command, cwd=work, capture_output=True, text=True, timeout=120, env=env)
    except subprocess.TimeoutExpired:
        return subprocess.CompletedProcess(command, 124, "", "timed out after 120 s: " + " ".join(command) + "\n")


def main():
    archive = tarfile.open(fileobj=io.BytesIO(sys.stdin.buffer.read()))
    manifest = json.load(archive.extractfile("__manifest__.json"))
    mirror = ensure_mirror(manifest["root"], manifest["compiler_dir"], manifest["stamp"])
    work = tempfile.mkdtemp(prefix="d012-snc-")
    try:
        archive.extractall(work, filter="data")
        os.makedirs(os.path.join(work, "__out__"), exist_ok=True)
        output = io.BytesIO()
        with tarfile.open(fileobj=output, mode="w") as result_archive:
            def run(index_job):
                index, job = index_job
                obj = f"__out__/{index}.o"
                if os.path.exists(os.path.join(work, obj)):
                    os.remove(os.path.join(work, obj))
                # Each job gets its own temp directory: SNC hands its assembly
                # to pspas through a temp file.
                temp = os.path.join(work, "__tmp__", str(index))
                os.makedirs(temp, exist_ok=True)
                env = dict(os.environ, TMP=temp, TEMP=temp, TMPDIR=temp)
                compiler = os.path.join(mirror, "compiler")
                if manifest.get("kind") == "gcc":
                    assembly = f"__out__/{index}.s"
                    command = [os.path.join(compiler, manifest["executable"]),
                               "-B" + os.path.join(compiler, manifest["internal"]) + "/",
                               *manifest["flags"], *manifest["includes"], "-S", job["source"], "-o", assembly]
                    process = run_step(command, work, env)
                    if process.returncode == 0:
                        assembler = [os.path.join(compiler, manifest["assembler"]), *manifest["assembler_flags"],
                                     "-o", obj, assembly]
                        second = run_step(assembler, work, env)
                        process = subprocess.CompletedProcess(command, second.returncode, process.stdout + second.stdout,
                                                              process.stderr + second.stderr)
                else:
                    command = [os.path.join(mirror, "wibo"), os.path.join(compiler, manifest["executable"]),
                               *manifest["flags"], *manifest["includes"], "-c", job["source"], "-o", obj]
                    process = run_step(command, work, env)
                has_object = process.returncode == 0 and os.path.exists(os.path.join(work, obj))
                return {"returncode": process.returncode, "stdout": process.stdout,
                        "stderr": process.stderr, "object": obj if has_object else None}
            workers = min(len(manifest["jobs"]), os.cpu_count() or 4, 16) or 1
            with ThreadPoolExecutor(workers) as pool:
                results = list(pool.map(run, enumerate(manifest["jobs"])))
            # Under heavy parallel load pspas has been seen to read SNC's temp
            # file as empty, and wibo has been seen to hang. Retry every failure
            # once on its own; real source errors simply fail again.
            for index, job in enumerate(manifest["jobs"]):
                if results[index]["returncode"]:
                    results[index] = run((index, job))
            for record in results:
                if record["object"]:
                    result_archive.add(os.path.join(work, record["object"]), arcname=record["object"])
            data = json.dumps(results).encode()
            info = tarfile.TarInfo("__results__.json")
            info.size = len(data)
            result_archive.addfile(info, io.BytesIO(data))
        sys.stdout.buffer.write(output.getvalue())
    finally:
        shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    main()

"""Validate derived link offsets on PSP and real unlinking under sanitizers."""
import os
import subprocess
import unittest
from pathlib import Path
from compile_psp import compile_object

ROOT = Path(__file__).resolve().parents[1]


class TaskUnlinkChecks(unittest.TestCase):
    def test_psp_derived_link_offsets_match_named_fields(self):
        directory = ROOT / "build/task-unlink-tests"
        directory.mkdir(parents=True, exist_ok=True)
        source = directory / "layout.cpp"
        source.write_text(r'''
#include "main/task_unlink_layout.h"
#define OFFSET(t,m) ((unsigned int)&(((t *)0)->m))
typedef char NextDerived[(OFFSET(MainTask,field_0034)==sizeof(MainTask)-sizeof(TaskQueueLinkSuffix))?1:-1];
typedef char PrevDerived[(OFFSET(MainTask,field_0038)==sizeof(MainTask)-sizeof(TaskQueueLinkSuffix)+sizeof(MainTask *))?1:-1];
typedef char AddressWidth[(sizeof(TaskQueueAddress)==4)?1:-1];
''', encoding="utf-8")
        result = compile_object("tools/compilers/pspsnc_1.2.7503.0/pspsnc.exe", ["-O2", "-g"],
                                source.relative_to(ROOT), (directory / "layout.raw.o").relative_to(ROOT))
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_host_empty_single_head_tail_and_interior_sanitized(self):
        directory = ROOT / "build/task-unlink-tests"
        directory.mkdir(parents=True, exist_ok=True)
        relative = directory.relative_to(ROOT).as_posix()
        commands = [["g++", "-std=c++17", "-O2", "-fsanitize=address,undefined", "-fno-sanitize-recover=all", "-Iinclude",
                     "tools/probes/task_queue_production_unlink_fixture.cpp", "-o", relative + "/fixture"],
                    ["./" + relative + "/fixture"]]
        for command in commands:
            if os.name == "nt":
                linux_root = "/mnt/" + ROOT.drive[0].lower() + ROOT.as_posix()[2:]
                command = ["wsl", "-d", "Ubuntu-24.04", "--cd", linux_root, "--", *command]
            result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

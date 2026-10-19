"""A raw match alone cannot prove a thunk's C argument/return semantics."""
import os
import subprocess
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class CallbackInterfaceChecks(unittest.TestCase):
    def test_real_source_forwards_values_callback_and_return(self):
        directory = ROOT / "build/callback-interface-test"
        directory.mkdir(parents=True, exist_ok=True)
        harness = directory / "harness.c"
        harness.write_text(r'''
#include "main/callback_runtime.h"
static int callback_calls, native_calls, result;
static unsigned int context_seen, first_seen;
static int second_seen;
static MainTaskCallback update_seen;
static MainTask *task_seen;
static void update(MainTask *task) { task_seen = task; callback_calls++; }
MainTask *func_088823C4(unsigned int context, unsigned int first,
                       int second, MainTaskCallback callback) {
    native_calls++;
    context_seen = context;
    first_seen = first;
    second_seen = second;
    update_seen = callback;
    callback((MainTask *)&result);
    return (MainTask *)&result;
}
int main(void) {
    MainCallbackBlock block = {0};
    block.field_0000 = 0xA1B2C3D4u;
    void *returned = func_08865088(&block, 0x12345678u, -1234567,
                                  update, "observed unused label");
    return !(native_calls == 1 && callback_calls == 1 &&
             context_seen == 0xA1B2C3D4u && first_seen == 0x12345678u &&
             second_seen == -1234567 && update_seen == update &&
             task_seen == (MainTask *)&result && returned == &result);
}
''', encoding="utf-8")
        relative = directory.relative_to(ROOT).as_posix()
        binary = relative + "/harness"
        commands = [["gcc", "-std=c99", "-O2", "-Wall", "-Wextra", "-Werror",
                     "-Wno-unused-parameter", "-Iinclude", "src/main/state_release.c",
                     relative + "/harness.c", "-o", binary], ["./" + binary]]
        for command in commands:
            if os.name == "nt":
                linux_root = "/mnt/" + ROOT.drive[0].lower() + ROOT.as_posix()[2:]
                command = ["wsl", "-d", "Ubuntu-24.04", "--cd", linux_root, "--", *command]
            result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)


if __name__ == "__main__": unittest.main()

"""Check native savedata state transitions, including terminal and error states."""
import os
import subprocess
import unittest
from pathlib import Path
from inspect_imports import inspect

ROOT = Path(__file__).resolve().parents[1]


class SavedataPollChecks(unittest.TestCase):
    def test_sdk_import_bindings(self):
        imports = inspect(ROOT / "analysis/ULUS10566/main/EBOOT.ELF")
        actual = {f["address"]: (library["library"], f["nid"])
                  for library in imports["libraries"] for f in library["functions"]}
        self.assertEqual(actual["0x089C5868"], ("sceUtility", "0x8874DBE0"))
        self.assertEqual(actual["0x089C5878"], ("sceUtility", "0x9790B33C"))
        self.assertEqual(actual["0x089C5898"], ("sceUtility", "0xD4B95FFB"))

    def test_real_handlers_wait_advance_and_reject_unknown_status(self):
        directory = ROOT / "build/savedata-poll-tests"
        directory.mkdir(parents=True, exist_ok=True)
        harness = directory / "harness.c"
        harness.write_text(r'''
#include "menu/savedata_dialog.h"
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
extern void func_089DD930(MenuRuntimePrefix *);
extern void func_089DDB68(MenuRuntimePrefix *);
static int status, get_calls, update_calls, shutdown_calls;
int func_089C5868(void) { get_calls++; return status; }
void func_089C5898(int mode) { assert(mode == 2); update_calls++; }
int func_089C5878(void) { shutdown_calls++; return -42; }
int main(void) {
    MenuExecutionState *state = calloc(1,sizeof(*state)); assert(state);
    MenuRuntimePrefix self = {0}; self.field_01DC = state;
    void (*handlers[])(MenuRuntimePrefix *) = {func_089DD930,func_089DDB68};
    int statuses[] = {0,1,2,3,4,5,-1,INT_MIN,INT_MAX};
    for (int h = 0; h < 2; h++) {
        for (unsigned int i = 0; i < sizeof(statuses)/sizeof(statuses[0]); i++) {
            status = statuses[i]; get_calls = update_calls = shutdown_calls = 0;
            state->field_0008 = 4; state->field_0DCC = 0x7;
            handlers[h](&self);
            assert(get_calls == 1 && update_calls == (status == 2));
            int invalid = status < 0 || status > 4;
            assert(shutdown_calls == (status == 3 || invalid));
            assert(state->field_0008 == (invalid ? 6 : status == 0 ? 5 : 4));
            assert(state->field_0DCC == (h == 1 && status == 0 ? 1 : 0x7));
        }
        state->field_0008 = 4; state->field_0DCC = 0;
        int cycle[] = {1,2,3,4,0};
        for (int i = 0; i < 5; i++) {
            status = cycle[i]; handlers[h](&self);
            assert(state->field_0008 == (i == 4 ? 5 : 4));
            assert(state->field_0DCC == (h == 1 && i == 4 ? 1 : 0));
        }
    }
    free(state); return 0;
}
''', encoding="utf-8")
        relative = directory.relative_to(ROOT).as_posix()
        commands = [["gcc", "-std=c99", "-O2", "-Wall", "-Wextra", "-Werror", "-Iinclude",
                     "src/menu/savedata_list_poll.c", "src/menu/savedata_load_poll.c",
                     relative + "/harness.c", "-o", relative + "/harness"], ["./" + relative + "/harness"]]
        for command in commands:
            if os.name == "nt":
                linux_root = "/mnt/" + ROOT.drive[0].lower() + ROOT.as_posix()[2:]
                command = ["wsl", "-d", "Ubuntu-24.04", "--cd", linux_root, "--", *command]
            result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)


if __name__ == "__main__": unittest.main()

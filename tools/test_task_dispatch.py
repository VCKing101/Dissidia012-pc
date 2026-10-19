"""Exercise native dispatcher source when callbacks mutate pending work."""
import os
import subprocess
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class TaskDispatchChecks(unittest.TestCase):
    def test_mutable_cursor_filter_and_nested_callback_context(self):
        directory = ROOT / "build/task-dispatch-tests"
        directory.mkdir(parents=True, exist_ok=True)
        harness = directory / "harness.cpp"
        harness.write_text(r'''
#include "main/task_runtime.h"
#include <assert.h>
#include <string.h>
extern "C" { MainTask *D_089D69B0; }
static MainTask tasks[5], nested_task, sentinel;
static MainTaskOwner owner, nested_owner;
static int calls[8], count, removals, scenario;
static void nested_update(MainTask *task) {
    assert(task == &nested_task && D_089D69B0 == task);
    assert(nested_owner.field_0010 == task && nested_owner.field_0014 == 0);
    calls[count++] = 99;
}
extern "C" void func_088822E0(MainTask *task) {
    assert(task->field_0034 == 0 && task->field_0038 == 0);
    removals++;
    memset(task,0,sizeof(*task));
}
static void update(MainTask *task) {
    assert(D_089D69B0 == task && owner.field_0010 == task);
    assert(task->field_0044 == 1);
    int index = (int)(task - tasks); calls[count++] = index;
    if (scenario == 1 && index == 0) {
        assert(owner.field_0014 == &tasks[1]);
        func_088828BC(&owner,&tasks[1]);
    }
    if (scenario == 1 && index == 3) {
        func_0888245C(&nested_owner,0,0);
        assert(D_089D69B0 == task && owner.field_0010 == task);
        assert(nested_owner.field_0010 == 0 && nested_owner.field_0014 == 0);
    }
    if (scenario == 2 && index == 0) {
        assert(owner.field_0014 == &tasks[2]);
        task->field_0048 = 0; /* A formerly eligible pending task must be rechecked. */
    }
}
static void initialize(void) {
    memset(tasks,0,sizeof(tasks)); memset(&owner,0,sizeof(owner));
    count = removals = 0; D_089D69B0 = &sentinel;
    for (int i = 0; i < 5; i++) {
        tasks[i].field_0024 = &owner; tasks[i].field_0000 = update;
        tasks[i].field_0038 = i ? &tasks[i-1] : 0;
        tasks[i].field_0034 = i < 4 ? &tasks[i+1] : 0;
    }
    owner.field_0004 = &tasks[0]; owner.field_0008 = &tasks[4]; owner.field_000C = 5;
}
int main(void) {
    D_089D69B0 = &sentinel; func_0888245C(&owner,1,1);
    assert(D_089D69B0 == &sentinel && count == 0);
    nested_task.field_0000 = nested_update; nested_task.field_0024 = &nested_owner;
    nested_owner.field_0004 = nested_owner.field_0008 = &nested_task;
    initialize(); scenario = 1;
    tasks[0].field_0028 = tasks[1].field_0028 = tasks[3].field_0028 = tasks[4].field_0028 = 1;
    tasks[4].field_0000 = 0;
    func_0888245C(&owner,1,1);
    assert(count == 3 && calls[0] == 0 && calls[1] == 3 && calls[2] == 99);
    assert(removals == 2 && owner.field_000C == 3 && owner.field_0008 == &tasks[3]);
    assert(owner.field_0010 == 0 && owner.field_0014 == 0 && D_089D69B0 == &sentinel);
    initialize(); scenario = 2;
    tasks[0].field_0028 = tasks[2].field_0028 = tasks[4].field_0028 = 1;
    func_0888245C(&owner,1,1);
    /* Slot 1 was skipped before the filter changed; dispatch resumes after the pending slot 2. */
    assert(count == 2 && calls[0] == 0 && calls[1] == 3 && removals == 0);
    assert(tasks[3].field_0048 == 0 && owner.field_0010 == 0 && owner.field_0014 == 0);
    assert(D_089D69B0 == &sentinel);
    return 0;
}
''', encoding="utf-8")
        relative = directory.relative_to(ROOT).as_posix()
        commands = [["g++", "-std=c++11", "-O2", "-Wall", "-Wextra", "-Werror", "-Iinclude",
                     "src/main/task_dispatch.cpp", "src/main/task_unlink.cpp", relative + "/harness.cpp", "-o", relative + "/harness"],
                    ["./" + relative + "/harness"]]
        for command in commands:
            if os.name == "nt":
                linux_root = "/mnt/" + ROOT.drive[0].lower() + ROOT.as_posix()[2:]
                command = ["wsl", "-d", "Ubuntu-24.04", "--cd", linux_root, "--", *command]
            result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)


if __name__ == "__main__": unittest.main()

"""Check task lifetime, mutation during removal, and native PSP layouts."""
import os
import subprocess
import unittest
from pathlib import Path
from compile_psp import compile_object

ROOT = Path(__file__).resolve().parents[1]


class TaskRuntimeChecks(unittest.TestCase):
    def test_psp_layout_and_signed_priority(self):
        directory = ROOT / "build/task-runtime-tests"
        directory.mkdir(parents=True, exist_ok=True)
        source = directory / "layout.c"
        source.write_text(r'''
#include "main/task_runtime.h"
#define OFFSET(t,m) ((unsigned int)&(((t *)0)->m))
typedef char TaskSize[(sizeof(MainTask)==0x4C)?1:-1];
typedef char OwnerSize[(sizeof(MainTaskOwner)==0x18)?1:-1];
typedef char OwnerAt[(OFFSET(MainTask,field_0024)==0x24)?1:-1];
typedef char FilterAt[(OFFSET(MainTask,field_0028)==0x28)?1:-1];
typedef char PriorityAt[(OFFSET(MainTask,field_002C)==0x2C)?1:-1];
typedef char FinalizerAt[(OFFSET(MainTask,field_0030)==0x30)?1:-1];
typedef char NextAt[(OFFSET(MainTask,field_0034)==0x34)?1:-1];
typedef char PreviousAt[(OFFSET(MainTask,field_0038)==0x38)?1:-1];
typedef char CursorAt[(OFFSET(MainTaskOwner,field_0014)==0x14)?1:-1];
''', encoding="utf-8")
        result = compile_object("tools/compilers/pspsnc_1.2.7503.0/pspsnc.exe", ["-O2", "-g"],
                                source.relative_to(ROOT), (directory / "layout.raw.o").relative_to(ROOT))
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_source_handles_filtered_removal_and_allocator_lifetime(self):
        directory = ROOT / "build/task-runtime-tests"
        directory.mkdir(parents=True, exist_ok=True)
        harness = directory / "harness.cpp"
        harness.write_text(r'''
#include "main/task_runtime.h"
#include <assert.h>
#include <string.h>
static MainTask tasks[4];
static MainTaskOwner owner;
static int allocator, freed_tasks, finalizers, freed_owner, destroyed;
static void update(MainTask *) {}
static void other_update(MainTask *) {}
static void finalize(MainTask *task) {
    assert(task->field_0024 == &owner);
    assert(task->field_0034 == 0 && task->field_0038 == 0);
    finalizers++;
}
extern "C" void func_0885C538(void *context, void *object, int reserved) {
    assert(context == &allocator && reserved == 0 && !destroyed);
    if (object == &owner) {
        assert(freed_tasks == 4 && finalizers == 4);
        freed_owner++;
        memset(&owner, 0, sizeof(owner));
    } else {
        assert(finalizers == freed_tasks + 1);
        freed_tasks++;
        /* Poison the removed task: iteration must cache its successor before deletion. */
        memset(object, 0, sizeof(MainTask));
    }
}
extern "C" void func_0885C488(void *context) {
    assert(context == &allocator && freed_owner == 1 && !destroyed);
    destroyed++;
}

int main(void) {
    owner.field_0000 = &allocator;
    owner.field_000C = 123; /* Traversal count must examine the links. */
    assert(func_088826D4(&owner) == 0 && !func_0888274C(&owner, update));
    for (int i = 0; i < 4; i++) {
        assert(func_088822BC(&tasks[i], &owner, i & 1, -70, update) == &tasks[i]);
        assert(tasks[i].field_002C == -70 && tasks[i].field_0030 == 0);
        assert(tasks[i].field_003C == 0 && tasks[i].field_0040 == 0);
        tasks[i].field_0030 = finalize;
        tasks[i].field_0034 = i < 3 ? &tasks[i+1] : 0;
        tasks[i].field_0038 = i ? &tasks[i-1] : 0;
    }
    owner.field_0004 = &tasks[0]; owner.field_0008 = &tasks[3];
    assert(func_088826D4(&owner) == 4 && func_0888274C(&owner, update));
    assert(!func_0888274C(&owner, other_update));
    owner.field_000C = 4;
    func_088825F0(&owner, 1, 1);
    assert(freed_tasks == 2 && finalizers == 2 && func_088826D4(&owner) == 2);
    assert(owner.field_0004 == &tasks[0] && owner.field_0008 == &tasks[2]);
    assert(tasks[0].field_0034 == &tasks[2] && tasks[2].field_0038 == &tasks[0]);
    func_08882704(&owner);
    assert(freed_tasks == 4 && finalizers == 4 && freed_owner == 1 && destroyed == 1);
    return 0;
}
''', encoding="utf-8")
        relative = directory.relative_to(ROOT).as_posix()
        sources = ["task_construct.c", "task_finalize.c", "task_remove_matching.cpp", "task_remove_all.c",
                   "task_count.cpp", "task_owner_finalize.c", "task_has_callback.cpp", "task_unlink.cpp"]
        commands = [["g++", "-std=c++11", "-O2", "-Wall", "-Wextra", "-Werror", "-Iinclude",
                     *("src/main/" + source for source in sources), relative + "/harness.cpp", "-o", relative + "/harness"],
                    ["./" + relative + "/harness"]]
        for command in commands:
            if os.name == "nt":
                linux_root = "/mnt/" + ROOT.drive[0].lower() + ROOT.as_posix()[2:]
                command = ["wsl", "-d", "Ubuntu-24.04", "--cd", linux_root, "--", *command]
            result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)


if __name__ == "__main__": unittest.main()

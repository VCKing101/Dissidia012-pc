"""Check resource-buffer allocation, packed origin arguments and conversions."""
import os
import subprocess
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class DrawingSupportChecks(unittest.TestCase):
    def test_real_sources_forward_alignment_flags_and_signed_values(self):
        directory = ROOT / "build/drawing-support-tests"
        directory.mkdir(parents=True, exist_ok=True)
        harness = directory / "harness.c"
        harness.write_text(r'''
#include "main/allocator_runtime.h"
#include "main/drawing_record.h"
#include <assert.h>
#include <limits.h>
typedef struct MainDrawingOriginPair { short first,second; } MainDrawingOriginPair;
typedef struct MainResourceBufferPrefix { unsigned char unknown[4]; unsigned int field_0004; void *field_0008; unsigned int field_000C; } MainResourceBufferPrefix;
static unsigned char manager[256], allocation[128];
static int allocator, record, allocations, settings, origins;
void *D_089D6C88 = manager;
unsigned int D_09B69878[2] = {17,23};
static float first_seen,second_seen;
extern void func_088D52AC(void *,void *,unsigned int);
extern void func_088A18B0(void *,int,int);
void *func_0885C6CC(void *heap,unsigned int size,int alignment,unsigned int unused,unsigned char first,unsigned char second) {
    assert(heap == &allocator && size == 0xE1000 && alignment == 128);
    assert(unused == 0 && first == 1 && second == 0); allocations++; return allocation;
}
void func_088A1BEC(void *self,float first,float second) {
    assert(self == manager + 0x7C); first_seen = first; second_seen = second; settings++;
}
void *func_088DA954(void *owner,int index,MainDrawingOriginPair origin,unsigned char flags) {
    assert(owner == manager && index == -17 && origin.first == 0 && origin.second == 0);
    assert(flags == 0xF1); origins++; return &record;
}
int main(void) {
    MainResourceBufferPrefix self = {{1,2,3,4},55,0,66};
    func_088D52AC(&self,&allocator,0xE1000);
    assert(allocations == 1 && self.field_0004 == 0 && self.field_0008 == allocation && self.field_000C == 0);
    assert(self.unknown[0] == 1 && self.unknown[3] == 4);
    assert(settings == 1 && first_seen == 100 && second_seen == 0);
    assert(D_09B69878[0] == 0 && D_09B69878[1] == 0);
    func_088A18B0(manager,-1234567,INT_MAX);
    assert(settings == 2 && first_seen == (float)-1234567 && second_seen == (float)INT_MAX);
    assert(func_088DAA40(manager,-17,0xF1) == &record && origins == 1);
    return 0;
}
''', encoding="utf-8")
        relative = directory.relative_to(ROOT).as_posix()
        commands = [["gcc", "-std=c99", "-O2", "-Wall", "-Wextra", "-Werror", "-Iinclude",
                     "src/main/resource_buffer_initialize.c", "src/main/resource_integer_pair.c",
                     "src/main/drawing_acquire_at_origin.c", relative + "/harness.c", "-o", relative + "/harness"],
                    ["./" + relative + "/harness"]]
        for command in commands:
            if os.name == "nt":
                linux_root = "/mnt/" + ROOT.drive[0].lower() + ROOT.as_posix()[2:]
                command = ["wsl", "-d", "Ubuntu-24.04", "--cd", linux_root, "--", *command]
            result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

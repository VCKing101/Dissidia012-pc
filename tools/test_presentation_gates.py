"""Exercise presentation render guards and real queued completion callbacks."""
import os
import subprocess
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class PresentationGateChecks(unittest.TestCase):
    def test_real_gates_nan_and_completion_argument(self):
        directory = ROOT / "build/presentation-gate-tests"
        directory.mkdir(parents=True, exist_ok=True)
        harness = directory / "harness.c"
        harness.write_text(r'''
#include "main/presentation_channel.h"
#include "menu/execution_state.h"
#include "menu/execution_callbacks.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
extern void func_08874118(MainPresentationChannel *);
extern void func_0887414C(MainPresentationChannel *);
extern void func_088740DC(MainPresentationChannel *);
static int render_calls,callbacks,argument_seen;
static const float *vector_seen;
static MainPresentationChannel channel;
MenuRuntimePrefix *D_089E09C0;
void func_088741C0(MainPresentationChannel *self,const float *vector) {
    assert(self == &channel); render_calls++; vector_seen = vector;
}
static void completed(int argument) {
    assert(channel.field_0234 == 0); callbacks++; argument_seen = argument;
}
int main(void) {
    float values[] = {-INFINITY,-1,0,1,INFINITY,NAN};
    for (int i = 0; i < 6; i++) {
        channel.value.values[3] = channel.interpolated_0010.values[3] = values[i];
        render_calls = 0; func_08874118(&channel);
        assert(render_calls == (i >= 3));
        if (render_calls) assert(vector_seen == channel.value.values);
        render_calls = 0; func_0887414C(&channel);
        assert(render_calls == (i >= 3));
        if (render_calls) assert(vector_seen == channel.interpolated_0010.values);
    }
    channel.field_0220 = 2; channel.field_0234 = 2;
    channel.events_0020[0].field_0014 = 0;
    channel.events_0020[1].field_0014 = completed;
    channel.events_0020[1].field_0018 = -1234567;
    func_088740DC(&channel);
    assert(callbacks == 1 && argument_seen == -1234567 && channel.field_0234 == 0);
    channel.field_0234 = 2; channel.events_0020[1].field_0014 = 0;
    func_088740DC(&channel); assert(callbacks == 1 && channel.field_0234 == 0);
    MenuExecutionState *state = calloc(1,sizeof(*state)); assert(state);
    MenuRuntimePrefix menu = {0}; menu.field_01DC = state; D_089E09C0 = &menu;
    channel.field_0234 = 2; channel.events_0020[1].field_0014 = func_089DE9D4;
    func_088740DC(&channel); assert(state->field_0048 == 1);
    free(state); return 0;
}
''', encoding="utf-8")
        relative = directory.relative_to(ROOT).as_posix()
        commands = [["gcc", "-std=c99", "-O2", "-Wall", "-Wextra", "-Werror", "-Wno-unused-parameter", "-Iinclude",
                     "src/main/presentation_render_gates.c", "src/main/presentation_finish.c",
                     "src/menu/presentation_complete.c", relative + "/harness.c", "-o", relative + "/harness"],
                    ["./" + relative + "/harness"]]
        for command in commands:
            if os.name == "nt":
                linux_root = "/mnt/" + ROOT.drive[0].lower() + ROOT.as_posix()[2:]
                command = ["wsl", "-d", "Ubuntu-24.04", "--cd", linux_root, "--", *command]
            result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

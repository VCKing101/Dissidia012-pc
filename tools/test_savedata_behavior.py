"""Exercise recovered handlers across SDK boundaries without an emulator."""
import os
import subprocess
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class SavedataBehaviorChecks(unittest.TestCase):
    def test_request_lifetime_selection_ties_and_sdk_failure(self):
        directory = ROOT / "build/savedata-behavior-tests"
        directory.mkdir(parents=True, exist_ok=True)
        harness = directory / "harness.c"
        harness.write_text(r'''
#include "menu/execution_state.h"
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
const char D_089E0704[] = "%s%02d", D_089E070C[] = "GameData";
const char D_089E0718[] = "GameData*", D_089E0724[] = "DISSIDIA.BIN";
const unsigned char D_089E06F1[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
const unsigned long long D_089E0738 = 0;
const char *D_089D6010 = "ULUS10566";
static int settings;
void *D_089D7280 = &settings;
static MenuExecutionState *state;
static MenuSavedataRequest *retained_request;
static int init_calls, rtc_calls, sdk_result, common_calls;
extern void func_089DD760(MenuRuntimePrefix *);
extern void func_089DD9DC(MenuRuntimePrefix *);
int func_0893476C(char *out, const char *format, ...) {
    va_list args; va_start(args, format);
    int result = vsnprintf(out,20,format,args); va_end(args); return result;
}
void *func_08935BB4(void *out, const void *in, unsigned int length) {
    return strncpy(out,in,length);
}
void *func_08935524(void *out, const void *in, unsigned int length) {
    return memcpy(out,in,length);
}
void *func_08935828(void *out, int value, unsigned int length) {
    assert(out == &state->savedata_007C && value == 0 && length == 0x600);
    return memset(out,value,length);
}
void func_088B168C(void *context, void *out, unsigned int length) {
    assert(context == &settings && out == &state->savedata_007C && length == 0x600);
    assert(state->savedata_007C.mode == 0); common_calls++;
    state->savedata_007C.common[0] = 0x60;
}
int func_089C5858(MenuSavedataRequest *request) {
    assert(request == &state->savedata_007C);
    assert(request->data == state->data_0DD0 && request->id_list == &state->slots_067C);
    retained_request = request; init_calls++; return sdk_result;
}
int func_089C5D78(const void *in, unsigned long long *tick) {
    const MenuSaveTimestamp *date = in;
    /* Large values test both words of the native unsigned 64-bit comparison. */
    *tick = ((unsigned long long)date->year << 32) | date->microsecond;
    rtc_calls++; return 0;
}
static void slot(int index, const char *name, unsigned short high, unsigned int low) {
    strncpy(state->entries_0688[index].name,name,20);
    state->entries_0688[index].modified.year = high;
    state->entries_0688[index].modified.microsecond = low;
}
int main(void) {
    state = calloc(1,sizeof(*state)); assert(state);
    MenuRuntimePrefix self = {0}; self.field_01DC = state;
    state->savedata_007C.mode = 99; state->field_0008 = 2;
    func_089DD760(&self);
    assert(init_calls == 1 && common_calls == 1 && state->field_0008 == 3);
    for (int i = 0; i < 20; i++) {
        char expected[20]; snprintf(expected,20,"GameData%02d",i);
        assert(!strcmp(state->names_0C28[i],expected));
    }
    assert(state->names_0C28[20][0] == 0);
    assert(retained_request->name_list == state->names_0C28);
    assert(retained_request->mode == 11 && retained_request->overwrite == 0);
    assert(!strcmp(retained_request->save_name,"GameData*"));
    assert(!strcmp(retained_request->file_name,"DISSIDIA.BIN"));
    assert(!strcmp(retained_request->game_name,"ULUS10566"));
    assert(retained_request->data_capacity == 0x3ECC8 && retained_request->data_size == 0x3ECC8);
    assert(retained_request->multi_status == 1 && retained_request->focus == 3);
    assert(!memcmp(retained_request->key,D_089E06F1,16));
    assert(state->slots_067C.capacity == 20 && state->slots_067C.entries == state->entries_0688);
    /* The SDK retains these pointers after the list handler has returned. */
    ((unsigned char *)retained_request->data)[0] = 0xAA;
    assert(state->data_0DD0[0] == 0xAA);
    slot(0,"GameData00",1,0xFFFFFFFFu);
    slot(3,"GameData03",2,2); slot(19,"GameData19",2,2);
    state->slots_067C.count = 1; /* Native selector scans the fixed twenty slots. */
    state->field_0008 = 4; func_089DD9DC(&self);
    assert(init_calls == 2 && rtc_calls == 3 && state->field_0008 == 5);
    assert(!strcmp(retained_request->save_name,"GameData19")); /* Last equal timestamp wins. */
    assert(state->cached_time_3FA98.year == 2 && state->cached_time_3FA98.microsecond == 2);
    assert(retained_request->mode == 0 && retained_request->multi_status == 0);
    sdk_result = -7; state->field_0008 = 4; func_089DD9DC(&self);
    assert(init_calls == 3 && state->field_0008 == 6);
    memset(state->entries_0688,0,sizeof(state->entries_0688));
    state->field_0008 = 4; func_089DD9DC(&self);
    assert(init_calls == 3 && rtc_calls == 6 && state->field_0008 == 6);
    state->field_0008 = 2; func_089DD760(&self);
    assert(init_calls == 4 && state->field_0008 == 6);
    free(state); return 0;
}
''', encoding="utf-8")
        relative = directory.relative_to(ROOT).as_posix()
        commands = [["gcc", "-std=c99", "-O2", "-Wall", "-Wextra", "-Werror", "-Iinclude",
                     "src/menu/savedata_list_setup.c", "src/menu/savedata_latest_slot.c",
                     relative + "/harness.c", "-o", relative + "/harness"], ["./" + relative + "/harness"]]
        for command in commands:
            if os.name == "nt":
                linux_root = "/mnt/" + ROOT.drive[0].lower() + ROOT.as_posix()[2:]
                command = ["wsl", "-d", "Ubuntu-24.04", "--cd", linux_root, "--", *command]
            result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)


if __name__ == "__main__": unittest.main()

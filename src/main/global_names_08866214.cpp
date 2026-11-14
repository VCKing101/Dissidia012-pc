#include "main/agent2_types.h"

extern "C" Agent2GlobalState *D_089D5EC0;
extern "C" int func_08935970(const char *, const char *);
extern "C" void func_08935BB4(char *, const char *, int);

extern "C" int func_08866214(const char *name) {
    Agent2GlobalState *state = D_089D5EC0;
    for (int index = 0; index < state->field_1AC4; index++) {
        if (!func_08935970(state->field_08C4[index].name, name)) {
            return state->field_08C4[index].value;
        }
    }
    return 0;
}

extern "C" void func_088662B4(const char *name, int value) {
    Agent2GlobalState *state = D_089D5EC0;
    if (state->field_2CCC && state->field_1AC4 < 0x80) {
        func_08935BB4(state->field_08C4[state->field_1AC4].name, name, 0x20);
        state->field_08C4[state->field_1AC4].value = value;
        state->field_1AC4++;
    }
}

extern "C" int func_08866340(const char *name) {
    Agent2GlobalState *state = D_089D5EC0;
    for (int index = 0; index < state->field_2CC8; index++) {
        if (!func_08935970(state->field_1AC8[index].name, name)) {
            return state->field_1AC8[index].value;
        }
    }
    return 0;
}

extern "C" void func_088663E0(const char *name, int value) {
    Agent2GlobalState *state = D_089D5EC0;
    if (state->field_2CCC && state->field_2CC8 < 0x80) {
        func_08935BB4(state->field_1AC8[state->field_2CC8].name, name, 0x20);
        state->field_1AC8[state->field_2CC8].value = value;
        state->field_2CC8++;
    }
}

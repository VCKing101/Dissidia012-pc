#include "main/agent2_types.h"

extern "C" int func_088661AC(Agent2GlobalState *, unsigned char);
extern "C" int func_089C5A98(void);

struct Agent2Mode {
    static int offset(Agent2GlobalState *self) {
        int offset = 0x288000;
        if (self->field_2CCF == 1) offset = 0x710000;
        return offset;
    }
    static int count(Agent2GlobalState *self) {
        int count = 1;
        if (self->field_2CCF == 1) count = 3;
        return count;
    }
};

extern "C" int func_08866EBC(Agent2GlobalState *self) {
    return func_088661AC(self, 1);
}

extern "C" int func_08866ED8(Agent2GlobalState *self) {
    return func_088661AC(self, 0);
}

extern "C" int func_08866EF4(Agent2GlobalState *self) {
    int offset = Agent2Mode::offset(self);
    return func_089C5A98() + offset;
}

extern "C" int func_08866F34(Agent2GlobalState *self) {
    return Agent2Mode::count(self);
}

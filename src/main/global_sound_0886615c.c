#include "main/agent2_types.h"

extern void func_089A0060(void);
extern int func_089C5A98(void);

void func_0886615C(void) {
    func_089A0060();
}

void func_08866178(Agent2GlobalState *self, unsigned char value) {
    if (self->field_2CCF == value) return;
    self->field_2CD0 = 1;
}

int func_088661A0(Agent2GlobalState *self) {
    return self->field_2CD0 == 0;
}

int func_088661AC(Agent2GlobalState *self, unsigned char mode) {
    int index = mode == 1 ? 1 - self->field_08C0 : self->field_08C0;
    int scale = 0x44000;
    if (self->field_2CCF == 1) scale = 0x88000;
    return func_089C5A98() + index * scale;
}

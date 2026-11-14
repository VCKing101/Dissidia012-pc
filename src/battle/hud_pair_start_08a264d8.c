#include "battle/agent31_types.h"

extern int func_089E99C0(int kind, int value);
extern void func_089F84E0(void *frame);
extern void func_089F85B0(void *frame, int value, int look);

int func_08A264D8(A31HudPair *self, int value, int arg_14, int arg_18)
{
    int kind;
    int look;

    if (!self->open) {
        return -1;
    }
    if (self->state != -1) {
        return -2;
    }
    kind = (int)(self->side + self->slot) % 2;
    look = func_089E99C0(kind, value);
    func_089F84E0(self->frame);
    func_089F85B0(self->frame, value, look);
    self->state = 0;
    self->time = 0.0f;
    self->value = value;
    self->look = look;
    self->arg_14 = arg_14;
    self->arg_18 = arg_18;
    return 0;
}

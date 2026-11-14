#include "battle/agent14_types.h"

extern void func_08A82478(SlotTable *self);

int func_08A824D8(SlotTable *self, void *a, void *b, void *c)
{
    func_08A82478(self);
    if (a == 0 || b == 0 || c == 0) {
        return -1;
    }
    self->field_04 = a;
    self->field_08 = b;
    self->field_0C = c;
    self->active = 1;
    return 0;
}

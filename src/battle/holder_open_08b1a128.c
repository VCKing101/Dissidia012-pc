#include "battle/agent27_types.h"

extern void func_08B1A0E8(ResHolder *self);

int func_08B1A128(ResHolder *self, int value)
{
    func_08B1A0E8(self);
    if (value == 0) {
        return -1;
    }
    self->field_04 = value;
    self->active = 1;
    return 0;
}

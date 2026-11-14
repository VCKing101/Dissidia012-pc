#include "battle/agent27_types.h"

extern void func_08B1C018(ResHolder *self);

int func_08B1C058(ResHolder *self, int value)
{
    func_08B1C018(self);
    if (value == 0) {
        return -1;
    }
    self->field_04 = value;
    self->active = 1;
    return 0;
}

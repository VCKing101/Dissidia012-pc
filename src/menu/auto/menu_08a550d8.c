#include "menu/auto_region2.h"

/* Item: in state 1 plays its scripts (func_08A55244) and func_08A55284. */
void func_08A550D8(Item *self)
{
    if (self->field_F4 == 1) {
        func_08A55244(self);
        func_08A55284(self);
    }
}

#include "menu/auto_region2.h"

/* Part5B5C0: in state 1 plays its scripts (func_08A591EC) and func_08A5923C. */
void func_08A5919C(Part5B5C0 *self)
{
    if (self->field_10 == 1) {
        func_08A591EC(self);
        func_08A5923C(self);
    }
}

#include "menu/auto_region2.h"

/* Obj5B958: func_08A8104C(field_40C, 1) when it exists. */
void func_08A5BB34(Obj5B958 *self)
{
    if (self->field_40C != 0) {
        func_08A8104C(self->field_40C, 1);
    }
}

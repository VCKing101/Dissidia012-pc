#include "menu/auto_region4.h"

/* List4440C: takes the heap of func_08B42B40 and clears the rest. */
void func_08B4440C(List4440C *self)
{
    self->heap = func_08B42B40();
    self->field_08 = 0;
    self->field_04 = 0;
    self->field_10 = 0;
    self->field_0C = 0;
}

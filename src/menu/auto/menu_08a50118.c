#include "menu/auto_region2.h"

/* Screen51CBC: resets it (and D_09B6DD00). */
void func_08A50118(Screen51CBC *self)
{
    D_09B6DD00 = 0;
    self->field_08 = 0;
    self->field_0C = 0;
    self->heap = 0;
    self->work = 0;
}

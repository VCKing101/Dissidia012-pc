#include "menu/auto_region1.h"

/* StepCounter: starts sound 0x61A84 into field_50 and sets field_3C. */
void func_089EB288(StepCounter *self)
{
    self->field_50 = func_088A24D4(0, 0x61A84, 1, 0);
    self->field_3C = 1;
}

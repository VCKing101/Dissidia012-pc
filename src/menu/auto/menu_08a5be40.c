#include "menu/auto_region2.h"

/* Obj5B958: func_08A80EF4 and func_08A80E34 on field_40C, state 3. */
void func_08A5BE40(Obj5B958 *self)
{
    func_08A80EF4(self->field_40C);
    func_08A80E34(self->field_40C);
    self->state = 3;
}

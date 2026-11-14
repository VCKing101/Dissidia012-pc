#include "menu/auto_region2.h"

/* Obj6ECF4: shuts down its object +8 and closes handle field_1D0. */
void func_08A6EEEC(Obj6ECF4 *self)
{
    func_0890A984(self->field_08);
    func_0890A69C(self->field_08);
    func_0890EA18(self->field_1D0);
    self->field_1D0 = -1;
}

#include "battle/auto_region3.h"

/* Part64E48 constructor; arg goes to func_08A63A2C. */
Part64E48 *func_08A637B8(Part64E48 *self, int arg)
{
    self->vtable = D_08A64E48;
    func_08AB2EF0(self->field_20);
    func_08A96398(self->field_530);
    func_08A870F0(self->field_5B0);
    func_08A80770(&self->field_C10);
    func_08AA4CB0(self->field_C80);
    func_08A656D8(&self->field_C9C);
    func_08A65E58(&self->field_FD0);
    func_08A9AC18(self->field_11C0);
    func_08A9A538(self->field_1330);
    func_08AA1ED0(self->field_135C);
    func_08A9A1B8(self->field_138C);
    func_08AA29F0(self->field_13A0);
    func_08A63950(self);
    func_08A63A2C(self, arg);
    return self;
}

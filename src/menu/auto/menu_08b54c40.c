#include "menu/auto_region4.h"

/* Obj54C40 constructor. */
Obj54C40 *func_08B54C40(Obj54C40 *self)
{
    func_0887B1E4(self->field_28);
    self->field_30 = 0;
    self->field_34 = 0;
    self->field_38 = 0;
    self->field_3C = 0;
    self->field_160 = 0;
    self->state = 0;
    func_08935828(self->field_00, 0, 0x24);
    func_08935828(self->field_24, 0, 4);
    return self;
}

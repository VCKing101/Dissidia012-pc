#include "main/auto_region2.h"

/* Constructor of Obj84F140: sixteen elements, field_200 = value, the five flags cleared. */
Obj84F140 *func_0884F140(Obj84F140 *self, int value)
{
    func_08993560(self, 0x10, 0x20, func_0884ECAC);
    self->field_200 = value;
    self->field_204 = 0;
    self->field_205 = 0;
    self->field_206 = 0;
    self->field_207 = 0;
    self->field_208 = 0;
    return self;
}

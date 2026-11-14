#include "menu/auto_region1.h"

/* Screen35A00: sets up its task (three callbacks, the 0x4000-byte block). */
void func_08A31180(Screen35A00 *self)
{
    func_0886503C(&self->task, func_08A32364, func_08A32384, func_08A323A4);
    func_0886504C(&self->task, self->field_00, 0x4000);
}

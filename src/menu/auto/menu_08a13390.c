#include "menu/auto_region1.h"

/* Screen15A88: hides handle 0x44, func_08A13988(0, 0), field_EDC = 0x69. */
void func_08A13390(Screen15A88 *self)
{
    func_088D9CFC(self->work->handles[0x44]);
    func_08A13988(self, 0, 0);
    self->work->field_EDC = 0x69;
}

#include "menu/auto_region1.h"

/* Screen0B6A0: keeps func_089F6184(9) in field_3454 and sets flag 0x100. */
void func_089F3748(Screen0B6A0 *self)
{
    self->work->field_3454 = func_089F6184(self, 9);
    self->work->flags = self->work->flags | 0x100;
    func_089F3790(self);
}

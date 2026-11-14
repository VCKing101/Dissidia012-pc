#include "menu/auto_region3.h"

/* Screen46AC: func_08AF55F0 on D_08B01CF8 with field_3630, then redraws. */
void func_08ADB6F8(Screen46AC *self)
{
    func_08AF55F0(D_08B01CF8, self->work->field_3630);
    func_08ADB784(self);
}

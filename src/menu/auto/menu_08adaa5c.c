#include "menu/auto_region3.h"

/* Screen46AC: hides handle 0x20 and redraws. */
void func_08ADAA5C(Screen46AC *self)
{
    func_088D9CFC(self->work->handles[0x20]);
    func_08ADCFD8(self, 1);
    func_08ADB784(self);
}

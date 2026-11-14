#include "menu/auto_region3.h"

/* Screen46AC: hides handle 0x21 and redraws. */
void func_08ADAB38(Screen46AC *self)
{
    func_088D9CFC(self->work->handles[0x21]);
    func_08ADD910(self);
    func_08ADB784(self);
}

#include "menu/auto_region3.h"

/* Screen46AC: plays motion 0x20 on handle 0x21 and redraws. */
void func_08ADAA9C(Screen46AC *self)
{
    func_088D9F5C(self->work->handles[0x21], 0x20, 1);
    func_08ADAAF0(self);
    func_08ADD910(self);
    func_08ADB784(self);
    func_08ADCAB8(self);
}

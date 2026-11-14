#include "menu/auto_region3.h"

/* Screen46AC: plays motion 0x20 on handle 0x20 and redraws. */
void func_08ADA9C4(Screen46AC *self)
{
    func_088D9F5C(self->work->handles[0x20], 0x20, 1);
    func_08ADAA1C(self);
    func_08ADCFD8(self, 1);
    func_08ADB784(self);
    func_08ADCAB8(self);
}

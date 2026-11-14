#include "menu/auto_region3.h"

/* Screen23F0: hides handles 0x28 and 0x29 and redraws. */
void func_08AEC0C8(Screen23F0 *self)
{
    func_088D9CFC(self->work->handles[0x28]);
    func_088D9CFC(self->work->handles[0x29]);
    func_08AEDBE8(self);
    func_08AEC3EC(self);
}

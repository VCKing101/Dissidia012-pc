#include "menu/auto_region3.h"

/* Screen23F0: plays motion 3 on handle 0x28 and redraws. */
void func_08AEBEFC(Screen23F0 *self)
{
    if ((self->work->flags & 0x2000) != 0) {
        func_08AEBFDC(self);
    }
    func_088D9F5C(self->work->handles[0x28], 3, 1);
    func_08AEBF6C(self);
    func_08AEDBE8(self);
    func_08AEC3EC(self);
    func_08AED2E8(self);
}

#include "menu/auto_region1.h"

/* Screen15A88: shows handle 0x44, func_08A155C0, field_EDC = 0xCD. */
void func_08A133E8(Screen15A88 *self)
{
    func_088D9CD8(self->work->handles[0x44]);
    func_08A155C0(self);
    self->work->field_EDC = 0xCD;
}

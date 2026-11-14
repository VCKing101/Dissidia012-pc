#include "menu/auto_region3.h"

/* ScreenC4560: clears field_8DC and flag 0x80. */
void func_08ABCD40(ScreenC4560 *self)
{
    func_08935828(self->work->field_8DC, 0, 0x14);
    self->work->flags = self->work->flags & 0xFFFFFF7F;
}

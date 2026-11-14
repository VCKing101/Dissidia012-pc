#include "menu/auto_region1.h"

/* ScreenE75C8: clears field_B74 and flag 0x200. */
void func_08A2B38C(ScreenE75C8 *self)
{
    func_08935828(&self->work->field_B74, 0, sizeof(self->work->field_B74));
    self->work->flags = self->work->flags & ~0x200;
}

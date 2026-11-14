#include "menu/auto_region3.h"

/* PanelC18: clears field_110 and flags 0x200 / 0x400. */
void func_08AC7578(PanelC18 *self)
{
    func_08935828(self->work->field_110, 0, 0x20);
    self->work->flags = self->work->flags & 0xFFFFF9FF;
}

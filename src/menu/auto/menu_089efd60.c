#include "menu/auto_region1.h"

/* Screen0B6A0: sets bit 1 of slot index's field_01. */
void func_089EFD60(Screen0B6A0 *self, int index)
{
    self->work->slots[index].field_01 = self->work->slots[index].field_01 | 1;
}

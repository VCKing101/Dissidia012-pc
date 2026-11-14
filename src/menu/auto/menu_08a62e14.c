#include "menu/auto_region2.h"

/* Panel63680: restarts the scroll timer from the shown row. */
void func_08A62E14(Panel63680 *self)
{
    self->scroll.field_10 = self->scroll.field_04;
    func_088D4A68(self->field_DE4, &self->scroll, self->field_D8, self->field_D0, 0, 0x14, 0);
}

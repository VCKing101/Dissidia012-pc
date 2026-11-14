#include "menu/auto_region2.h"

/* Panel68180: restarts the scroll timer from the shown row. */
void func_08A67540(Panel68180 *self)
{
    self->scroll.field_10 = self->scroll.field_04;
    func_088D4A68(self->field_A4, &self->scroll, self->field_40, self->field_38, 8, 0x14, 0);
}

#include "menu/auto_region2.h"

extern int D_08A5DDC8;

/* Panel5DB80: sets up the cell cursor (D_08A5DDC8 per page) and the page cursor. */
void func_08A5D580(Panel5DB80 *self)
{
    func_088D46E4((MenuCursor *)&self->field_5E8, 0, 0, D_08A5DDC8 - 1, 0, -1);
    func_088D46E4((MenuCursor *)&self->field_614, 0, 0, 1, 0, -1);
    self->field_614.flags = self->field_614.flags | 1;
    func_08A5D5F4(self);
}

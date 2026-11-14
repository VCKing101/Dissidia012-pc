#include "menu/auto_region2.h"

/* Screen4FFF4: Panel69A78 mode 2, clears work fields 0x7190, 0x7194, 0x719C. */
void func_08A4D984(Screen4FFF4 *self)
{
    func_08A697B0(self->work->field_6FF8, 2, -1);
    self->work->field_7190 = 0;
    self->work->field_7194 = 0;
    self->work->field_719C = 0;
}

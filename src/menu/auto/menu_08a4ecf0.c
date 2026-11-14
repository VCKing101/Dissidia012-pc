#include "menu/auto_region2.h"

/* Screen4FFF4: field_40 = 1 and state 3. */
void func_08A4ECF0(Screen4FFF4 *self)
{
    self->field_40 = 1;
    func_08A4D07C(self, 0, 1);
    self->work->state = 3;
}

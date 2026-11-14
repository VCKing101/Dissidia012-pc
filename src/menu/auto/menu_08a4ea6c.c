#include "menu/auto_region2.h"

/* Screen4FFF4: field_40 = 0 and state 3. */
void func_08A4EA6C(Screen4FFF4 *self)
{
    func_08A4D07C(self, 0, 1);
    self->field_40 = 0;
    self->work->state = 3;
}

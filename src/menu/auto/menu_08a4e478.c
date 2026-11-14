#include "menu/auto_region2.h"

/* Screen4FFF4: Obj5B958 to state 2, then state 0xD. */
void func_08A4E478(Screen4FFF4 *self)
{
    func_08A5BB08(self->work->field_6FE8);
    func_08A4D07C(self, 0, 0);
    self->work->state = 0xD;
}

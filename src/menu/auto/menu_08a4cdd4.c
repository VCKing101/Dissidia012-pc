#include "menu/auto_region2.h"

/* Screen4FFF4: opens Panel69A78 and Obj5B958, then state 0xB. */
void func_08A4CDD4(Screen4FFF4 *self)
{
    func_08A4C3DC(self);
    func_08A69714(self->work->field_6FF8, self->work->messages, 8);
    func_08A697B0(self->work->field_6FF8, 5, 4);
    func_08A5B9D0(self->work->field_6FE8, 1);
    func_08A4CFD4(self, 0);
    self->work->state = 0xB;
}

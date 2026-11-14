#include "menu/auto_region2.h"

/* Obj5B958: frees field_40C, resets its layout, state 0. */
void func_08A5BE78(Obj5B958 *self)
{
    func_08A80D40(self->field_40C, 2);
    func_0885C538(self->heap, self->field_40C, 0);
    self->field_40C = 0;
    func_088D3A04(&self->layout);
    func_088D38F0(&self->layout);
    self->state = 0;
}

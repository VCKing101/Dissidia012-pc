#include "menu/auto_region2.h"

/* Obj5B958 reset. */
void func_08A5B980(Obj5B958 *self)
{
    self->state = 0;
    self->heap = 0;
    self->field_40C = 0;
    func_088D38F0(&self->layout);
}

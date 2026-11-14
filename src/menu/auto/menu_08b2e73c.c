#include "menu/auto_region4.h"

void func_08B2E73C(Obj2D604 *self)
{
    self->work->flags = self->work->flags | 0x20;
    self->work->field_0C = 0;
    if (self->work->field_528 != 0) {
        func_08B30838(self->work->field_528);
    }
}

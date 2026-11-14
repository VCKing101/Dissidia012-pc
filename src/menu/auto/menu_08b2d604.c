#include "menu/auto_region4.h"

int func_08B2D604(Obj2D604 *self)
{
    func_08B2D7A4(self);
    self->work->state = 8;
    if ((self->work->flags & 0x20) != 0) {
        func_08B30838(self->work->field_528);
    }
    return 0;
}

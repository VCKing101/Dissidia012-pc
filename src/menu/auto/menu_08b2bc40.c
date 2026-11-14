#include "menu/auto_region4.h"

void func_08B2BC40(Obj29F3C *self)
{
    Obj2D604 *child;

    self->work->flags = self->work->flags | 1;
    self->work->field_10 = 0;
    child = self->work->field_708;
    if (child != 0) {
        func_08B2E780(child, 0);
        func_08B2E73C(self->work->field_708);
    }
}

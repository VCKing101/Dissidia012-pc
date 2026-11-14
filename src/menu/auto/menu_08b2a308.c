#include "menu/auto_region4.h"

int func_08B2A308(Obj29F3C *self)
{
    func_08B2A61C(self);
    self->work->state = 10;
    if ((self->work->flags & 1) != 0) {
        func_08B2E780(self->work->field_708, 0);
        func_08B2E73C(self->work->field_708);
    }
    return 0;
}

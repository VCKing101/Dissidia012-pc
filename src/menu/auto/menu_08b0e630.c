#include "menu/auto_region4.h"

int func_08B0E630(ScreenB1CB00 *self)
{
    int picked;

    picked = func_08B100F0(self);
    self->work->field_24814 = picked;
    func_08B1B1A4(self, 0);
    func_08B0B9C4(self);
    func_08885528(D_089D69E8);
    self->work->state = 2;
    return 0;
}

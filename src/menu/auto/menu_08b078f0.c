#include "menu/auto_region4.h"

void func_08B078F0(ScreenB1CB00 *self)
{
    func_088D9F5C(self->work->handles[0x23], 0x2C, 1);
    ((MenuModelView *)self->work->handles[0x23])->field_0E = -1;
    func_08B072A8(self);
    func_08B097A4(self);
}

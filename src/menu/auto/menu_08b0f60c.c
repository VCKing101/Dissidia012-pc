#include "menu/auto_region4.h"

extern const unsigned short D_08B1BC64[];

void func_08B0F60C(ScreenB1CB00 *self)
{
    func_088D3CF0(&self->work->layout, self->work->handles, D_08B1BC64);
    ((MenuModelView *)self->work->handles[0x10A])->field_0E = 9;
    func_088D9CFC(self->work->handles[0x10E]);
}

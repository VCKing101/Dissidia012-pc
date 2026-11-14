#include "menu/auto_region4.h"

extern const unsigned short D_08B1BC64[];
extern const unsigned short D_08B1BC94[];

void func_08B0F6B0(ScreenB1CB00 *self)
{
    func_088D3EA0(&self->work->layout, self->work->handles, D_08B1BC64);
    func_088D442C(self->work->handles, self->work->texts, D_08B1BC94);
}

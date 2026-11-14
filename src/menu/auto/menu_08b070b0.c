#include "menu/auto_region4.h"

extern const unsigned short D_08B1C1E8[];
extern const unsigned short D_08B1C268[];
extern const unsigned short D_08B1C288[];

void func_08B070B0(ScreenB1CB00 *self)
{
    func_088D3EA0(&self->work->layout, self->work->handles, D_08B1C1E8);
    func_088D442C(self->work->handles, self->work->texts, D_08B1C288);
    func_088D4664(self->work->handles, self->work->texts, D_08B1C268);
    func_08B0785C(self, 1);
    func_08B06110(self);
}

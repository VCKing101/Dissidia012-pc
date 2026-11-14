#include "menu/auto_region4.h"

extern const unsigned short D_08B1C102[];
extern const unsigned short D_08B1C15A[];
extern const unsigned short D_08B1C1B2[];

void func_08B06A38(ScreenB1CB00 *self)
{
    func_088D3EA0(&self->work->layout, self->work->handles, D_08B1C102);
    func_088D442C(self->work->handles, self->work->texts, D_08B1C15A);
    func_088D442C(self->work->handles, self->work->texts, D_08B1C1B2);
    func_08B0785C(self, 1);
    func_08B06110(self);
}

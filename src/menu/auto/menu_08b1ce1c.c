#include "menu/auto_region4.h"

extern const unsigned short D_08B22770[];

void func_08B1CE1C(ScreenB22E98 *self)
{
    func_088D3CF0(&self->work->layout, self->work->handles, D_08B22770);
    func_08B1CE5C(self);
}

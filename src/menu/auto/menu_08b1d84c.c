#include "menu/auto_region4.h"

extern const short D_08B224A8[];

void func_08B1D84C(ScreenB22E98 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08B224A8);
    func_08B1D774(self);
    func_08B1CCB0(self);
    func_08B1CDF0(self);
    func_08B2041C(self);
}

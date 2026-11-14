#include "menu/auto_region4.h"

extern const short D_08B2F564[];
extern const unsigned short D_08B2F60C[];

void func_08B2D8D0(Obj2D604 *self)
{
    func_088D3F00(self->work->layout, self->work->handles, D_08B2F564);
    func_088D44EC(self->work->handles, self->work->texts, D_08B2F60C);
    func_08B2DC3C(self);
}

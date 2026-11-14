#include "menu/auto_region4.h"

extern const short D_08B2D0D4[];
extern const unsigned short D_08B2D104[];

void func_08B2AF98(Obj29F3C *self)
{
    func_088D3F00(self->work->layout, self->work->handles, D_08B2D0D4);
    func_088D44EC(self->work->handles, self->work->texts, D_08B2D104);
    func_08B2B064(self);
}

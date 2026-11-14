#include "menu/auto_region4.h"

extern const short D_08B2CCF0[];
extern const unsigned short D_08B2CEE4[];

void func_08B2AE90(Obj29F3C *self)
{
    func_088D3F00(self->work->layout, self->work->handles, D_08B2CCF0);
    func_088D44EC(self->work->handles, self->work->texts, D_08B2CEE4);
}

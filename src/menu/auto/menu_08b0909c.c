#include "menu/auto_region4.h"

extern const short D_08B1C3E8[];
extern const unsigned short D_08B1C420[];

void func_08B0909C(ScreenB1CB00 *self)
{
    WorkB1CB00 *work;

    work = self->work;
    if ((work->flags & 0x1010) != 0) {
        func_088D3F00(&work->layout, work->handles, D_08B1C3E8);
        func_088D44EC(self->work->handles, self->work->texts, D_08B1C420);
        func_08B09238(self);
    }
}

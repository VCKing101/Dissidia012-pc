#include "menu/auto_region4.h"

extern const short D_08B1C102[];
extern const unsigned short D_08B1C15A[];
extern const unsigned short D_08B1C1B2[];

void func_08B069A8(ScreenB1CB00 *self)
{
    if ((self->work->handles[0xA1]->field_0C != -1) && (func_08B06BA0(self) == 0)) {
        func_088D3F00(&self->work->layout, self->work->handles, D_08B1C102);
        func_088D44EC(self->work->handles, self->work->texts, D_08B1C15A);
        func_088D44EC(self->work->handles, self->work->texts, D_08B1C1B2);
    }
}

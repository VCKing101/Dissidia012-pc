#include "menu/auto_region4.h"

extern const short D_08B1C850[];
extern const unsigned short D_08B1C888[];

void func_08B0E32C(ScreenB1CB00 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08B1C850);
    func_088D44EC(self->work->handles, self->work->texts, D_08B1C888);
    func_08B0E53C(self);
}

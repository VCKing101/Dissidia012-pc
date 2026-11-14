#include "menu/auto_region4.h"

extern const short D_08B1C6C0[];
extern const unsigned short D_08B1C778[];

void func_08B0E084(ScreenB1CB00 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08B1C6C0);
    func_088D44EC(self->work->handles, self->work->texts, D_08B1C778);
    func_08B0E0DC(self);
}

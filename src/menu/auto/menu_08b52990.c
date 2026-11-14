#include "menu/auto_region4.h"

extern const short D_08B52E58[];
extern const unsigned short D_08B52E78[];

void func_08B52990(Widget525B0 *self)
{
    func_088D3F00(0, self->handles, D_08B52E58);
    func_088D44EC(self->handles, self->texts, D_08B52E78);
}

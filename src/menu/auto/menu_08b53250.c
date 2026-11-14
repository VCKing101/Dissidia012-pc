#include "menu/auto_region4.h"

extern const short D_08B53530[];
extern const unsigned short D_08B53548[];

void func_08B53250(Widget52EB0 *self)
{
    func_088D3F00(0, self->handles, D_08B53530);
    func_088D44EC(self->handles, self->texts, D_08B53548);
}

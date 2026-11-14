#include "menu/auto_region4.h"

extern const short D_08B4C798[];
extern const unsigned short D_08B4C7B0[];

void func_08B4C330(Widget4C010 *self)
{
    func_088D3F00(0, self->handles, D_08B4C798);
    func_088D44EC(self->handles, self->texts, D_08B4C7B0);
}

#include "menu/auto_region4.h"

extern const short D_08B53B58[];
extern const unsigned short D_08B53B70[];

void func_08B538C8(Widget535C8 *self)
{
    func_088D3F00(0, self->handles, D_08B53B58);
    func_088D44EC(self->handles, self->texts, D_08B53B70);
}

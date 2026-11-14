#include "menu/auto_region4.h"

extern const unsigned short D_08B53B58[];

void func_08B53984(Widget535C8 *self)
{
    self->handles[0] = func_088DAA68(self->id);
    self->handles[1] = func_088DAA68(self->id);
    func_088D3CF0(0, self->handles, D_08B53B58);
}

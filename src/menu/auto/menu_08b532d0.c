#include "menu/auto_region4.h"

extern const unsigned short D_08B53530[];

void func_08B532D0(Widget52EB0 *self)
{
    self->handles[1] = func_088DAA68(self->id);
    self->handles[0] = func_088DAA68(self->id);
    func_088D3CF0(0, self->handles, D_08B53530);
}

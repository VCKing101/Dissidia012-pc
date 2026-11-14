#include "menu/auto_region4.h"

extern const unsigned short D_08B4CA68[];

void func_08B4C938(Widget4C7F8 *self)
{
    self->handles[0] = func_088DAA68(self->id);
    func_088D3CF0(0, self->handles, D_08B4CA68);
    self->state = 1;
}

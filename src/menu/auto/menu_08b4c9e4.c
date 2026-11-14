#include "menu/auto_region4.h"

extern const short D_08B4CA68[];

void func_08B4C9E4(Widget4C7F8 *self)
{
    if (self->state == 1) {
        func_088D3F00(0, self->handles, D_08B4CA68);
    }
}

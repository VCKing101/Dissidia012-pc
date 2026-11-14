#include "menu/auto_region4.h"

extern const short D_08B52598[];

void func_08B52444(Widget52288 *self)
{
    if (self->state == 1) {
        func_088D3F00(0, self->handles, D_08B52598);
    }
}

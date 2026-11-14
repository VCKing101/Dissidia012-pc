#include "menu/auto_region4.h"

void func_08B37A0C(ScreenB41CC0 *self)
{
    int done;

    done = func_08B4E7D4(self->field_C8->field_24);
    if (done != 0) {
        self->field_C4->state = 6;
    }
}

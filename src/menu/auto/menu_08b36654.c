#include "menu/auto_region4.h"

void func_08B36654(ScreenB41CC0 *self)
{
    int done;

    done = func_08B530A0(self->field_C8->field_0C);
    if (done != 0) {
        func_08B530C4(self->field_C8->field_0C);
        func_08B35030(self);
        func_08B3B764(self, 0);
        self->field_C4->state = 3;
    }
}

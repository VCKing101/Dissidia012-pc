#include "menu/auto_region4.h"

void func_08B37A4C(ScreenB41CC0 *self)
{
    int answer;

    answer = func_08B4F5C4(self->field_C8->field_34);
    if (answer == 1) {
        func_08B508C0(self->field_C8->field_30, self->field_38, 1);
        self->field_C4->state = 6;
    }
}

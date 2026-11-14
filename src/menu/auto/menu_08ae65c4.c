#include "menu/auto_region3.h"

/* ScreenA690: plays motions 6 / 2 (or 5 / 3 with field_266C) on handles 0x6B and 0x6C. */
void func_08AE65C4(ScreenA690 *self)
{
    int first;
    int second;

    first = 6;
    second = 2;
    if (self->work->field_266C != 0) {
        first = 5;
        second = 3;
    }
    func_088D9F5C(self->work->handles[0x6B], first, 1);
    func_088D9F5C(self->work->handles[0x6C], second, 1);
}

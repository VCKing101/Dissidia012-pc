#include "menu/auto_region1.h"

/* Declared with an int here: retail passes the loaded byte on unchanged
 * (func_089EFD0C's char declaration would sign-extend it again). */
extern unsigned short func_089EF880(Screen0B6A0 *self, int value);

/* Screen0B6A0: hands the current entry's field_01 to func_089F7224, or
 * func_089F72DC's value when func_089EF880 maps it to 0xF. */
void func_089F73AC(Screen0B6A0 *self)
{
    int index;
    int value;

    index = func_089F2FF0(self);
    value = self->work->entries[index].field_01;
    index = func_089EF880(self, value);
    if (index == 0xF) {
        value = func_089F72DC(self);
    }
    func_089F7224(self, value);
}

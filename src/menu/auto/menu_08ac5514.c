#include "menu/auto_region3.h"

/* ScreenC47C4: 2 if func_088F1094, else 3 if func_08AC4930 on field_984, else 4. */
int func_08AC5514(ScreenC47C4 *self)
{
    int found;
    int result;

    result = 4;
    found = func_08AC4930(self, self->work->field_984);
    if (found != 0) {
        result = 3;
    }
    found = func_088F1094();
    if (found != 0) {
        result = 2;
    }
    return result;
}

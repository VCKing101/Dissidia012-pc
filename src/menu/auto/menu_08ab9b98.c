#include "menu/auto_region3.h"

/* ScreenBB9E4: field_E60, at most 5. */
int func_08AB9B98(ScreenBB9E4 *self)
{
    int result;
    int count;

    count = self->work->field_E60;
    result = 5;
    if (count < 5) {
        result = count;
    }
    return result;
}

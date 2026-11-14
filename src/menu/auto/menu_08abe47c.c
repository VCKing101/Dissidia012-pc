#include "menu/auto_region3.h"

/* ScreenC4560: hides handles 1..4. */
void func_08ABE47C(ScreenC4560 *self)
{
    int i;

    for (i = 1; i < 5; i++) {
        func_088D9CFC(self->work->handles[i]);
    }
}

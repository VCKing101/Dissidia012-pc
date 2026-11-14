#include "menu/auto_region3.h"

/* ScreenAB7CFC: hides handles 0x49..0x52 and 0x19..0x22. */
void func_08AA9EF8(ScreenAB7CFC *self)
{
    int i;

    for (i = 0; i < 10; i++) {
        func_088D9CFC(self->work->handles[i + 0x49]);
    }
    for (i = 0; i < 10; i++) {
        func_088D9CFC(self->work->handles[i + 0x19]);
    }
}

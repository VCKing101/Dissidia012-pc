#include "menu/auto_region3.h"

/* ScreenC4560: frees texts 4..0x13. */
void func_08ABC8C4(ScreenC4560 *self)
{
    int i;

    for (i = 4; i < 0x14; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
}

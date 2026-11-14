#include "menu/auto_region1.h"

/* Screen38EA8: func_0887B944 on texts 8..0x13. */
void func_08A37180(Screen38EA8 *self)
{
    int i;

    for (i = 8; i < 0x14; i++) {
        if (self->work->texts[i] != 0) {
            func_0887B944(self->work->texts[i]);
        }
    }
}

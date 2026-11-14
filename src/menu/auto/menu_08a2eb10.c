#include "menu/auto_region1.h"

/* Screen35550: frees its 0x14 texts. */
void func_08A2EB10(Screen35550 *self)
{
    int i;

    for (i = 0; i < 0x14; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
}

#include "menu/auto_region1.h"

/* Screen38EA8: frees its 0x17 texts. */
void func_08A35A98(Screen38EA8 *self)
{
    int i;

    for (i = 0; i < 0x17; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
}

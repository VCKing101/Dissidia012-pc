#include "menu/auto_region3.h"

/* View8E3B4: releases its texts. */
void func_08A8ED84(View8E3B4 *self)
{
    int i;

    for (i = 0; i < 6; i++) {
        if (self->work->prompt_texts[i] != 0) {
            func_0887BA5C(self->work->prompt_texts[i]);
            self->work->prompt_texts[i] = 0;
        }
    }
    for (i = 0; i < 0x19; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
    func_08A8DE60(self, 1, -1);
}

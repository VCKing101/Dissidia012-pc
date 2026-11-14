#include "menu/auto_region3.h"

/* ScreenA7ED0 deleting destructor: plays the close script and frees the texts. */
void func_08AA507C(ScreenA7ED0 *self, unsigned int flags)
{
    int i;

    if (self != 0) {
        self->vtable = D_08AA7ED0;
        func_08AA46AC(self);
        for (i = 0; i < 0x21; i++) {
            if (self->work->texts[i] != 0) {
                func_0887BA5C(self->work->texts[i]);
                self->work->texts[i] = 0;
            }
        }
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}

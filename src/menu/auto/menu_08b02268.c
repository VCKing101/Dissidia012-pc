#include "menu/auto_region4.h"

/* Screen02144: releases texts 0x22..0x3A and every handle, then plays the
 * closing layout script. */
void func_08B02268(Screen02144 *self)
{
    const short *script;
    int i;

    script = func_08B045B8();
    func_08B03584(self, 1, -1);
    func_08B02638(self);
    for (i = 0; i < 0x19; i++) {
        if (self->work->texts[i + 0x22] != 0) {
            func_0887BA5C(self->work->texts[i + 0x22]);
            self->work->texts[i + 0x22] = 0;
        }
    }
    for (i = 0; i < 0x53; i++) {
        if (self->work->handles[i] != 0) {
            func_088DA060(self->work->handles[i]);
            self->work->handles[i] = 0;
        }
    }
    func_08AF6DB8(D_08B01CF8);
    func_088D3A5C(&self->work->layout, script, 0xF);
}

#include "menu/auto_region3.h"

/* Screen850: frees its texts and handles and plays the closing script. */
void func_08ADFF7C(Screen850 *self)
{
    const short *script;
    int i;

    script = func_08AE46B0();
    func_08AE20B0(self);
    for (i = 0; i < 0x11; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
    func_08AE0F70(self, 1, -1);
    for (i = 0; i < 0x32; i++) {
        if (self->work->handles[i] != 0) {
            func_088DA060(self->work->handles[i]);
            self->work->handles[i] = 0;
        }
    }
    func_08AF6DB8(D_08B01CF8);
    func_088D3A5C(&self->work->layout, script, 0xF);
}

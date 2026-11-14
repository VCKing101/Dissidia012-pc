#include "menu/auto_region3.h"

/* ScreenC4560: resets the layout, frees the texts and runs func_08885220. */
void func_08ABCE68(ScreenC4560 *self)
{
    int i;

    func_088D3A04(&self->work->layout);
    for (i = 0; i < 0x35; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
    func_08885220(D_089D69E8);
}

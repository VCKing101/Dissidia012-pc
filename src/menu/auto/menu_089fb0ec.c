#include "menu/auto_region1.h"

extern const short D_08A09506[];

/* Screen0B6A0: frees texts 0x71..0x77, plays the release script D_08A09506
 * and sets field_3460 to -1. */
void func_089FB0EC(Screen0B6A0 *self, void **texts)
{
    int i;

    for (i = 0x71; i < 0x78; i++) {
        if (texts[i] != 0) {
            func_0887BA5C(texts[i]);
            texts[i] = 0;
        }
    }
    func_088D3C24(&self->work->layout, self->work->handles, D_08A09506, 0x180);
    self->work->field_3460 = -1;
}

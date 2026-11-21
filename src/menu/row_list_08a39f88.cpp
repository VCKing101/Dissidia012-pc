#include "menu/agent36_types.h"

/* Builds the list of rows to show (at most 0x200): rows marked always, and
 * the others whose item has no id or a known one. Returns how many, at
 * least 1. */

struct Row39F88 {
    char pad_00[0x3];
    unsigned char always;     /* +0x03 */
};

extern "C" {
int func_08A3C010(void);
Row39F88 *func_08A3C1C0(int index);
}


extern "C" int func_08A39F88(ShopScreen36 *self)
{
    int count = func_08A3C010();
    int n;

    if (count > 0x200) {
        count = 0x200;
    }
    func_08935828(self->work->list, 0, 0x400);
    n = 0;
    for (int i = 0; i < count; i++) {
        if (func_08A3C1C0(i)->always == 0) {
            self->work->list[n] = i;
            n++;
        } else {
            ShopItem36 *item = func_088CC364(i);
            int shown = 1;
            if (item->id != 0xFFFF) {
                shown = Known36::Of(item->id);
            }
            if (shown) {
                self->work->list[n] = i;
                n++;
            }
        }
    }
    if (n == 0) {
        n = 1;
    }
    return n;
}

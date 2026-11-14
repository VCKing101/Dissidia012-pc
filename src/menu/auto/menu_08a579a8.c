#include "menu/auto_region2.h"

extern const short D_08A585FC[];
extern const unsigned short D_08A5862C[];

/* Panel58420: plays its scripts, then updates its items. */
void func_08A579A8(Panel58420 *self)
{
    int i;

    func_088D3F00(0, self->handles, D_08A585FC);
    func_088D44EC(self->handles, self->texts, D_08A5862C);
    for (i = 0; i < 6; i++) {
        func_08A550D8(self->items[i]);
    }
}

#include "menu/auto_region2.h"

extern const short D_08A5DC00[];
extern const unsigned short D_08A5DC28[];

/* Panel5DB80: plays its scripts, then each cell's. */
void func_08A5D078(Panel5DB80 *self)
{
    int i;

    func_088D3F00(0, self->handles, D_08A5DC00);
    func_088D44EC(self->handles, self->texts, D_08A5DC28);
    for (i = 0; i < 16; i++) {
        func_08A5C7D8(&self->cells[i]);
    }
}

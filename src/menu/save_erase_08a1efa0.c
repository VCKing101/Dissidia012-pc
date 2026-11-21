#include "menu/agent36_types.h"

/* Menu root: erases the save shown at list place index, from the network
 * copy or from the save data, then rebuilds the list order. */
void func_08A1EFA0(MenuRoot36 *self, int index)
{
    unsigned short slot;

    slot = self->work->records[self->work->order[index]].slot;
    if (func_08A1D8E4(self) != 0) {
        func_088F7BA0(self->work->net, slot);
        self->work->records[slot].flags &= ~1;
        self->work->used--;
        func_08A1CD14(self);
        self->work->net_flags &= ~0x40;
    } else {
        func_088F53F4(D_089D712C->slots, slot);
        func_08A1C0B8(self);
    }
    func_088ACA10(D_089D712C);
    func_08A1C2F8(self);
}

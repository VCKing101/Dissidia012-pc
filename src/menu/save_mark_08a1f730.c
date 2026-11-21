#include "menu/agent36_types.h"

/* Menu root: marks the save at list place index (not the "new save"
 * place 25), in the network copy or in the save data. */
void func_08A1F730(MenuRoot36 *self, int index)
{
    int place;
    unsigned short slot;

    place = self->work->order[index];
    if (place == 25) {
        return;
    }
    slot = self->work->records[place].slot;
    if (func_08A1D8E4(self) != 0) {
        func_088F7DE0(self->work->net, slot);
        func_088F7E2C(self->work->net, slot);
    } else {
        SaveSlot36 *slots = D_089D712C->slots;
        func_088F55D0(slots, slot);
        func_088F55F4(slots, slot);
    }
    func_088ACA10(D_089D712C);
}

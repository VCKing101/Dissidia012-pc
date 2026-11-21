#include "menu/agent36_types.h"

/* Menu root: reads the summary part of save slot slot, from the network
 * copy or from the save data. */
void func_08A20828(MenuRoot36 *self, int slot, SaveSummary36 *out)
{
    SaveSlot36 info;

    if (func_08A1D8E4(self) != 0) {
        func_088F7D8C(self->work->net, slot, &info);
    } else {
        func_088F5584(D_089D712C->slots, slot, &info);
    }
    *out = info.summary;
}

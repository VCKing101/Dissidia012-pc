#include "menu/agent36_types.h"

/* Menu root: reads the save shown at list place index into the effect
 * block, from the network copy or from the save data. */
void func_08A1F2CC(MenuRoot36 *self, int index)
{
    unsigned short slot;
    int none = 0;

    func_08A1F108(self);
    func_088AEFA0(D_089D7260);
    slot = self->work->records[self->work->order[index]].slot;
    if (func_08A1D8E4(self) != 0) {
        func_088F7D8C(self->work->net, slot, &self->effect->save_info);
    } else {
        func_088F5584(D_089D712C->slots, slot, &self->effect->save_info);
    }
    func_088F5198(&self->effect->save_info);
    func_08B5810C(none);
}

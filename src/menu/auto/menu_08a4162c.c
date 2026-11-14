#include "menu/auto_region2.h"

/* MenuRoot2: empties the pair queue and clears flag 0x40. */
void func_08A4162C(MenuRoot2 *self)
{
    func_08935828(&self->work->queue, 0, sizeof(PairQueue));
    self->work->flags = self->work->flags & ~0x40;
}

#include "menu/auto_region4.h"

/* Block4229C: returns its block to the heap and clears itself. */
void func_08B422B8(Block4229C *self)
{
    func_0885C538(self->heap, self->block, 0x40);
    func_08B423BC(self);
}

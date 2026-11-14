#include "menu/auto_region4.h"

/* HeapBlock: returns its block to the heap and clears itself. */
void func_08B4280C(HeapBlock *self)
{
    func_0885C538(self->heap, self->block, 0);
    func_08B42A58(self);
}

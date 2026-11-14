#include "menu/auto_region4.h"

/* HeapBlock: returns its block to the heap. */
void func_08B436D8(HeapBlock *self)
{
    func_0885C538(self->heap, self->block, 0);
}

#include "main/auto_region4.h"

/* BlockRef: releases the heap and the block. */
void func_088DFBF8(BlockRef *self)
{
    func_0885C488(self->heap);
    self->heap = 0;
    func_0888208C(self->block);
    self->block = 0;
}

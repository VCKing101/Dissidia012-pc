#include "main/auto_region2.h"

/* Frees the block to its heap. */
void func_0887B3B8(BlockRef *self)
{
    func_0885C538(self->heap, self->block, 0);
    self->block = 0;
}

#include "main/auto_region2.h"

/* Frees the blocks to their heaps, releases the heaps and frees the main block. */
void func_0886D038(Pool86CE7C *self)
{
    func_0885C538(self->heap_14, self->block_0C, 0);
    func_0885C488(self->heap_14);
    self->heap_14 = 0;
    func_0885C488(self->heap_18);
    self->heap_18 = 0;
    func_0885C538(self->heap_04, self->block_08, 0);
    func_0885C538(self->heap_04, self->block_10, 0);
    func_0885C488(self->heap_04);
    self->heap_04 = 0;
    func_0888218C(self->block, 4);
    self->block = 0;
}

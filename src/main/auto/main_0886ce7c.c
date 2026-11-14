#include "main/auto_region2.h"

/* Constructor of Pool86CE7C: no heaps or blocks, then the elements and items. */
Pool86CE7C *func_0886CE7C(Pool86CE7C *self)
{
    self->block = 0;
    self->heap_04 = 0;
    self->block_08 = 0;
    self->block_0C = 0;
    self->block_10 = 0;
    self->heap_14 = 0;
    self->heap_18 = 0;
    func_08993560(self->elems, 0xc0, 0x13c, func_08869C40);
    func_08993560(self->items, 0x180, 0x1c, &D_0886546C);
    return self;
}

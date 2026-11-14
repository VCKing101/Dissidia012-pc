#include "main/auto_region2.h"

/* Sets up the world: its heap, empty lists, then func_08808364 and func_0884E628. */
void func_08841C4C(WorldView *self, void *heap)
{
    self->heap = heap;
    self->field_10 = 0;
    self->actors = 0;
    self->field_18 = 0;
    self->first_14 = 0;
    self->field_40 = 0;
    self->field_3C = 0;
    self->field_48 = 0;
    self->field_44 = 0;
    func_08808364();
    func_0884E628();
}

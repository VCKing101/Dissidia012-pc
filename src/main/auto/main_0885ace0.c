#include "main/auto_region2.h"

/* Frees the block to its heap and releases the handle. */
void func_0885ACE0(HandleSlot *self)
{
    if (self->heap != 0) {
        func_0885C538(self->heap, self->block, 0);
    }
    self->block = 0;
    self->heap = 0;
    if (self->handle != 0) {
        func_0899BB1C(self->handle);
    }
    self->handle = 0;
}

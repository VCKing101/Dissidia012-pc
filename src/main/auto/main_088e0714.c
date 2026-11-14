#include "main/auto_region4.h"

/* Obj9CC358: copies size bytes of field_162C into a new block from its heap,
 * records the block, and hands the bytes to the next table entry. */
void func_088E0714(int size, Obj9CC358 *self)
{
    void *block;
    void *source;

    block = func_0885C6CC(self->heap, size, 0x40, 0, 1, 0);
    source = self->field_162C;
    self->blocks[self->count] = block;
    self->sizes[self->count] = size;
    func_08935524(block, source, size);
    func_088FE944(&self->field_08->entries[self->count], self->field_162C, size);
    self->field_17AE = 0;
    self->count = self->count + 1;
}

#include "main/auto_region2.h"

/* Releases the table's heap and frees its block. */
void func_088440F8(Table9D4960 *self)
{
    func_0885C488(self->heap);
    self->heap = 0;
    func_08882158(self->block);
    self->block = 0;
}

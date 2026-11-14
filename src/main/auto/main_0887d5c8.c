#include "main/auto_region2.h"

/* Copies head and table from other. */
void func_0887D5C8(Obj87D574 *self, const Obj87D574 *other)
{
    func_08935524(self->head, other->head, 0x10);
    func_08935524(self->table, other->table, 0x300);
}

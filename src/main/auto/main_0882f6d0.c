#include "main/auto_region2.h"

/* Whether the table holds an entry for key. */
unsigned char func_0882F6D0(Obj82F470 *self, int key)
{
    Entry82F664 *entry;

    entry = func_08934944(key, self->table->entries, self->table->count, 8, func_0882EF04);
    return entry != 0;
}

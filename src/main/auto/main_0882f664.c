#include "main/auto_region2.h"

/* The id of the table entry for index (field_50 when index is negative); -1 if there is none. */
unsigned int func_0882F664(Obj82F470 *self, int index)
{
    unsigned int result;
    int key;
    Entry82F664 *entry;

    if (index < 0) {
        index = self->field_50;
    }
    if (index < 0) {
        result = 0xffffffff;
    } else {
        key = func_0882FE28(self, index);
        entry = func_08934944(key, self->table->entries, self->table->count, 8, func_0882EF04);
        result = entry->id;
    }
    return result;
}

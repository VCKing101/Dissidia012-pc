#include "main/auto_region2.h"

/* Overwrites the stored record with the same id as record. */
void func_08847068(Records9D4968 *self, Record9D4968 *record)
{
    Record9D4968 *slot;

    slot = func_08934944(record->id, self->table->records, self->table->count, 0x44, func_08846C0C);
    func_08935524(slot, record, 0x44);
}

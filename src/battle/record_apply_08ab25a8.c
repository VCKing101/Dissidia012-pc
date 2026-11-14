#include "battle/agent23_types.h"

typedef struct SlotRecord {
    int id;
    char values[0xC];
} SlotRecord;

extern SlotRecord *func_08AAD070(UnitParamTable *table, unsigned int slot, int index);
extern int func_08AB2AAC(UnitParamOwner *self, unsigned int slot, void *values);

/* Applies record index of slot; -1 when inactive, out of range or missing. */
int func_08AB25A8(UnitParamOwner *self, unsigned int slot, int index)
{
    SlotRecord *record;
    if (self->active == 0) {
        return -1;
    }
    if (slot >= 3 || index < 0) {
        return -1;
    }
    record = func_08AAD070(func_08A80200(D_08A80218), slot, index);
    if (record == 0) {
        return -1;
    }
    return func_08AB2AAC(self, slot, record->values);
}

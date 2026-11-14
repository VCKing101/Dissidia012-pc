typedef struct UnitEntry {
    int id;
    char pad_004[0x184];
} UnitEntry;

typedef struct UnitList {
    unsigned char active;
    char pad_001[0x14F];
    int count;
    UnitEntry *entries;
} UnitList;

UnitEntry *func_08AACA24(UnitList *self, int id)
{
    if (self->active == 0) {
        return 0;
    }
    {
        int i;
        for (i = 0; i < self->count; i++) {
            UnitEntry *entry = &self->entries[i];
            if (entry->id == id) {
                return entry;
            }
        }
    }
    return 0;
}

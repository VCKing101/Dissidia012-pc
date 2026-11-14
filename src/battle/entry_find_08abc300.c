typedef struct Entry4C {
    int id;
    char pad_04[0x48];
} Entry4C;

typedef struct EntryTable4C {
    unsigned char active;
    char pad_01[0x1F];
    int count;
    Entry4C *entries;
} EntryTable4C;

/* The entry with the given id, 0 when there is none. */
Entry4C *func_08ABC300(EntryTable4C *self, int id)
{
    if (self->active == 0) {
        return 0;
    }
    {
        int i;
        for (i = 0; i < self->count; i++) {
            Entry4C *entry = &self->entries[i];
            if (entry->id == id) {
                return entry;
            }
        }
    }
    return 0;
}

typedef struct ResolveEntry {
    char pad_00[0x8];
    short index;
} ResolveEntry;

typedef struct ResolveTable {
    char pad_00[0x4];
    void *table;
} ResolveTable;

typedef struct ResolveOwner {
    char pad_00[0x4];
    ResolveTable *tables;
} ResolveOwner;

extern int func_08AB332C(void *table, int index);

int func_08A7BEC0(ResolveOwner *self, ResolveEntry *entry)
{
    short *index;

    if (entry == 0) {
        return 0;
    }
    index = &entry->index;
    if (*index < 0) {
        return 0;
    }
    if (self->tables->table == 0) {
        return 0;
    }
    return func_08AB332C(self->tables->table, *index);
}

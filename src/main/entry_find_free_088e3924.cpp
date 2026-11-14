/* Returns the first of the 64 entries at 0x160 whose value is 0. */
struct FreeEntry {
    int key;
    int value;

    static bool isFree(FreeEntry *e)
    {
        return e->value == 0;
    }
};

struct FreeTable {
    char pad_000[0x160];
    FreeEntry entries[64];
};

extern "C" {
FreeEntry *func_088E3924(FreeTable *table)
{
    for (int i = 0; i < 64; i++) {
        if (FreeEntry::isFree(&table->entries[i])) {
            return &table->entries[i];
        }
    }
    return 0;
}
}

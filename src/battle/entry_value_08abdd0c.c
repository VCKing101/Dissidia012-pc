typedef struct Entry4C {
    int id;
    char pad_04[0x44];
    int value;
} Entry4C;

typedef struct EntryTable4C EntryTable4C;

typedef struct TableRef {
    int pad_00;
    EntryTable4C *table;
} TableRef;

extern Entry4C *func_08ABC300(EntryTable4C *table, int id);

/* Value of entry 1, 0 without both arguments or the entry. */
int func_08ABDD0C(TableRef *self, void *a, void *b)
{
    Entry4C *entry;
    if (a == 0 || b == 0) {
        return 0;
    }
    entry = func_08ABC300(self->table, 1);
    if (entry == 0) {
        return 0;
    }
    return entry->value;
}

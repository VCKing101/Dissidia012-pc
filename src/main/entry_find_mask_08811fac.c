/* Returns the next entry after `after` whose kind bit is set in mask, or 0. */
typedef struct Entry08811FAC {
    int field_00;
    unsigned char kind;
    char pad_05[0x1B];
} Entry08811FAC;

typedef struct Table08811FAC {
    int field_00;
    int field_04;
    int count;
    int field_0C;
    Entry08811FAC entries[1];
} Table08811FAC;

Entry08811FAC *func_08811FAC(Table08811FAC *table, unsigned int mask, Entry08811FAC *after)
{
    Entry08811FAC *base = table->entries;
    Entry08811FAC *end = base + table->count;
    if (after == 0) {
        after = base;
    } else {
        after++;
    }
    for (; after < end; after++) {
        if (mask & (1 << after->kind)) {
            return after;
        }
    }
    return 0;
}

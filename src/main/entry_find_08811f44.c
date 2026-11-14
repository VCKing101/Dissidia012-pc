/* Returns the next entry after `after` whose kind matches (0x12 matches any), or 0. */
typedef struct Entry08811F44 {
    int field_00;
    unsigned char kind;
    char pad_05[0x1B];
} Entry08811F44;

typedef struct Table08811F44 {
    int field_00;
    int field_04;
    int count;
    int field_0C;
    Entry08811F44 entries[1];
} Table08811F44;

Entry08811F44 *func_08811F44(Table08811F44 *table, unsigned int kind, Entry08811F44 *after)
{
    Entry08811F44 *base = table->entries;
    Entry08811F44 *end = base + table->count;
    if (after == 0) {
        after = base;
    } else {
        after++;
    }
    for (; after < end; after++) {
        if (kind == 0x12 || after->kind == kind) {
            return after;
        }
    }
    return 0;
}

/* Returns the entry after `cur` (the first one when cur is 0), or 0 at the end. */
typedef struct Entry0881E610 {
    char data[0x24];
} Entry0881E610;

typedef struct Table0881E610 {
    unsigned char valid;
    char pad_01[0x87];
    int count;
    Entry0881E610 *entries;
} Table0881E610;

Entry0881E610 *func_0881E610(Table0881E610 *t, Entry0881E610 *cur)
{
    int index;
    if (!t->valid) {
        return 0;
    }
    index = 0;
    if (cur != 0) {
        index = cur - t->entries + 1;
    }
    if (index == t->count) {
        return 0;
    }
    return &t->entries[index];
}

/* Returns the entry a key refers to, or 0. */
typedef struct Entry0881E780 {
    char data[0x68];
} Entry0881E780;

typedef struct Key0881E780 {
    short field_00;
    short index;
} Key0881E780;

typedef struct Table0881E780 {
    unsigned char valid;
    char pad_01[0x4B];
    unsigned int count;
    Entry0881E780 *entries;
} Table0881E780;

Entry0881E780 *func_0881E780(Table0881E780 *t, Key0881E780 *key)
{
    if (!t->valid) {
        return 0;
    }
    if (key == 0) {
        return 0;
    }
    if (key->index >= t->count) {
        return 0;
    }
    return &t->entries[key->index];
}

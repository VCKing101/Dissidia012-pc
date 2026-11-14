/* Binary-searches the owner's table of 0x54-byte entries for a key. */
typedef struct SortedTable088CB870 {
    int field_00;
    int count;
    char entries[1];
} SortedTable088CB870;

typedef struct Owner088CB870 {
    char pad_00[0x4];
    SortedTable088CB870 *table;
} Owner088CB870;

void *func_08934944(const void *key, const void *base, int count, int size, int (*compare)(const void *, const void *));
int func_088CB834(const void *a, const void *b);

void *func_088CB870(Owner088CB870 *self, const void *key)
{
    SortedTable088CB870 *table = self->table;
    return func_08934944(key, table->entries, table->count, 0x54, func_088CB834);
}

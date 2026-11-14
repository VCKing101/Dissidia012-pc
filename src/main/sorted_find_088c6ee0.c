/* Binary-searches the owner's table of 0x58-byte entries for a key. */
typedef struct SortedTable088C6EE0 {
    int field_00;
    int count;
    char entries[1];
} SortedTable088C6EE0;

typedef struct Owner088C6EE0 {
    char pad_00[0x14];
    SortedTable088C6EE0 *table;
} Owner088C6EE0;

void *func_08934944(const void *key, const void *base, int count, int size, int (*compare)(const void *, const void *));
int func_088C6B58(const void *a, const void *b);

void *func_088C6EE0(Owner088C6EE0 *self, const void *key)
{
    SortedTable088C6EE0 *table = self->table;
    return func_08934944(key, table->entries, table->count, 0x58, func_088C6B58);
}

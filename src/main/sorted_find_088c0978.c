/* Binary-searches the owner's table of 0x1C-byte entries for a key. */
typedef struct SortedTable088C0978 {
    int field_00;
    int count;
    char entries[1];
} SortedTable088C0978;

typedef struct Owner088C0978 {
    SortedTable088C0978 *table;
} Owner088C0978;

void *func_08934944(const void *key, const void *base, int count, int size, int (*compare)(const void *, const void *));
int func_088C07A8(const void *a, const void *b);

void *func_088C0978(Owner088C0978 *self, const void *key)
{
    SortedTable088C0978 *table = self->table;
    return func_08934944(key, table->entries, table->count, 0x1C, func_088C07A8);
}

/* Binary-searches the owner's table of 0x70-byte entries for a key. */
typedef struct SortedTable088C1B68 {
    int field_00;
    int count;
    char entries[1];
} SortedTable088C1B68;

typedef struct Owner088C1B68 {
    SortedTable088C1B68 *table;
} Owner088C1B68;

void *func_08934944(const void *key, const void *base, int count, int size, int (*compare)(const void *, const void *));
int func_088C1B30(const void *a, const void *b);

void *func_088C1B68(Owner088C1B68 *self, const void *key)
{
    SortedTable088C1B68 *table = self->table;
    return func_08934944(key, table->entries, table->count, 0x70, func_088C1B30);
}

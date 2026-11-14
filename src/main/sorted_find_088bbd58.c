/* Binary-searches the owner's table of 0x24-byte entries for a key. */
typedef struct SortedTable088BBD58 {
    int field_00;
    int count;
    char entries[1];
} SortedTable088BBD58;

typedef struct Owner088BBD58 {
    SortedTable088BBD58 *table;
} Owner088BBD58;

void *func_08934944(const void *key, const void *base, int count, int size, int (*compare)(const void *, const void *));
int func_088BBAA8(const void *a, const void *b);

void *func_088BBD58(Owner088BBD58 *self, const void *key)
{
    SortedTable088BBD58 *table = self->table;
    return func_08934944(key, table->entries, table->count, 0x24, func_088BBAA8);
}

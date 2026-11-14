/* Binary-searches the owner's table of 0x7E-byte entries for a key. */
typedef struct SortedTable088BC764 {
    int field_00;
    int count;
    char entries[1];
} SortedTable088BC764;

typedef struct Owner088BC764 {
    SortedTable088BC764 *table;
} Owner088BC764;

void *func_08934944(const void *key, const void *base, int count, int size, int (*compare)(const void *, const void *));
int func_088BBE50(const void *a, const void *b);

void *func_088BC764(Owner088BC764 *self, const void *key)
{
    SortedTable088BC764 *table = self->table;
    return func_08934944(key, table->entries, table->count, 0x7E, func_088BBE50);
}

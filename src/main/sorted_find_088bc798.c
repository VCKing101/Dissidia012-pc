/* Binary-searches the owner's table of 0x5E-byte entries for a key. */
typedef struct SortedTable088BC798 {
    int field_00;
    int count;
    char entries[1];
} SortedTable088BC798;

typedef struct Owner088BC798 {
    char pad_00[0x4];
    SortedTable088BC798 *table;
} Owner088BC798;

void *func_08934944(const void *key, const void *base, int count, int size, int (*compare)(const void *, const void *));
int func_088BBE20(const void *a, const void *b);

void *func_088BC798(Owner088BC798 *self, const void *key)
{
    SortedTable088BC798 *table = self->table;
    return func_08934944(key, table->entries, table->count, 0x5E, func_088BBE20);
}

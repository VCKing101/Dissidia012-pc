/* Binary-searches the owner's table of 0x18-byte entries for a key. */
typedef struct SortedTable088BC9F8 {
    int field_00;
    int count;
    char entries[1];
} SortedTable088BC9F8;

typedef struct Owner088BC9F8 {
    char pad_00[0x10];
    SortedTable088BC9F8 *table;
} Owner088BC9F8;

void *func_08934944(const void *key, const void *base, int count, int size, int (*compare)(const void *, const void *));
int func_088BBE44(const void *a, const void *b);

void *func_088BC9F8(Owner088BC9F8 *self, const void *key)
{
    SortedTable088BC9F8 *table = self->table;
    return func_08934944(key, table->entries, table->count, 0x18, func_088BBE44);
}

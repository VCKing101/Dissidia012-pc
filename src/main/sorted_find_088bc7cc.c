/* Binary-searches the owner's table of 0x24-byte entries for a key. */
typedef struct SortedTable088BC7CC {
    int field_00;
    int count;
    char entries[1];
} SortedTable088BC7CC;

typedef struct Owner088BC7CC {
    char pad_00[0x8];
    SortedTable088BC7CC *table;
} Owner088BC7CC;

void *func_08934944(const void *key, const void *base, int count, int size, int (*compare)(const void *, const void *));
int func_088BBE2C(const void *a, const void *b);

void *func_088BC7CC(Owner088BC7CC *self, const void *key)
{
    SortedTable088BC7CC *table = self->table;
    return func_08934944(key, table->entries, table->count, 0x24, func_088BBE2C);
}

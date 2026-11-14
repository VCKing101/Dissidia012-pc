/* Binary-searches the owner's table of 0x68-byte entries for a key. */
typedef struct SortedTable088B3B20 {
    int field_00;
    int count;
    char entries[1];
} SortedTable088B3B20;

typedef struct Owner088B3B20 {
    char pad_00[0x18];
    SortedTable088B3B20 *table;
} Owner088B3B20;

void *func_08934944(const void *key, const void *base, int count, int size, int (*compare)(const void *, const void *));
int func_088B3824(const void *a, const void *b);

void *func_088B3B20(Owner088B3B20 *self, const void *key)
{
    SortedTable088B3B20 *table = self->table;
    return func_08934944(key, table->entries, table->count, 0x68, func_088B3824);
}

/* Address of entry i of a table whose signed count byte sits at offset 1; 0 when out of range. */
typedef struct Entry088B33EC {
    int field_00;
    int field_04;
} Entry088B33EC;

typedef struct Table088B33EC {
    char field_00;
    signed char count;
    char pad_02[6];
    Entry088B33EC entries[1];
} Table088B33EC;

Entry088B33EC *func_088B33EC(Table088B33EC *table, int i)
{
    Entry088B33EC *result = 0;
    if (i >= 0 && i < table->count) {
        Entry088B33EC *entries = table->entries;
        result = &entries[i];
    }
    return result;
}

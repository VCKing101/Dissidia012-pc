/* Last entry whose key byte is not above the value; 0 when none. */
struct Entry088B6E14 {
    signed char key;
    char pad_01[7];
};

struct Table088B6E14 {
    int field_00;
    int count;
    Entry088B6E14 *entries;
};

extern "C" {

Entry088B6E14 *func_088B6E14(Table088B6E14 *table, int value)
{
    Entry088B6E14 *result = 0;
    for (int i = table->count - 1; i >= 0; i--) {
        if (table->entries[i].key <= value) {
            result = &table->entries[i];
            break;
        }
    }
    return result;
}

}

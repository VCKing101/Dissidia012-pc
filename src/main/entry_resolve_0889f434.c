typedef struct Entry0889F434 {
    short field_00;
    short id;
    unsigned char unknown_04[0x28];
} Entry0889F434;

typedef struct Extra0889F434 {
    int field_00;
    unsigned char data[0x80];
} Extra0889F434;

typedef struct Table0889F434 {
    int field_00;
    int field_04;
    Entry0889F434 *entries;
    unsigned char field_0C[0x34];
    int count;
    Extra0889F434 *extra;
} Table0889F434;

void *func_0887AFBC(void *cache, int id, int flags);

void *func_0889F434(Table0889F434 *table, int index) {
    void *result = 0;
    if (index < table->count) {
        Entry0889F434 *entry = &table->entries[index];
        if (entry != 0) {
            result = func_0887AFBC(table->field_0C, entry->id, 0);
        }
    } else {
        result = table->extra[index - table->count].data;
    }
    return result;
}

typedef struct Entry0889F3E4 {
    unsigned char data[0x2C];
} Entry0889F3E4;

typedef struct Table0889F3E4 {
    int field_00;
    int field_04;
    Entry0889F3E4 *entries;
} Table0889F3E4;

int func_0889F5E8(Table0889F3E4 *table, int key);

Entry0889F3E4 *func_0889F3E4(Table0889F3E4 *table, int key) {
    int index = func_0889F5E8(table, key);
    if (index >= 0) {
        return &table->entries[index];
    } else {
        return 0;
    }
}

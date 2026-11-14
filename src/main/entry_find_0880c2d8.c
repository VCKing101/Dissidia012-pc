typedef struct Entry0880C2D8 {
    short id;
    unsigned char unknown_02[0x3E];
} Entry0880C2D8;

typedef struct Table0880C2D8 {
    unsigned char unknown_0000[0x20];
    int count;
    Entry0880C2D8 *entries;
} Table0880C2D8;

int func_0880C2D8(Table0880C2D8 *table, int id) {
    int i;
    for (i = 0; i < table->count; i++) {
        if (table->entries[i].id == id) {
            return i;
        }
    }
    return -1;
}

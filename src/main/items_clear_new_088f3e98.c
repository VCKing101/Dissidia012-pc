/* Clears the "new" byte of every item and accessory entry. */
typedef struct ItemEntry {
    short id;
    unsigned char fresh;
    unsigned char count;
} ItemEntry;

typedef struct ItemTables {
    ItemEntry items[0x2AB];
    ItemEntry accessories[0x276];
} ItemTables;

typedef struct SaveData {
    char pad_00000[0xF6E0];
    ItemTables tables;
} SaveData;

extern SaveData *D_089D712C;

void func_088F3E98(void)
{
    ItemTables *tables = &D_089D712C->tables;
    int i;

    for (i = 0; i < 0x2AB; i++) {
        tables->items[i].fresh = 0;
    }
    for (i = 0; i < 0x276; i++) {
        tables->accessories[i].fresh = 0;
    }
}

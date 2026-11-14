typedef struct SlotTable {
    char pad_00[0x1E0];
    int count;
} SlotTable;

extern int func_088D93D8(SlotTable *table);

int func_088D9518(SlotTable *table)
{
    if (func_088D93D8(table) == 0) {
        return -1;
    }
    table->count = 0;
    return 0;
}

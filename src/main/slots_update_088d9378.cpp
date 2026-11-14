typedef struct Slot44 {
    char data[0x44];
} Slot44;

typedef struct SlotTable {
    unsigned char active;
    char pad_01[3];
    Slot44 slots[7];
} SlotTable;

extern "C" {
extern int func_088D93D8(SlotTable *table);
extern void func_088D8BF0(Slot44 *slot);

int func_088D9378(SlotTable *table)
{
    if (func_088D93D8(table) == 0) {
        return -1;
    }
    for (int i = 0; i < 7; i++) {
        func_088D8BF0(&table->slots[i]);
    }
    return 0;
}
}

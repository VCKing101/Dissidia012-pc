typedef struct Slot44 {
    char data[0x44];
} Slot44;

typedef struct SlotTable {
    unsigned char active;
    char pad_01[3];
    Slot44 slots[7];
} SlotTable;

extern "C" {
extern void func_088D9254(SlotTable *table);
extern int func_088D8B58(Slot44 *slot, void *first, void *second, void *third, int index);

int func_088D92AC(SlotTable *table, void *first, void *second, void *third)
{
    func_088D9254(table);
    if (first == 0 || second == 0 || third == 0) {
        return -1;
    }
    for (int i = 0; i < 7; i++) {
        if (func_088D8B58(&table->slots[i], first, second, third, i) != 0) {
            return -2;
        }
    }
    table->active = 1;
    return 0;
}
}

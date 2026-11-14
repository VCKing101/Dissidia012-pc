/* Resets the seven 0x44-byte slots, then the owner. */
typedef struct Slot44 {
    char data[0x44];
} Slot44;

typedef struct SlotTable {
    unsigned char active;
    char pad_01[3];
    Slot44 slots[7];
} SlotTable;

extern "C" {
extern void func_088D8A54(Slot44 *slot);
extern void func_088D9248(SlotTable *table);

void func_088D9254(SlotTable *table)
{
    for (int i = 0; i < 7; i++) {
        func_088D8A54(&table->slots[i]);
    }
    func_088D9248(table);
}
}

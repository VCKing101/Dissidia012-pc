typedef struct Slot08811EFC {
    unsigned char unknown_00[4];
    unsigned char key;
    unsigned char unknown_05[0x1B];
} Slot08811EFC;

typedef struct Table08811EFC {
    unsigned char unknown_00[8];
    int count;
    unsigned char unknown_0C[4];
    Slot08811EFC slots[1];
} Table08811EFC;

Slot08811EFC *func_08811EFC(Table08811EFC *table, int key) {
    Slot08811EFC *slot = table->slots;
    Slot08811EFC *end = slot + table->count;
    for (; slot < end; slot++) {
        if (slot->key == key) {
            return slot;
        }
    }
    return 0;
}

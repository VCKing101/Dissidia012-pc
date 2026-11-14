typedef struct Agent4Entry34 {
    unsigned char data[0x34];
} Agent4Entry34;

typedef struct Agent4EntryBank {
    int field_00;
    int field_04;
    Agent4Entry34 entries[1];
} Agent4EntryBank;

extern Agent4EntryBank D_09AFE090;

Agent4Entry34 *func_08830C9C(void *self, int index) {
    return &D_09AFE090.entries[index];
}

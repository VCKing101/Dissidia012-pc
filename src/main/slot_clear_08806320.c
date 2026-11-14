typedef struct Agent4Slot {
    unsigned char field_00;
    unsigned char field_01;
    unsigned char field_02;
    unsigned char field_03;
    float field_04;
} Agent4Slot;

typedef struct Agent4SlotOwner {
    unsigned char unknown_0000[0x584];
    Agent4Slot slots_a[16];
    Agent4Slot slots_b[16];
} Agent4SlotOwner;

void func_08806320(Agent4SlotOwner *self) {
    int i;
    Agent4Slot *slot;
    for (i = 0; i < 16; i++) {
        slot = &self->slots_b[i];
        slot->field_00 = 0;
        slot->field_01 = 0;
        slot->field_02 = 0;
        slot->field_03 = 0;
        slot->field_04 = 0.0f;
    }
}

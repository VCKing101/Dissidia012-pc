typedef struct MenuSaveSlots {
    char pad_0000[0x79A4];
    int field_79A4;
    int field_79A8;
    int field_79AC;
    int field_79B0[5];
    int field_79C4[5];
} MenuSaveSlots;

typedef struct MenuSaveOwner {
    char pad_00[0x58];
    MenuSaveSlots *field_58;
} MenuSaveOwner;

extern "C" void func_08A510E0(MenuSaveOwner *self, int value) {
    self->field_58->field_79A4 = 0;
    self->field_58->field_79AC = 0;
    for (int i = 0; i < 5; i++) {
        self->field_58->field_79B0[i] = 0;
        self->field_58->field_79C4[i] = 0;
    }
    self->field_58->field_79B0[0] = value;
}

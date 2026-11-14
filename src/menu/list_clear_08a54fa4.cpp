struct A6Clear {
    int field_00[7];
    char pad_1C[0x38];
    int field_54;
    int field_58;
    int field_5C[12];
    char pad_8C[0x68];
    int field_F4;
    char field_F8;
    char pad_F9[3];
    int field_FC;
    char pad_100[0x2C];
    int field_12C;
};

extern "C" {
void func_08A54FA4(A6Clear *self) {
    for (int i = 0; i < 7; i++) {
        self->field_00[i] = 0;
    }
    for (int i = 0; i < 12; i++) {
        self->field_5C[i] = 0;
    }
    self->field_F4 = 0;
    self->field_F8 = 0;
    self->field_FC = 0;
    self->field_54 = 0;
    self->field_12C = 0;
}
}

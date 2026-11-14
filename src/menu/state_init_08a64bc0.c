typedef struct A6Init2 {
    char pad_00[3];
    char field_03;
    char pad_04[0xC];
    int field_10;
    char pad_14[4];
    int field_18[6];
    char pad_30[0x30];
    int field_60[2];
} A6Init2;

void func_08A638C4(A6Init2 *self);

void func_08A64BC0(A6Init2 *self) {
    int i;
    func_08A638C4(self);
    self->field_03 = 1;
    self->field_10 = 0;
    for (i = 0; i < 6; i++) {
        self->field_18[i] = 0;
    }
    for (i = 0; i < 2; i++) {
        self->field_60[i] = 0;
    }
}

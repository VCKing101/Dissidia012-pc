typedef struct A6Clear3 {
    char pad_00[0xC];
    int field_0C;
    int field_10;
    int field_14;
    int field_18[8];
    char pad_38[0x40];
    int field_78[8];
} A6Clear3;

void func_08A59064(A6Clear3 *self) {
    int i;
    self->field_0C = 0;
    self->field_10 = 0;
    self->field_14 = 0;
    for (i = 0; i < 8; i++) {
        self->field_18[i] = 0;
    }
    for (i = 0; i < 8; i++) {
        self->field_78[i] = 0;
    }
}

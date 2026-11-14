typedef struct A6Clear2 {
    char pad_00[0xC];
    int field_0C;
    int field_10;
    int field_14[5];
    char pad_28[0x28];
    int field_50[2];
    int field_58;
    int field_5C[3];
} A6Clear2;

void func_08A588A4(A6Clear2 *self) {
    int i;
    self->field_0C = 0;
    self->field_10 = 0;
    for (i = 0; i < 5; i++) {
        self->field_14[i] = 0;
    }
    for (i = 0; i < 2; i++) {
        self->field_50[i] = 0;
    }
    self->field_58 = 0;
    for (i = 0; i < 3; i++) {
        self->field_5C[i] = 0;
    }
}

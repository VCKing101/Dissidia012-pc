typedef struct A6State2 {
    int field_00;
    int field_04[5];
    char pad_18[0x28];
    int field_40[2];
    char field_48;
    char field_49;
    char field_4A;
    char pad_4B;
    int field_4C;
    int field_50;
} A6State2;

void func_08A5C5F0(A6State2 *self) {
    int i;
    self->field_00 = 0;
    for (i = 0; i < 5; i++) {
        self->field_04[i] = 0;
    }
    for (i = 0; i < 2; i++) {
        self->field_40[i] = 0;
    }
    self->field_48 = 0;
    self->field_49 = 0;
    self->field_4A = 1;
    self->field_4C = -1;
    self->field_50 = -1;
}

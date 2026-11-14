typedef struct A6List {
    int field_00;
    int field_04[8];
    char pad_24[0x40];
    int field_64[4];
    char pad_74[0xc];
    char field_80;
} A6List;

void func_08A52D98(A6List *self) {
    int i;
    self->field_00 = 0;
    for (i = 0; i < 8; i++) {
        self->field_04[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        self->field_64[i] = 0;
    }
    self->field_80 = 0;
}

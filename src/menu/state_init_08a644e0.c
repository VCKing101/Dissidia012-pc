typedef struct A6Init {
    char pad_00[3];
    char field_03;
    char field_04;
    char pad_05[0xB];
    int field_10;
    int field_14[4];
    char pad_24[0x20];
    int field_44[2];
} A6Init;

void func_08A638C4(A6Init *self);

void func_08A644E0(A6Init *self) {
    int i;
    func_08A638C4(self);
    self->field_03 = 1;
    self->field_04 = 1;
    self->field_10 = 0;
    for (i = 0; i < 4; i++) {
        self->field_14[i] = 0;
    }
    for (i = 0; i < 2; i++) {
        self->field_44[i] = 0;
    }
}

typedef struct A6Toggle {
    char pad_00[0x49];
    unsigned char field_49;
} A6Toggle;

void func_08A5CAE0(A6Toggle *self);

void func_08A5C724(A6Toggle *self) {
    self->field_49 = self->field_49 == 0;
    func_08A5CAE0(self);
}

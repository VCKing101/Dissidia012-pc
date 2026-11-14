typedef struct A6Guard {
    char pad_00[0x2C];
    int field_2C;
    int field_30;
} A6Guard;

void func_08A60310(A6Guard *self, int a, int b, int c);

void func_08A603C0(A6Guard *self, int v) {
    if (self->field_2C == 0) {
        return;
    }
    func_08A60310(self, self->field_2C, self->field_30, v);
}

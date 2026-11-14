typedef struct A6SelInner {
    char pad_000[0x2B4];
    int field_2B4;
    char pad_2B8[0x2C];
    int field_2E4;
    char pad_2E8[0x20];
    int field_308[1];
} A6SelInner;

typedef struct A6SelSelf {
    char pad_00[8];
    A6SelInner *field_08;
} A6SelSelf;

void func_08A8E7AC(A6SelSelf *self);

void func_08A8ED2C(A6SelSelf *self, int keep) {
    func_08A8E7AC(self);
    self->field_08->field_2E4 = -1;
    if (keep == 0) {
        A6SelInner *in = self->field_08;
        in->field_2E4 = in->field_308[in->field_2B4];
    }
}

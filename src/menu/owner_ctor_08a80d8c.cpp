struct A6Src2 {
    int field_00;
    char pad_04[2];
    unsigned short field_06;
    int field_08;
};

struct A6Inner2 {
    char pad_000[0x404];
    int field_404;
    char pad_408[0x188];
    int field_590;
    char pad_594[0x60];
    int field_5F4;
};

struct A6Owner2 {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    int field_14;
    int field_18;
    A6Src2 *field_1C;
    int field_20;
    int field_24;
    A6Inner2 *field_28;
};

extern "C" {
void *func_0885C6CC(int a, int b, int c, int d, int e, int f);
void func_08A81BB8(A6Owner2 *self);

void func_08A80D8C(A6Owner2 *self, int a1, A6Src2 *a2, int a3, int t0, int t1, int t2, int t3, int s0, int s1) {
    self->field_00 = a1;
    self->field_04 = a3;
    self->field_1C = a2;
    self->field_08 = ((unsigned char *)a2)[4];
    self->field_20 = t0;
    self->field_14 = t1;
    self->field_18 = t2;
    self->field_24 = t3;
    self->field_0C = s0;
    self->field_10 = s1;
    int one = 1;
    A6Inner2 *in = (A6Inner2 *)func_0885C6CC(a1, 0x7DC, 0, 0, one, 0);
    self->field_28 = in;
    in->field_590 = self->field_1C->field_08;
    self->field_28->field_5F4 = self->field_1C->field_06;
    self->field_28->field_404 = self->field_1C->field_00;
    func_08A81BB8(self);
}
}

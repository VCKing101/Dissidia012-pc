struct A6Cell2 {
    char data[0x14];
};

struct A6Gauge {
    char pad_00[0x10];
    A6Cell2 field_10[5];
    int field_74;
};

extern "C" {
void func_088D3174(A6Cell2 *c);
void func_088D319C(A6Cell2 *c);

void func_08A760DC(A6Gauge *self, int n) {
    for (int i = 0; i < 5; i++) {
        if (i < n) {
            func_088D3174(&self->field_10[i]);
        } else {
            func_088D319C(&self->field_10[i]);
        }
    }
    self->field_74 = n;
}
}

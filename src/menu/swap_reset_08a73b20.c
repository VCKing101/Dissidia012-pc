typedef struct A6Sel {
    char pad_000[0x148];
    int field_148;
} A6Sel;

void func_08A73B8C(A6Sel *self, int a, int b);

void func_08A73B20(A6Sel *self) {
    int old = self->field_148;
    int m1 = -1;
    self->field_148 = m1;
    func_08A73B8C(self, m1, old);
}

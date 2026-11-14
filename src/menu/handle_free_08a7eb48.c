typedef struct A6Hold {
    int field_00;
    char pad_04[0x24];
    int field_28;
} A6Hold;

void func_08A80C24(A6Hold *self);
void func_0885C538(int a, int b, int c);

void func_08A7EB48(A6Hold *self) {
    int h;
    func_08A80C24(self);
    h = self->field_28;
    func_0885C538(self->field_00, h, 0);
    self->field_28 = 0;
}

struct A6Cell3 {
    char data[0x14];
};

struct A6Cells {
    char pad_00[4];
    A6Cell3 field_04[10];
};

extern "C" {
void func_08A75BFC(A6Cell3 *c);
void func_088D2F30(A6Cells *self);

void func_08A76214(A6Cells *self) {
    for (int i = 0; i < 10; i++) {
        func_08A75BFC(&self->field_04[i]);
    }
    func_088D2F30(self);
}
}

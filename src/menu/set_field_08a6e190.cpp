struct A6Setter {
    char pad_00[4];
    int field_04;
};

extern "C" {
void func_08A6E388(A6Setter *self);

void func_08A6E190(A6Setter *self, int v) {
    func_08A6E388(self);
    self->field_04 = v;
}
}

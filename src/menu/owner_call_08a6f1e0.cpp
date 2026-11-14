class A6Target {
public:
    virtual void x0(int, int, int);
};

struct A6Caller {
    char pad_00[4];
    A6Target *field_04;
};

extern "C" {
void func_08A6E32C(char *p, A6Caller *owner, int a, int b);

void func_08A6F1E0(A6Caller *self) {
    func_08A6E32C((char *)self + 0x1D4, self, 0, 0);
    self->field_04->x0(0, 0, 0);
}

void func_08A6F23C(A6Caller *self) {
    func_08A6E32C((char *)self + 0x1D4, self, 1, 0);
    self->field_04->x0(1, 0, 0);
}

void func_08A6F298(A6Caller *self) {
    func_08A6E32C((char *)self + 0x1D4, self, 2, 0);
    self->field_04->x0(2, 0, 0);
}

void func_08A6F2F4(A6Caller *self) {
    func_08A6E32C((char *)self + 0x1D4, self, 4, 0);
    self->field_04->x0(4, 0, 0);
}
}

class A6Target2 {
public:
    virtual void x0(int, int, int);
};

struct A6Caller2 {
    char pad_00[4];
    A6Target2 *field_04;
};

extern "C" {
void func_08A6E32C(char *p, A6Caller2 *owner, int a, int b);

void func_08A6F4BC(A6Caller2 *self) {
    func_08A6E32C((char *)self + 0x1D4, self, 7, 0);
    self->field_04->x0(7, 0, 0);
}
}

class A6Target3 {
public:
    virtual void x0(int);
};

struct A6Menu3 {
    char pad_000[0x1FC];
    A6Target3 *field_1FC;
    char pad_200[0x14];
    int field_214;
};

extern "C" {
void func_08904F24(A6Menu3 *self);

void func_08A6FBC4(A6Menu3 *self) {
    func_08904F24(self);
    if (self->field_214 != 0) {
        A6Target3 *t = self->field_1FC;
        if (t != 0) {
            t->x0(self->field_214);
        }
        self->field_214 = 0;
    }
}
}

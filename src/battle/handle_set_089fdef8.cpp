extern "C" {
extern void func_088DA060(void *);
extern void func_089FDEB0(void *);
}

struct HandleSetDEF8 {
    int field_00;
    void *field_04;
    void *field_08[3];
};

extern "C" {
void func_089FDEF8(HandleSetDEF8 *self)
{
    for (int i = 0; i < 3; i++) {
        if (self->field_08[i]) {
            func_088DA060(self->field_08[i]);
            self->field_08[i] = 0;
        }
    }
    if (self->field_04) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_089FDEB0(self);
}
}

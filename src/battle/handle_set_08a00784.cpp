extern "C" {
extern void func_088DA060(void *);
extern void func_08A00738(void *);
}

struct HandleSet0784 {
    int field_00;
    void *field_04;
    void *field_08[4];
};

extern "C" {
void func_08A00784(HandleSet0784 *self)
{
    for (int i = 0; i < 4; i++) {
        if (self->field_08[i]) {
            func_088DA060(self->field_08[i]);
            self->field_08[i] = 0;
        }
    }
    if (self->field_04) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_08A00738(self);
}
}

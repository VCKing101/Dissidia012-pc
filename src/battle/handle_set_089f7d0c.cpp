extern "C" {
extern void func_088DA060(void *);
extern void func_089F7CD8(void *);
}

struct HandleSet7D0C {
    int field_00;
    void *field_04[4];
};

extern "C" {
void func_089F7D0C(HandleSet7D0C *self)
{
    for (int i = 0; i < 4; i++) {
        if (self->field_04[i]) {
            func_088DA060(self->field_04[i]);
            self->field_04[i] = 0;
        }
    }
    func_089F7CD8(self);
}
}

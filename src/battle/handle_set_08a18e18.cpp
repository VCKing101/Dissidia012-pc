extern "C" {
extern void func_088DA060(void *);
extern void func_08A18DD8(void *);
}

struct HandleSet8E18 {
    int field_00;
    void *field_04[2];
};

extern "C" {
void func_08A18E18(HandleSet8E18 *self)
{
    for (int i = 0; i < 2; i++) {
        if (self->field_04[i]) {
            func_088DA060(self->field_04[i]);
            self->field_04[i] = 0;
        }
    }
    func_08A18DD8(self);
}
}

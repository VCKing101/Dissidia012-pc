extern "C" {
extern void func_088DA060(void *);
extern void func_089E82C8(void *);
}

struct HandleSet {
    char pad_000[0x14];
    void *field_14[4];
    void *field_24[4];
};

extern "C" {
void func_089E831C(HandleSet *self)
{
    for (int i = 0; i < 4; i++) {
        if (self->field_24[i]) {
            func_088DA060(self->field_24[i]);
            self->field_24[i] = 0;
        }
        if (self->field_14[i]) {
            func_088DA060(self->field_14[i]);
            self->field_14[i] = 0;
        }
    }
    func_089E82C8(self);
}
}

extern "C" {
extern void func_088DA060(void *);
extern void func_089E8CA8(void *);
}

struct HandleSet84E4 {
    char pad_000[0x8];
    unsigned char field_08;
    char pad_009[0xB];
    void *field_14[4];
    void *field_24[4];
};

extern "C" {
int func_089E84E4(HandleSet84E4 *self)
{
    if (!self->field_08) {
        return -1;
    }
    func_089E8CA8(self);
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
    return 0;
}
}

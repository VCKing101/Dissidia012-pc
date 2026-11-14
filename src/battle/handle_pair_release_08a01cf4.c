typedef struct HandlePair {
    unsigned char active;
    char pad_01[0x3];
    void *field_04;
    void *field_08;
} HandlePair;

extern void func_088DA060(void *handle);

int func_08A01CF4(HandlePair *self)
{
    if (!self->active) {
        return -1;
    }
    if (self->field_08 != 0) {
        func_088DA060(self->field_08);
        self->field_08 = 0;
    }
    if (self->field_04 != 0) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    return 0;
}

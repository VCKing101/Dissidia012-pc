typedef struct HandlePair {
    unsigned char active;
    char pad_01[0x7];
    void *field_08;
    void *field_0C;
} HandlePair;

extern void func_088DA060(void *handle);

int func_08A16C58(HandlePair *self)
{
    if (!self->active) {
        return -1;
    }
    if (self->field_0C != 0) {
        func_088DA060(self->field_0C);
        self->field_0C = 0;
    }
    if (self->field_08 != 0) {
        func_088DA060(self->field_08);
        self->field_08 = 0;
    }
    return 0;
}

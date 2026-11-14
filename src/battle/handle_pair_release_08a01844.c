typedef struct HandlePair {
    char pad_00[0x4];
    void *field_04;
    void *field_08;
} HandlePair;

extern void func_088DA060(void *handle);
extern void func_08A01818(HandlePair *self);

void func_08A01844(HandlePair *self)
{
    if (self->field_08 != 0) {
        func_088DA060(self->field_08);
        self->field_08 = 0;
    }
    if (self->field_04 != 0) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_08A01818(self);
}

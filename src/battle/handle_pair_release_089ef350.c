typedef struct HandlePair {
    char pad_00[0x8];
    void *field_08;
    char pad_0C[0xC];
    void *field_18;
} HandlePair;

extern void func_088DA060(void *handle);
extern void func_089EF320(HandlePair *self);

void func_089EF350(HandlePair *self)
{
    if (self->field_18 != 0) {
        func_088DA060(self->field_18);
        self->field_18 = 0;
    }
    if (self->field_08 != 0) {
        func_088DA060(self->field_08);
        self->field_08 = 0;
    }
    func_089EF320(self);
}

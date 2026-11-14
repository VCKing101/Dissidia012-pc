typedef struct HandlePair {
    char pad_00[0x8];
    void *field_08;
    void *field_0C;
} HandlePair;

extern void func_088DA060(void *handle);
extern void func_08A11338(HandlePair *self);

void func_08A11350(HandlePair *self)
{
    if (self->field_0C != 0) {
        func_088DA060(self->field_0C);
        self->field_0C = 0;
    }
    if (self->field_08 != 0) {
        func_088DA060(self->field_08);
        self->field_08 = 0;
    }
    func_08A11338(self);
}

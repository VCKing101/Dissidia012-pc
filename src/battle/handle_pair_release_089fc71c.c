typedef struct HandlePair {
    char pad_00[0x4];
    void *field_04;
    void *field_08;
} HandlePair;

extern void func_088DA060(void *handle);
extern void func_089FC6F8(HandlePair *self);

void func_089FC71C(HandlePair *self)
{
    if (self->field_08 != 0) {
        func_088DA060(self->field_08);
        self->field_08 = 0;
    }
    if (self->field_04 != 0) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_089FC6F8(self);
}

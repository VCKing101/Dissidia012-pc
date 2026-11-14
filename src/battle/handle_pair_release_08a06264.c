typedef struct HandlePair {
    char pad_00[0x8];
    void *field_08;
    char pad_0C[0x8];
    void *field_14;
} HandlePair;

extern void func_088DA060(void *handle);
extern void func_08A06228(HandlePair *self);

void func_08A06264(HandlePair *self)
{
    if (self->field_14 != 0) {
        func_088DA060(self->field_14);
        self->field_14 = 0;
    }
    if (self->field_08 != 0) {
        func_088DA060(self->field_08);
        self->field_08 = 0;
    }
    func_08A06228(self);
}

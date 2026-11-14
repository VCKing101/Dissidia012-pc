typedef struct HandleTriple {
    char pad_00[0x4];
    void *field_04;
    void *field_08;
    void *field_0C;
} HandleTriple;

extern void func_088DA060(void *handle);
extern void func_089F38E8(HandleTriple *self);

void func_089F3908(HandleTriple *self)
{
    if (self->field_0C != 0) {
        func_088DA060(self->field_0C);
        self->field_0C = 0;
    }
    if (self->field_08 != 0) {
        func_088DA060(self->field_08);
        self->field_08 = 0;
    }
    if (self->field_04 != 0) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_089F38E8(self);
}

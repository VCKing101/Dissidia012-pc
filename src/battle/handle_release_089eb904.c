typedef struct HandleOwner {
    char pad_000[0x8];
    void *field_08;
} HandleOwner;

extern void func_088DA060(void *handle);
extern void func_089EB8F0(HandleOwner *self);

void func_089EB904(HandleOwner *self)
{
    if (self->field_08) {
        func_088DA060(self->field_08);
        self->field_08 = 0;
    }
    func_089EB8F0(self);
}

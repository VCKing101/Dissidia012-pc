typedef struct Holder201C8 {
    char pad_00[0x4];
    char field_04[0x20];
    void *field_24;
} Holder201C8;

extern void func_088DA060(void *handle);
extern void func_089F7D0C(void *sub);
extern void func_08A201AC(Holder201C8 *self);

void func_08A201C8(Holder201C8 *self)
{
    if (self->field_24 != 0) {
        func_088DA060(self->field_24);
        self->field_24 = 0;
    }
    func_089F7D0C(self->field_04);
    func_08A201AC(self);
}

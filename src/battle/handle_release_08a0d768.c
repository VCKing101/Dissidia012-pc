typedef struct HandleOwnerD768 {
    char pad_000[0x4];
    void *field_04;
} HandleOwnerD768;

extern void func_088DA060(void *handle);
extern void func_08A0D750(HandleOwnerD768 *self);

void func_08A0D768(HandleOwnerD768 *self)
{
    if (self->field_04) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_08A0D750(self);
}

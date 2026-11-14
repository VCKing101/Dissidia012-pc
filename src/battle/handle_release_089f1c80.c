typedef struct HandleOwnerF1C80 {
    char pad_000[0x4];
    void *field_04;
} HandleOwnerF1C80;

extern void func_088DA060(void *handle);
extern void func_089F1C68(HandleOwnerF1C80 *self);

void func_089F1C80(HandleOwnerF1C80 *self)
{
    if (self->field_04) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_089F1C68(self);
}

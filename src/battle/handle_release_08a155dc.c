typedef struct HandleOwner155DC {
    char pad_000[0x4];
    void *field_04;
} HandleOwner155DC;

extern void func_088DA060(void *handle);
extern void func_08A155D0(HandleOwner155DC *self);

void func_08A155DC(HandleOwner155DC *self)
{
    if (self->field_04) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_08A155D0(self);
}

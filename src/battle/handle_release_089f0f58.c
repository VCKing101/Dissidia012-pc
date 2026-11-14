typedef struct HandleOwnerF0F58 {
    unsigned char active;
    char pad_001[0x3];
    int field_04;
    void *field_08;
} HandleOwnerF0F58;

extern void func_088DA060(void *handle);
extern void func_089F0F40(HandleOwnerF0F58 *self);

void func_089F0F58(HandleOwnerF0F58 *self)
{
    if (self->field_08) {
        func_088DA060(self->field_08);
        self->field_08 = 0;
    }
    func_089F0F40(self);
}

int func_089F0F98(HandleOwnerF0F58 *self, int value)
{
    func_089F0F58(self);
    if (!value) {
        return -1;
    }
    self->field_04 = value;
    self->active = 1;
    return 0;
}

typedef struct HandleOwner1104C {
    unsigned char active;
    char pad_001[0x3];
    int field_04;
    void *field_08;
} HandleOwner1104C;

extern void func_088DA060(void *handle);
extern void func_08A11038(HandleOwner1104C *self);

void func_08A1104C(HandleOwner1104C *self)
{
    if (self->field_08) {
        func_088DA060(self->field_08);
        self->field_08 = 0;
    }
    func_08A11038(self);
}

int func_08A1108C(HandleOwner1104C *self, int value)
{
    func_08A1104C(self);
    if (!value) {
        return -1;
    }
    self->field_04 = value;
    self->active = 1;
    return 0;
}

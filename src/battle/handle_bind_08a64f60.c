typedef struct HandleBind64F60 {
    unsigned char active;
    char pad_001[0x3];
    int field_04;
} HandleBind64F60;

extern void func_08A64F44(HandleBind64F60 *self);

int func_08A64F60(HandleBind64F60 *self, int value)
{
    func_08A64F44(self);
    if (!value) {
        return -1;
    }
    self->field_04 = value;
    self->active = 1;
    return 0;
}

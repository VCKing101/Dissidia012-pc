typedef struct HandleBind62340 {
    unsigned char active;
    char pad_001[0x3];
    int field_04;
} HandleBind62340;

extern void func_08A62324(HandleBind62340 *self);

int func_08A62340(HandleBind62340 *self, int value)
{
    func_08A62324(self);
    if (!value) {
        return -1;
    }
    self->field_04 = value;
    self->active = 1;
    return 0;
}

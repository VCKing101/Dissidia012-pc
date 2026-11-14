typedef struct PairSlot7D74 {
    unsigned char field_00;
    char pad_01[0x13];
    void *field_14;
    void *field_18;
} PairSlot7D74;

extern void func_089F7D0C(PairSlot7D74 *self);

int func_089F7D74(PairSlot7D74 *self, void *data, void *extra)
{
    func_089F7D0C(self);
    if (data == 0 || extra == 0) {
        return -1;
    }
    self->field_14 = data;
    self->field_18 = extra;
    self->field_00 = 1;
    return 0;
}

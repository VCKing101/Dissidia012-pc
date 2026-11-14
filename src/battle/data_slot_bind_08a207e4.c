typedef struct DataSlot {
    unsigned char field_00;
    char pad_01[0x17];
    unsigned int field_18;
    void *field_1C;
} DataSlot;

extern void func_08A20764(DataSlot *self);

int func_08A207E4(DataSlot *self, unsigned int kind, void *data)
{
    func_08A20764(self);
    if (kind >= 2 || data == 0) {
        return -1;
    }
    self->field_18 = kind;
    self->field_1C = data;
    self->field_00 = 1;
    return 0;
}

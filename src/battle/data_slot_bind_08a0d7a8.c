typedef struct DataSlot {
    unsigned char field_00;
    char pad_01[0x7];
    unsigned int field_08;
    void *field_0C;
} DataSlot;

extern void func_08A0D768(DataSlot *self);

int func_08A0D7A8(DataSlot *self, unsigned int kind, void *data)
{
    func_08A0D768(self);
    if (kind >= 2 || data == 0) {
        return -1;
    }
    self->field_08 = kind;
    self->field_0C = data;
    self->field_00 = 1;
    return 0;
}

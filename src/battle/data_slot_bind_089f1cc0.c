typedef struct DataSlot {
    unsigned char field_00;
    char pad_01[0x7];
    unsigned int field_08;
    void *field_0C;
} DataSlot;

extern void func_089F1C80(DataSlot *self);

int func_089F1CC0(DataSlot *self, unsigned int kind, void *data)
{
    func_089F1C80(self);
    if (kind >= 2 || data == 0) {
        return -1;
    }
    self->field_08 = kind;
    self->field_0C = data;
    self->field_00 = 1;
    return 0;
}

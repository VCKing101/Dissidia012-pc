typedef struct DataSlot {
    unsigned char field_00;
    char pad_01[0x1B];
    unsigned int field_1C;
    void *field_20;
} DataSlot;

extern void func_089EF350(DataSlot *self);

int func_089EF3A8(DataSlot *self, unsigned int kind, void *data)
{
    func_089EF350(self);
    if (kind >= 2 || data == 0) {
        return -1;
    }
    self->field_1C = kind;
    self->field_20 = data;
    self->field_00 = 1;
    return 0;
}

typedef struct DataSlot {
    unsigned char field_00;
    char pad_01[0xF];
    unsigned int field_10;
    void *field_14;
} DataSlot;

extern void func_089FC71C(DataSlot *self);

int func_089FC774(DataSlot *self, unsigned int kind, void *data)
{
    func_089FC71C(self);
    if (kind >= 2 || data == 0) {
        return -1;
    }
    self->field_10 = kind;
    self->field_14 = data;
    self->field_00 = 1;
    return 0;
}

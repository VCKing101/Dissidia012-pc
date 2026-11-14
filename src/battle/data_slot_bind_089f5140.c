typedef struct TripleSlot {
    unsigned char field_00;
    char pad_01[0x37];
    unsigned int field_38;
    void *field_3C;
    void *field_40;
} TripleSlot;

extern void func_089F5098(TripleSlot *self);

int func_089F5140(TripleSlot *self, unsigned int kind, void *data, void *extra)
{
    func_089F5098(self);
    if (kind >= 3 || data == 0 || extra == 0) {
        return -1;
    }
    self->field_38 = kind;
    self->field_3C = data;
    self->field_40 = extra;
    self->field_00 = 1;
    return 0;
}

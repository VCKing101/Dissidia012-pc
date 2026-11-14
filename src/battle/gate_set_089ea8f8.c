typedef struct GateA8F8 {
    unsigned char field_00;
    char pad_001[0x3];
    int field_04;
} GateA8F8;

int func_089EA8F8(GateA8F8 *self, int value)
{
    if (!self->field_00) {
        return -1;
    }
    self->field_04 = value;
    return 0;
}

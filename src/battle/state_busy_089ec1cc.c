typedef struct StateBusy {
    char pad_000[0x8];
    unsigned char field_08;
    char pad_009[0x3];
    int field_0C;
    char pad_010[0x14];
    unsigned char field_24;
} StateBusy;

unsigned char func_089EC1CC(StateBusy *self)
{
    int result;
    if (!self->field_08) {
        return 0;
    }
    result = 0;
    if (self->field_0C == 1 || self->field_24 == 1) {
        result = 1;
    }
    return result;
}

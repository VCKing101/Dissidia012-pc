typedef struct StateIdle {
    char pad_000[0x8];
    unsigned char field_08;
    char pad_009[0x3];
    int field_0C;
    char pad_010[0x14];
    unsigned char field_24;
} StateIdle;

unsigned char func_089EC188(StateIdle *self)
{
    int result;
    if (!self->field_08) {
        return 0;
    }
    result = 0;
    if (self->field_0C == 0 && self->field_24 == 0) {
        result = 1;
    }
    return result;
}

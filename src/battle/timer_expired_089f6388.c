typedef struct Timer6388 {
    unsigned char field_00;
    char pad_001[0x2F];
    unsigned char field_30;
    char pad_031[3];
    float field_34;
    char pad_038[8];
    float field_40;
} Timer6388;

unsigned char func_089F6388(Timer6388 *self)
{
    if (!self->field_00) {
        return 0;
    }
    if (!self->field_30) {
        return 0;
    }
    return (unsigned char)(self->field_34 < self->field_40 ? 0 : 1);
}

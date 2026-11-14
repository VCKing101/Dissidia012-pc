typedef struct Gate5B5C {
    unsigned char field_00;
    char pad_001[0xFF];
    unsigned char field_100;
} Gate5B5C;

int func_089F5B5C(Gate5B5C *self)
{
    if (!self->field_00) {
        return -1;
    }
    self->field_100 = 1;
    return 0;
}

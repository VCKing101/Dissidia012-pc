typedef struct Gate7E18 {
    unsigned char field_00;
    unsigned char field_01;
} Gate7E18;

int func_089F7E18(Gate7E18 *self)
{
    if (!self->field_00) {
        return -1;
    }
    self->field_01 = 1;
    return 0;
}

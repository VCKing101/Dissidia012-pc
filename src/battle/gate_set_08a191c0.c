typedef struct Gate91C0 {
    unsigned char field_00;
    unsigned char field_01;
} Gate91C0;

int func_08A191C0(Gate91C0 *self)
{
    if (!self->field_00) {
        return -1;
    }
    self->field_01 = 1;
    return 0;
}

typedef struct Fighter {
    char pad_00[0x60C];
    short field_60C;
    char pad_60E[5];
    unsigned char field_613;
} Fighter;

void func_088513BC(Fighter *self, int value)
{
    self->field_613 = 1;
    self->field_60C = value;
}

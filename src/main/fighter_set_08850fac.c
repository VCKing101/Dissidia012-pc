typedef struct Fighter {
    char pad_00[0x5F4];
    short field_5F4;
    short field_5F6;
    char pad_5F8[0x16];
    unsigned char field_60E;
} Fighter;

void func_08850FAC(Fighter *self, int a, int b)
{
    self->field_5F4 = a;
    self->field_5F6 = b;
    self->field_60E = 1;
}

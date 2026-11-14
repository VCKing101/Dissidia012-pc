typedef struct Fighter {
    char pad_00[0x5D4];
    int actor;
    char pad_5D8[0x32];
    short field_60A;
    char pad_60C[4];
    unsigned char field_610;
} Fighter;

void func_08851058(Fighter *self, int actor, int b, unsigned char c)
{
    self->actor = actor;
    self->field_60A = b;
    self->field_610 = c;
}

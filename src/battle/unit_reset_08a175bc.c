typedef struct Unit75BC {
    char field_00;
    char pad_001[0x3];
    int field_04;
    float field_08;
    char field_0C;
    char pad_00D[0x3];
    int field_10;
    int field_14;
    char pad_018[0xF4];
    int field_10C;
    int field_110;
} Unit75BC;

void func_08A175BC(Unit75BC *self)
{
    self->field_00 = 0;
    self->field_04 = -1;
    self->field_08 = 0.0f;
    self->field_0C = 0;
    self->field_10 = 0;
    self->field_14 = 0;
    self->field_10C = -1;
    self->field_110 = -1;
}

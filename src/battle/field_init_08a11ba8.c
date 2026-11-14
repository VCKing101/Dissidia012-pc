typedef struct FieldInit08A11BA8 {
    char pad_000[0x4];
    unsigned char field_04;
    char pad_005[0x3];
    int field_08;
    unsigned char field_0C;
    unsigned char field_0D;
    char pad_00E[0x2];
    float field_10;
    int field_14;
    int field_18;
    char pad_01C[0x298];
    int field_2B4;
    int field_2B8;
    int field_2BC;
    int field_2C0;
} FieldInit08A11BA8;

void func_08A11BA8(FieldInit08A11BA8 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_0D = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_2B4 = -1;
    self->field_2B8 = -1;
    self->field_2BC = 0;
    self->field_2C0 = 0;
}

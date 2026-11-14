typedef struct FieldInit08A26294 {
    unsigned char field_00;
    char pad_001[0x3];
    int field_04;
    float field_08;
    int field_0C;
    unsigned char field_10;
    char pad_011[0x3];
    int field_14;
    int field_18;
    char pad_01C[0xF4];
    int field_110;
    int field_114;
} FieldInit08A26294;

void func_08A26294(FieldInit08A26294 *self)
{
    self->field_00 = 0;
    self->field_04 = -1;
    self->field_08 = 0.0f;
    self->field_0C = 0;
    self->field_10 = 0;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_110 = -1;
    self->field_114 = -1;
}

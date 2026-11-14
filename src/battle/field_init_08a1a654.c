typedef struct FieldInit08A1A654 {
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
    char pad_01C[0x150];
    int field_16C;
    int field_170;
    int field_174;
    int field_178;
} FieldInit08A1A654;

void func_08A1A654(FieldInit08A1A654 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_0D = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_16C = -1;
    self->field_170 = -1;
    self->field_174 = 0;
    self->field_178 = 0;
}

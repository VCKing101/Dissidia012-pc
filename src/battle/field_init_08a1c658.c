typedef struct FieldInit08A1C658 {
    char pad_000[0x4];
    unsigned char field_04;
    char pad_005[0x3];
    int field_08;
    unsigned char field_0C;
    unsigned char field_0D;
    char pad_00E[0x2];
    float field_10;
    char pad_014[0x110];
    int field_124;
    int field_128;
    int field_12C;
    int field_130;
} FieldInit08A1C658;

void func_08A1C658(FieldInit08A1C658 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_0D = 0;
    self->field_10 = 0.0f;
    self->field_124 = -1;
    self->field_128 = -1;
    self->field_12C = 0;
    self->field_130 = 0;
}

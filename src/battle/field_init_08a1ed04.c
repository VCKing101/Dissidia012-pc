typedef struct FieldInit08A1ED04 {
    char pad_000[0x4];
    unsigned char field_04;
    char pad_005[0x3];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x3];
    float field_10;
    int field_14;
    int field_18;
    char pad_01C[0x10C];
    int field_128;
    int field_12C;
    int field_130;
    int field_134;
} FieldInit08A1ED04;

void func_08A1ED04(FieldInit08A1ED04 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_128 = -1;
    self->field_12C = -1;
    self->field_130 = 0;
    self->field_134 = 0;
}

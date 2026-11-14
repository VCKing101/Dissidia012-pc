typedef struct FieldInit08A269A8 {
    char pad_000[0x4];
    unsigned char field_04;
    char pad_005[0x3];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x3];
    float field_10;
    unsigned char field_14;
    char pad_015[0x3];
    int field_18;
    char pad_01C[0x6C];
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
} FieldInit08A269A8;

void func_08A269A8(FieldInit08A269A8 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_88 = -1;
    self->field_8C = -1;
    self->field_90 = 0;
    self->field_94 = 0;
}

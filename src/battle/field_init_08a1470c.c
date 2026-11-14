typedef struct FieldInit08A1470C {
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
    char pad_01C[0x15C];
    int field_178;
    int field_17C;
    int field_180;
    int field_184;
} FieldInit08A1470C;

void func_08A1470C(FieldInit08A1470C *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_0D = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_178 = -1;
    self->field_17C = -1;
    self->field_180 = 0;
    self->field_184 = 0;
}

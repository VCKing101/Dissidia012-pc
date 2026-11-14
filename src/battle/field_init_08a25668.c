typedef struct FieldInit08A25668 {
    unsigned char field_00;
    char pad_001[0x3];
    int field_04;
    float field_08;
    int field_0C;
    int field_10;
    char pad_014[0xAC];
    int field_C0;
} FieldInit08A25668;

void func_08A25668(FieldInit08A25668 *self)
{
    self->field_00 = 0;
    self->field_04 = -1;
    self->field_08 = 0.0f;
    self->field_0C = 0;
    self->field_10 = 0;
    self->field_C0 = -1;
}

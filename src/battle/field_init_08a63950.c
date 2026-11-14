typedef struct FieldInit08A63950 {
    unsigned char field_00;
    char pad_001[0x3];
    int field_04;
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x3];
    float field_10;
} FieldInit08A63950;

void func_08A63950(FieldInit08A63950 *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = 0;
    self->field_10 = 0.0f;
}

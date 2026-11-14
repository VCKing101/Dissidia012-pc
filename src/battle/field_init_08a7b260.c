typedef struct FieldInit08A7B260 {
    unsigned char field_00;
    char pad_001[0x3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    unsigned char field_14;
    char pad_015[0x3];
    int field_18;
    float field_1C;
} FieldInit08A7B260;

void func_08A7B260(FieldInit08A7B260 *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = 0;
    self->field_10 = 0;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_1C = 0.0f;
}

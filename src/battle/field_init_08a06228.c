typedef struct FieldInit08A06228 {
    unsigned char field_00;
    char pad_001[0x3];
    int field_04;
    int field_08;
    float field_0C;
    int field_10;
    int field_14;
    float field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
} FieldInit08A06228;

void func_08A06228(FieldInit08A06228 *self)
{
    self->field_00 = 0;
    self->field_04 = -1;
    self->field_08 = 0;
    self->field_0C = 0.0f;
    self->field_10 = -1;
    self->field_14 = 0;
    self->field_18 = 0.0f;
    self->field_1C = -1;
    self->field_20 = 0;
    self->field_24 = 0;
    self->field_28 = 0;
    self->field_2C = 0;
}

typedef struct UnitF320 {
    char field_00;
    char pad_001[0x3];
    int field_04;
    int field_08;
    float field_0C;
    int field_10;
    float field_14;
    int field_18;
    int field_1C;
    int field_20;
} UnitF320;

void func_089EF320(UnitF320 *self)
{
    self->field_00 = 0;
    self->field_04 = -1;
    self->field_08 = 0;
    self->field_0C = 0.0f;
    self->field_10 = 0;
    self->field_14 = 0.0f;
    self->field_18 = 0;
    self->field_1C = -1;
    self->field_20 = 0;
}

typedef struct UnitB638 {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char field_0D;
    char pad_00E[0x2];
    float field_10;
    int field_14;
    char pad_018[0x154];
    int field_16C;
    int field_170;
    int field_174;
    int field_178;
} UnitB638;

void func_08A1B638(UnitB638 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_0D = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_16C = -1;
    self->field_170 = -1;
    self->field_174 = 0;
    self->field_178 = 0;
}

typedef struct UnitC6F8 {
    char field_00;
    char pad_001[0x3];
    int field_04;
    int field_08;
    float field_0C;
    int field_10;
    int field_14;
} UnitC6F8;

void func_089FC6F8(UnitC6F8 *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = 0.0f;
    self->field_10 = -1;
    self->field_14 = 0;
}

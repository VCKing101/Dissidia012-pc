typedef struct UnitE500 {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char pad_00D[0x3];
    float field_10;
    char pad_014[0x198];
    int field_1AC;
    int field_1B0;
    int field_1B4;
    int field_1B8;
} UnitE500;

void func_089FE500(UnitE500 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_1AC = -1;
    self->field_1B0 = -1;
    self->field_1B4 = 0;
    self->field_1B8 = 0;
}

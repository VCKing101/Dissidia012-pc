typedef struct UnitBCA0 {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char pad_00D[0x3];
    float field_10;
    char pad_014[0x60];
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
} UnitBCA0;

void func_08A0BCA0(UnitBCA0 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_74 = -1;
    self->field_78 = -1;
    self->field_7C = 0;
    self->field_80 = 0;
    self->field_84 = 0;
}

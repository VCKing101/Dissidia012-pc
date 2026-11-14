typedef struct Unit153C {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char pad_00D[0x3];
    float field_10;
    char pad_014[0x14];
    int field_28;
    int field_2C;
    int field_30;
} Unit153C;

void func_089F153C(Unit153C *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_28 = -1;
    self->field_2C = 0;
    self->field_30 = 0;
}

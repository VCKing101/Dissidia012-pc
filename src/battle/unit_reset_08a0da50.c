typedef struct UnitDA50 {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    int field_0C;
    char field_10;
    char field_11;
    char pad_012[0x2];
    float field_14;
    char pad_018[0x90];
    int field_A8;
    int field_AC;
    int field_B0;
    int field_B4;
} UnitDA50;

void func_08A0DA50(UnitDA50 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = -1;
    self->field_10 = 0;
    self->field_11 = 0;
    self->field_14 = 0.0f;
    self->field_A8 = -1;
    self->field_AC = -1;
    self->field_B0 = 0;
    self->field_B4 = 0;
}

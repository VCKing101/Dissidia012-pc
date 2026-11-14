typedef struct Unit00CC {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char pad_00D[0x3];
    float field_10;
    char pad_014[0x8C];
    int field_A0;
    int field_A4;
    int field_A8;
    int field_AC;
    int field_B0;
} Unit00CC;

void func_08A100CC(Unit00CC *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_A0 = -1;
    self->field_A4 = -1;
    self->field_A8 = 0;
    self->field_AC = 0;
    self->field_B0 = 0;
}

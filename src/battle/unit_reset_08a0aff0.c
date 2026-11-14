typedef struct UnitAFF0 {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char pad_00D[0x3];
    float field_10;
    char pad_014[0xB8];
    int field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
} UnitAFF0;

void func_08A0AFF0(UnitAFF0 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_CC = -1;
    self->field_D0 = -1;
    self->field_D4 = 0;
    self->field_D8 = 0;
    self->field_DC = 0;
}

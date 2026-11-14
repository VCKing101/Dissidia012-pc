typedef struct UnitCC20 {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char pad_00D[0x3];
    float field_10;
    char pad_014[0xA8];
    int field_BC;
    int field_C0;
    int field_C4;
    int field_C8;
} UnitCC20;

void func_089FCC20(UnitCC20 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_BC = -1;
    self->field_C0 = -1;
    self->field_C4 = 0;
    self->field_C8 = 0;
}

typedef struct UnitC704 {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char pad_00D[0x3];
    float field_10;
    char pad_014[0x84];
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
} UnitC704;

void func_08A0C704(UnitC704 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_98 = -1;
    self->field_9C = -1;
    self->field_A0 = 0;
    self->field_A4 = 0;
}

typedef struct UnitCFA8 {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char pad_00D[0x3];
    float field_10;
    char pad_014[0x58];
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
} UnitCFA8;

void func_08A0CFA8(UnitCFA8 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_6C = -1;
    self->field_70 = -1;
    self->field_74 = 0;
    self->field_78 = 0;
}

typedef struct Unit7DD4 {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char field_0D;
    char pad_00E[0x2];
    float field_10;
    char pad_014[0x134];
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
} Unit7DD4;

void func_08A17DD4(Unit7DD4 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_0D = 0;
    self->field_10 = 0.0f;
    self->field_148 = -1;
    self->field_14C = -1;
    self->field_150 = 0;
    self->field_154 = 0;
}

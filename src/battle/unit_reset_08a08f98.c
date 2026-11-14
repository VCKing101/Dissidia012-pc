typedef struct Unit8F98 {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char pad_00D[0x3];
    float field_10;
    char pad_014[0xC8];
    int field_DC;
    int field_E0;
    int field_E4;
    int field_E8;
} Unit8F98;

void func_08A08F98(Unit8F98 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_DC = -1;
    self->field_E0 = -1;
    self->field_E4 = 0;
    self->field_E8 = 0;
}

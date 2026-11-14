typedef struct UnitAD30 {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char pad_00D[0x3];
    float field_10;
    int field_14;
    int field_18;
    char pad_01C[0xE0];
    int field_FC;
    int field_100;
    int field_104;
    int field_108;
} UnitAD30;

void func_089FAD30(UnitAD30 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_FC = -1;
    self->field_100 = -1;
    self->field_104 = 0;
    self->field_108 = 0;
}

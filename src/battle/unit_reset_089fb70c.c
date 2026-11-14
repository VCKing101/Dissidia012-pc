typedef struct UnitB70C {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char pad_00D[0x3];
    float field_10;
    int field_14;
    char pad_018[0xE4];
    int field_FC;
    int field_100;
    int field_104;
    int field_108;
} UnitB70C;

void func_089FB70C(UnitB70C *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_FC = -1;
    self->field_100 = -1;
    self->field_104 = 0;
    self->field_108 = 0;
}

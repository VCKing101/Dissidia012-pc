typedef struct Unit0E1C {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char pad_00D[0x3];
    float field_10;
    char pad_014[0xEC];
    int field_100;
    int field_104;
    int field_108;
    int field_10C;
} Unit0E1C;

void func_08A00E1C(Unit0E1C *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_100 = -1;
    self->field_104 = -1;
    self->field_108 = 0;
    self->field_10C = 0;
}

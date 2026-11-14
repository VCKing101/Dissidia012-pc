typedef struct UnitA484 {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char pad_00D[0x3];
    float field_10;
    char pad_014[0x998];
    int field_9AC;
    int field_9B0;
    int field_9B4;
    int field_9B8;
    int field_9BC;
    int field_9C0;
} UnitA484;

void func_08A0A484(UnitA484 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_9AC = -1;
    self->field_9B0 = -1;
    self->field_9B4 = 0;
    self->field_9B8 = 0;
    self->field_9BC = 0;
    self->field_9C0 = 0;
}

typedef struct Unit9408 {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char field_0D;
    char pad_00E[0x2];
    float field_10;
    char pad_014[0x310];
    int field_324;
    int field_328;
    int field_32C;
} Unit9408;

void func_08A19408(Unit9408 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_0D = 0;
    self->field_10 = 0.0f;
    self->field_324 = -1;
    self->field_328 = 0;
    self->field_32C = 0;
}

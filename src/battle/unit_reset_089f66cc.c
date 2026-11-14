typedef struct Unit66CC {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char pad_00D[0x3];
    float field_10;
    char field_14;
    char pad_015[0x3];
    int field_18;
    char pad_01C[0x98];
    int field_B4;
    int field_B8;
    int field_BC;
    int field_C0;
} Unit66CC;

void func_089F66CC(Unit66CC *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_B4 = -1;
    self->field_B8 = -1;
    self->field_BC = 0;
    self->field_C0 = 0;
}

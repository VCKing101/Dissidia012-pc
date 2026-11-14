typedef struct Unit2A8C {
    char pad_000[0x4];
    char field_04;
    char pad_005[0x3];
    int field_08;
    char field_0C;
    char pad_00D[0x3];
    float field_10;
    int field_14;
    int field_18;
    char pad_01C[0x208];
    int field_224;
    int field_228;
    int field_22C;
    int field_230;
} Unit2A8C;

void func_089F2A8C(Unit2A8C *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_224 = -1;
    self->field_228 = -1;
    self->field_22C = 0;
    self->field_230 = 0;
}

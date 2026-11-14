typedef struct FieldInit08A1D614 {
    char pad_000[0x4];
    unsigned char field_04;
    char pad_005[0x3];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x3];
    float field_10;
    int field_14;
    int field_18;
    unsigned char field_1C;
    char pad_01D[0x247];
    int field_264;
    int field_268;
    int field_26C;
    int field_270;
} FieldInit08A1D614;

void func_08A1D614(FieldInit08A1D614 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_1C = 0;
    self->field_264 = -1;
    self->field_268 = -1;
    self->field_26C = 0;
    self->field_270 = 0;
}

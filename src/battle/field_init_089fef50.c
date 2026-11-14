typedef struct FieldInitFEF50 {
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
    char pad_01D[0x25B];
    int field_278;
    int field_27C;
    int field_280;
    int field_284;
} FieldInitFEF50;

void func_089FEF50(FieldInitFEF50 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_1C = 0;
    self->field_278 = -1;
    self->field_27C = -1;
    self->field_280 = 0;
    self->field_284 = 0;
}

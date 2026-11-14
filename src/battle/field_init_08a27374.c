typedef struct FieldInit08A27374 {
    char pad_000[0x4];
    unsigned char field_04;
    char pad_005[0x3];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x3];
    float field_10;
    char pad_014[0x34];
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
} FieldInit08A27374;

void func_08A27374(FieldInit08A27374 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_48 = -1;
    self->field_4C = -1;
    self->field_50 = 0;
    self->field_54 = 0;
}

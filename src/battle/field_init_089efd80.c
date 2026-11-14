typedef struct FieldInitEFD80 {
    char pad_000[0x4];
    unsigned char field_04;
    char pad_005[0x3];
    int field_08;
    unsigned char field_0C;
    unsigned char field_0D;
    char pad_00E[0x2];
    float field_10;
    int field_14;
    int field_18;
    char pad_01C[0x12C];
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
} FieldInitEFD80;

void func_089EFD80(FieldInitEFD80 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_0D = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_148 = -1;
    self->field_14C = -1;
    self->field_150 = 0;
    self->field_154 = 0;
}

typedef struct FieldInit08A2A64C {
    char pad_000[0x4];
    unsigned char field_04;
    char pad_005[0x3];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x3];
    float field_10;
    int field_14;
    int field_18;
    char pad_01C[0xAC];
    int field_C8;
    int field_CC;
    int field_D0;
} FieldInit08A2A64C;

void func_08A2A64C(FieldInit08A2A64C *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_C8 = -1;
    self->field_CC = 0;
    self->field_D0 = 0;
}

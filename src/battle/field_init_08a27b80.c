typedef struct FieldInit08A27B80 {
    char pad_000[0x4];
    unsigned char field_04;
    char pad_005[0x3];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x3];
    float field_10;
    int field_14;
    int field_18;
    char pad_01C[0xA8];
    int field_C4;
    int field_C8;
    int field_CC;
    int field_D0;
} FieldInit08A27B80;

void func_08A27B80(FieldInit08A27B80 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_C4 = -1;
    self->field_C8 = -1;
    self->field_CC = 0;
    self->field_D0 = 0;
}

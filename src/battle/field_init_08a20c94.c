typedef struct FieldInit08A20C94 {
    char pad_000[0x4];
    unsigned char field_04;
    char pad_005[0x3];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x3];
    float field_10;
    char pad_014[0xD8];
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
} FieldInit08A20C94;

void func_08A20C94(FieldInit08A20C94 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_EC = -1;
    self->field_F0 = -1;
    self->field_F4 = 0;
    self->field_F8 = 0;
}

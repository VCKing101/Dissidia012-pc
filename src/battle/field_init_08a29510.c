typedef struct FieldInit08A29510 {
    char pad_000[0x4];
    unsigned char field_04;
    char pad_005[0x3];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x3];
    float field_10;
    int field_14;
    int field_18;
    char pad_01C[0xD8];
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
} FieldInit08A29510;

void func_08A29510(FieldInit08A29510 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_F4 = -1;
    self->field_F8 = -1;
    self->field_FC = 0;
    self->field_100 = 0;
}

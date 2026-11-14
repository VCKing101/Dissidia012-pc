typedef struct FieldInit089FFD78 {
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
    char pad_01D[0xDB];
    int field_F8;
    int field_FC;
    int field_100;
    int field_104;
} FieldInit089FFD78;

void func_089FFD78(FieldInit089FFD78 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0.0f;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_1C = 0;
    self->field_F8 = -1;
    self->field_FC = -1;
    self->field_100 = 0;
    self->field_104 = 0;
}

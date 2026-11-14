typedef struct FieldInit08A00738 {
    unsigned char field_00;
    char pad_001[0x3];
    int field_04;
    int field_08[4];
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
} FieldInit08A00738;

void func_08A00738(FieldInit08A00738 *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    {
        int i;
        for (i = 0; i < 4; i++) {
            self->field_08[i] = 0;
        }
    }
    self->field_18 = -1;
    self->field_1C = -1;
    self->field_20 = 0;
    self->field_24 = 0;
    self->field_28 = -1;
    self->field_2C = 0;
    self->field_30 = -1;
    self->field_34 = 0;
}

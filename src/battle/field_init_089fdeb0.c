typedef struct FieldInitFDEB0 {
    unsigned char field_00;
    char pad_001[0x3];
    int field_04;
    int field_08[3];
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
} FieldInitFDEB0;

void func_089FDEB0(FieldInitFDEB0 *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    {
        int i;
        for (i = 0; i < 3; i++) {
            self->field_08[i] = 0;
        }
    }
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_1C = -1;
    self->field_20 = -1;
    self->field_24 = -1;
    self->field_28 = 0;
    self->field_2C = 0;
}

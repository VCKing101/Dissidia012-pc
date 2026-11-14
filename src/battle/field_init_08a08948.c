typedef struct FieldInit08A08948 {
    unsigned char field_00;
    char pad_001[0x3];
    int field_04;
    int field_08[2];
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
} FieldInit08A08948;

void func_08A08948(FieldInit08A08948 *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    {
        int i;
        for (i = 0; i < 2; i++) {
            self->field_08[i] = 0;
        }
    }
    self->field_10 = 0;
    self->field_14 = 0;
    self->field_18 = -1;
    self->field_1C = -1;
    self->field_20 = -1;
    self->field_24 = 0;
    self->field_28 = 0;
}

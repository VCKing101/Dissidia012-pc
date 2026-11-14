typedef struct FieldInit08A20730 {
    unsigned char field_00;
    char pad_001[0x3];
    int field_04;
    int field_08[4];
    int field_18;
    int field_1C;
} FieldInit08A20730;

void func_08A20730(FieldInit08A20730 *self)
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
    self->field_1C = 0;
}

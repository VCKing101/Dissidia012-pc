typedef struct FieldInit4FD00 {
    int field_00;
    int field_04;
    int field_08;
    char pad_00C[0x24];
    float field_30;
    int field_34;
    int field_38[8];
} FieldInit4FD00;

void func_08A4FD00(FieldInit4FD00 *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_30 = 0.0f;
    self->field_34 = 0;
    {
        int i;
        for (i = 0; i < 8; i++) {
            self->field_38[i] = 0;
        }
    }
}

typedef struct FieldInit61F84 {
    unsigned char field_00;
    char pad_001[0x3];
    float field_04[10];
} FieldInit61F84;

void func_08A61F84(FieldInit61F84 *self)
{
    self->field_00 = 0;
    {
        int i;
        for (i = 0; i < 10; i++) {
            self->field_04[i] = 0.0f;
        }
    }
}

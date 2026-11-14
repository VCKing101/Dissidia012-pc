typedef struct FieldInit23298 {
    unsigned char field_00;
    char pad_001[0x3];
    float field_04;
    unsigned char field_08[2];
    unsigned char field_0A[2];
    unsigned char field_0C;
    char pad_00D[0xE3];
    int field_F0;
    int field_F4;
} FieldInit23298;

void func_08A23298(FieldInit23298 *self)
{
    self->field_00 = 0;
    self->field_04 = 0.0f;
    {
        int i;
        for (i = 0; i < 2; i++) {
            *((unsigned char *)self + 0x08 + i) = 0;
            *((unsigned char *)self + 0x0A + i) = 0;
        }
    }
    self->field_0C = 0;
    self->field_F0 = 0;
    self->field_F4 = 0;
}

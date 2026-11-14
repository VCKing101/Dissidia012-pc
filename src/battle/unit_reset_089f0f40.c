typedef struct Unit0F40 {
    char field_00;
    char pad_001[0x3];
    int field_04;
    int field_08;
    float field_0C;
} Unit0F40;

void func_089F0F40(Unit0F40 *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = 0.0f;
}

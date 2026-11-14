typedef struct TableF {
    unsigned char field_00;
    char pad_001[0x53];
    unsigned int field_54;
    int *field_58;
} TableF;

float func_089EF05C(TableF *self, unsigned int index)
{
    if (!self->field_00) {
        return 0.0f;
    }
    if (index >= self->field_54) {
        return 0.0f;
    }
    return self->field_58[index];
}

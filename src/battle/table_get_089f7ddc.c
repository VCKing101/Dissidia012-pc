typedef struct Table7DDC {
    unsigned char field_00;
    char pad_001[3];
    int field_04[4];
} Table7DDC;

int func_089F7DDC(Table7DDC *self, unsigned int index)
{
    if (!self->field_00) {
        return 0;
    }
    if (index >= 4) {
        return 0;
    }
    return self->field_04[index];
}

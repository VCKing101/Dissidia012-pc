typedef struct Item8 { int field_00; int field_04; } Item8;
typedef struct Table8 {
    unsigned char field_00;
    char pad_001[0x3B];
    unsigned int field_3C;
    Item8 *field_40;
} Table8;

Item8 *func_089EEFD4(Table8 *self, unsigned int index)
{
    if (!self->field_00) {
        return 0;
    }
    if (index >= self->field_3C) {
        return 0;
    }
    return &self->field_40[index];
}

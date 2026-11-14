typedef struct ItemC { int field_00; int field_04; int field_08; } ItemC;
typedef struct TableC {
    unsigned char field_00;
    char pad_001[0x47];
    unsigned int field_48;
    ItemC *field_4C;
} TableC;

ItemC *func_089EF014(TableC *self, unsigned int index)
{
    if (!self->field_00) {
        return 0;
    }
    if (index >= self->field_48) {
        return 0;
    }
    return &self->field_4C[index];
}

typedef struct IdTable {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    int ids[16];
    int field_50;
    int field_54;
    int field_58;
    int field_5C;
    unsigned char field_60;
    unsigned char field_61;
} IdTable;

IdTable *func_08844B08(IdTable *self)
{
    int i;

    self->field_00 = 0;
    self->field_04 = 0;
    self->field_0C = 0;
    self->field_08 = 0;
    for (i = 0; i < 16; i++) {
        self->ids[i] = -1;
    }
    self->field_50 = 0;
    self->field_54 = 0;
    self->field_58 = 0;
    self->field_5C = 0;
    self->field_60 = 0;
    self->field_61 = 0;
    return self;
}

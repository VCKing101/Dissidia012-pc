/* Clears a 0x50-byte block and marks it valid. */
typedef struct ClearBlock {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    int field_4C;
    unsigned char field_50;
} ClearBlock;

ClearBlock *func_088410E4(ClearBlock *self)
{
    unsigned char one = 1;

    self->field_00 = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_10 = 0;
    self->field_0C = 0;
    self->field_18 = 0;
    self->field_14 = 0;
    self->field_20 = 0;
    self->field_1C = 0;
    self->field_28 = 0;
    self->field_24 = 0;
    self->field_30 = 0;
    self->field_2C = 0;
    self->field_38 = 0;
    self->field_34 = 0;
    self->field_40 = 0;
    self->field_3C = 0;
    self->field_48 = 0;
    self->field_44 = 0;
    self->field_4C = 0;
    self->field_50 = one;
    return self;
}

typedef struct StateBlock {
    unsigned char field_00;
    unsigned char field_01;
    unsigned char field_02;
    char pad_03[0x49];
    unsigned char field_4C;
    unsigned char field_4D;
    unsigned char field_4E;
    unsigned char field_4F;
    int field_50;
    float field_54;
    int field_58;
    int field_5C;
    unsigned char field_60;
    char pad_61[0x3];
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
} StateBlock;

void func_08A65F24(StateBlock *self)
{
    self->field_00 = 0;
    self->field_01 = 0;
    self->field_02 = 0;
    self->field_4C = 0;
    self->field_4D = 0;
    self->field_4E = 0;
    self->field_4F = 0;
    self->field_50 = -1;
    self->field_54 = 0.0f;
    self->field_58 = -1;
    self->field_5C = 0;
    self->field_60 = 0;
    self->field_64 = 0;
    self->field_68 = 0;
    self->field_6C = 0;
    self->field_70 = 0;
}

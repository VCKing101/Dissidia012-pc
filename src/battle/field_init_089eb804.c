typedef struct LinkState {
    int field_00;
    int field_04;
    unsigned char field_08;
    unsigned char field_09;
    char pad_0A[0x2];
    int field_0C;
    int field_10[3];
    unsigned char field_1C;
    char pad_1D[0x3];
    int field_20;
    unsigned char field_24;
    unsigned char field_25;
    char pad_26[0x2];
    int field_28;
} LinkState;

void func_089EB804(LinkState *self)
{
    int none = -1;
    int i;
    self->field_00 = none;
    self->field_04 = none;
    self->field_08 = 0;
    self->field_09 = 0;
    self->field_0C = none;
    for (i = 0; i < 3; i++) {
        self->field_10[i] = 0;
    }
    self->field_1C = 0;
    self->field_20 = none;
    self->field_24 = 0;
    self->field_25 = 0;
    self->field_28 = 0;
}

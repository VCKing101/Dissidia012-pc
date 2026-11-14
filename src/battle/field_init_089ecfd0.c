typedef struct CueState {
    unsigned char field_00;
    char pad_01[0x3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    float field_14;
    unsigned char field_18[2];
    unsigned char field_1A;
    unsigned char field_1B;
    int field_1C;
} CueState;

void func_089ECFD0(CueState *self)
{
    int i;
    self->field_00 = 0;
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = -1;
    self->field_14 = 0.0f;
    for (i = 0; i < 2; i++) {
        *((unsigned char *)self + 0x18 + i) = 0; /* self->field_18[i] */
    }
    self->field_1A = 0;
    self->field_1B = 0;
    self->field_1C = 0;
}

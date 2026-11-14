struct Reset8DD8 {
    unsigned char field_00;
    unsigned char field_01;
    char pad_002[2];
    int field_04[2];
    int field_0C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
};
extern "C" {
void func_08A18DD8(Reset8DD8 *self)
{
    self->field_00 = 0;
    self->field_01 = 0;
    for (int i = 0; i < 2; i++) {
        self->field_04[i] = 0;
    }
    self->field_0C = -1;
    self->field_10 = 0;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_1C = 0;
}
}

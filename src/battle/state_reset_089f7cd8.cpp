struct Reset7CD8 {
    unsigned char field_00;
    unsigned char field_01;
    unsigned char field_02;
    char pad_003;
    int field_04[4];
    int field_14;
    int field_18;
};
extern "C" {
void func_089F7CD8(Reset7CD8 *self)
{
    self->field_00 = 0;
    self->field_01 = 0;
    self->field_02 = 0;
    for (int i = 0; i < 4; i++) {
        self->field_04[i] = 0;
    }
    self->field_14 = 0;
    self->field_18 = 0;
}
}

struct Reset20C4 {
    unsigned char field_00;
    char pad_001[3];
    int field_04;
    char pad_008[0x40];
    unsigned char field_48[4];
    char pad_04C[0x104];
    int field_150;
    int field_154;
    int field_158;
    int field_15C;
    int field_160;
    unsigned char field_164;
};
extern "C" {
void func_089F20C4(Reset20C4 *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    for (int i = 0; i < 4; i++) {
        *((unsigned char *)self + 0x48 + i) = 0;
    }
    self->field_150 = -1;
    self->field_154 = -1;
    self->field_158 = 0;
    self->field_15C = 0;
    self->field_160 = 0;
    self->field_164 = 0;
}
}

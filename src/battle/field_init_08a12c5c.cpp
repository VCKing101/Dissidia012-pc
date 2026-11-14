struct GridState12C5C {
    unsigned char field_00;
    char pad_01[0x3];
    int field_04;
    char pad_08[0x20];
    int field_28[4][2];
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
    int field_58;
    int field_5C;
    int field_60;
};

extern "C" void func_08A12C5C(GridState12C5C *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            self->field_28[i][j] = 0;
        }
    }
    self->field_48 = -1;
    self->field_4C = 0;
    self->field_50 = 0;
    self->field_54 = -1;
    self->field_58 = 0;
    self->field_5C = 0;
    self->field_60 = 0;
}

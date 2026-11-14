struct GridState {
    unsigned char field_00;
    char pad_01[0x3];
    int field_04;
    int field_08[4][3];
    int field_38;
    int field_3C;
    int field_40;
};

extern "C" void func_089F5048(GridState *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            self->field_08[i][j] = 0;
        }
    }
    self->field_38 = -1;
    self->field_3C = 0;
    self->field_40 = 0;
}

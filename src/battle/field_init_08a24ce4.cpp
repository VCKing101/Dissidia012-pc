struct GridState24CE4 {
    unsigned char field_00;
    char pad_01[0x3];
    int field_04;
    char pad_08[0x20];
    int field_28[4];
    int field_38[4][4];
    int field_78;
    int field_7C;
    int field_80;
};

extern "C" void func_08A24CE4(GridState24CE4 *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    for (int i = 0; i < 4; i++) {
        self->field_28[i] = 0;
        for (int j = 0; j < 4; j++) {
            self->field_38[i][j] = 0;
        }
    }
    self->field_78 = -1;
    self->field_7C = 0;
    self->field_80 = 0;
}

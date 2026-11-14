struct PanelState133C8 {
    char pad_000[0x4];
    unsigned char field_04;
    char pad_005[0x3];
    int field_08;
    unsigned char field_0C;
    unsigned char field_0D;
    char pad_00E[0x2];
    float field_10;
    int field_14[2];
    int field_1C[2];
    char pad_024[0x1D0];
    int field_1F4;
    int field_1F8;
    int field_1FC;
    int field_200;
};

extern "C" void func_08A133C8(PanelState133C8 *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_0D = 0;
    self->field_10 = 0.0f;
    for (int i = 0; i < 2; i++) {
        self->field_14[i] = 0;
        self->field_1C[i] = 0;
    }
    self->field_1F4 = -1;
    self->field_1F8 = -1;
    self->field_1FC = 0;
    self->field_200 = 0;
}

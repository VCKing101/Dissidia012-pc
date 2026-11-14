typedef struct PanelState {
    char pad_000[0x4];
    unsigned char field_04;
    char pad_005[0x3];
    int field_08;
    unsigned char field_0C;
    unsigned char field_0D;
    char pad_00E[0x2];
    float field_10;
    unsigned char field_14[2];
    char pad_016[0x2];
    int field_18[2];
    char pad_020[0x3A8];
    int field_3C8;
    int field_3CC;
    int field_3D0;
    int field_3D4;
} PanelState;

extern "C" void func_089F3FF8(PanelState *self)
{
    self->field_04 = 0;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_0D = 0;
    self->field_10 = 0.0f;
    for (int i = 0; i < 2; i++) {
        *((unsigned char *)self + 0x14 + i) = 0; /* self->field_14[i] */
        self->field_18[i] = 0;
    }
    self->field_3C8 = -1;
    self->field_3CC = -1;
    self->field_3D0 = 0;
    self->field_3D4 = 0;
}

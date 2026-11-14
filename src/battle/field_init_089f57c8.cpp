struct TrackSet {
    unsigned char field_00;
    char pad_001[0xDB];
    int field_DC[3];
    int field_E8[3];
    int field_F4;
    float field_F8;
    float field_FC;
    unsigned char field_100;
};

extern "C" void func_089F57C8(TrackSet *self)
{
    self->field_00 = 0;
    for (int i = 0; i < 3; i++) {
        self->field_DC[i] = -1;
        self->field_E8[i] = 0;
    }
    self->field_F4 = -1;
    self->field_F8 = 0.0f;
    self->field_FC = 0.0f;
    self->field_100 = 0;
}

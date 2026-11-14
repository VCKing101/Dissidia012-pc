typedef struct TrackState {
    char pad_00[0x8];
    unsigned char field_08;
    char pad_09[0x3];
    int field_0C;
    float field_10;
    int field_14[4];
    int field_24[4];
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    float field_4C;
} TrackState;

void func_089E82C8(TrackState *self)
{
    int none = -1;
    float zero = 0.0f;
    int i;
    self->field_08 = 0;
    self->field_0C = none;
    self->field_10 = zero;
    for (i = 0; i < 4; i++) {
        self->field_14[i] = 0;
        self->field_24[i] = 0;
    }
    self->field_34 = 0;
    self->field_38 = 0;
    self->field_3C = none;
    self->field_40 = 0;
    self->field_44 = 0;
    self->field_48 = 0;
    self->field_4C = zero;
}

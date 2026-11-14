typedef struct TimerReset {
    unsigned char active;
    char pad_001[0xF];
    int field_10;
    char pad_014[0x4];
    float field_18;
} TimerReset;

int func_08A064CC(TimerReset *self)
{
    if (!self->active) {
        return -1;
    }
    self->field_10 = 0;
    self->field_18 = 0.0f;
    return 0;
}

int func_08A064F8(TimerReset *self)
{
    int result = -1;
    if (!self->active) {
        return result;
    }
    self->field_10 = result;
    self->field_18 = 0.0f;
    return 0;
}

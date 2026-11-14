typedef struct Agent4Speed {
    unsigned char unknown_0000[0x3B4];
    float inverse;
    float field_03B8;
} Agent4Speed;

void func_08834D30(Agent4Speed *self, float rate, float value) {
    self->inverse = rate == 0.0f ? -1.0f : -1.0f / rate;
    self->field_03B8 = value;
}

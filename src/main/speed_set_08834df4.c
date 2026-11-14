typedef struct Agent4Speed {
    unsigned char unknown_0000[0x370];
    float current;
    unsigned char unknown_0374[0x3C];
    float base;
    float inverse;
} Agent4Speed;

void func_08834DF4(Agent4Speed *self, float value) {
    self->base = value;
    self->inverse = 0.0f;
    self->current = value;
}

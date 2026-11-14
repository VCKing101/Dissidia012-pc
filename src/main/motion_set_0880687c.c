typedef struct Agent4Motion {
    unsigned char unknown_0000[0xB00];
    int mode;
    float field_0B04;
    float field_0B08;
    float field_0B0C;
    float field_0B10;
    float field_0B14;
    float field_0B18;
} Agent4Motion;

void func_0880687C(Agent4Motion *self, float a, float b, float c, float d, float e) {
    float zero = 0.0f;
    self->mode = 0;
    self->field_0B0C = zero;
    self->field_0B10 = a;
    self->field_0B14 = b;
    self->field_0B04 = c;
    self->field_0B08 = d;
    self->field_0B18 = e;
}

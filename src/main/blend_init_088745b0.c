typedef struct Blend { float v[6]; int enabled; unsigned char f1C; unsigned char f1D; unsigned char f1E; } Blend;
Blend *func_088745B0(Blend *self, int enabled)
{
    float zero = 0.0f;

    self->f1C = 0;
    self->v[0] = zero;
    self->v[1] = zero;
    self->v[2] = zero;
    self->v[3] = zero;
    self->v[4] = zero;
    self->v[5] = zero;
    self->f1D = 0;
    self->f1E = 0;
    self->enabled = enabled != 0;
    return self;
}

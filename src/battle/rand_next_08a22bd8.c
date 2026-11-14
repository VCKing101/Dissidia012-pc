typedef struct RandState {
    unsigned char active;
    char pad_001[0x3];
    int seed;
} RandState;

int func_08A22BD8(RandState *self)
{
    if (!self->active) {
        return 0;
    }
    self->seed = self->seed * 0x343FD + 0x269EC3;
    return (self->seed >> 16) & 0x7FFF;
}

float func_08A22C1C(RandState *self)
{
    if (!self->active) {
        return 0.0f;
    }
    return (float)func_08A22BD8(self) * (1.0f / 32768.0f);
}

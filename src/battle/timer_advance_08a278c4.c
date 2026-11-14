typedef struct TimerAdvance278C4 {
    char pad_000[0x8];
    int state;
    char pad_00C[0x4];
    float elapsed;
} TimerAdvance278C4;

extern float D_08A27A80;
extern float D_08A27A84;

int func_08A278C4(TimerAdvance278C4 *self, float dt)
{
    self->elapsed += dt;
    if (!(self->elapsed < D_08A27A80)) {
        self->state = 1;
    }
    return 0;
}

int func_08A278F8(TimerAdvance278C4 *self, float dt)
{
    self->elapsed += dt;
    if (!(self->elapsed < D_08A27A84)) {
        self->state = 2;
        self->elapsed = 0.0f;
    }
    return 0;
}

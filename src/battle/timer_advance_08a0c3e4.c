typedef struct TimerAdvance {
    char pad_000[0x8];
    int state;
    char pad_00C[0x4];
    float elapsed;
} TimerAdvance;

extern float D_08A0C5F8;
extern float D_08A0C5F0;

int func_08A0C3E4(TimerAdvance *self, float dt)
{
    self->elapsed += dt;
    if (!(self->elapsed < D_08A0C5F8 - D_08A0C5F0)) {
        self->state = 2;
        self->elapsed = 0.0f;
    }
    return 0;
}

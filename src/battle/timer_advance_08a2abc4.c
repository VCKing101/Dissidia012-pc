typedef struct TimerAdvance2ABC4 {
    char pad_000[0x8];
    int state;
    char pad_00C[0x4];
    float elapsed;
} TimerAdvance2ABC4;

extern float D_08A2ACF4;

int func_08A2ABC4(TimerAdvance2ABC4 *self, float dt)
{
    self->elapsed += dt;
    if (!(self->elapsed < D_08A2ACF4)) {
        self->state = 2;
        self->elapsed = 0.0f;
    }
    return 0;
}

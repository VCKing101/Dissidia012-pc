typedef struct TimerAdvance2703C {
    char pad_000[0x8];
    int state;
    char pad_00C[0x4];
    float elapsed;
} TimerAdvance2703C;

extern float D_08A272B0;

int func_08A2703C(TimerAdvance2703C *self, float dt)
{
    self->elapsed += dt;
    if (!(self->elapsed < D_08A272B0)) {
        self->state = 1;
        self->elapsed = 0.0f;
    }
    return 0;
}

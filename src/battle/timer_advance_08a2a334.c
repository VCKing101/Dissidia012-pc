typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char field_14[0x4];
} TimerTask;

extern float D_08A2A568;
extern void func_08A01AE0(void *sub, int arg1, int arg2);

int func_08A2A334(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A2A568) {
        func_08A01AE0(self->field_14, 0, -1);
        self->state = 1;
        self->elapsed = 0.0f;
    }
    return 0;
}

typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char field_14[0x4];
} TimerTask;

extern float D_08A09864;
extern void func_08A01AE0(void *sub, int arg1, int arg2);

int func_08A095E4(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A09864) {
        func_08A01AE0(self->field_14, 1, -1);
        self->state = 2;
        self->elapsed = 0.0f;
    }
    return 0;
}

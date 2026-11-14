typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x10];
    char field_24[0x4];
} TimerTask;

extern float D_08A14508;
extern void func_08A01AE0(void *sub, int arg1, int arg2);

int func_08A13EB4(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A14508) {
        func_08A01AE0(self->field_24, 2, -1);
        self->state = 1;
        self->elapsed = 0.0f;
    }
    return 0;
}

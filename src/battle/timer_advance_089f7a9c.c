typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x8];
    char field_1C[0x4];
} TimerTask;

extern float D_089F7C30;
extern void func_08A10DB0(void *timer);

int func_089F7A9C(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_089F7C30) {
        func_08A10DB0(self->field_1C);
        self->state = 4;
        self->elapsed = 0.0f;
    }
    return 0;
}

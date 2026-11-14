typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x78];
    char field_8C[0x4];
} TimerTask;

extern float D_08A1D4AC;
extern void func_089F854C(void *sub);

int func_08A1D224(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A1D4AC) {
        func_089F854C(self->field_8C);
        self->state = 5;
        self->elapsed = 0.0f;
    }
    return 0;
}

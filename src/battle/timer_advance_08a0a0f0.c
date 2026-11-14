typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x94];
    char field_A8[0x4];
} TimerTask;

extern float D_08A0A2C0;
extern void func_089F854C(void *sub);

int func_08A0A0F0(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A0A2C0) {
        func_089F854C(self->field_A8);
        self->state = 2;
        self->elapsed = 0.0f;
    }
    return 0;
}

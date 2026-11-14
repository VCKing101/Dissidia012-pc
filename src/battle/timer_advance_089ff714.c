typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0xC];
    char field_20[0x4];
} TimerTask;

extern float D_089FFB90;
extern void func_08A10DB0(void *sub);

int func_089FF714(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_089FFB90) {
        func_08A10DB0(self->field_20);
        self->state = 1;
    }
    return 0;
}

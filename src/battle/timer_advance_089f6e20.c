typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x8];
    char field_1C[0x4];
} TimerTask;

extern float D_089F70F0;
extern void func_08A10DB0(void *timer);

int func_089F6E20(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_089F70F0) {
        func_08A10DB0(self->field_1C);
        self->state = 1;
    }
    return 0;
}

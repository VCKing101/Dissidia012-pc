typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char field_14[0x4];
} TimerTask;

extern float D_08A0AEE0;
extern void func_08A10DB0(void *timer);

int func_08A0AB88(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A0AEE0) {
        func_08A10DB0(self->field_14);
        self->state = 1;
        self->elapsed = 0.0f;
    }
    return 0;
}

typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x3C];
    char field_50[0x4];
} TimerTask;

extern float D_08A272BC;
extern void func_089F854C(void *sub);

int func_08A27134(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A272BC) {
        func_089F854C(self->field_50);
        self->state = -1;
    }
    return 0;
}

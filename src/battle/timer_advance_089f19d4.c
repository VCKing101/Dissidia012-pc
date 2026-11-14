typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char field_14[0x4];
} TimerTask;

extern float D_089F1BD0;
extern void func_089F1DE0(void *sub);

int func_089F19D4(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_089F1BD0) {
        func_089F1DE0(self->field_14);
        self->state = 1;
        self->elapsed = 0.0f;
    }
    return 0;
}

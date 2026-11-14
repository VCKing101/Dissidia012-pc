typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x8];
    char field_1C[0x4];
} TimerTask;

extern float D_089F0D18;
extern void func_089EF414(void *sub);

int func_089F0600(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_089F0D18) {
        func_089EF414(self->field_1C);
        self->state = 1;
        self->elapsed = 0.0f;
    }
    return 0;
}

typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x1DC];
    char field_1F0[0x4];
} TimerTask;

extern float D_089F3408;
extern void func_08A08350(void *sub);

int func_089F32E0(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_089F3408) {
        func_08A08350(self->field_1F0);
        self->state = 5;
    }
    return 0;
}

typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x34];
    char field_48[0x4];
} TimerTask;

extern float D_089F7C34;
extern float D_089F7C30;
extern void func_08A08350(void *sub);

int func_089F7AF4(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_089F7C34 - D_089F7C30) {
        func_08A08350(self->field_48);
        self->state = 5;
        self->elapsed = 0.0f;
    }
    return 0;
}

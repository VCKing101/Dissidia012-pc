typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x2C];
    char field_40[0x4];
} TimerTask;

extern float D_08A0C5FC;
extern float D_08A0C5F0;
extern void func_08A08350(void *sub);

int func_08A0C4B4(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A0C5FC - D_08A0C5F0) {
        func_08A08350(self->field_40);
        self->state = 6;
        self->elapsed = 0.0f;
    }
    return 0;
}

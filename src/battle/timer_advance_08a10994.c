typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x58];
    char field_6C[0x4];
} TimerTask;

extern float D_08A10AC4;
extern void func_08A08350(void *sub);

int func_08A10994(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A10AC4) {
        func_08A08350(self->field_6C);
        self->state = 5;
        self->elapsed = 0.0f;
    }
    return 0;
}

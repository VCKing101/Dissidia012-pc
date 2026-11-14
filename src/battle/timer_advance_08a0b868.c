typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x2C];
    char field_40[0x4];
} TimerTask;

extern float D_08A0BBA8;
extern float D_08A0BBA0;
extern void func_08A01AE0(void *sub, int arg1, int arg2);

int func_08A0B868(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A0BBA8 - D_08A0BBA0) {
        func_08A01AE0(self->field_40, 4, -1);
        self->state = 2;
        self->elapsed = 0.0f;
    }
    return 0;
}

typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0xB8];
    char field_CC[0x4];
} TimerTask;

extern float D_08A1E10C;
extern void func_08A01AE0(void *sub, int arg1, int arg2);

int func_08A1DE0C(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A1E10C) {
        func_08A01AE0(self->field_CC, 6, -1);
        self->state = 2;
    }
    return 0;
}

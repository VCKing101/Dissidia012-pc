typedef struct TimerTaskE008 {
    char pad_00[0x8];
    int state;
    char pad_0C[0x8];
    float elapsed;
    char field_18[0x4];
} TimerTaskE008;

extern float D_08A0E390;
extern void func_08A01AE0(void *sub, int arg1, int arg2);

int func_08A0E008(TimerTaskE008 *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A0E390) {
        func_08A01AE0(self->field_18, 3, -1);
        self->state = 1;
        self->elapsed = 0.0f;
    }
    return 0;
}

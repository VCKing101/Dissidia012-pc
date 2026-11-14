typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0xDC];
    char field_F0[0x4];
} TimerTask;

extern float D_08A1D4B0;
extern void func_08A08350(void *sub);

int func_08A1D310(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A1D4B0) {
        func_08A08350(self->field_F0);
        self->state = 8;
    }
    return 0;
}

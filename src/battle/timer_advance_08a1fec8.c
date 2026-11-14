typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x8];
    char field_1C[0x4];
} TimerTask;

extern float D_08A200F0;
extern void func_08A10DB0(void *sub);

int func_08A1FEC8(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A200F0) {
        func_08A10DB0(self->field_1C);
        self->state = 1;
    }
    return 0;
}

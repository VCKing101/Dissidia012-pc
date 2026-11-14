typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x1AC];
    char field_1C0[0x4];
} TimerTask;

extern float D_08A14510;
extern void func_08A08350(void *sub);

int func_08A14380(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A14510) {
        func_08A08350(self->field_1C0);
        self->state = 11;
    }
    return 0;
}

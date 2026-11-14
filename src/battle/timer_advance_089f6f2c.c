typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x68];
    char field_7C[0x4];
} TimerTask;

extern float D_089F70FC;
extern void func_089F854C(void *sub);

int func_089F6F2C(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_089F70FC) {
        func_089F854C(self->field_7C);
        self->state = -1;
    }
    return 0;
}

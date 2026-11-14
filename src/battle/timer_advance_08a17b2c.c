typedef struct TimerTask17B2C {
    char pad_00[0x4];
    int state;
    float elapsed;
    char pad_0C[0xC];
    char field_18[0x4];
} TimerTask17B2C;

extern float D_08A17CA4;
extern void func_089F854C(void *sub);

int func_08A17B2C(TimerTask17B2C *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A17CA4) {
        func_089F854C(self->field_18);
        self->state = 2;
        self->elapsed = 0.0f;
    }
    return 0;
}

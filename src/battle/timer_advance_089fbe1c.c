typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0xB4];
    char field_C8[0x4];
} TimerTask;

extern float D_089FBF4C;
extern void func_08A08350(void *sub);

int func_089FBE1C(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_089FBF4C) {
        func_08A08350(self->field_C8);
        self->state = 3;
        self->elapsed = 0.0f;
    }
    return 0;
}

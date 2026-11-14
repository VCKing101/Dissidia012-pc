typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0xE0];
    char field_F4[0x4];
} TimerTask;

extern float D_08A1F6E4;
extern void func_08A08350(void *sub);

int func_08A1F5AC(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A1F6E4) {
        func_08A08350(self->field_F4);
        self->state = 6;
        self->elapsed = 0.0f;
    }
    return 0;
}

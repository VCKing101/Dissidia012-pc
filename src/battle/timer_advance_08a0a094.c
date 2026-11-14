typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    int field_14;
    char field_18[0x4];
} TimerTask;

extern float D_08A0A2B8;
extern void func_089F8BCC(void *sub, int arg1);

int func_08A0A094(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A0A2B8) {
        func_089F8BCC(self->field_18, self->field_14);
        self->state = 1;
        self->elapsed = 0.0f;
    }
    return 0;
}

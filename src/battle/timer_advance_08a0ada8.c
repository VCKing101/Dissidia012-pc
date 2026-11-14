typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x964];
    char field_978[0x4];
} TimerTask;

extern float D_08A0AEEC;
extern float D_08A0AEE0;
extern void func_08A08350(void *sub);

int func_08A0ADA8(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A0AEEC - D_08A0AEE0) {
        func_08A08350(self->field_978);
        self->state = 5;
        self->elapsed = 0.0f;
    }
    return 0;
}

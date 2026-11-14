typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x84];
    char field_98[0x4];
    char pad_9C[0x28];
    char field_C4[0x4];
} TimerTask;

extern float D_08A12B3C;
extern void func_08A10DB0(void *sub);

int func_08A12744(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A12B3C) {
        func_08A10DB0(self->field_98);
        func_08A10DB0(self->field_C4);
        self->state = 6;
        self->elapsed = 0.0f;
    }
    return 0;
}

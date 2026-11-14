typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0xB0];
    char field_C4[0x4];
    char pad_C8[0x1B8];
    char field_280[0x4];
} TimerTask;

extern float D_08A12B3C;
extern void func_08A10DB0(void *sub);
extern void func_08A08350(void *sub);

int func_08A1298C(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A12B3C) {
        func_08A10DB0(self->field_C4);
        func_08A08350(self->field_280);
        self->state = 11;
        self->elapsed = 0.0f;
    }
    return 0;
}

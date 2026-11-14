typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    int field_14;
    int field_18;
    char pad_1C[0xA8];
    char field_C4[0x4];
    char pad_C8[0x34];
    char field_FC[0x4];
} TimerTask;

extern float D_08A154FC;
extern float D_08A15500;
extern void func_089F6208(void *arg0, int arg1, int arg2, float arg3);
extern void func_089F854C(void *arg0);

int func_08A15228(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A154FC) {
        func_089F6208(self->field_FC, self->field_14, self->field_18, D_08A15500);
        func_089F854C(self->field_C4);
        self->state = 6;
        self->elapsed = 0.0f;
    }
    return 0;
}

typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    int field_14;
    char pad_18[0x4];
    int field_1C;
    char pad_20[0x80];
    char field_A0[0x4];
    char pad_A4[0xD4];
    char field_178[0x4];
} TimerTask;

extern float D_08A14510;
extern float D_08A1450C;
extern float D_08A14514;
extern void func_089F6208(void *arg0, int arg1, int arg2, float arg3);
extern void func_089F854C(void *arg0);

int func_08A141EC(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A14510 - D_08A1450C) {
        func_089F6208(self->field_178, self->field_14, self->field_1C, D_08A14514);
        func_089F854C(self->field_A0);
        self->state = 7;
        self->elapsed = 0.0f;
    }
    return 0;
}

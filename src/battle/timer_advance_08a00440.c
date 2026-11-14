typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    int field_14;
    int field_18;
    unsigned char field_1C;
    char pad_1D[0x5B];
    char field_78[0x4];
    char pad_7C[0x34];
    char field_B0[0x4];
} TimerTask;

extern float D_08A0069C;
extern float D_08A006A0;
extern void func_089F6208(void *arg0, int arg1, int arg2, float arg3);
extern void func_089F854C(void *arg0);

int func_08A00440(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A0069C) {
        if (self->field_1C == 0) {
            func_089F6208(self->field_B0, self->field_14, self->field_18, D_08A006A0);
            func_089F854C(self->field_78);
        }
        self->state = 2;
        self->elapsed = 0.0f;
    }
    return 0;
}

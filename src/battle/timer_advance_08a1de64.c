typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    int field_14;
    int field_18;
    unsigned char field_1C;
    char pad_1D[0x2F];
    char field_4C[0x4];
    char pad_50[0x34];
    char field_84[0x4];
} TimerTask;

extern float D_08A1E110;
extern float D_08A1E114;
extern void func_089F6208(void *arg0, int arg1, int arg2, float arg3);
extern void func_089F854C(void *arg0);

int func_08A1DE64(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A1E110) {
        if (self->field_1C == 0) {
            func_089F6208(self->field_84, self->field_14, self->field_18, D_08A1E114);
            func_089F854C(self->field_4C);
        }
        self->state = 3;
        self->elapsed = 0.0f;
    }
    return 0;
}

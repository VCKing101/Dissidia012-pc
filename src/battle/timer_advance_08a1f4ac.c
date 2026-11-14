typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    int field_14;
    int field_18;
    char pad_1C[0x58];
    char field_74[0x4];
    char pad_78[0x34];
    char field_AC[0x4];
} TimerTask;

extern float D_08A1F6E0;
extern float D_08A1F6E8;
extern void func_089F6208(void *arg0, int arg1, int arg2, float arg3);
extern void func_089F854C(void *arg0);

int func_08A1F4AC(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A1F6E0) {
        func_089F6208(self->field_AC, self->field_14, self->field_18, D_08A1F6E8);
        func_089F854C(self->field_74);
        self->state = 3;
        self->elapsed = 0.0f;
    }
    return 0;
}

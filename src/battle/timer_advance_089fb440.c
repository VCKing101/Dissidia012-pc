typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    int field_14;
    int field_18;
    char pad_1C[0x2C];
    char field_48[0x4];
    char pad_4C[0x34];
    char field_80[0x4];
} TimerTask;

extern float D_089FB61C;
extern float D_089FB624;
extern void func_089F6208(void *arg0, int arg1, int arg2, float arg3);
extern void func_089F854C(void *arg0);

int func_089FB440(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_089FB61C) {
        func_089F6208(self->field_80, self->field_14, self->field_18, D_089FB624);
        func_089F854C(self->field_48);
        self->state = 2;
        self->elapsed = 0.0f;
    }
    return 0;
}

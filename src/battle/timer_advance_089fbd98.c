typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    int field_14;
    char field_18[0x4];
    char pad_1C[0x74];
    char field_90[0x4];
} TimerTask;

extern float D_089FBF48;
extern void func_08A219FC(void *arg0, int arg1);
extern void func_089F854C(void *arg0);

int func_089FBD98(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_089FBF48) {
        func_08A219FC(self->field_18, self->field_14);
        func_089F854C(self->field_90);
        self->state = 1;
        self->elapsed = 0.0f;
    }
    return 0;
}

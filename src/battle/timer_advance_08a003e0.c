typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0xC];
    char field_20[0x4];
} TimerTask;

extern float D_08A00698;
extern void func_08A01AE0(void *arg0, int arg1, int arg2);

int func_08A003E0(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A00698) {
        func_08A01AE0(self->field_20, 0, 1);
        self->state = 1;
    }
    return 0;
}

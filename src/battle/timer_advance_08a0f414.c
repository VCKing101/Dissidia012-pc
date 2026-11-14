typedef struct TimerTask {
    char pad_00[0x4];
    int state;
    float elapsed;
    char pad_0C[0x111];
    unsigned char field_11D;
} TimerTask;

extern float D_08A0F8F4;
extern void func_08A0F784(TimerTask *self, int arg1);

int func_08A0F414(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A0F8F4) {
        if (self->field_11D == 0) {
            func_08A0F784(self, 1);
        }
        self->state = 4;
    }
    return 0;
}

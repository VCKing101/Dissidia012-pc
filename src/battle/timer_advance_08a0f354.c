typedef struct TimerTask {
    char pad_00[0x4];
    int state;
    float elapsed;
    char pad_0C[0x110];
    unsigned char field_11C;
} TimerTask;

extern float D_08A0F8EC;
extern void func_08A0F784(TimerTask *self, int arg1);

int func_08A0F354(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A0F8EC) {
        if (self->field_11C == 0) {
            func_08A0F784(self, 0);
        }
        self->state = 2;
    }
    return 0;
}

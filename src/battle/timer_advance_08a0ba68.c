typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x84];
    char field_98[0x4];
} TimerTask;

extern float D_08A0BBAC;
extern float D_08A0BBA0;
extern void func_08A08350(void *sub);

int func_08A0BA68(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A0BBAC - D_08A0BBA0) {
        func_08A08350(self->field_98);
        self->state = 7;
        self->elapsed = 0.0f;
    }
    return 0;
}

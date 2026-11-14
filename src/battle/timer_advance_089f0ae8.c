typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x100];
    char field_114[0x4];
} TimerTask;

extern float D_089F0D24;
extern void func_08A08350(void *sub);

int func_089F0AE8(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_089F0D24) {
        func_08A08350(self->field_114);
        self->state = 9;
    }
    return 0;
}

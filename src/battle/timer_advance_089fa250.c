typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0xB8];
    char field_CC[0x4];
} TimerTask;

extern float D_089FA37C;
extern void func_08A08350(void *sub);

int func_089FA250(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_089FA37C) {
        func_08A08350(self->field_CC);
        self->state = 5;
        self->elapsed = 0.0f;
    }
    return 0;
}

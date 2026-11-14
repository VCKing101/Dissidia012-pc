typedef struct TimerSignal {
    char pad_00[0x8];
    int state;
    unsigned char ready;
    char pad_0D[0x3];
    float elapsed;
    unsigned char done;
    char pad_15[0x3];
    int field_18;
    char pad_1C[0x60];
    char field_7C[0x4];
    char pad_80[0x34];
    int field_B4;
    int field_B8;
} TimerSignal;

extern float D_089F7C38;
extern int func_089E99C0(int parity, int value);
extern void func_089F85B0(void *sub, int value, int result);

int func_089F7990(TimerSignal *self, float dt)
{
    float before = self->elapsed;
    self->elapsed = before + dt;
    if (!self->done && self->elapsed >= D_089F7C38 && before < D_089F7C38) {
        int result = func_089E99C0((self->field_B4 + self->field_B8) % 2, self->field_18);
        func_089F85B0(self->field_7C, self->field_18, result);
    }
    if (self->ready == 1) {
        self->state = 2;
        self->elapsed = 0.0f;
    }
    return 0;
}

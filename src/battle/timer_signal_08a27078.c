typedef struct TimerSignal {
    char pad_00[0x8];
    int state;
    unsigned char ready;
    char pad_0D[0x3];
    float elapsed;
    unsigned char done;
    char pad_15[0x3];
    int field_18;
    char pad_1C[0x34];
    char field_50[0x4];
    char pad_54[0x34];
    int field_88;
    int field_8C;
} TimerSignal;

extern float D_08A272B8;
extern int func_089E99C0(int parity, int value);
extern void func_089F85B0(void *sub, int value, int result);

int func_08A27078(TimerSignal *self, float dt)
{
    float before = self->elapsed;
    self->elapsed = before + dt;
    if (!self->done && self->elapsed >= D_08A272B8 && before < D_08A272B8) {
        int result = func_089E99C0((self->field_88 + self->field_8C) % 2, self->field_18);
        func_089F85B0(self->field_50, self->field_18, result);
    }
    if (self->ready == 1) {
        self->state = 2;
        self->elapsed = 0.0f;
    }
    return 0;
}

typedef struct TimerTask {
    char pad_00[0x4];
    int state;
    float elapsed;
    char pad_0C[0x8];
    int field_14;
    int field_18;
    char field_1C[0x38];
    char field_54[0xBC];
    int field_110;
    int field_114;
} TimerTask;

extern void func_08A253CC(void *sub, int value);
extern void func_08889F70(int parity, int value);
extern int func_089E99C0(int parity, int value);
extern void func_089F84E0(void *sub);
extern void func_089F85B0(void *sub, int value, int result);

int func_08A26728(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= 16.0f) {
        int parity;
        int result;
        func_08A253CC(self->field_54, self->field_18);
        parity = (self->field_110 + self->field_114) % 2;
        func_08889F70(parity, self->field_110 + 1);
        result = func_089E99C0(parity, self->field_14);
        func_089F84E0(self->field_1C);
        func_089F85B0(self->field_1C, self->field_14, result);
        self->state = 2;
        self->elapsed = 0.0f;
    }
    return 0;
}

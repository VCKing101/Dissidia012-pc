typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x134];
    int field_148;
    int field_14C;
} TimerTask;

extern float D_08A16680;
extern void func_08889F70(int parity, int value);

int func_08A1646C(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A16680) {
        func_08889F70((self->field_148 + self->field_14C) % 2, self->field_148 + 5);
        self->state = 3;
        self->elapsed = 0.0f;
    }
    return 0;
}

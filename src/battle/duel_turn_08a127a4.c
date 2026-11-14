typedef struct DuelTask {
    void *vtable;
    int field_04;
    int state;
    char pad_0C[0x4];
    float elapsed;
    int first;
    int second;
    char pad_1C[0xD4];
    char second_part[0x40];
    char first_part[0x184];
    int round;
    int turn;
} DuelTask;

extern float D_08A12B3C;
extern float D_08A12B40;
extern int func_089E99C0(int side, int unit);
extern void func_08A1197C(void *part, int unit, int value);
extern void func_089F85B0(void *part, int unit, int value);

/* After the delay, picks this turn's side and starts both units' parts (state 7). */
int func_08A127A4(DuelTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A12B40 - D_08A12B3C) {
        int side = (self->round + self->turn) % 2;
        int first = func_089E99C0(side, self->first);
        int second = func_089E99C0(side, self->second);
        func_08A1197C(self->first_part, self->first, first);
        func_089F85B0(self->second_part, self->second, second);
        self->state = 7;
        self->elapsed = 0.0f;
    }
    return 0;
}

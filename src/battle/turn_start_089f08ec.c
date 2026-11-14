typedef struct TurnTask {
    void *vtable;
    int field_04;
    int state;
    char pad_0C[0x4];
    float elapsed;
    int unit;
    int team;
    char pad_1C[0x28];
    char part_44[0x38];
    char part_7C[0xCC];
    int round;
    int turn;
} TurnTask;

extern float D_089F0D1C;
extern int func_089E99C0(int side, int unit);
extern void func_08A0E69C(void *part, int unit, int value, int team);
extern void func_089F854C(void *part);
extern void func_08889F28(int side, int round);

/* After the delay, picks the side for this turn and starts its parts (state 5). */
int func_089F08EC(TurnTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_089F0D1C) {
        int side = (self->round + self->turn) % 2;
        int value = func_089E99C0(side, self->unit);
        func_08A0E69C(self->part_7C, self->unit, value, self->team);
        func_089F854C(self->part_44);
        func_08889F28(side, self->round + 5);
        self->state = 5;
    }
    return 0;
}

typedef struct RoundStart {
    void *vtable;
    int state;
    float elapsed;
    int unit;
    unsigned char side;
    char pad_11[0x7];
    int mode;
    char part_1C[0x38];
    char part_54[0xBC];
    int round;
    int turn;
} RoundStart;

extern float D_08A268D8;
extern void func_08A251B8(void *part, int unit, unsigned char side, int mode);
extern void func_089F854C(void *part);
extern void func_08889F28(int side, int round);

/* After the delay, starts the unit's parts and announces the round (state 1). */
int func_08A26698(RoundStart *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A268D8) {
        func_08A251B8(self->part_54, self->unit, self->side, self->mode);
        func_089F854C(self->part_1C);
        func_08889F28((self->round + self->turn) % 2, self->round + 1);
        self->state = 1;
        self->elapsed = 0.0f;
    }
    return 0;
}

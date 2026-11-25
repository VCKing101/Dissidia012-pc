/* Which burst follow-up the fighter takes from its current state: -1 when
 * not ready or in state 0x19 or 0x13; from the dash states (0xC, 0xD, and
 * 0xE unless grounded) 0 with skill 0x116, 1 with skill 0x117; else -1. */
typedef struct PickState {
    volatile int kind;
} PickState;

typedef struct PickMove {
    unsigned char mode;
} PickMove;

typedef struct PickFighter {
    unsigned char pad_000[0x3A4];
    PickState *state;
    PickMove *move;
} PickFighter;

extern void *func_08835778(PickFighter *fighter);
extern void *func_088BF4CC(void *skills, int id);
extern int func_08B305E8(PickFighter *fighter);

int func_08A52ED8(PickFighter *self)
{
    PickState *state;
    int c19;
    if (func_08B305E8(self) == 0) {
        return -1;
    }
    state = self->state;
    c19 = 0x19;
    if (state->kind == c19) {
        return -1;
    }
    if (state->kind == 0x13) {
        return -1;
    }
    if (state->kind == 0xC || state->kind == 0xD || (state->kind == 0xE && self->move->mode != 2)) {
        if ((unsigned char)(func_088BF4CC(func_08835778(self), 0x116) != 0)) {
            return 0;
        }
        if ((unsigned char)(func_088BF4CC(func_08835778(self), 0x117) != 0)) {
            return 1;
        }
    }
    return -1;
}

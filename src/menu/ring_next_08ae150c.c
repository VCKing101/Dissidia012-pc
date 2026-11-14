typedef struct RingState {
    char pad_000[0x5C4];
    int field_5C4;
    int field_5C8;
    int field_5CC;
} RingState;

typedef struct RingOwner {
    char pad_000[0x850];
    RingState *field_850;
} RingOwner;

int func_08AE150C(RingOwner *self)
{
    RingState *state = self->field_850;
    int next[2];
    next[0] = state->field_5C4 + state->field_5C8;
    if (next[0] >= state->field_5CC) {
        next[0] = next[0] - state->field_5CC;
    }
    return next[0];
}

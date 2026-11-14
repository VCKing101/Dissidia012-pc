typedef struct CounterFighter {
    char pad_000[0x5C4];
    int field_5C4;
} CounterFighter;

typedef struct CounterState {
    int mode;
    char pad_004[0x14];
    float total;
    float step;
    int count;
} CounterState;

typedef struct CounterStep {
    CounterFighter *fighter;
    char pad_004[0x10];
    CounterState *state;
} CounterStep;

extern void func_08A6427C(int id, int value);

void func_08A39B48(CounterStep *self)
{
    CounterState *state = self->state;
    if (state->mode != 1) {
        return;
    }
    if (state->count < (int)(state->total / state->step)) {
        func_08A6427C(self->fighter->field_5C4, 1);
        self->state->count++;
    }
}

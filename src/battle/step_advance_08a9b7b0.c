typedef struct SequenceStep {
    int done;
    int kind;
} SequenceStep;

typedef struct Sequence {
    char pad_00[0x34];
    SequenceStep steps[16];
    unsigned int count;
    unsigned int index;
} Sequence;

extern void func_08A9B928(Sequence *self);

int func_08A9B7B0(Sequence *self)
{
    unsigned int index = self->index;

    if (index < self->count) {
        SequenceStep *step = &self->steps[index];
        int finished;
        if (step->done == 0) {
            int kind = step->kind;
            finished = (kind == 4) | (kind == 8);
        } else {
            finished = 1;
        }
        if (finished == 1) {
            self->index = index + 1;
            func_08A9B928(self);
        }
    }
    return 0;
}

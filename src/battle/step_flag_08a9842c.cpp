typedef struct StepEntry {
    char pad_00[0xC];
    signed char flag;
    char pad_0D[0x3];
} StepEntry;

typedef struct StepList {
    unsigned char active;
    char pad_01[0x73];
    StepEntry *steps;
    int count;
    int index;
} StepList;

extern "C" {

extern int func_08899944(void);
extern int func_08A98828(StepList *self);

bool func_08A9842C(StepList *self)
{
    if (!self->active) {
        return false;
    }
    if (func_08899944() == 1) {
        return false;
    }
    if (func_08A98828(self) == 0) {
        return false;
    }
    if (self->index >= self->count) {
        return false;
    }
    return self->steps[self->index].flag != 0;
}

}

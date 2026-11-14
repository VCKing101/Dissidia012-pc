typedef struct StepMachine {
    char pad_00[0x8];
    int state;
    int busy;
} StepMachine;

extern void func_08A0E008(StepMachine *self);
extern void func_08A0E068(StepMachine *self);
extern void func_08A0E0B8(StepMachine *self);
extern void func_08A0E16C(StepMachine *self);
extern void func_08A0E1AC(StepMachine *self);
extern void func_08A0E278(StepMachine *self);

/* Runs the step for the current state, then the idle step when nothing is busy. */
int func_08A0DF54(StepMachine *self)
{
    switch (self->state) {
    case 0:
        func_08A0E008(self);
        break;
    case 1:
        func_08A0E068(self);
        break;
    case 2:
        func_08A0E0B8(self);
        break;
    case 3:
        func_08A0E16C(self);
        break;
    case 4:
        func_08A0E1AC(self);
        break;
    }
    if (self->busy == 0) {
        func_08A0E278(self);
    }
    return 0;
}

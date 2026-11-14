#include "battle/agent28_types.h"

/* Runs the handler of the relay phase's current state. */
extern int func_08A0C38C(PhaseState *phase);
extern int func_08A0C3E4(PhaseState *phase);
extern int func_08A0C42C(PhaseState *phase);
extern int func_08A0C44C(PhaseState *phase);
extern int func_08A0C45C(PhaseState *phase);
extern int func_08A0C4B4(PhaseState *phase);
extern int func_08A0C51C(PhaseState *phase);

int func_08A0C2DC(PhaseState *phase)
{
    switch (phase->state) {
    case 0:
        return func_08A0C38C(phase);
    case 1:
        return func_08A0C3E4(phase);
    case 2:
        return func_08A0C42C(phase);
    case 3:
        return func_08A0C44C(phase);
    case 4:
        return func_08A0C45C(phase);
    case 5:
        return func_08A0C4B4(phase);
    case 6:
        return func_08A0C51C(phase);
    }
    return 0;
}
